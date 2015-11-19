// Test client 
// Luc Bettaieb

#include <ros/ros.h>

#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>

#include <control_msgs/FollowJointTrajectoryAction.h>
#include <control_msgs/FollowJointTrajectoryGoal.h>


// This function will be called once when the goal completes
// this is optional, but it is a convenient way to get access to the "result" message sent by the server

int main(int argc, char** argv) {
        ros::init(argc, argv, "traj_action_client_node"); // name this node 
        int g_count = 0;
        // here is a "goal" object compatible with the server, as defined in example_action_server/action
        control_msgs::FollowJointTrajectoryGoal goal;
        
        // use the name of our server, which is: example_action (named in example_action_server.cpp)
        actionlib::SimpleActionClient<control_msgs::FollowJointTrajectoryAction> action_client("/robot/limb/left/follow_joint_trajectory", true);
        
        // attempt to connect to the server:
        ROS_INFO("waiting for server: ");
        bool server_exists = action_client.waitForServer(ros::Duration(5.0)); // wait for up to 5 seconds
        // something odd in above: does not seem to wait for 5 seconds, but returns rapidly if server not running


        if (!server_exists) {
            ROS_WARN("could not connect to server; will wait forever");
            return 0; // bail out; optionally, could print a warning message and retry
        }
        server_exists = action_client.waitForServer(); //wait forever
        
       
        ROS_INFO("connected to action server");  // if here, then we connected to the server;

        while(true) {
        // stuff a goal message:
        g_count++;
        goal.traj_id = g_count; // this merely sequentially numbers the goals sent
        ROS_INFO("sending traj_id %d",g_count);
        //action_client.sendGoal(goal); // simple example--send goal, but do not specify callbacks
        action_client.sendGoal(goal); // we could also name additional callback functions here, if desired
        //    action_client.sendGoal(goal, &doneCb, &activeCb, &feedbackCb); //e.g., like this
        
        bool finished_before_timeout = action_client.waitForResult(ros::Duration(5.0));
        //bool finished_before_timeout = action_client.waitForResult(); // wait forever...
        if (!finished_before_timeout) {
            ROS_WARN("giving up waiting on result for goal number %d",g_count);
            return 0;
        }
        else {
            ROS_INFO("finished before timeout");
        }
        
        }

    return 0;
}
