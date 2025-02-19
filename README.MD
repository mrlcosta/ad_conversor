# Projeto de Controle de LEDs RGB e Display SSD1306 com Joystick no RP2040

## Descrição

Este projeto visa consolidar os conceitos sobre o uso de conversores analógico-digitais (ADC) no RP2040 e explorar as funcionalidades da placa de desenvolvimento **BitDogLab**. O sistema é controlado por um joystick e interage com LEDs RGB e um display SSD1306.

### Objetivos

- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.
- Utilizar o PWM para controlar a intensidade de dois LEDs RGB com base nos valores do joystick.
- Representar a posição do joystick no display SSD1306 por meio de um quadrado móvel.
- Aplicar o protocolo de comunicação I2C na integração com o display.

## Componentes Utilizados

- **Placa de Desenvolvimento BitDogLab**
- **LED RGB** conectado às GPIOs (11, 12 e 13)
- **Joystick** conectado às GPIOs (26 e 27)
- **Botão do Joystick** conectado à GPIO 22
- **Botão A** conectado à GPIO 5
- **Display SSD1306** conectado via I2C nas GPIOs (14 e 15)

## Funcionalidades

### LEDs RGB

- **LED Azul**: O brilho é controlado pelo eixo Y do joystick. O LED acende conforme a posição do joystick (para cima ou para baixo).
  - Posição central (valor 2048): LED apagado.
  - Movimentação para cima (valores menores): LED acende com brilho crescente.
  - Movimentação para baixo (valores maiores): LED acende com brilho crescente até o valor máximo.
  
- **LED Vermelho**: O brilho é controlado pelo eixo X do joystick. O LED acende conforme a posição do joystick (para a esquerda ou para a direita).
  - Posição central (valor 2048): LED apagado.
  - Movimentação para a esquerda (valores menores): LED acende com brilho crescente.
  - Movimentação para a direita (valores maiores): LED acende com brilho crescente até o valor máximo.

### Display SSD1306

- Um quadrado de **8x8 pixels** é exibido no display.
  - O quadrado se move de acordo com os valores do joystick, representando a posição no eixo X e Y.

### Botões do Joystick

- **Botão do Joystick** (GPIO 22):
  - Alterna o estado do **LED Verde** a cada pressionamento.
  - Modifica a borda do display a cada pressionamento, alternando entre diferentes estilos de borda.

- **Botão A** (GPIO 5):
  - Ativa ou desativa os LEDs PWM a cada pressionamento.

## Requisitos do Projeto

1. **Uso de Interrupções**: Todas as funcionalidades relacionadas aos botões devem ser implementadas utilizando rotinas de interrupção (IRQ).
2. **Debouncing**: Implementação do tratamento de bouncing dos botões via software para garantir leituras estáveis e precisas.
3. **Utilização do Display 128x64**: O display deve ser utilizado para representar o quadrado móvel e os efeitos visuais com base na interação do joystick e dos botões.
4. **Organização do Código**: O código deve ser bem estruturado, modular e comentado para facilitar o entendimento e a manutenção.

## Estrutura do Projeto

A estrutura básica do código será organizada em módulos para:

- **Leitura do Joystick**: Capturar os valores analógicos dos eixos X e Y do joystick.
- **Controle dos LEDs RGB via PWM**: Ajustar a intensidade dos LEDs com base nos valores do joystick.
- **Exibição no Display SSD1306**: Atualizar a posição do quadrado no display.
- **Gerenciamento dos Botões**: Utilizar interrupções para alternar o estado dos LEDs e modificar a borda do display.
  

## Video demonstrativo
[![Assista ao vídeo demonstrativo](https://i.ytimg.com/vi/m4CvZlHNlw0/hq720_2.jpg?sqp=-oaymwEoCJUDENAFSFryq4qpAxoIARUAAIhC0AEB2AEB4gEKCBgQAhgGOAFAAQ==&rs=AOn4CLDEAiMRf0h5bnH_93G-STIvsmkGAQ)](https://www.youtube.com/shorts/0Hp303Xu79E)


## Considerações Finais

Este projeto permite a exploração de diversos conceitos importantes como ADC, PWM, comunicação I2C e manipulação de interrupções, além de trabalhar com a integração de múltiplos componentes no RP2040. O código deve ser desenvolvido com foco na clareza, organização e eficiência, seguindo as melhores práticas de programação embarcada.
