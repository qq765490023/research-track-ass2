#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "ass_2/speed_control.h"
ros::Publisher pub;
float speed_factor = 5;

void scanner_callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{   

//printf("sizeof range %d",sizeof(msg->ranges));//printf the size of laser data
int i =0;
float mimimum_left = 100;
float mimimum_fleft = 100;
float mimimum_front = 100;
float mimimum_fright = 100;
float mimimum_right = 100;

float foctor_rot = 1;
float det = 0;

// divide laser_scan data into 180 pieces and choose a range and take the mimimun
for(i = 0;i<720;i++)
{
if(36*4<=i && i<72*4)  				//choose angle 36 -> 72 as fleft
{
if(msg->ranges[i]<mimimum_fleft)
mimimum_fleft = msg->ranges[i];
}
else if(85*4<=i && i<95*4)  				//choose angle 85 -> 95 as front
{
if(msg->ranges[i]<mimimum_front)
mimimum_front = msg->ranges[i];
}
else if(108*4<=i && i<144*4)  				//choose angle 108 -> 144 as fright
{
if(msg->ranges[i]<mimimum_fright)
mimimum_fright = msg->ranges[i];
}


}
//////////////////////find case ///////////////////////////
int case_ = -1;
float tht_f = 0.8;
float tht_f_l_r= 0.6;

geometry_msgs::Twist my_vel;

if(
mimimum_fleft >= tht_f_l_r && 
mimimum_front >= tht_f && 
mimimum_fright >= tht_f_l_r  
)
{
case_ = 1;//keep moving
my_vel.linear.x = 0.6 *speed_factor;
my_vel.angular.z = 0 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft >= tht_f_l_r && 
mimimum_front >= tht_f && 
mimimum_fright < tht_f_l_r  
)
{
case_ = 2;//turn left
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = -0.3 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft >= tht_f_l_r && 
mimimum_front < tht_f && 
mimimum_fright >= tht_f_l_r  
)
{
case_ = 3;//turn right
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = 0.3 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft >= tht_f_l_r && 
mimimum_front < tht_f && 
mimimum_fright < tht_f_l_r  
)
{
case_ = 4;//turn left 
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = -0.3 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft < tht_f_l_r && 
mimimum_front >= tht_f && 
mimimum_fright >= tht_f_l_r  
)
{
case_ = 5;//turn right
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = 0.3 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft < tht_f_l_r && 
mimimum_front >= tht_f && 
mimimum_fright < tht_f_l_r  
)
{
case_ = 6;//keep moving
my_vel.linear.x = 0.6 *speed_factor;
my_vel.angular.z = 0 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft < tht_f_l_r && 
mimimum_front < tht_f && 
mimimum_fright >= tht_f_l_r  
)
{
case_ = 7;//turn right
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = 0.3 * foctor_rot * speed_factor;

}
else if(
mimimum_fleft < tht_f_l_r && 
mimimum_front < tht_f && 
mimimum_fright < tht_f_l_r
)
{
case_ = 8;//turn right
my_vel.linear.x = 0*speed_factor;
my_vel.angular.z = 0.3 * foctor_rot * speed_factor;

}
else printf("exception l:%f f: %f r:%f",mimimum_fleft,mimimum_front,mimimum_fright);
printf("run case: %d \n",case_);

pub.publish(my_vel);

}

bool speed_control_callback(ass_2::speed_control::Request &req,ass_2::speed_control::Response &res)
{
if(req.cmmd=='w')
{
speed_factor += speed_factor*0.1;
res.current_speed_factor = speed_factor;
}
else if (req.cmmd=='s')
{
speed_factor -= speed_factor*0.1;
res.current_speed_factor = speed_factor;
}

return 1;

}

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS
//system
ros::init(argc, argv, "robot_controller");//ros::init()

ros::NodeHandle nh; //ros::NodeHandle nh
// Define the subscriber to laser scanner
ros::Subscriber sub = nh.subscribe("/base_scan", 1,scanner_callback);//"hello" is the name of the turtle
pub = nh.advertise<geometry_msgs::Twist> ("/cmd_vel", 1);//for publishing speed
ros::ServiceServer service = nh.advertiseService("/speed_control",speed_control_callback);//speed control service 
ros::spin();
return 0;
}


