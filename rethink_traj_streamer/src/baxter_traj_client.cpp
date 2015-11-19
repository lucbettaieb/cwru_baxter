/*
 * baxter_traj_client
 *
 * Copyright (c) 2015, Luc Bettaieb
 *
 * Based off of 'joint_trajectory_client.py' (c) 2013-2015, Rethink Robotics
 *
 * BSD Licensed
 */

#include "rethink_traj_streamer/baxter_traj_client.h"

#include <string>
#include <vector>

BaxterTrajectory::BaxterTrajectory(std::string limb)
{
  action_client = new actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction>("baxter_traj_as", true);

  goal.goal_time_tolerance = ros::Duration(0.1);

  if (!action_client.waitForResult(ros::Duration(5.0)))
  {
    ROS_ERROR("Could not contact action server");
    ros::shutdown();
  }

  BaxterTrajectory::clear(limb);
}

BaxterTrajectory::~BaxterTrajectory()
{
  delete action_client;
}

BaxterTrajectory::add_point(std::vector<float> positions, float time)
{
  trajectory_msgs::JointTrajectoryPoint point;

  for (uint i = 0; i < positions.size(); i++)
    point.positions.push_back(positions[i]);

  point.time_from_start(ros::Duration(time));
  goal.trajectory.points.push_back(point);
}

BaxterTrajectory::start()
{
}

BaxterTrajectory::stop()
{
}

BaxterTrajectory::wait(float timeout)
{
}

BaxterTrajectory::clear(std::string limb)
{
  goal.goal_time_tolerance = ros::Duration(0.1);

  goal.trajectory_joint_names.push_back(limb + "_s0");
  goal.trajectory_joint_names.push_back(limb + "_s1");
  goal.trajectory_joint_names.push_back(limb + "_e0");
  goal.trajectory_joint_names.push_back(limb + "_e1");
  goal.trajectory_joint_names.push_back(limb + "_w0");
  goal.trajectory_joint_names.push_back(limb + "_w1");
  goal.trajectory_joint_names.push_back(limb + "_w2");
}
