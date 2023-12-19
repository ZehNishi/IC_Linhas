#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#define leituras 1146  //número total de leituras do sensor
#define range_max 12	//alcance maximo do sensor
#define abertura_graus_min 5	//abertura em graus 0-->180
#define abertura_graus_max -5	//abertura em graus 0-->180
#define abertura_ignorada_min 0	//ângulo para não ser lido em graus 0-->180
#define abertura_ignorada_max 0	//ângulo para não ser lido em graus 0-->180	

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float abertura_min = (abertura_graus_min*M_PI)/180;
	float abertura_max = (abertura_graus_max*M_PI)/180;
	float abertura_ig_min = (abertura_ignorada_min*M_PI)/180;
	float abertura_ig_max = (abertura_ignorada_max*M_PI)/180;
	float sum=0,dif=0,ang=0;
	float dist[leituras];
	int count=0;

	for (int i=0; i<leituras; i++){
		ang = msg->angle_min + i*msg->angle_increment;
		if((ang<=abertura_min && ang>=abertura_max) && (ang>=abertura_ig_min || ang<=abertura_ig_max)){
			if(msg->ranges[i]<=range_max){
				dist[count]= (msg->ranges[i]*cos(ang));
			}else{
				dist[count]=(range_max*cos(ang));
			}
			printf("ang:%f %f\n",ang,dist[count]);
			count++;
		}
	}
	
	for (int i=0; i < count-1; i++){
		
		//printf("%f\n",sum);		
		dif = abs(dist[i] - dist[i+1]);
		//printf("%f\n",dif); 
		sum += 1000*dif;
		
	}
	
	sum = sum/count;
	
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
ros::init(argc, argv, "vison_robo2");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/scan", 1, subCallback);
	ros::spin();
	ros::Rate loop_rate(1);
	return 0;
	}
