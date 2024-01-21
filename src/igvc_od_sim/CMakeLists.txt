cmake_minimum_required(VERSION 3.0.2)
project(igvc_od_sim)

find_package(catkin REQUIRED COMPONENTS
  joy
  rospy
  std_msgs
)

catkin_package()

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

