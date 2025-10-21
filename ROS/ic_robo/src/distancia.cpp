#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#define leituras 1146 
#define range_max 12
#define abertura_graus 180    //abertura em graus

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float abertura = (abertura_graus*M_PI)/180;
	float D = 99, Dc = 99, E = 99, Ec = 99,ang=0;
	float rang[leituras*2]={0};

	for (int i=0; i<leituras; i++){
		rang[i]=msg->ranges[i];
		if(rang[i]>12){
			rang[i]=12;		
		}
		
		ang = msg->angle_min + i*msg->angle_increment;
		
		if(abs(ang)<=abertura/2){
			
			if(ang>=-abertura/2 & ang<-abertura/4 & rang[i]<D){
				D=rang[i];
			}
			if(ang>=-abertura/4 & ang<0 & rang[i]<Dc){
				Dc=rang[i];
			}
			if(ang>=0 & ang<abertura/4 & rang[i]<Ec){
				Ec=rang[i];
			}
			if(ang>=abertura/4 & ang<abertura/2 & rang[i]<E){
				E=rang[i];
			}
		
		}
		
	}	
printf("D : %f\nDc : %f\nEc : %f\nE : %f\n", D, Dc, Ec, E);
}
	
int main(int argc, char **argv){
ros::init(argc, argv, "dist_robo");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/scan", 1, subCallback);
	ros::spin();
	ros::Rate loop_rate(1);
	return 0;
	}
