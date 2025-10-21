#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Image  # Mensagem ROS para imagem
from cv_bridge import CvBridge, CvBridgeError
import torch
import torchvision.transforms as transforms
from PIL import Image as PILImage  # Renomear para evitar conflitos com sensor_msgs
import cv2
import torch.nn as nn
import torch.optim as optim
import torchvision.models as models
import time

# Caminho do modelo treinado
#model_path = '/home/jose/Projeto_RP/Codes/squeezenet_model.pth'
model_path = '/home/jose/Projeto_RP/Codes/squeezenet_model_derivate.pth'

# Classe para conversão de imagens ROS -> OpenCV
bridge = CvBridge()

# Dispositivo (CPU ou GPU)
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

# Carregar o modelo SqueezeNet pré-treinado
model = models.squeezenet1_1(pretrained=False)
model.classifier[1] = nn.Conv2d(512, 4, kernel_size=(1, 1), stride=(1, 1))  # 4 classes
model.num_classes = 4
model.load_state_dict(torch.load(model_path))
model = model.to(device)
model.eval()  # Definir o modelo em modo de avaliação

# Transformações que foram usadas no treinamento
data_transforms = transforms.Compose([
    transforms.Resize((224, 224)),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])

# Nomes das classes
class_names = ['nada', 'isolador', 'amortecedor', 'sinalizador']

# Variáveis para medir a taxa de previsões
count = 0
start_time = time.time()

def image_callback(ros_image):
    global count, start_time

    try:
        # Converter ROS Image para OpenCV (BGR)
        cv_image = bridge.imgmsg_to_cv2(ros_image, desired_encoding="passthrough")
        
        # Converter a imagem para RGB (se necessário)
        if len(cv_image.shape) == 2 or cv_image.shape[2] == 1:
            cv_image = cv2.cvtColor(cv_image, cv2.COLOR_GRAY2RGB)

        # Converter OpenCV imagem (numpy.ndarray) para PIL.Image
        pil_image = PILImage.fromarray(cv2.cvtColor(cv_image, cv2.COLOR_BGR2RGB))

        # Aplicar as transformações (agora em pil_image)
        image = data_transforms(pil_image)
        image = image.unsqueeze(0)  # Adicionar a dimensão do batch

        # Mover a imagem para o dispositivo (CPU ou GPU)
        image = image.to(device)

        # Fazer a previsão
        outputs = model(image)
        _, preds = torch.max(outputs, 1)

        # Obter a classe prevista
        predicted_class = class_names[preds.item()]
        rospy.loginfo(f'Classe prevista: {predicted_class}')

        # Contar a previsão
        count += 1

        # Calcular e imprimir a taxa de previsões a cada segundo
        elapsed_time = time.time() - start_time
        if elapsed_time >= 1.0:  # A cada segundo
            rospy.loginfo(f'Taxa de previsões: {count} Hz')
            count = 0  # Reiniciar contador
            start_time = time.time()  # Reiniciar temporizador

    except CvBridgeError as e:
        rospy.logerr(f"Erro ao converter imagem: {str(e)}")

def listener():
    # Inicializar o nó ROS
    rospy.init_node('image_classifier_node', anonymous=True)
    
    # Inscrever-se no tópico de profundidade
    #rospy.Subscriber('/sim_ros_interface/spherical1/depth', Image, image_callback)
    rospy.Subscriber('/output/gray_image', Image, image_callback)
    
    # Manter o nó ativo
    rospy.spin()

if __name__ == '__main__':
    listener()

