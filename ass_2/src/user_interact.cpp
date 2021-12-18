#include "ros/ros.h"
#include "ass_2/speed_control.h"
#include "std_srvs/Empty.h"

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS
//system
ros::init(argc, argv, "ass_2_user_interact");//ros::init()

//call spaw
ros::NodeHandle nh;
char tem = 0;

ros::ServiceClient client_reset_position = nh.serviceClient<std_srvs::Empty>("/reset_positions");//this is to reset the position
ros::ServiceClient client_speed_control = nh.serviceClient<ass_2::speed_control>("/speed_control");//this is to reset the position

while(1)
{
scanf("%c",&tem);
fflush(stdin);
switch(tem)
{
case 'w' : {

ass_2::speed_control sc;
sc.request.cmmd = 'w';

client_speed_control.waitForExistence();
client_speed_control.call(sc);


};break;
case 's' : {

ass_2::speed_control sc;
sc.request.cmmd = 's';

client_speed_control.waitForExistence();
client_speed_control.call(sc);


};break;
case 'r' : {
std_srvs::Empty rp;

client_reset_position.waitForExistence();
client_reset_position.call(rp);



};break;
default : break;
}


}


//ros::Subscriber rssb = rnh.subscribe("/turtle1/pose");
ros::spin();
return 0;
}
