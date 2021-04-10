#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
//TODO: Include the ball_chaser "DriveToTarget" header file
//eriks code:
#include "ball_chaser/DriveToTarget.h" //there is no header file yet and do i need to put something before the header file name like the other include lines above
// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
//erik's code:
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res){
  // Create a motor_command object of type geometry_msgs::Twist
        geometry_msgs::Twist cmd_vel;
        // Set wheel velocities 
        //cmd_vel.linear.x = 0.5;
	cmd_vel.linear.x = req.linear_x;
	//req.linear_x;
        //cmd_vel.angular.z = 0.0;
	cmd_vel.angular.z = req.angular_z;
	//req.angular_z;
        // Publish angles to drive the robot
        motor_command_publisher.publish(cmd_vel);
        //return response message
	res.msg_feedback = "Linear velocity x set to: " + std::to_string(cmd_vel.linear.x) + "Angular velocity z set to: " + std::to_string(cmd_vel.angular.z); 
	ROS_INFO_STREAM(res.msg_feedback);
        //res.msg_feedback = "Angular velocity z set to: " + cmd_vel.angular.z + ROS_INFO_STREAM(res.msg_feedback);

        return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    // TODO: Define a drive /ball_chaser/command_robot service with a handle_drive_request callback function
    //eriks code:
    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send drive commands");
    // TODO: Delete the loop, move the code to the inside of the callback function and make the necessary changes to publish the requested velocities instead of constant values 
    //erik - deleted and moved

    // TODO: Handle ROS communication events
    ros::spin();

    return 0;
}
