// This node: check a predetermined-size box in front of the robot and publish whether if there is any obsticle in it.
#include <ros/ros.h> //Must include this for all ROS cpp projects
#include <sensor_msgs/LaserScan.h>
#include <stdr_wall_following_controller/ExampleServiceMsg.h>
#include <std_msgs/Float32.h> //Including the Float32 class from std_msgs
#include <std_msgs/Bool.h> // boolean message 
#include <geometry_msgs/Twist.h> //twist control
#include <vector>
#include <cmath>

double sample_dt = 0.01;        //specify a sample period of 10ms  
bool leftObstacle = false;
bool frontObstacle = false;
int state = 0;
double speed = 0.5;

ros::Subscriber lidar_alarm_front_subscriber_;
ros::Subscriber lidar_alarm_left_subscriber_;

ros::Publisher stdr_cmd_vel_publisher_;

void frontCallback (const std_msgs::Bool& check) {
    frontObstacle = (bool) check.data;
}

void leftCallback (const std_msgs::Bool& check) {
    leftObstacle = (bool) check.data;
}

void stateCheck () {
    
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "stdr_wall_following_controller"); //name this node
    ros::NodeHandle nh;

    ros::Publisher twist_commander = nh.advertise<geometry_msgs::Twist>("/robot0/cmd_vel", 1);

    // Create a Subscriber object and have it subscribe to the lidar topic
    ros::Subscriber lidar_alarm_front_subscriber_ = nh.subscribe("robot0/lidar_alarm_front", 1, frontCallback);
    ros::Subscriber lidar_alarm_left_subscriber_ = nh.subscribe("robot0/lidar_alarm_left", 1, leftCallback);

    ros::ServiceClient rotate_client_ = nh.serviceClient<stdr_wall_following_controller::ExampleServiceMsg>("heading_controller_service");
    stdr_wall_following_controller::ExampleServiceMsg angle_msg_;
    angle_msg_.request.desired_heading = M_PI/2;

    geometry_msgs::Twist twist_cmd; 
    twist_cmd.linear.x=0.0;
    twist_cmd.linear.y=0.0;    
    twist_cmd.linear.z=0.0;
    twist_cmd.angular.x=0.0;
    twist_cmd.angular.y=0.0;
    twist_cmd.angular.z=0.0;

    ros::Rate loop_timer(1/sample_dt); 
    ros::spinOnce();

    rotate_client_.call(angle_msg_);

    while(ros::ok){
        ROS_INFO("current state = %i",state);
        ROS_INFO("left obstacle?: %f", (double) leftObstacle);
        ROS_INFO("front obstacle?: %f", (double) frontObstacle);
        ros::spinOnce();
        switch (state) {
            case 0:
                ROS_INFO("Case 0 is running");
                twist_cmd.linear.x=speed;
                for (int i = 1;i>10;i++) ros::spinOnce();
                if (leftObstacle == false){
                    ros::Rate lazy(1/0.4);
                    lazy.sleep();
                    twist_cmd.linear.x=0.0;
                    for (int i = 0; i < 10; i++){
                        twist_commander.publish(twist_cmd);
                    }
                    state = 2;
                    break;
                }
                if (frontObstacle == true){
                    twist_cmd.linear.x=0.0;
                    for (int i = 0; i < 10; i++){
                        twist_commander.publish(twist_cmd);
                    }
                    state = 1;
                    break;
                }
                twist_commander.publish(twist_cmd);
                break;           
            case 1:
                // rotate and check if front is block
                rotate_client_.call(angle_msg_);
                ros::spinOnce();
                if (frontObstacle == false){
                    state = 0;
                }
                break;
            
            case 2:
                rotate_client_.call(angle_msg_);
                for (int i = 1;i>10;i++) ros::spinOnce();
                if (leftObstacle == false) state = 3;
                else state = 1;
                break;
            case 3:
                twist_cmd.linear.x=speed;
                
                while (!leftObstacle){
                    if (frontObstacle) {
                        twist_cmd.linear.x=0;
                        for (int i = 0; i < 10; i++){
                            twist_commander.publish(twist_cmd);
                        }
                        state = 0;
                        break;
                    }
                    twist_commander.publish(twist_cmd);
                    ros::spinOnce();
                    ROS_INFO("stuck in case 2");
                }
                state = 0;
                break;
        }
        loop_timer.sleep();
    }

    //ros::spin();    // this is essentially a "while(1)" statement, except it
                    // forces refreshing wakeups upon new data arrival
                    // main program essentially hangs here, but it must stay alive to keep the callback function alive
    return 0; // should never get here, unless roscore dies
}