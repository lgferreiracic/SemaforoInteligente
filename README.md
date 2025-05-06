# EmbarcaTech_SemaforoInteligente  
<p align="center">
  <img src="Group 658.png" alt="EmbarcaTech" width="300">
</p>

## Atividade: Semáforo Inteligente com FreeRTOS

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white)
![Windows 11](https://img.shields.io/badge/Windows%2011-%230079d5.svg?style=for-the-badge&logo=Windows%2011&logoColor=white)

## 🧠 Descrição do Projeto

Este projeto implementa um sistema de semáforo inteligente baseado em FreeRTOS, utilizando o microcontrolador RP2040. O sistema alterna entre os modos diurno e noturno, e controla múltiplos periféricos como LEDs, display OLED e buzzers, cada um gerenciado por uma task específica do RTOS. O objetivo é simular o funcionamento realista de um semáforo com controle modular e multitarefa.

## 🔌 Componentes Utilizados

- **Raspberry Pi Pico (RP2040)**
- **BitDogLab - Botão A (GPIO 5)**: Alterna entre modo diurno e noturno.
- **LED RGB (GPIOs 11, 12 e 13)**: Representa o estado atual do semáforo.
- **Matriz de LEDs WS2812B (GPIO 7)**: Iluminação completa conforme a cor do sinal.
- **Display OLED SSD1306 (GPIOs 14 e 15 - I2C)**: Exibe mensagens com o estado atual.
- **Buzzers (GPIOs 10 e 21)**: Emitem sinais sonoros conforme o modo e estado do semáforo.

## 💻 Ambiente de Desenvolvimento

- **VS Code** com extensão da Raspberry Pi
- **Linguagem C**
- **Pico SDK**
- **FreeRTOS**

## ⚙️ Guia de Instalação

1. Clone o repositório.
2. Abra o projeto com a extensão da Raspberry Pi no VS Code.
3. Compile utilizando CMake.
4. Para executar na BitDogLab, coloque o arquivo `.uf2` no modo bootsel.
5. Para simular, utilize a extensão do Wokwi com suporte ao RP2040.

## ▶️ Guia de Uso

1. Ligue a placa com os periféricos corretamente conectados.
2. O sistema inicia em modo diurno com o LED verde aceso.
3. Pressione o **botão A** para alternar entre os modos:
   - **Modo Diurno:** Cores verde, amarelo e vermelho são alternadas ciclicamente.
   - **Modo Noturno:** LED e matriz piscam em amarelo a cada 2 segundos.
4. O **display OLED** mostra o nome do modo ou o estado atual do semáforo.
5. Os **buzzers** emitem sinais sonoros distintos conforme o modo e cor ativa.

## 📊 Estrutura com FreeRTOS

Cada componente é controlado por uma tarefa dedicada:

- `vTaskButtons` – Lê o botão A e alterna o modo.
- `vTaskLEDs` – Controla o LED RGB.
- `vTaskMatrix` – Controla a matriz WS2812B.
- `vTaskDisplay` – Atualiza o display OLED.
- `vTaskBuzzers` – Emite sinais sonoros com base nas flags.

## 🧪 Testes

Testes manuais foram realizados para verificar:
- Alternância correta de modos e estados
- Resposta precisa ao botão
- Funcionamento coordenado dos periféricos

## 👤 Desenvolvedor

[Lucas Gabriel Ferreira](https://github.com/usuario-lider)

## 📽️ Vídeo da Solução

[Link do YouTube](https://www.youtube.com/watch?v=MHolUQz0k04)
