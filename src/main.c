#include "main.h"
#include "stm32f10x_usart.h"

static void delay(int millis) {
    while (millis-- > 0) {
        volatile int x = 5971;
        while (x-- > 0) {
            __asm("nop");
        }
    }
}


static void udelay(int micro) {
    while (micro-- > 0) {
        volatile int x = 59;
        while (x-- > 0) {
            __asm("nop");
        }
    }
}

static void init_GPIO_output(GPIO_TypeDef* PORT, uint32_t PIN)
{
    // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable clock on APB2
    if (PORT == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    }

    GPIO_InitStructure.GPIO_Pin = PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(PORT, &GPIO_InitStructure);
}

static void init_USART3_output()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);
}

static void USART3_Print(const char* msg, unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i) {
        while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
        USART_SendData(USART3, msg[i]);
    }
}

int main(void) {

    const char* data = "Hello\r\n";
    init_GPIO_output(GPIOC, GPIO_Pin_13);
    init_USART3_output();

    // main loop
    while(1) {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);    // turn the LED on
        delay(DELAY);
        USART3_Print(data, 8);

        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // turn the LED off
        delay(DELAY);
    }
}
