#include <thread>
#include <pthread.h>

#include <gtk/gtk.h>
#include <lcm/lcm-cpp.hpp>

#include "vx/vx.h"
#include "vx/vx_util.h"
#include "vx/gtk/vx_gtk_display_source.h"
#include "vx/vxo_drawables.h"
#include "common/zarray.h"
#include "common/getopt.h"
#include "imagesource/image_source.h"
#include "imagesource/image_convert.h"
#include "imagesource/image_u32.h"
#include "imagesource/image_util.h"
#include "math/point.hpp"

#include "hsv.hpp"
#include "a2_inv_kin.hpp"
#include "image_processor.hpp"
using namespace std;

typedef struct state state_t;
struct state
{
  getopt_t *gopt;

  lcm_t *lcm;
  const char *command_channel;
  const char *status_channel;

  char *camera_url;
  char *pic_url;
  bool usePic;
  image_processor im_proc;
  vx_application_t vxapp;
  vx_world_t *vxworld;
  zhash_t *layers;
  vx_event_handler_t *vxeh;
  vx_mouse_event_t last_mouse_event;
  vx_gtk_display_source_t *appwrap;

  max_min_hsv cyan_hsv;  
  eecs467::Point<float> corner_coords[2];

  thread status_thread;
  thread command_thread;
  thread animate_thread;

  pthread_mutex_t mutex;
  pthread_mutex_t data_mutex;
};
state_t state;

static void status_handler(const lcm_recv_buf_t *rbuf,
			   const char *channel,
			   const dynamixel_status_list_t *msg,
			   void *user){
  //do stuff
}

void status_loop(){
  dynamixel_status_list_t_subscribe (state.lcm,
				     state.status_channel,
				     status_handler,
				     &state);
  while(1){
    lcm_handle(state.lcm);
  }
}

void command_loop(){
  dynamixel_command_list_t cmds;
  cmds.len = NUM_SERVOS;
  cmds.commands = (dynamixel_command_t*) calloc(NUM_SERVOS, sizeof(dynamixel_command_t));
  while(1){
    for(int id = 0; id < NUM_SERVOS; ++id){
      cmds.commands[id].utime = utime_now();
      cmds.commands[id].position_radians = 0.0;
      cmds.commands[id].speed = 0.0;
      cmds.commands[id].max_torque = 0.0;
    }
    dynamixel_command_list_t_publish (state.lcm, state.command_channel, &cmds);
  }
  free(cmds.commands);

  
}

void render_loop(){
  int fps = 60;
  image_source_t *isrc = NULL;
  if(!state.usePic){
    isrc = image_source_open(state.camera_url);
    if(isrc == NULL){
      cout << "Error opening device" << endl;
      exit(1);
    }
    else
      isrc->start(isrc);
  }
  while(1){
    pthread_mutex_lock(&state.data_mutex);
    vx_buffer_t *buf = vx_world_get_buffer(state.vxworld,"image");
    image_u32_t *im; 
    std::vector<int> cyan_center_list;
    if(state.usePic){
      im = image_u32_create_from_pnm(state.pic_url); 
    }
    else if(isrc != NULL){
      image_source_data_t *frmd = (image_source_data_t*) calloc(1,sizeof(*frmd));
      int res = isrc->get_frame(isrc,frmd);
      if(res < 0){
	printf("get_frame fail\n");
      }
      else{
	im = image_convert_u32(frmd);
      }
      fflush(stdout);
      isrc->release_frame(isrc,frmd);
    }
    if((state.corner_coords[0].x != -1) && (state.corner_coords[0].y != -1) && (state.corner_coords[1].x != -1) && (state.corner_coords[1].y != -1)){  
      state.im_proc.image_masking(im, state.corner_coords[0].x, state.corner_coords[1].x, state.corner_coords[0].y, state.corner_coords[1].y);
      cyan_center_list = state.im_proc.blob_detection(im, state.corner_coords[0].x, state.corner_coords[1].x, state.corner_coords[0].y, state.corner_coords[1].y, state.cyan_hsv);
    }
    if(!cyan_center_list.empty()){
      for(int i=0; i<cyan_center_list.size(); ++i){
	int y = (cyan_center_list[i]) / im->width;
	int x = (cyan_center_list[i]) % im->width;
	state.im_proc.draw_circle(im, x, y, 20.0, 0xffffff00);
      }
    }
    if(im != NULL){
      vx_object_t *vim = vxo_image_from_u32(im,
					    VXO_IMAGE_FLIPY,
					    VX_TEX_MIN_FILTER | VX_TEX_MAG_FILTER);
      //use pix coords to make a fix image
      vx_buffer_add_back (buf,vxo_chain (
					 vxo_mat_translate3 (-im->width/2., -im->height/2., 0.),
					 vim));
      image_u32_destroy (im);
      
    }
    pthread_mutex_unlock(&state.data_mutex);
    vx_buffer_swap(buf);
    usleep(1000000/fps);
  }
}

static int mouse_event(vx_event_handler_t *vxeh, vx_layer_t *vl, vx_camera_pos_t *pos, vx_mouse_event_t *mouse){
  pthread_mutex_lock(&state.data_mutex);
  if((mouse->button_mask & VX_BUTTON1_MASK) && !(state.last_mouse_event.button_mask & VX_BUTTON1_MASK)){
    vx_ray3_t ray;
    vx_camera_pos_compute_ray (pos, mouse->x, mouse->y, &ray);
    double ground[3];
    vx_ray3_intersect_xy(&ray, 0, ground);
    printf ("Mouse clicked at coords: [%8.3f, %8.3f] Ground clicked at coords: [%6.3f, %6.3f]\n",
	    mouse->x, mouse->y, ground[0], ground[1]);
  }
  state.last_mouse_event = *mouse;
  pthread_mutex_unlock(&state.data_mutex);
}

static int key_event(vx_event_handler_t *vxeh, vx_layer_t *vl, vx_key_event_t *key){
  return 0;
}

static int touch_event(vx_event_handler_t *vxeh, vx_layer_t *vl, vx_camera_pos_t *pos, vx_touch_event_t *mouse){
  return 0;
}

static void display_finished(vx_application_t *app, vx_display_t *disp){
  pthread_mutex_lock(&state.mutex);
  vx_layer_t *layer = NULL;
  zhash_remove(state.layers, &disp, NULL, &layer);
  vx_layer_destroy(layer);
  pthread_mutex_unlock(&state.mutex);
}

static void display_started(vx_application_t *app, vx_display_t *disp){
  vx_layer_t *layer = vx_layer_create(state.vxworld);
  vx_layer_set_display(layer, disp);
  pthread_mutex_lock(&state.mutex);
  zhash_put(state.layers, &disp, &layer, NULL, NULL);
  pthread_mutex_unlock(&state.mutex);
}

static void init_stuff(){
  //GUI init stuff
  state.vxworld = vx_world_create();
  printf("create world\n");
  state.vxeh = (vx_event_handler_t*) calloc(1,sizeof(*state.vxeh));
  state.vxeh->mouse_event = mouse_event;
  state.vxeh->touch_event = touch_event;
  state.vxeh->key_event = key_event;
  state.vxeh->dispatch_order = 100;
  state.vxeh->impl = &state;
  printf("create vxeh\n");
  state.vxapp.impl= &state;
  state.vxapp.display_started = display_started;
  state.vxapp.display_finished = display_finished;
  state.layers = zhash_create(sizeof(vx_display_t*), sizeof(vx_layer_t*), zhash_ptr_hash, zhash_ptr_equals);
  
  pthread_mutex_init (&state.mutex, NULL);
  pthread_mutex_init (&state.data_mutex,NULL);
  
  
  state.gopt = getopt_create(); 
  FILE *fp = fopen("../calibration/mask_rect.txt","r");
  fscanf(fp,"%f %f %f %f\n",&state.corner_coords[0].x,&state.corner_coords[1].x,&state.corner_coords[0].y,&state.corner_coords[1].y);
  printf("coord: %f %f %f %f\n",state.corner_coords[0].x,state.corner_coords[1].x,state.corner_coords[0].y,state.corner_coords[1].y);
  //red_hsv.read_hsv_from_file("../calibration/red_hsv_range.txt");
  //green_hsv.read_hsv_from_file("../calibration/green_hsv_range.txt");
  state.cyan_hsv.read_hsv_from_file("../calibration/cyan_hsv_range.txt");
}

static void destroy_stuff(){
  vx_world_destroy(state.vxworld);
  if(zhash_size(state.layers) != 0){
    zhash_destroy(state.layers);
  }
  if(state.gopt){
    getopt_destroy(state.gopt);
  }
  lcm_destroy(state.lcm);
  pthread_mutex_destroy(&state.mutex);
}

int main(int argc, char *argv[]){
  
  init_stuff();

  getopt_add_bool (state.gopt, 'h', "help", 0, "Show this help screen");
  getopt_add_bool (state.gopt, 'i', "idle", 0, "Command all servos to idle");
  getopt_add_string (state.gopt, '\0', "status-channel", "ARM_STATUS", "LCM status channel");
  getopt_add_string (state.gopt, '\0', "command-channel", "ARM_COMMAND", "LCM command channel");
  getopt_add_string (state.gopt, 'f', "picurl", "", "Picture URL");


  if (!getopt_parse (state.gopt, argc, argv, 1) || getopt_get_bool (state.gopt, "help")) {
    getopt_do_usage (state.gopt);
    exit (EXIT_FAILURE);
  }

  if (strncmp (getopt_get_string (state.gopt, "picurl"), "", 1)) {
    state.pic_url = strdup (getopt_get_string (state.gopt, "picurl"));
    state.usePic = true;
    printf ("URL: %s\n", state.pic_url);
  }
  else{
    printf("camera find\n");
    zarray_t *urls = image_source_enumerate();
    if(0 == zarray_size(urls)){
      printf("No camera found.\n");
      exit(1);
    }
    printf("find camera\n");
    zarray_get(urls,0,&state.camera_url);
    printf("get camera address\n");
  }

  state.lcm = lcm_create (NULL);
  state.command_channel = getopt_get_string (state.gopt, "command-channel");
  state.status_channel = getopt_get_string (state.gopt, "status-channel");

  //vx
  gdk_threads_init();
  gdk_threads_enter();
  gtk_init(&argc, &argv);

  state.status_thread = thread(status_loop);
  state.command_thread = thread(command_loop);
  state.animate_thread = thread(render_loop);

  state.appwrap = vx_gtk_display_source_create(&state.vxapp);
  GtkWidget * window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  GtkWidget * canvas = vx_gtk_display_source_get_widget(state.appwrap);
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
  gtk_container_add(GTK_CONTAINER(window), canvas);
  gtk_widget_show (window);
  gtk_widget_show (canvas); // XXX Show all causes errors!
  g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_main(); // Blocks as long as GTK window is open

  gdk_threads_leave();
  vx_gtk_display_source_destroy(state.appwrap);



  state.status_thread.join();
  state.command_thread.join();
  state.animate_thread.join();

  destroy_stuff();
  return 0;
}
