#!/bin/bash

DONT_RUN=1 make px4_sitl_nolockstep gazebo_sarax

# Check the rosversion and the path!!
source /opt/ros/noetic/setup.bash

# Check the path, this should source the workspace that have mavros in it!!
source $(pwd)/../../devel/setup.bash

source Tools/setup_gazebo.bash $(pwd) $(pwd)/build/px4_sitl_nolockstep
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$(pwd)
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:$(pwd)/Tools/sitl_gazebo

roslaunch px4 ns_sarax_mavros_posix_sitl.launch
