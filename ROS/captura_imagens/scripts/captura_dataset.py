#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2
import os
import csv

# Definindo diretórios de saída
output_dir = '/home/jose/Projeto_RP/imagens_simu'
depth_dir = os.path.join(output_dir, 'nada')
#rgb_dir = os.path.join(output_dir, 'rgb')
gray_dir = os.path.join(output_dir, 'derivadas_nada')  # Diretório para imagens gray
gray_csv_file = os.path.join(output_dir, 'nada_derivado.csv')  # CSV para média e variância das imagens gray
depth_csv_file = os.path.join(output_dir, 'nada.csv')  # CSV para média e variância das imagens depth

img_count_depth = 0
img_count_rgb = 0
img_count_gray = 0  # Contador para imagens gray

# Criando os diretórios, caso não existam
if not os.path.exists(depth_dir):
    os.makedirs(depth_dir)
if not os.path.exists(gray_dir):
    os.makedirs(gray_dir)

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

        # Calcular a média e variância
        media = cv_image.mean()
        variancia = cv_image.var()

        # Salvar os valores no CSV de profundidade
        with open(depth_csv_file, mode='a') as file:
            writer = csv.writer(file)
            writer.writerow([img_filename, media, variancia])
        
        rospy.loginfo('Média (depth): {:.2f}, Variância (depth): {:.2f} (salvo no CSV)'.format(media, variancia))

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

# Função para salvar as imagens gray e armazenar a média e variância
def gray_callback(image_msg):
    global img_count_gray
    try:
        # Converter imagem ROS para OpenCV
        cv_image = bridge.imgmsg_to_cv2(image_msg, desired_encoding="mono8")
        img_filename = os.path.join(gray_dir, 'gray_{:04d}.png'.format(img_count_gray))
        cv2.imwrite(img_filename, cv_image)
        rospy.loginfo('Imagem gray salva: {}'.format(img_filename))
        
        # Calcular a média e variância
        media = cv_image.mean()
        variancia = cv_image.var()

        # Salvar os valores no CSV das imagens gray
        with open(gray_csv_file, mode='a') as file:
            writer = csv.writer(file)
            writer.writerow([img_filename, media, variancia])
        
        rospy.loginfo('Média (gray): {:.2f}, Variância (gray): {:.2f} (salvo no CSV)'.format(media, variancia))

        img_count_gray += 1
    except Exception as e:
        rospy.logerr("Erro ao processar a imagem gray: {}".format(e))

def main():
    rospy.init_node('captura_imagens', anonymous=True)

    # Subscrição aos tópicos
    rospy.Subscriber("/sim_ros_interface/spherical1/depth", Image, depth_callback)
    #rospy.Subscriber("/sim_ros_interface/spherical1/rgb", Image, rgb_callback)
    rospy.Subscriber("/output/gray_image", Image, gray_callback)

    # Criar arquivo CSV de profundidade com cabeçalho, se não existir
    if not os.path.exists(depth_csv_file):
        with open(depth_csv_file, mode='w') as file:
            writer = csv.writer(file)
            writer.writerow(['Imagem', 'Media', 'Variancia'])

    # Criar arquivo CSV das imagens gray com cabeçalho, se não existir
    if not os.path.exists(gray_csv_file):
        with open(gray_csv_file, mode='w') as file:
            writer = csv.writer(file)
            writer.writerow(['Imagem', 'Media', 'Variancia'])

    rospy.loginfo("Capturando imagens e calculando média/variância. Pressione Ctrl+C para parar.")
    rospy.spin()

if __name__ == '__main__':
    main()

