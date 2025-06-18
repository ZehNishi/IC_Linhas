# Repositório dos trabalhos desenvolvidos para o projeto de iniciação científica para construção de robôs autônomos para inspeção de linhas de transmissões de alta tensão

Este repositório contém todos os arquivos, códigos e modelos 3D desenvolvidos no âmbito do projeto de Iniciação Científica focado na criação de robôs autônomos para a inspeção de linhas de transmissão de energia de alta tensão.

## 📝 Sobre o Projeto

O objetivo principal deste projeto é desenvolver uma solução robótica autônoma capaz de navegar e inspecionar componentes em linhas de transmissão. A inspeção automatizada busca aumentar a segurança dos operadores, reduzir custos e otimizar o tempo de manutenção, prevenindo falhas no sistema elétrico.

A solução envolve a simulação de um ambiente realista, o desenvolvimento de algoritmos de controle e a criação de modelos 3D fiéis aos componentes encontrados em campo.

## 📂 Conteúdo do Repositório

Este repositório está organizado nas seguintes seções principais:

### 🤖 Códigos de controle e configuração do ROS

* **Pacotes ROS:** Contém os nós para controle de movimentação, drivers, leitura de sensores e comunicação entre os diferentes módulos do robô.

### ⚙️ Cenas simuladas no CoppeliaSim

Aqui estão localizadas as cenas de simulação desenvolvidas no ambiente CoppeliaSim (anteriormente V-REP).

* **Cenas Completas (`.ttt`):** Ambientes de simulação contendo a linha de transmissão, o robô e outros elementos relevantes para os testes.
* **Scripts Embarcados (Lua):** Scripts utilizados dentro do CoppeliaSim para controlar a dinâmica da simulação, sensores e atuadores.
* **Modelos 3D('.obj'):** Modelos tridimensionais dos componentes da linha de transmissão e do próprio robô.
