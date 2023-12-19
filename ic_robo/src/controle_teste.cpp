#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int kbhit(void)  //captura de teclado sem travamento//
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}


using namespace std;
int main(int argc, char **argv){
	ros::init(argc, argv, "controle_robo_teste");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cont", 1000);
		
	ros::Rate loop_rate(10);
	if (ros::ok()){
		geometry_msgs::Twist msg;
		float vel_lin=2;
		ros::spinOnce();
		
		while(1){
msg.linear.x = 0;
                        pub.publish(msg);
                        sleep(1);
                        if(kbhit()){
                                break;  
                        }

			if(kbhit()){
				break;	
			}
			msg.linear.z = 3;
			ROS_INFO("Trava 1 aclopada");
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			msg.linear.z = 0;
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			
			msg.linear.x = vel_lin;
			pub.publish(msg);
			sleep(3);
			if(kbhit()){
				break;	
			}
			
			msg.linear.x = 0;
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			
			msg.linear.z = 1;
			ROS_INFO("Trava 1 desaclopada");
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			msg.linear.z = 0;
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			
			msg.linear.z = 3;
			ROS_INFO("Trava 1 aclopada");
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			msg.linear.z = 0;
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			
			msg.linear.x = -vel_lin;
			pub.publish(msg);
			sleep(3);
			if(kbhit()){
				break;	
			}
msg.linear.x = 0;
                        pub.publish(msg);
                        sleep(1);
                        if(kbhit()){
                                break;  
                        }

			
			msg.linear.z = 1;
			ROS_INFO("Trava 1 desaclopada");
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			msg.linear.z = 0;
			pub.publish(msg);
			sleep(1);
			if(kbhit()){
				break;	
			}
			
		}
		ROS_WARN("Trajeto finalizado...");	
	}
	return 0;
}
