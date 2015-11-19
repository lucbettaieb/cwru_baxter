/*
 * baxter_traj_client
 *
 * Copyright (c) 2015, Luc Bettaieb
 * BSD Licensed
 */

#ifndef BAXTER_TRAJ_CLIENT_H
#define BAXTER_TRAJ_CLIENT_H

#include <ros/ros.h>

#include <trajectory_msgs/JointTrajectoryPoint.h>
#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/FollowJointTrajectoryGoal.h>

#include <actionlib/client/simple_action_client.h>

#include <string>
#include <vector>

class BaxterTrajectory
{
public:
  explicit BaxterTrajectory(std::string limb);
  virtual ~BaxterTrajectory();

private:
  actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> action_client;
  control_msgs::FollowJointTrajectoryGoal goal;

public:
  void addPoint(std::vector<float> positions, float time);
  void start();
  void stop();
  void wait(float timeout);
  void clear(std::string limb);
};


#endif  // BAXTER_TRAJ_CLIENT_H
