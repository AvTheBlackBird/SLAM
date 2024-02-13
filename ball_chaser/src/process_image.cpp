#include "ros/ros.h"
#include <ros/console.h>
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "geometry_msgs/Twist.h"

ros::ServiceClient client;

void drive_robot(float lin_x, float angular_z)
{
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = angular_z;
    if (!client.call(srv))
    {
        ROS_ERROR("Failed to move the robot");
    }
}

void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel = 230;
    int right=0;
    int left=0;
    bool is_white=false;
    uint32_t integer_step = img.step;
    uint32_t img_width = img.width;
    uint32_t img_height = img.height;

    for (int j=1; j<img_height; j++)
    {
        for (int i=1; i<integer_step/3; i++)
        {
            if ((img.data[i*j*3] == white_pixel) && (img.data[i*j*3 + 1] == white_pixel) && (img.data[i*j*3 + 2] == white_pixel))
            {   
                is_white = true;

                if (int(((i*j)%img_width)/(img_width/2)))
                {
                    right++;
                }
                else
                {
                    left++;
                }
            }
        }
    }

    if (is_white)
    {
        if (right>left)
        {
            std::cout<<"Turning Right"<<"\n";
            
            drive_robot(5.0, -10.5);
        }
        else if (left>right)    
        {
            std::cout<<"Turning Left"<<"\n";

            drive_robot(5.0, 10.5);   
        }
    }
    else
    {
        drive_robot(0.0,0.0);
    }
    
    
}
int main(int argc, char **argv)
{
    ros::init(argc, argv, "process_image");

    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber process_image_sub = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}