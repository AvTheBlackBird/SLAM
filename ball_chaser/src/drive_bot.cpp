#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

bool handle_drive_request (ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
    geometry_msgs::Twist motor_command;
    
    // Set wheel velocities, forward [0.5, 0.0]
    motor_command.linear.x = req.linear_x;
    motor_command.angular.z = req.angular_z;
   
    // Publish angles to drive the robot
    motor_command_publisher.publish(motor_command);

    res.msg_feedback = "Linear_x: " + std::to_string(req.linear_x) + ", Angular_z: " + std::to_string(req.angular_z);
    
    return true;
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "drive_bot");

    ros::NodeHandle n;

    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceServer drive_server = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    
    ros::spin();

    return 0;
}