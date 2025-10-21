#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Image
from std_msgs.msg import Float32MultiArray
from cv_bridge import CvBridge
import cv2
import os
import csv

# Definindo diretórios de saída
output_dir = '/home/jose/Documents/TCC/Dados/Multimodal/V_Alexandre'
depth_dir = os.path.join(output_dir, 'depth','nada')
rgb_dir = os.path.join(output_dir, 'rgb','nada')
lidar_dir = os.path.join(output_dir, 'lidar')
csv_file_path = os.path.join(lidar_dir, 'nada.csv')

img_count_depth = 0
img_count_rgb = 0


# Criando os diretórios, caso não existam
if not os.path.exists(output_dir):
    os.makedirs(output_dir)
    
if not os.path.exists(depth_dir):
    os.makedirs(depth_dir)
    
if not os.path.exists(rgb_dir):
    os.makedirs(rgb_dir)
    
if not os.path.exists(lidar_dir):
    os.makedirs(lidar_dir)

# Iniciando o bridge do OpenCV
bridge = CvBridge()

# Função para salvar as imagens de profundidade e calcular média/variância
def depth_callback(image_msg):
    global img_count_depth
    try:
        # Converter imagem ROS para OpenCV
        cv_image = bridge.imgmsg_to_cv2(image_msg, desired_encoding="passthrough")
        img_filename = os.path.join(depth_dir, 'depth_{:04d}.png'.format(img_count_depth))
        cv2.imwrite(img_filename, cv_image)
        rospy.loginfo('Imagem de profundidade salva: {}'.format(img_filename))

        img_count_depth += 1
    except Exception as e:
        rospy.logerr("Erro ao processar a imagem de profundidade: {}".format(e))

# Função para salvar as imagens RGB
def rgb_callback(image_msg):
    global img_count_rgb
    try:
        cv_image = bridge.imgmsg_to_cv2(image_msg, desired_encoding="bgr8")
        img_filename = os.path.join(rgb_dir, 'rgb_{:04d}.png'.format(img_count_rgb))
        cv2.imwrite(img_filename, cv_image)
        rospy.loginfo('Imagem RGB salva: {}'.format(img_filename))
        img_count_rgb += 1
    except Exception as e:
        rospy.logerr("Erro ao processar a imagem RGB: {}".format(e))

def vison_callback(scan_msg):
    try:
        ranges = scan_msg.data
        # Abre o arquivo CSV em modo de anexação e salva os dados
        with open(csv_file_path, mode='a', newline='') as csv_file:
            writer = csv.writer(csv_file)
            writer.writerow(ranges)  # Salva apenas os valores de ranges no CSV

        rospy.loginfo("Dados do lidar salvos no arquivo CSV.")
    except Exception as e:
        rospy.logerr("Erro ao processar os dados do tópico /vison: {}".format(e))

def main():
    rospy.init_node('captura_imagens', anonymous=True)

    # Subscrição aos tópicos
    rospy.Subscriber("/sim_ros_interface/spherical1/depth", Image, depth_callback)
    #rospy.Subscriber("/vison", LaserScan, vison_callback)
    #rospy.Subscriber("/sim_ros_interface/spherical1/rgb", Image, rgb_callback)

    rospy.Subscriber("/processed_lidar", Float32MultiArray, vison_callback)
    #rospy.Subscriber("/camera/depth/image_rect_raw", Image, depth_callback)
    #rospy.Subscriber("/camera/color/image_raw", Image, rgb_callback)


    rospy.loginfo("Capturando imagens. Pressione Ctrl+C para parar.")
    rospy.spin()

if __name__ == '__main__':
	if not os.path.exists(csv_file_path):
		with open(csv_file_path, mode='w', newline='') as csv_file:
			writer = csv.writer(csv_file)
			writer.writerow([f"range_{i}" for i in range(63)]) 

	main()
