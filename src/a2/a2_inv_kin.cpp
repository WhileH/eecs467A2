#include "a2_inv_kin.hpp"
using namespace std;

inv_kinematics::inv_kinematics()
{
  fasttrig_init();
  cmds.len = NUM_SERVOS;
  cmds.commands = (dynamixel_command_t*) calloc(NUM_SERVOS, sizeof(dynamixel_command_t));
  lcm = lcm_create(NULL);
  
  /*
  for (int id = 0; id < NUM_SERVOS; id++) {
    cmds.commands[id].utime = utime_now ();
    if(id == 5){
      cmds.commands[id].max_torque = 0.75;
      cmds.commands[id].speed = 0.1;
    }
    else{
      cmds.commands[id].max_torque = 0.5;
      cmds.commands[id].speed = 0.075;
    }
    
    dynamixel_command_list_t_publish (lcm, command_channel, &cmds);
  }
  */
}


inv_kinematics::inv_kinematics(lcm_t *l, const char *com_chan)
{
  fasttrig_init();
  cmds.len = NUM_SERVOS;
  cmds.commands = (dynamixel_command_t*) calloc(NUM_SERVOS, sizeof(dynamixel_command_t));
  lcm = l;
  command_channel = (char*) com_chan;

  for (int id = 0; id < NUM_SERVOS; id++) {
    cmds.commands[id].utime = utime_now ();
    if(id == 5){
      cmds.commands[id].max_torque = 0.75;
      cmds.commands[id].speed = 0.1;
    }
    else{
      cmds.commands[id].max_torque = 0.5;
      cmds.commands[id].speed = 0.075;
    }
    
    dynamixel_command_list_t_publish (lcm, command_channel, &cmds);
  }
}

inv_kinematics::~inv_kinematics()
{
}

void inv_kinematics::move_2_pos(double x, double y)
{
  vector<double> angles(5);
  /* angle[0] = base
   * angle[1] = shoulder
   * angle[2] = elbow
   * angle[3] = wrist bend
   * angle[4] = wrist rotate
   */
  double R_squared = sq(x) + sq(y);
  double M_squared = R_squared + sq(d4 + h - d1);
  double alpha = atan2(d4 + h - d1, sqrt(R_squared));
  double beta = facos((-(d3*d3) + (d2*d2) + M_squared) / (2.0 * d2 * sqrt(M_squared)));
  double gamma = facos(((-1)*M_squared + (d2*d2) + (d3*d3)) / (2.0 * d2 * d3));
  
  angles[0] = atan2(y, x);
  angles[1] = (M_PI/2.0) - alpha - beta;
  angles[2] = M_PI - gamma;
  angles[3] = M_PI - angles[1] - angles[2];
  angles[4] = 0;
  cout << "base: " << angles[0] << endl;
  cout << "shoulder: " << angles[1] << endl;
  cout << "elbow: " << angles[2] << endl;
  cout << "wrist: " << angles[3] << endl;

  int servo_order[5] = {0, 4, 3, 2, 1};
  for(auto i : servo_order){
    cmds.commands[i].utime = utime_now();
    cmds.commands[i].position_radians = angles[i];
    dynamixel_command_list_t_publish(lcm, command_channel, &cmds);
    usleep(1000000);
  }
  return;
}

void inv_kinematics::go_home()
{
  vector<double> angles(5);

  angles[0] = angles[1] = angles[2] = angles[3] = angles[4] = 0.0;
  int servo_order[5] = {1, 2, 3, 4, 0};
  for(auto i : servo_order){
    cmds.commands[i].utime = utime_now();
    cmds.commands[i].position_radians = angles[i];
    dynamixel_command_list_t_publish(lcm, command_channel, &cmds);
    usleep(300000);
  }
  return;
}

void inv_kinematics::pick_up(double x, double y)
{
  move_2_pos(x, y);
  cmds.commands[5].position_radians = FINGERS_CLOSE;
  usleep(2500000);
  go_home();
  return;
}

void inv_kinematics::place(double x, double y)
{
  move_2_pos(x, y);
  cmds.commands[5].position_radians = FINGERS_OPEN;
  dynamixel_command_list_t_publish(lcm, command_channel, &cmds);
  usleep(2500000);
  go_home();
  return;
}

void inv_kinematics::set_cmds(dynamixel_command_list_t &command_list)
{
  cmds = command_list;
  return;
}

void inv_kinematics::set_lcm(lcm_t *l)
{
  lcm = l;
  return;
}

void inv_kinematics::set_com_channel(const char *com_chan)
{
  command_channel = (char*) com_chan;
  return;
}