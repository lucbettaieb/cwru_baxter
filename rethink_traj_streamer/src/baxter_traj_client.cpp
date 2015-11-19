/*
 * baxter_traj_client
 *
 * Copyright (c) 2015, Luc Bettaieb
 * 'joint_trajectory_client.py' (c) 2013-2015, Rethink Robotics
 *
 * BSD Licensed
 */

#include "rethink_traj_streamer/baxter_traj_client.h"

#include <string>
#include <vector>

ros::Subscriber g_LeftJointListener;
double leftJointAngles [7];

BaxterTrajectory::BaxterTrajectory(ros::NodeHandle nh, std::string limb) :
action_client("robot/limb/" + limb + "/follow_joint_trajectory", true)
{
  nh_ = nh;
  goal.goal_time_tolerance = ros::Duration(0.1);

  // if (!action_client.waitForResult(ros::Duration(5.0)))
  // {
  //   ROS_ERROR("Could not contact action server");
  //   ros::shutdown();
  // }

  BaxterTrajectory::clear(limb);

  g_LeftJointListener = nh_.subscribe("/robot/joint_states", 3, &BaxterTrajectory::updateLeftJointAngles, this);
}

BaxterTrajectory::~BaxterTrajectory()
{
  // delete action_client;
}

void BaxterTrajectory::updateLeftJointAngles(const sensor_msgs::JointState& jointstate)
{
  ros::spinOnce();
  for (uint i = 0; i < 7; i++)
  {
    leftJointAngles[i] = jointstate.position.at(i+2);
    //std::cout << leftJointAngles[i] << std::endl;
  }
}

void BaxterTrajectory::addPoint(std::vector<float> positions, float time)
{
  trajectory_msgs::JointTrajectoryPoint point;

  for (uint i = 0; i < positions.size(); i++)
    point.positions.push_back(positions[i]);

  point.time_from_start = ros::Duration(time);
  goal.trajectory.points.push_back(point);
}

void BaxterTrajectory::start()
{
  goal.trajectory.header.stamp = ros::Time::now();
  ROS_INFO("Sending goal");
  action_client.sendGoal(goal);  // TODO(lucbettaieb): Maybe add in a callback
}

void BaxterTrajectory::stop()
{
  action_client.cancelGoal();
}

void BaxterTrajectory::wait(float timeout = 15.0)
{
  action_client.waitForResult(ros::Duration(timeout));
}

void BaxterTrajectory::clear(std::string limb)
{
  goal.goal_time_tolerance = ros::Duration(0.1);

  goal.trajectory.joint_names.push_back(limb + "_s0");
  goal.trajectory.joint_names.push_back(limb + "_s1");
  goal.trajectory.joint_names.push_back(limb + "_e0");
  goal.trajectory.joint_names.push_back(limb + "_e1");
  goal.trajectory.joint_names.push_back(limb + "_w0");
  goal.trajectory.joint_names.push_back(limb + "_w1");
  goal.trajectory.joint_names.push_back(limb + "_w2");
}

std::vector<float> BaxterTrajectory::getVecFromJointPos()
{
  std::vector<float> v;

  for (uint i = 0; i < 7; i++)
    v.push_back(leftJointAngles[i]);

  return v;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "baxter_traj_client");
  ros::NodeHandle nh;

  BaxterTrajectory b(nh, "left");

  std::vector<float> pose_vec;
  pose_vec.push_back(.2);
  pose_vec.push_back(1.11);
  pose_vec.push_back(-0.446);
  pose_vec.push_back(0.735);
  pose_vec.push_back(-1.00);
  pose_vec.push_back(2.09);
  pose_vec.push_back(0.0);

  ROS_INFO("???");
  std::vector<float> init_vec;
  init_vec.push_back(-0.1280);
  init_vec.push_back(1.62870);
  init_vec.push_back(-0.5165);
  init_vec.push_back(0.01380);
  init_vec.push_back(-2.862);
  init_vec.push_back(0.1257);
  init_vec.push_back(-2.9601);
  
  b.addPoint(b.getVecFromJointPos(), 0.0);
  //b.addPoint(init_vec, 1.0);
  b.addPoint(pose_vec, 7.0);

  b.start();
  b.wait(15.0);

  
}
