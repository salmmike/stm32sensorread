#include "main.h"
#include "mpsgpio.h"
#include "mpsdht11.h"
#include "mpsdelay.h"
#include "mpsuart.h"
#include "mpslcd.h"

#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#define LED_Pin GPIO_Pin_13

int main(void) {

    init_GPIO_output(GPIOC, LED_Pin);
    init_USART3_output();
    int temp, hum;
    lcd_init();
    lcd_clear();

    lcd_set_cursor(0, 0);

    // main loop
    while(1) {
        GPIO_SetBits(GPIOC, LED_Pin);
        mpsdelay(DELAY);
        DHT11_Read(&temp, &hum);
        char datastr[100] = "Temperature: ";
        mscanf_int(datastr, temp);

        USART3_Print(datastr);
        char datastr2[100] = "Humidity: ";
        mscanf_int(datastr2, hum);
        lcd_clear();
        lcd_set_cursor(0, 0);

        USART3_Print(datastr2);

        lcd_send_string(datastr);
        lcd_set_cursor(1, 0);
        lcd_send_string(datastr2);

        GPIO_ResetBits(GPIOC, LED_Pin);
        USART3_Print("");
        mpsdelay(DELAY*10);
    }
}
