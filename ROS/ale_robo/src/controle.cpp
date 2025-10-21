#include "ros/ros.h"
#include "std_msgs/Int16MultiArray.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


int kbhit(void) // captura de teclado sem travamento//
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

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

using namespace std;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "controle_robo");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<std_msgs::Int16MultiArray>("/cont", 1000);

    ros::Rate loop_rate(10);
    
    
    if (ros::ok())
    {
        std_msgs::Int16MultiArray msg;
        
        
        uint16_t pwm = 0,motor_passo_atual = 0, motor_dc_atual = 0;
        uint16_t passo[7] = {};
        bool controle_2DC = 0;
        
        ros::spinOnce();
        msg.data.resize(10);
        
        char comando = '0', holder = '0';
        system("clear");
	printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
        while (comando != 'p')
        {
            if (kbhit())
            {
                comando = getchar();
                
                switch (comando)
                {
			case '0':
				
				for (int i=1;i<=9;i++){
					msg.data[i] = 0;
				}
				msg.data[0] = 1;
				motor_dc_atual = 0;
				motor_passo_atual = 0;
				pub.publish(msg);
				ros::spinOnce();
				system("clear");
				printf("Motores Parados");
				break;
				
			case '1':
			
				motor_passo_atual = 1;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '2':
			
				motor_passo_atual = 2;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '3':
			
				motor_passo_atual = 3;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '4':
			
				motor_passo_atual = 4;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '5':
			
				motor_passo_atual = 5;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '6':
			
				motor_passo_atual = 6;
				motor_dc_atual = 0;
				system("clear");
				printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
				break;
				
			case '7':
			
				motor_dc_atual = 7;
				motor_passo_atual = 0;
				system("clear");
				printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
				break;
				
			case '8':
			
				motor_dc_atual = 8;
				motor_passo_atual = 0;
				system("clear");
				printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
				break;
				
			case '9':
			
				motor_dc_atual = 9;
				motor_passo_atual = 0;
				system("clear");
				printf("Atuador selecionado");
				break;
				
			case 'q':
			
				for (int i=1;i<=9;i++){
					msg.data[i] = 0;
				}
				msg.data[0] = 0;
				if (motor_passo_atual != 0){
					msg.data[motor_passo_atual] = passo[motor_passo_atual];
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0){
					msg.data[motor_dc_atual] = pwm;
					if (motor_dc_atual == 9) msg.data[9]=1;
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				if (controle_2DC){
					msg.data[7] = pwm;
					msg.data[8] = pwm;
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				break;

			case 'w':
				
				for (int i=1;i<=9;i++){
					msg.data[i] = 0;
				}
				msg.data[0] = 1;
				pub.publish(msg);
				ros::spinOnce();
				break;

			case 'e':
				
				for (int i=1; i<=9; i++){
					msg.data[i] = 0;
				}
				msg.data[0] = 0;
				if (motor_passo_atual != 0){
					controle_2DC = 0;
					msg.data[motor_passo_atual] = -passo[motor_passo_atual];
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0){
					controle_2DC = 0;
					msg.data[motor_dc_atual] = -pwm;
					if (motor_dc_atual == 9) msg.data[9] = -1;
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				if (controle_2DC){
					msg.data[7] = -pwm;
					msg.data[8] = -pwm;
					pub.publish(msg);
					ros::spinOnce();
					break;
				}
				break;
				
			case 'a':
			
				if (motor_passo_atual != 0){
					passo[motor_passo_atual] += 100;
					system("clear");
					printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0 & motor_dc_atual != 9){
					pwm += 10;
					if (pwm > 255) pwm = 255;
					pub.publish(msg);
					system("clear");
					printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
					break;
				}
				break;
				
			case 's':
			
				if (motor_passo_atual != 0){
					passo[motor_passo_atual] -= 100;
					system("clear");
					printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0 & motor_dc_atual != 9){
					pwm -= 10;
					if (pwm > 255) pwm = 0;
					pub.publish(msg);
					system("clear");
					printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
					break;
				}
				break;
				
			case 'd':
			
				if (motor_passo_atual != 0){
					passo[motor_passo_atual] += 10;
					system("clear");
					printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0 & motor_dc_atual != 9){
					pwm += 1;
					if (pwm > 255) pwm = 255;
					pub.publish(msg);
					system("clear");
					printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
					break;
				}
				break;
				
			case 'f':
			
				if (motor_passo_atual != 0){
					passo[motor_passo_atual] -= 10;
					system("clear");
					printf("Motor de passo atual = %d\nPasso atual = %d",motor_passo_atual, passo[motor_passo_atual]);
					ros::spinOnce();
					break;
				}
				
				if (motor_dc_atual != 0 & motor_dc_atual != 9){
					pwm -= 1;
					if (pwm > 255) pwm = 0;
					pub.publish(msg);
					system("clear");
					printf("Motor DC atual = %d\nPWM atual = %d",motor_dc_atual-6, pwm);
					break;
				}
				break;

			case 'r':
			
				system("clear");
				controle_2DC = !controle_2DC;
				motor_dc_atual = 0;
				motor_passo_atual = 0;
				if (controle_2DC) printf("Controle dos dois motores DC\nAtivado");
				else printf("Controle dos dois motores DC\nDesativado");
				ros::spinOnce();
				break;
				
			case 't':
			
				system("clear");
				printf("Rotina 2 iniciada");
				for (int i = 1;i<7;i++){
					msg.data[i] = passo[i];
				}
				pub.publish(msg);
				for (int j = 0; j < 20; ++j){
					if (kbhit()) break;
					ros::Duration(0.1).sleep();
				}
				for (int i = 1;i<7;i++){
					msg.data[i] = -passo[i];
				}
				pub.publish(msg);
				ros::spinOnce();
				break;
				
			case 'y':
			
				system("clear");
				printf("Rotina 1 iniciada");
				msg.data[0] = 0;
				msg.data[3] = passo[3];
				while (!kbhit()){
					if (kbhit()) break;
					msg.data[3] = -msg.data[3];
					pub.publish(msg);
					for (int j = 0; j < 20; ++j)
					    {
						if (kbhit()) break;
						ros::Duration(0.1).sleep();
					    }
				}
				ros::spinOnce();
				break;
			

			default:

				break;

		}
	     }
	}
	system("clear");
	printf("Trajeto finalizado\n");
    }
    return 0;
}

//Funções



