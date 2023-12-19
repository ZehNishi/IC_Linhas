#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#define leituras 400  //número total de leituras do sensor

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float sum=0,dif=0,ang=0;
	float dist[leituras];

	for (int i=0; i<leituras; i++){
		ang = msg->angle_min + i*msg->angle_increment;
		dist[i]= (msg->ranges[i]*cos(ang));
		printf("%f\n",dist[i]);
	}
	
	for (int i=0; i < leituras-1; i++){
			
		dif = abs(dist[i] - dist[i+1]); 
		sum += 1000*dif;
		
	}
	
	sum = sum/leituras;
	
	printf("Diferença de altura: [%f]\n", sum);
	
//	if(sum<15){
//		printf("Chao\n");
//	}else{
//		if(sum<100){
//			printf("Carro\n");
//			}else{
//				if(sum<350){
//					printf("Arvore\n");
//					}else{
//						printf("Desconhecido\n");
//					}
//		}
//	}	
}

int main(int argc, char **argv){
ros::init(argc, argv, "vison_robo");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/vison", 1000, subCallback);
	ros::spin();
	return 0;
	}
