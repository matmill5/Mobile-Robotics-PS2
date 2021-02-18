# Mobile-Robotics-PS2
In order to catkin_make this code, please use catkin_simple.

## Usage
In order to run the simulation,
`roslaunch stdr_launchers server_with_map_and_gui_plus_robot.launch`

To run the lidar obstacle-sensing box:
1. Front:
`rosrun stdr_wall_following_controller lidar_alarm_front `
2. Left:
`rosrun stdr_wall_following_controller lidar_alarm_left`

Then, run the service:
`rosrun stdr_wall_following_controller example_ros_service`

Finally,
`rosrun stdr_wall_following_controller stdr_wall_following_controller`

