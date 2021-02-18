cmake_minimum_required(VERSION 2.8.3)
project(stdr_wall_following_controller)

find_package(catkin_simple REQUIRED)

catkin_simple()

# Executables
cs_add_executable(example_ros_service src/example_ros_service.cpp)
cs_add_executable(example_ros_client src/example_ros_client.cpp)
cs_add_executable(lidar_alarm_front src/lidar_alarm_front.cpp)
cs_add_executable(lidar_alarm_left src/lidar_alarm_left.cpp)
cs_add_executable(stdr_wall_following_controller src/stdr_wall_following_controller.cpp)

cs_install()
cs_export()
    
