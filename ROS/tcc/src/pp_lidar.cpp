#include "sensor_msgs/LaserScan.h"
#include "std_msgs/Float32MultiArray.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>

#define leituras 63  //número total de leituras do sensor
//#define leituras 1146  //número total de leituras do sensor
#define range_max 12	//alcance maximo do sensor
#define abertura_graus 10  //abertura em graus 0-->180

ros::Publisher pub;

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float abertura_min = (abertura_graus*M_PI)/180;
	float dif=0,ang=0;
	float dist[leituras];
	int aprox[leituras];
	int count=0;
	std::vector<float> valores;
	

	for (int i=0; i<leituras; i++){
		ang = msg->angle_min + i*msg->angle_increment;
		if(abs(ang)<=abertura_min){
			if(msg->ranges[i]<=range_max){
				dist[count] = msg->ranges[i];
				valores.push_back(dist[count]);
				aprox[count] = (int)roundf(dist[count]*10);
				
			}else{
				dist[count] = range_max;
				valores.push_back(dist[count]);
				aprox[count] = range_max*5;
			}
			std::cout << dist[count] << ",";
			count++;
		}
	}
	std::cout << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	char matriz[range_max*3][63]; // 12 linhas x 63 colunas
	for (int i = 0; i < range_max*3; i++) {
		for (int j = 0; j < 63; j++) {
		    matriz[i][j] = ' '; // Inicializar com espaço em branco
		}
	}

	// Preencher a matriz com asteriscos
	for (int j = 0; j < 63; j++) {
		int valor = aprox[j];
		for (int i = 0; i < valor && i < range_max*3; i++) {
		    matriz[range_max*3 - 1 - i][j] = '*'; // Preencher com asterisco até o valor
		}
	}

	// Imprimir a matriz
	std::cout << "Leitura do lidar:" << std::endl;
	for (int i = 0; i < range_max*3; i++) {
		for (int j = 0; j < 63; j++) {
		    std::cout << matriz[i][j];
		}
		std::cout << std::endl;
	}
	
	std_msgs::Float32MultiArray msg_pub;
	msg_pub.data.clear();
	for (float valor : valores) {
		msg_pub.data.push_back(valor);
	}

	pub.publish(msg_pub);
	
	
	
}

int main(int argc, char **argv){

	ros::init(argc, argv, "vison_robo");
	ros::NodeHandle n;
	
	pub = n.advertise<std_msgs::Float32MultiArray>("/processed_lidar", 1);
	
	//ros::Subscriber sub = n.subscribe("/scan", 1, subCallback);
	ros::Subscriber sub = n.subscribe("/vison", 1, subCallback);
	
	ros::spin();
	return 0;
	
}
