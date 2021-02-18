//example ROS client:
// first run: rosrun stdr_wall_following_controller stdr_wall_following_controller
// then start this node:  rosrun stdr_wall_following_controller heading_ros_client



#include <ros/ros.h>
#include <stdr_wall_following_controller/ExampleServiceMsg.h> // this message type is defined in the current package
#include <iostream>
#include <string>
using namespace std;

int main(int argc, char **argv) {
    ros::init(argc, argv, "example_ros_client");
    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<stdr_wall_following_controller::ExampleServiceMsg>("heading_controller_service");
    stdr_wall_following_controller::ExampleServiceMsg srv;
    double desired_heading;
    while (ros::ok()) {
        cout << "Enter a desired heading: ";
    	cin  >> desired_heading;
    	srv.request.desired_heading = desired_heading;
   	client.call(srv);
    }
 
    return 0;
}
