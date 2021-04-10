#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;
  //client call
	if (!client.call(srv)){
    ROS_ERROR("Failed to call service ");
  }
    
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{
	//work in progress 

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
	//tip for function design: Brolin answer: https://knowledge.udacity.com/questions/502203
	int size = img.step * img.height;
  bool rotate_right = false;
  bool rotate_left = false;
  bool move_forward = false;
  int pixel_loc;
  int left_center = img.step * 1/3;
  int right_center = img.step * 2/3;
  /*
  ROS_INFO("image step");
  ROS_INFO("%i", img.step);
  ROS_INFO("image height");
  ROS_INFO("%i", img.height);
  ROS_INFO("image width");
  ROS_INFO("%i", img.width);
  ROS_INFO("image data 1, 2, 3, 4, 5, 6");
  ROS_INFO("%i", img.data[1]);
  ROS_INFO("%i", img.data[2]);
  ROS_INFO("%i", img.data[3]);
  ROS_INFO("%i", img.data[4]);
  ROS_INFO("%i", img.data[5]);
  ROS_INFO("%i", img.data[6]);
  */

	for (int i = 0; i < size; i+=3){
		if (img.data[i] == 255 && img.data[i+1] == 255 && img.data[i+2] == 255){ //detect all RGB values at 255, consider white
      //ROS_INFO("white pixel detected");
      pixel_loc = (i % img.step);
      
			if(pixel_loc <= left_center){
        rotate_left = true;
        rotate_right = false;
        move_forward = false;
				ROS_INFO("setting rotating left");
        ROS_INFO("pixel_loc:");
        ROS_INFO("%i", pixel_loc);
        //ROS_INFO("img.step:");
        //ROS_INFO("%i", img.step);
        //ROS_INFO("img.step*(1/3):");
        //ROS_INFO("%i", ((img.step)*(1/3)));
				break;	
			}
			else if(pixel_loc >= right_center){
        rotate_left = false;
        rotate_right = true;
        move_forward = false;
				ROS_INFO("setting rotating right");
        ROS_INFO("pixel_loc:");
        ROS_INFO("%i", pixel_loc);
        //ROS_INFO("img.step:");
        //ROS_INFO("%i", img.step);
        //ROS_INFO("img.step*(2/3):");
        //ROS_INFO("%i", ((img.step)*(2/3)));
				break;
			}
			else{
        rotate_left = false;
        rotate_right = false;
        move_forward = true;
				ROS_INFO("setting moving forward");
        ROS_INFO("pixel_loc:");
        ROS_INFO("%i", pixel_loc);
        ROS_INFO("img.step:");
        ROS_INFO("%i", img.step);
				break;
			}
		}		
	}
  if(rotate_right == true){
    drive_robot(0.0, -0.1); //rotate right
    //ROS_INFO("commanding rotate right");
    
  }
  else if(rotate_left == true){
    drive_robot(0.0, 0.1); //rotate left
    //ROS_INFO("commanding rotate left");
    
  }
  else if(move_forward == true){ 
    drive_robot(0.1, 0.0); //move forward
    //ROS_INFO("commanding move forward");
    
  }
  else{
    drive_robot(0.0, 0.0); //hold steady
    
  }
}


int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
