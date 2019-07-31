cmake_minimum_required(VERSION 2.8.3)
project(sc_localization)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  tf
  apriltag_ros
  roborts_base
)


catkin_package()


include_directories( include ${catkin_INCLUDE_DIRS})


add_executable(sc_localization odom_update_node.cpp)
target_link_libraries(sc_localization ${catkin_LIBRARIES})

