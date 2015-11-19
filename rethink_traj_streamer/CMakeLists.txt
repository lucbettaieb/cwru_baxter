cmake_minimum_required(VERSION 2.8.3)
project(rethink_traj_streamer)

find_package(catkin REQUIRED)
find_package(actionlib REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  trajectory_msgs
  control_msgs
)

catkin_package(
  INCLUDE_DIRS include
  CATKIN_DEPENDS roscpp trajectory_msgs control_msgs
)

include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${PROJECT_NAME}/include
)

## Declare a C++ library
# add_library(rethink_traj_streamer
#   src/${PROJECT_NAME}/rethink_traj_streamer.cpp
# )


## Declare a C++ executable
add_executable(baxter_traj_client src/baxter_traj_client.cpp)
target_link_libraries(baxter_traj_client
  ${catkin_LIBRARIES}
)