#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#define leituras 1146  //número total de leituras do sensor
#define range_max 12	//alcance maximo do sensor
#define abertura_graus 10	//abertura em graus 0-->180

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float abertura_min = (abertura_graus*M_PI)/180;
	float sum=0,dif=0,ang=0;
	float dist[leituras];
	int count=0;
	
	std::vector<float> valores;

	for (int i=0; i<leituras; i++){
		ang = msg->angle_min + i*msg->angle_increment;
		if(abs(ang)<=abertura_min){
		std::cout << "Angulo: " << ang << "\n";
			if(msg->ranges[i]<=range_max){
				dist[count]= (msg->ranges[i]*cos(ang));
				dist[count]= std::round(dist[count]*100)/100;
				valores.push_back(dist[count]);
			}else{
				dist[count]=(range_max*cos(ang));
				dist[count]= std::round(dist[count]*100)/100;
				valores.push_back(dist[count]);
			}
			count++;
		}
	}
	std::cout << "Contagem: " << count << "\n";
	
	for (int i=0; i < count-1; i++){
			
		dif = abs(dist[i] - dist[i+1]);
		sum += 1000.0*dif;
		
	}
	
	sum = sum/count;
	sum= std::round(sum*100)/100;
	valores.push_back(sum);
	std::stringstream ss;
	    for (int i = 0; i < valores.size(); i++) {
		ss << valores[i];
		if (i != valores.size() - 1) {
		    ss << ", ";
		} else {
		    ss << "\n";
		}
	    }
	    
    std::cout << ss.str();
	
}

int main(int argc, char **argv){
ros::init(argc, argv, "vison_robo");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/scan", 1, subCallback);
	ros::spin();
	ros::Rate loop_rate(1);
	return 0;
	}
