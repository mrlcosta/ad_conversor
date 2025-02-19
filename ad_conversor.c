#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "lib/ssd1306.h"

#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15
#define OLED_ADDR 0x3C
ssd1306_t oled;

#define JOYSTICK_X 27
#define JOYSTICK_Y 26

#define BT_JOYSTICK 22
#define BT_A 5

#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12

int x_axys_value = 0;
int y_axys_value = 0;

int x_axys_pwm_value = 0;
int y_axys_pwm_value = 0;

uint x_axys_center = 0;
uint y_axys_center = 0;

bool button_a_state = 1;
bool button_joystick_state = 0;

absolute_time_t debounce_bt_a;
absolute_time_t debounce_joystick;

bool debounce_bt(uint pino, absolute_time_t *ultimo_tempo) // realiza o debounce 
{
    absolute_time_t agora = get_absolute_time();
    if (absolute_time_diff_us(*ultimo_tempo, agora) >= 200000)
    {
        *ultimo_tempo = agora;
        return (gpio_get(pino) == 0);
    }
    return false;
}

void gpio_irq_handler(uint gpio, uint32_t events) // callback da interrupção
{

    if (events & GPIO_IRQ_EDGE_FALL) // borda de decida
    {

        if (gpio == BT_A)
        {
            if (debounce_bt(BT_A, &debounce_bt_a))//debounce
            {
                button_a_state = !button_a_state;
                printf("estado botao A: %d\n", button_a_state);
                
                    pwm_set_gpio_level(LED_RED, 0);
                    pwm_set_gpio_level(LED_BLUE, 0);
                
            }
        }

        else if (gpio == BT_JOYSTICK)
        {
            if (debounce_bt(BT_JOYSTICK, &debounce_joystick))
            {
                button_joystick_state = !button_joystick_state;
                gpio_put(LED_GREEN, button_joystick_state);
                printf("estado botao joystick: %d\n", button_a_state);
            }
        }
    }
}
void setup() //inicializa cada periferico da placa.
{
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);

    gpio_init(BT_JOYSTICK);
    gpio_pull_up(BT_JOYSTICK);
    gpio_set_dir(BT_JOYSTICK, GPIO_IN);

    gpio_init(BT_A);
    gpio_pull_up(BT_A);
    gpio_set_dir(BT_A, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BT_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BT_A, GPIO_IRQ_EDGE_FALL, true);


    gpio_set_function(LED_BLUE, GPIO_FUNC_PWM);
    gpio_set_function(LED_RED, GPIO_FUNC_PWM);

    pwm_set_wrap(pwm_gpio_to_slice_num(LED_BLUE), 4096);
    pwm_set_wrap(pwm_gpio_to_slice_num(LED_RED), 4096);

    pwm_set_enabled(pwm_gpio_to_slice_num(LED_BLUE), true);
    pwm_set_enabled(pwm_gpio_to_slice_num(LED_RED), true);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    ssd1306_init(&oled, 128, 64, false, OLED_ADDR, I2C_PORT);
    ssd1306_fill(&oled, false);
    ssd1306_config(&oled);
    ssd1306_send_data(&oled);

}
void read_joystick_axys() //lê os potenciometros e converte seus valores
{
    adc_select_input(1);
    x_axys_value = adc_read();
    x_axys_pwm_value = abs(x_axys_value - x_axys_center);
    adc_select_input(0);

    y_axys_value = adc_read();
    y_axys_pwm_value = abs(y_axys_value - y_axys_center);

    if((x_axys_pwm_value) < 50 ){ // tolerancia do potenciometro.
        x_axys_pwm_value = 0;
    }

    if((y_axys_pwm_value) < 50 ){
        y_axys_pwm_value = 0;
    }
}

void adjust_pwm_slice()//atualiza o PWM no canal
{

    if (button_a_state)
    {
        pwm_set_gpio_level(LED_RED, y_axys_pwm_value * 2);
        pwm_set_gpio_level(LED_BLUE, x_axys_pwm_value * 2);
    }
}

void update_display() {
    // Mapeia os valores do joystick para a posição do quadrado
    uint8_t x = x_axys_value * 120 / 4095;
    uint8_t y = (y_axys_value * -56 / 4095+120) ;

    // Limpa o display
    ssd1306_fill(&oled, false);
    ssd1306_rect(&oled, y, x, 8, 8, true, true);
    if (button_joystick_state) {
        ssd1306_rect(&oled, 0, 0, 127, 63, 1, false);
    } else {
        ssd1306_rect(&oled, 0, 0, 127, 63, 1, false);
        ssd1306_rect(&oled, 2, 2, 123, 59, 1, false);
        ssd1306_rect(&oled, 4, 4, 119, 55, 1, false);
    }
    ssd1306_send_data(&oled);
}

int main()
{
    stdio_init_all();
    setup();

    //pega o primeiro valor de centro do joystick, para ficar apagado no centro.
    adc_select_input(1);
    x_axys_center = adc_read();
    adc_select_input(0);
    y_axys_center = adc_read();

    // pega o primeiro valor de referencia para o debounce.
    debounce_bt_a = get_absolute_time();
    debounce_joystick = get_absolute_time();

    while (true)
    {
        read_joystick_axys(); // le os ADs referente ao joystick.
        adjust_pwm_slice();
        update_display();
    }
}