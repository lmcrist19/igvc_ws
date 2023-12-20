#include <Arduino.h>
#include <ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32MultiArray.h>
#include <Servo.h>

#define in1_l 2
#define in2_l 3
#define ena_l 9
#define in3_r  4
#define in4_r  5
#define enb_r  10

float test_length;
float test_time;
float track_width;
float wheel_radius;
float rpm_to_analog;

float vel_ang;
float vel_lin;

float rpm_l;
float rpm_r;

float pi = 3.14159265358;

ros::NodeHandle  nh;
geometry_msgs::Twist msg_vel;
std_msgs::Float32MultiArray msg_config;

//////////////////////
// Velocity callback
//////////////////////
void cmdVelMsg( const geometry_msgs::Twist& msg_vel){
  vel_ang = msg_vel.angular.z;
  vel_lin = msg_vel.linear.x;

  rpm_l = (60*(vel_lin-(track_width*vel_ang)))/(2*pi*wheel_radius); 
  rpm_r = (60*(vel_lin+(track_width*vel_ang)))/(2*pi*wheel_radius);
  
  analogWrite(ena_l, rpm_l*rpm_to_analog);
  analogWrite(enb_r, rpm_r*rpm_to_analog);
  digitalWrite(in1_l, LOW);
  digitalWrite(in2_l, HIGH);
  digitalWrite(in3_r, HIGH);
  digitalWrite(in4_r, LOW);
}

///////////////////////
// Store message data
///////////////////////
void pythonCommsMsg(const std_msgs::Float32MultiArray& msg_config){
  test_length = msg_config.data[0];
  test_time = msg_config.data[1];
  track_width = msg_config.data[2];
  wheel_radius = msg_config.data[3];

  // Max RPM's based on callibration run
  float circumference = 2*pi*wheel_radius;
  float rotations = test_length/circumference;
  float max_rpm = (60*(rotations/test_time));
  rpm_to_analog = 255/max_rpm;
}

/////////////////////////////////////
// Define subscribers and publishers
/////////////////////////////////////
ros::Subscriber<geometry_msgs::Twist> sub_twist("/cmd_vel", cmdVelMsg);
ros::Subscriber<std_msgs::Float32MultiArray> sub_serial("comms/python", pythonCommsMsg);

void setup()
{
  Serial.begin(57600);
  pinMode(in1_l, OUTPUT);
  pinMode(in2_l, OUTPUT);
  pinMode(in3_r,   OUTPUT);
  pinMode(in4_r, OUTPUT);
  pinMode(ena_l, OUTPUT); 
  pinMode(enb_r, OUTPUT);

  nh.initNode(); // Initialize node
  Serial.println("Arduino Communication node initialized");
  nh.subscribe(sub_twist);
  nh.subscribe(sub_serial);
}

/////////////////
// Main function
/////////////////
void loop()
{
  nh.spinOnce();
}