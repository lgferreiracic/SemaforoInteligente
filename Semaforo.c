#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include <stdio.h>
#include "lib/ssd1306.h"
#include "lib/font.h"
#include "lib/matrix.h"
#include "lib/buzzer.h"
#include "lib/button.h"
#include "lib/leds.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

//Constantes para as GPIOs
#define BUTTON_A 5
#define GREEN_LED 11
#define BLUE_LED 12
#define RED_LED 13

bool night_mode = false; // Variável booleana para o modo noturno
int traffic_light_state = 0; // Variável para o estado do semáforo (0: verde, 1: amarelo, 2: vermelho)

//Estrutura para o display
ssd1306_t ssd;

//Tarefa para gerenciar os botões
void vTaskButtonA(){
    button_init(BUTTON_A);
    int previus_state_button_a = true;

    while(true){
        int current_state_button_a = gpio_get(BUTTON_A);

        //Se o botão A for pressionado, alterna entre os modos dia e noite
        if(previus_state_button_a == true && current_state_button_a == false){ 
            night_mode = !night_mode;
            traffic_light_state = 0;
            led_rgb_off();
        }
        previus_state_button_a = current_state_button_a;
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//Tarefa para gerenciar os LEDs RGB
void vTaskLEDs(){
    led_init_all();
    bool blink = false;

    while(true){
        // Se o modo noturno estiver ativado, pisca o LED em amarelo
        if(night_mode == true){
            if(blink == true)
                led_rgb_yellow_color();
            else
                led_rgb_off();
            blink = !blink;
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
        else{ // Se o modo noturno estiver desativado, acende o LED correspondente ao estado do semáforo
            switch(traffic_light_state){
                case 0: // Acende o LED verde
                    led_rgb_green_color();
                    vTaskDelay(pdMS_TO_TICKS(7000));
                    break;
                case 1: // Acende o LED amarelo
                    led_rgb_yellow_color();
                    vTaskDelay(pdMS_TO_TICKS(3000));
                    break;
                case 2: // Acende o LED vermelho
                    led_rgb_red_color();
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    break;
            }
            traffic_light_state++;
            traffic_light_state = traffic_light_state % 3; 
        }
    }
}

//Tarefa para gerenciar a matriz de LEDs WS2812
void vTaskMatrix(){
    matrix_init();
    while (true){
        // Se o modo noturno estiver ativado, pisca a matriz de LEDs em amarelo
        if(night_mode == true){
            if(gpio_get(GREEN_LED) == true && gpio_get(RED_LED) == true)
                draw_traffic_light_matrix(1);
            else
                draw_traffic_light_matrix(-1);
        }
        else // Se o modo noturno estiver desativado, acende a matriz de LEDs na cor correspondente ao estado do semáforo
            draw_traffic_light_matrix(traffic_light_state);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//Tarefa para gerenciar o display OLED SSD1306
void vTaskDisplay(){
    // Inicializa o display OLED SSD1306
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ADRESS, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    while(true){
        ssd1306_fill(&ssd, false);
        //Se o modo noturno estiver ativado, exibe "Night Mode" no display
        if(night_mode == true){  
            ssd1306_draw_string(&ssd, "Modo", 45, 25);
            ssd1306_draw_string(&ssd, "Noturno", 35, 35);
        }
        else{
            // Se o modo noturno estiver desativado, exibe o estado do semáforo no display
            if(traffic_light_state == 0){ // Verde
                ssd1306_draw_string(&ssd, "Sinal", 40, 25);
                ssd1306_draw_string(&ssd, "Verde", 40, 35);
            }else if(traffic_light_state == 1){ // Amarelo
                ssd1306_draw_string(&ssd, "Sinal", 40, 25);
                ssd1306_draw_string(&ssd, "Amarelo", 35, 35);
            }else if(traffic_light_state == 2){ // Vermelho
                ssd1306_draw_string(&ssd, "Sinal", 40, 25);
                ssd1306_draw_string(&ssd, "Vermelho", 32, 35);
            }
        }
        ssd1306_rect(&ssd, 3, 2, 120, 61, true, false);     
        ssd1306_send_data(&ssd);        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

//Tarefa para gerenciar os buzzers
void vTaskBuzzers(){
    buzzer_init_all();
    while(true){
        // Se o modo noturno estiver ativado, toca um beep lento a cada 2 segundos
        if(night_mode == true){
            play_buzzers();
            vTaskDelay(pdMS_TO_TICKS(200));
            stop_buzzers();
            vTaskDelay(pdMS_TO_TICKS(1800));
        } // Representa o verde para os surdos com um beep curto a cada 1 segundo
        else if(traffic_light_state == 0){ // Verde
            play_buzzers();
            vTaskDelay(pdMS_TO_TICKS(200));
            stop_buzzers();
            vTaskDelay(pdMS_TO_TICKS(800));
        } // Representa o amarelo para os surdos com beeps curtos e intermitentes
        else if(traffic_light_state == 1){ // Amarelo
            for(int i = 0; i < 8; i++){
                play_buzzers();
                vTaskDelay(pdMS_TO_TICKS(50));
                stop_buzzers();
                vTaskDelay(pdMS_TO_TICKS(50));
            };
        } // Representa o vermelho para os surdos com um beep num tom contínuo curto (500ms ligado e 1500ms desligado)
        else if(traffic_light_state == 2){ // Vermelho
            play_buzzers();
            vTaskDelay(pdMS_TO_TICKS(500));
            stop_buzzers();
            vTaskDelay(pdMS_TO_TICKS(1500));
        }
        else{
            stop_buzzers();
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}

int main(){
    stdio_init_all();

    xTaskCreate(vTaskButtonA, "ButtonA", 256, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTaskLEDs, "LEDs", 256, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTaskBuzzers, "Buzzers", 256, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTaskMatrix, "Matrix", 256, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTaskDisplay, "Display", 256, NULL, tskIDLE_PRIORITY, NULL);
    vTaskStartScheduler(); 
}