#include <ros/ros.h>
#include <sensor_msgs/Image.h>

#include <cmath>
#include <vector>
#include <algorithm>

ros::Publisher gray_image_pub;

// Função para calcular a média do vetor
double calcular_media(const std::vector<uint8_t>& vetor) {
    double soma = 0.0;
    for (size_t i = 0; i < vetor.size(); i++) {
        soma += vetor[i];
    }
    return soma / vetor.size();
}

// Função para calcular a variância do vetor
double calcular_variancia(const std::vector<uint8_t>& vetor, double media) {
    double soma = 0.0;
    for (size_t i = 0; i < vetor.size(); i++) {
        soma += pow(vetor[i] - media, 2);
    }
    return soma / vetor.size();
}

void imageCallback(const sensor_msgs::Image::ConstPtr& msg) {
    // Verifica se a codificação é "rgb8"
    if (msg->encoding == "rgb8") {
        // Cria uma nova mensagem para a imagem em escala de cinza
        sensor_msgs::Image gray_image;
        gray_image.header = msg->header;  // Copia o cabeçalho
        gray_image.height = msg->height;
        gray_image.width = msg->width;
        gray_image.encoding = "mono8";  // Define a nova codificação
        gray_image.is_bigendian = msg->is_bigendian;
        gray_image.step = msg->width;  // Um byte por pixel em mono8
        gray_image.data.resize(msg->height * gray_image.step);  // Aloca espaço para os dados

        // Realiza a conversão de rgb8 para mono8
        for (size_t i = 0; i < msg->height; ++i) {
            for (size_t j = 0; j < msg->width; ++j) {
                size_t rgb_index = (i * msg->width + j) * 3;  // Índice no vetor rgb
                size_t gray_index = i * gray_image.step + j;   // Índice no vetor gray

                // Calcula a intensidade em escala de cinza
                uint8_t gray_value = static_cast<uint8_t>(
                    0.299 * msg->data[rgb_index] +
                    0.587 * msg->data[rgb_index + 1] +
                    0.114 * msg->data[rgb_index + 2]
                );

                gray_image.data[gray_index] = gray_value;  // Armazena o valor em escala de cinza
            }
        }

        // Cálculo de derivadas (bordas)
        for (int i = gray_image.height - 1; i >= 0; --i) {
            for (int j = gray_image.width - 1; j >= 0; --j) {
                size_t gray_index = i * gray_image.step + j;

                if (i == 0 || j == 0) {
                    // Primeira linha ou primeira coluna
                    gray_image.data[gray_index] = 0;
                } else {
                    // Cálculo da diferença
                    float diff_x = gray_image.data[gray_index] - gray_image.data[gray_index - 1]; // Pixel à esquerda
                    float diff_y = gray_image.data[gray_index] - gray_image.data[gray_index - gray_image.step]; // Pixel acima

                    // Calculando a raiz quadrada da soma dos quadrados das diferenças
                    float derivative = std::sqrt(diff_x * diff_x + diff_y * diff_y) * 20;

                    // Limita o valor a 255 e armazena no próprio vetor data
                    gray_image.data[gray_index] = static_cast<uint8_t>(std::min(derivative, 255.0f));
                }
            }
        }

        // Extração de features a partir de gray_image.data
        double media = calcular_media(gray_image.data);
        double variancia = calcular_variancia(gray_image.data, media);

        // Exibe as features
        ROS_INFO("Media: %.2f, Variancia: %.2f", media, variancia);
        

        // Publica a imagem convertida
        gray_image_pub.publish(gray_image);
    } else {
        ROS_WARN("Unsupported encoding: %s", msg->encoding.c_str());
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "image_converter");
    ros::NodeHandle nh;

    // Cria o publisher para a imagem convertida
    gray_image_pub = nh.advertise<sensor_msgs::Image>("/output/gray_image", 10);

    // Cria o subscriber para a imagem original
    ros::Subscriber image_sub = nh.subscribe<sensor_msgs::Image>(
        "/sim_ros_interface/spherical1/depth", 10,
        imageCallback
    );

    ros::spin();
    return 0;
}

