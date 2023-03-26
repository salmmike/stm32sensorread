#include "main.h"
#include "mpsgpio.h"
#include "mpsdht11.h"
#include "mpsdelay.h"
#include "mpsuart.h"

#include "stm32f10x_usart.h"
#include "stm32f10x_i2c.h"

#define LED_Pin GPIO_Pin_13

int main(void) {

    //const char* data = "Hello\r\n";
    init_GPIO_output(GPIOC, LED_Pin);
    init_USART3_output();
    char test[35] = "NUMBER: ";
    mscanf_int(test, 123456789);
    USART3_Print(test);
    int temp, hum;

    // main loop
    while(1) {
        GPIO_SetBits(GPIOC, LED_Pin);    // turn the LED on
        mpsdelay(DELAY);
        DHT11_Read(&temp, &hum);
        GPIO_ResetBits(GPIOC, LED_Pin);  // turn the LED off
        USART3_Print("");
        mpsdelay(DELAY*10);
    }
}
