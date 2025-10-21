#include "sensor_msgs/LaserScan.h"
#include "ros/ros.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#define leituras 1146  //número total de leituras do sensor
#define range_max 12	//alcance maximo do sensor
#define abertura_graus_min 25	//abertura em graus 0-->180
//#define abertura_graus_max -5	//abertura em graus 0-->180
#define abertura_ignorada_min 0	//ângulo para não ser lido em graus 0-->180
#define abertura_ignorada_max 0	//ângulo para não ser lido em graus 0-->180


#define NUM_POINTS 2217  // Número de pontos de dados no arquivo CSV
#define DIMENSIONS 158   // Número de leituras de sensores
#define MAX_CLASS_NAME_LENGTH 50


double data[NUM_POINTS][DIMENSIONS];
int labels[NUM_POINTS];
char classes[NUM_POINTS][MAX_CLASS_NAME_LENGTH];


double euclidean_distance(double *a, double *b, int dimensions) {
    double distance = 0.0;
    for (int i = 0; i < dimensions; i++) {
        distance += pow(a[i] - b[i], 2);
    }
    return sqrt(distance);
}

int knn_predict(double data[][DIMENSIONS], int labels[], double *input, int k) {
    
    double mean=0;

    std::vector<std::pair<double, int>> distances;
    for (int i = 0; i < NUM_POINTS; i++) {
        double dist = euclidean_distance(data[i], input, DIMENSIONS);
        distances.push_back(std::make_pair(dist, labels[i]));
    }

    // Ordenar as distâncias
    std::sort(distances.begin(), distances.end());
    
    mean = distances[0].first + distances[1].first + distances[2].first + distances[3].first + distances[4].first + distances[5].first;
    mean = mean/6;
    std::cout << "Distancia: " << std::fixed << std::setprecision(2) << mean << std::endl;

    // Contar as classes dos k vizinhos mais próximos
    std::map<int, int> class_count;
    for (int i = 0; i < k; i++) {
        int label = distances[i].second;
        class_count[label]++;
    }
    
      // Encontrar a classe que mais aparece
    int max_count = 0;
    int predicted_class = -1;
    for (const auto& pair : class_count) {
        if (pair.second > max_count) {
            max_count = pair.second;
            predicted_class = pair.first;
        }
    }

    return predicted_class;
    
}

void load_data() {
    FILE *file = fopen("/home/jose/bagfiles/knn_data.csv", "r");
    for (int i = 0; i < NUM_POINTS; i++) {
        for (int j = 0; j < DIMENSIONS; j++) {
            fscanf(file, "%lf,", &data[i][j]);
        }
    }
    fclose(file);

    file = fopen("/home/jose/bagfiles/knn_labels.csv", "r");
    for (int i = 0; i < NUM_POINTS; i++) {
        fscanf(file, "%d,", &labels[i]);
    }
    fclose(file);
}

void load_classes() {
    FILE *file = fopen("/home/jose/bagfiles/knn_classes.csv", "r");
    int i = 0;
    while (fgets(classes[i], MAX_CLASS_NAME_LENGTH, file)) {
        classes[i][strcspn(classes[i], "\n")] = '\0';
        i++;
    }
    fclose(file);
}	

void subCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

	float abertura_min = (abertura_graus_min*M_PI)/180;
	//float abertura_max = (abertura_graus_max*M_PI)/180;
	float abertura_ig_min = (abertura_ignorada_min*M_PI)/180;
	float abertura_ig_max = (abertura_ignorada_max*M_PI)/180;
	float sum=0,dif=0,ang=0;
	int count=0;
	
	double dist[leituras];
	std::vector<float> valores;

	for (int i=0; i<leituras; i++){
		ang = msg->angle_min + i*msg->angle_increment;
		if(abs(ang)<abertura_min && ang>-0.433134){
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
	
	for (int i=0; i < count-1; i++){
				
		dif = abs(dist[i] - dist[i+1]);
		sum += 1000*dif;
		
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
	    
	 //std::cout << ss.str();
	 
	  int k = 1;
	    int prediction = knn_predict(data, labels, dist, k);
	    
	    std::stringstream ss2;
	    ss2 << classes[prediction];
	    ss2 << "\n";
	    std::cout << ss2.str();
	
	

}

int main(int argc, char **argv){
ros::init(argc, argv, "vison_robo2");
	ros::NodeHandle n;
	
	load_data();
	load_classes();
	
	ros::Subscriber sub = n.subscribe("/scan", 1000, subCallback);
	ros::spin();
	ros::Rate loop_rate(1);
	return 0;
	}
