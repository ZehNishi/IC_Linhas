#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Image
import numpy as np
import cv2
from cv_bridge import CvBridge
import os

# Diretório para salvar as imagens
output_dir = "/home/jose/dados"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Inicialização do nó ROS
rospy.init_node('depth_image_processor', anonymous=True)

# Inicialização do objeto CvBridge
bridge = CvBridge()

# Contador para as imagens
image_counter = 0

def image_callback(msg):
    # Converter a mensagem ROS para uma imagem OpenCV
    global image_counter
    try:
        image = bridge.imgmsg_to_cv2(msg, "rgb8")
    except Exception as e:
        rospy.logerr("Erro ao converter a imagem: {}".format(e))
        return
    
    # Calcular a derivada discreta usando Sobel
    sobel_x = cv2.Sobel(image, cv2.CV_64F, 1, 0, ksize=5)
    sobel_y = cv2.Sobel(image, cv2.CV_64F, 0, 1, ksize=5)
    magnitude = cv2.magnitude(sobel_x, sobel_y)*50

    # Normalizar a magnitude para o intervalo [0, 255]
    magnitude = cv2.normalize(magnitude, None, 0, 255, cv2.NORM_MINMAX)
    magnitude = np.uint8(magnitude)

    output_path = os.path.join(output_dir, f"depth_image_result_{image_counter}.png")
    cv2.imwrite(output_path, magnitude)
    rospy.loginfo("Imagem resultante salva em: {}".format(output_path))
    
    # Incrementar o contador de imagens
    image_counter += 1

def main():
    # Inscrever-se no tópico
    rospy.Subscriber('/sim_ros_interface/spherical1/depth', Image, image_callback)

    rospy.loginfo("Aguardando mensagens de imagem...")
    rospy.spin()

if __name__ == '__main__':
    main()

