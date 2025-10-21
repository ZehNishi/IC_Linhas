#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void controle(int op, int vel, double *msgX, double *msgZ);

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
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cont", 1000);

    ros::Rate loop_rate(10);
    if (ros::ok())
    {
        geometry_msgs::Twist msg;
        float vel_lin = 2;
        ros::spinOnce();
        char comando = '0';
        bool x1 = 1;
        bool x2 = 1;
        while (comando != 'p')
        {
            if (kbhit())
            {
                comando = getchar();
                switch (comando)
                {
                case 'w':
                    controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '1':
                    if (x1)
                    {
                        controle(4, vel_lin, &msg.linear.x, &msg.linear.z);
                        cout << "Trava 1 aclopada" << endl;
                        pub.publish(msg);
                        sleep(1);
                        controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                        pub.publish(msg);
                        ros::spinOnce();
                        x1 = !x1;
                    }
                    else
                    {
                        cout << "Trava 1 ja esta aclopada" << endl;
                    }
                    break;
                case '2':
                    if (x2)
                    {
                        msg.linear.z = 2;
                        cout << "Trava 2 aclopada" << endl;
                        pub.publish(msg);
                        sleep(1);
                        msg.linear.z = 0;
                        pub.publish(msg);
                        ros::spinOnce();
                        x2 = !x2;
                    }
                    else
                    {
                        cout << "Trava 2 ja esta aclopada" << endl;
                    }
                    break;
                case '3':
                    if (!x1)
                    {
                        controle(5, vel_lin, &msg.linear.x, &msg.linear.z);
                        cout << "Trava 1 desaclopada" << endl;
                        pub.publish(msg);
                        sleep(1);
                        controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                        pub.publish(msg);
                        ros::spinOnce();
                        x1 = !x1;
                    }
                    else
                    {
                        cout << "Trava 1 ja esta desaclopada" << endl;
                    }
                    break;
                case '4':
                    if (!x2)
                    {
                        msg.linear.z = 4;
                        cout << "Trava 2 desaclopada" << endl;
                        pub.publish(msg);
                        sleep(1);
                        msg.linear.z = 0;
                        pub.publish(msg);
                        ros::spinOnce();
                        x2 = !x2;
                    }
                    else
                    {
                        cout << "Trava 2 ja esta desaclopada" << endl;
                    }
                    break;
                case 'e':
                    controle(2, vel_lin, &msg.linear.x, &msg.linear.z);
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case 'q':
                    controle(3, vel_lin, &msg.linear.x, &msg.linear.z);
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '5':
                    msg.linear.y = 1;
                    cout << "Rotacao traseira" << endl;
                    pub.publish(msg);
                    sleep(1);
                    msg.linear.y = 0;
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '6':
                    msg.linear.y = 2;
                    cout << "Rotacao dianteira" << endl;
                    pub.publish(msg);
                    sleep(1);
                    msg.linear.y = 0;
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '7':
                    msg.linear.y = 3;
                    cout << "Volta traseira" << endl;
                    pub.publish(msg);
                    sleep(1);
                    msg.linear.y = 0;
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '8':
                    msg.linear.y = 4;
                    cout << "Volta dianteira" << endl;
                    pub.publish(msg);
                    sleep(1);
                    msg.linear.y = 0;
                    pub.publish(msg);
                    ros::spinOnce();
                    break;
                case '9':
                    vel_lin += 0.5;
                    cout << "Velocidade atual: " << vel_lin << endl;
                    ros::spinOnce();
                    break;
                case '0':
                    vel_lin -= 0.5;
                    cout << "Velocidade atual: " << vel_lin << endl;
                    ros::spinOnce();
                    break;
                case 'r':
                    while (!(kbhit()))
                    {
                        cout << "Rotina iniciada..." << endl;
                        if (x1)
                        {
                            controle(4, vel_lin, &msg.linear.x, &msg.linear.z);
                            cout << "Trava 1 aclopada" << endl;
                            pub.publish(msg);
                            sleep(1);
                            controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                            pub.publish(msg);
                            ros::spinOnce();
                            x1 = !x1;
                        }

                        sleep(2);
                        controle(2, vel_lin, &msg.linear.x, &msg.linear.z);
                        pub.publish(msg);
                        ros::spinOnce();
                        sleep(2);
                        controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                        if (!x1)
                        {
                            controle(5, vel_lin, &msg.linear.x, &msg.linear.z);
                            cout << "Trava 1 desaclopada" << endl;
                            pub.publish(msg);
                            sleep(1);
                            controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                            pub.publish(msg);
                            ros::spinOnce();
                            x1 = !x1;
                        }
                        sleep(2);
                        if (x1)
                        {
                            controle(4, vel_lin, &msg.linear.x, &msg.linear.z);
                            cout << "Trava 1 aclopada" << endl;
                            pub.publish(msg);
                            sleep(1);
                            controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                            pub.publish(msg);
                            ros::spinOnce();
                            x1 = !x1;
                        }
                        sleep(2);
                        controle(3, vel_lin, &msg.linear.x, &msg.linear.z);
                        pub.publish(msg);
                        ros::spinOnce();
                        sleep(2);
                        controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                        if (!x1)
                        {
                            controle(5, vel_lin, &msg.linear.x, &msg.linear.z);
                            cout << "Trava 1 desaclopada" << endl;
                            pub.publish(msg);
                            sleep(1);
                            controle(1, vel_lin, &msg.linear.x, &msg.linear.z);
                            pub.publish(msg);
                            ros::spinOnce();
                            x1 = !x1;
                        }
                        sleep(2);
                    }

                    controle(1, vel_lin, &msg.linear.x, &msg.linear.z);

                    break;
                default:

                    break;
                }
            }
        }
        cout << "Trajeto finalizado..." << endl;
    }
    return 0;
}

//Funções

void controle(int op, int vel, double *msgX, double *msgZ)
{
    switch (op)
    {
    case 1: //Stop
        *msgX = 0;
        *msgZ = 0;
        break;

    case 2: //Forward
        *msgX = vel;
        *msgZ = 0;
        break;

    case 3: //Backward
        *msgX = -vel;
        *msgZ = 0;
        break;

    case 4: //Travar
        *msgX = 0;
        *msgZ = 1;
        break;

    case 5: //Destravar
        *msgX = 0;
        *msgZ = 3;
        break;
    }
}


