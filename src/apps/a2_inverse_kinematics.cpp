#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <sys/select.h>
#include <sys/time.h>
#include <thread>
#include <vector>

#include <lcm/lcm.h>
#include "lcmtypes/dynamixel_command_list_t.h"
#include "lcmtypes/dynamixel_command_t.h"
#include "lcmtypes/dynamixel_status_list_t.h"
#include "lcmtypes/dynamixel_status_t.h"

#include "common/getopt.h"
#include "common/timestamp.h"
#include "math/math_util.h"
#include "math/matd.h"
#include "math/fasttrig.h"
using namespace std;

#define NUM_SERVOS 6

const double inches_to_meters = 0.0254;

const double d1 = 0.12;
const double d2 = 0.10;
const double d3 = 0.10;
const double d4 = 0.115;

typedef struct state state_t;
struct state
{
  getopt_t *gopt;

  // LCM
  lcm_t *lcm;
  const char *command_channel;
  const char *status_channel;
  
  thread status_thread;
  thread command_thread;
};

state_t state;

static void
status_handler (const lcm_recv_buf_t *rbuf,
                const char *channel,
                const dynamixel_status_list_t *msg,
                void *user)
{
  // Print out servo positions
  /*
  for (int id = 0; id < msg->len; id++) {
    dynamixel_status_t stat = msg->statuses[id];
    printf ("[id %d]=%6.3f ",id, stat.position_radians);
  }
  printf ("\n");
  */
}

void
status_loop ()
{
  
  dynamixel_status_list_t_subscribe (state.lcm,
				     state.status_channel,
				     status_handler,
				     &state);
  const int hz = 15;
  while (1) {
    // Set up the LCM file descriptor for waiting. This lets us monitor it
    // until something is "ready" to happen. In this case, we are ready to
    // receive a message.
    int status = lcm_handle_timeout (state.lcm, 1000/hz);
    if (status <= 0)
      continue;

    // LCM has events ready to be processed
  }

  return;
}

vector<double> calculate_angles(vector<double> coords){
  /* coords:
   * [0] = x
   * [1] = y
   * [2] = z
   * [3] = theta1 ?
   * [4] = theta2 ?
   * [5] = theta3 ?
   */
  vector<double> angles(5);
  /* angle[0] = base
   * angle[1] = shoulder
   * angle[2] = elbow
   * angle[3] = wrist bend
   * angle[4] = wrist rotate
   */
  
  double h = coords[2];
  double R_squared = sq(coords[0]) + sq(coords[1]);
  double M_squared = R_squared + sq(d4 + h - d1);
  double alpha = atan2(d4 + h - d1, sqrt(R_squared));
  double beta = facos((-(d3*d3) + (d2*d2) + M_squared) / (2.0 * d2 * sqrt(M_squared)));
  double gamma = facos(((-1)*M_squared + (d2*d2) + (d3*d3)) / (2 * d2 * d3));

  angles[0] = atan2(coords[1], coords[0]);
  angles[1] = -1*((M_PI/2.0) - alpha - beta);
  angles[2] = -1*(M_PI - gamma);
  angles[3] = -1*M_PI - angles[1] - angles[2];
  angles[4] = 0;

  cout << "R_squared: " << R_squared << endl;
  cout << "M_squared: " << M_squared << endl;
  cout << "alpha: " << alpha << endl;
  //cout << "beta_num: " << (-(d3*d3) + (d2*d2) + M_squared) << endl;
  //cout << "beta_denom: " << (2.0 * d2 * sqrt(M_squared)) << endl;
  cout << "beta: " << beta << endl << endl;
  //cout << "gamma_num: " << ((-1)*M_squared + (d2*d2) + (d3*d3)) << endl;
  //cout << "gamma_denom: " << (2 * d2 * d3) << endl;
  cout << "gamma: " << gamma << endl << endl;
  cout << endl;
  cout << "Base: " << angles[0] << endl;
  cout << "Shoulder: " << angles[1] << endl;
  cout << "Elbow: " << angles[2] << endl;
  cout << "Wrist Bend: " << angles[3] << endl;
  cout << "Wrist Rotate: " << angles[4] << endl;
  return angles;
}

void command_loop()
{
  const int hz = 30;

  dynamixel_command_list_t cmds;
  cmds.len = NUM_SERVOS;
  cmds.commands = (dynamixel_command_t*) calloc(NUM_SERVOS, sizeof(dynamixel_command_t));
  
  // set arm to home position
  for (int id = 0; id < NUM_SERVOS; id++) {
    cmds.commands[id].utime = utime_now ();
    if(id == 5)
      cmds.commands[id].position_radians = M_PI/2.0;
    else
      cmds.commands[id].position_radians = 0.0;
    cmds.commands[id].speed = 0.075;
    cmds.commands[id].max_torque = 0.75;
    dynamixel_command_list_t_publish (state.lcm, state.command_channel, &cmds);
    usleep (1000000/hz);
  }
  
  
  while(1){
    // cin x, y, z, theta1, theta2, theta3
    vector<double> desired(6);  // x, y, z, theta1, theta2, theta3
    // (x, y, z) = location in global position to move grippers to
    // theta1 = roll
    // theta2 = pitch
    // theta3 = yaw ? 
    desired[0] = desired[1] = desired[2] = desired[3] = desired[4] = desired[5] = 0.31;
    cout << "Select a position to move to" << endl;
    while((desired[0] > 0.15) || (desired[0] < -0.15)){
      cout << "x: ";
      cin >> desired[0];
    }
    while((desired[1] > 0.15) || (desired[1] < -0.15)){
      cout << endl << "y: ";
      cin >> desired[1];
    }
    while((desired[2] > 0.15) || (desired[2] < -0.15)){
      cout << endl << "z: ";
      cin >> desired[2];
    }
    cout << endl << "theta1: ";
    cin >> desired[3];
    cout << endl << "theta2: ";
    cin >> desired[4];
    cout << endl << "theta3: ";
    cin >> desired[5];
    cout << endl << endl;
    // calculate servo positions
    vector<double> angles(5);
    angles = calculate_angles(desired);
    for( int i = 0; i < 5; ++i){
      //cout << "angles[" << i << "]: " << angles[i] << endl;
      //if(i == 0){
	cmds.commands[i].utime = utime_now();
	cmds.commands[i].position_radians = angles[i];
	cmds.commands[i].speed = 0.075;
	cmds.commands[i].max_torque = 0.45;
	dynamixel_command_list_t_publish (state.lcm, state.command_channel, &cmds);
	//}
      usleep (3000000/hz);
    }
    cout << endl << endl;
  }
}

int main(int argc, char *argv[])
{

  getopt_t *gopt = getopt_create ();
  getopt_add_bool (gopt, 'h', "help", 0, "Show this help screen");
  getopt_add_bool (gopt, 'i', "idle", 0, "Command all servos to idle");
  getopt_add_string (gopt, '\0', "status-channel", "ARM_STATUS", "LCM status channel");
  getopt_add_string (gopt, '\0', "command-channel", "ARM_COMMAND", "LCM command channel");

  if (!getopt_parse (gopt, argc, argv, 1) || getopt_get_bool (gopt, "help")) {
    getopt_do_usage (gopt);
    exit (EXIT_FAILURE);
  }

  
  state.gopt = gopt;
  state.lcm = lcm_create (NULL);
  state.command_channel = getopt_get_string (gopt, "command-channel");
  state.status_channel = getopt_get_string (gopt, "status-channel");

  state.status_thread = thread(status_loop);
  state.command_thread = thread(command_loop);

  state.status_thread.join();
  state.command_thread.join();

  lcm_destroy(state.lcm);
  getopt_destroy(gopt);
  return 0;
}
