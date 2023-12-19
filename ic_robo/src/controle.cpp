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
	ros::init(argc, argv, "controle_robo");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cont", 1000);
	
	
	
	ros::Rate loop_rate(10);
	if (ros::ok()){
		geometry_msgs::Twist msg;
		float vel_lin=2;
		ros::spinOnce();
		char comando='0';
		bool x = 0;
		while(comando!='p'){
			if(kbhit()){
				comando = getchar();
				switch(comando){
					case 'w':
					msg.linear.x = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '1':
					if(x){
					msg.linear.z = 1;
					ROS_INFO("Trava 1 aclopada");
					pub.publish(msg);
					sleep(1);
					msg.linear.z = 0;
					pub.publish(msg);
					ros::spinOnce();
					x=!x;
					}
					else{
					ROS_INFO("Trava 1 ja esta aclopada");
					}
					break;
					case '2':
					msg.linear.z = 2;
					ROS_INFO("Trava 2 aclopada");
					pub.publish(msg);
					sleep(1);
					msg.linear.z = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '3':
					if(!x){
					msg.linear.z = 3;
					ROS_INFO("Trava 1 desaclopada");
					pub.publish(msg);
					sleep(1);
					msg.linear.z = 0;
					pub.publish(msg);
					ros::spinOnce();
					x=!x;
					}
					else{
					ROS_INFO("Trava 1 ja esta desaclopada");
					}
					break;
					case '4':
					msg.linear.z = 4;
					ROS_INFO("Trava 2 desaclopada");
					pub.publish(msg);
					sleep(1);
					msg.linear.z = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case 'e':
					msg.linear.x = vel_lin;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case 'q':
					msg.linear.x = -vel_lin;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '5':
					msg.linear.y = 1;
					ROS_INFO("Rotacao traseira");
					pub.publish(msg);
					sleep(1);
					msg.linear.y = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '6':
					msg.linear.y = 2;
					ROS_INFO("Rotacao dianteira");
					pub.publish(msg);
					sleep(1);
					msg.linear.y = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '7':
					msg.linear.y = 3;
					ROS_INFO("Volta traseira");
					pub.publish(msg);
					sleep(1);
					msg.linear.y = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;
					case '8':
					msg.linear.y = 4;
					ROS_INFO("Volta dianteira");
					pub.publish(msg);
					sleep(1);
					msg.linear.y = 0;
					pub.publish(msg);
					ros::spinOnce();
					break;	
					case '9':
					vel_lin+=0.5;
					ROS_INFO("Velocidade atual: %f", vel_lin);
					ros::spinOnce();
					break;	
					case '0':
					vel_lin-=0.5;
					ROS_INFO("Velocidade atual: %f", vel_lin);
					ros::spinOnce();
					break;			
					default :
					
					break;
										
				}
			}
		}
		ROS_WARN("Trajeto finalizado...");	
	}
	return 0;
}
