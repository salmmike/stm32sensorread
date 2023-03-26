#include "main.h"
#include "stm32f10x_usart.h"

#define DHT11_PIN GPIO_Pin_5
#define DHT11_PORT GPIOA
#define LED_Pin GPIO_Pin_13
#define ZERO_BIT_MAX_T 120

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
    if (PORT == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    }
    else if (PORT == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);
    }
    else if (PORT == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    }

    GPIO_InitStructure.GPIO_Pin = PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(PORT, &GPIO_InitStructure);
}

int mscanf_int(char* buff, int num)
{
    int i = 0;
    char temp[20] = "";
    do {
        char c = '0';
        c += num % 10;
        temp[i] = c;
        num /= 10;
        ++i;
    } while (num);
    for (int k = i; k >= 0; --k) {
        buff[i-k] = temp[k];
    }
    return i + 1;
}

static void init_GPIO_input(GPIO_TypeDef* PORT, uint32_t PIN)
{
    // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable clock on APB2
    if (PORT == GPIOA) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    }
    else if (PORT == GPIOB) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);
    }
    else if (PORT == GPIOC) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    }

    GPIO_InitStructure.GPIO_Pin = PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
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

static void USART3_Print(const char* msg)
{
    for (unsigned int i = 0; i < 4098; ++i) {
        while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
        USART_SendData(USART3, msg[i]);
        if (msg[i] == '\n') {
            break;
        }
    }
}

static short DHT11_ReadBit()
{
    unsigned int deltaT = 0;

    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {
        deltaT++;
    }
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}
    return deltaT > ZERO_BIT_MAX_T;
}

static uint8_t DHT11_ReadByte()
{
    uint8_t output = 0;
    for (int i = 0; i < 8; ++i) {
        output |= DHT11_ReadBit() << (7-i);
        while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}
    }

    return output;
}

static int DHT11_Read(int* temperature, int* humidity)
{
    uint8_t data[5] = {0};

    USART3_Print("Start read\r\n");
    init_GPIO_output(DHT11_PORT, DHT11_PIN);
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    delay(20);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    udelay(20);
    init_GPIO_input(DHT11_PORT, DHT11_PIN);
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}


    for (int i = 0; i < 5; ++i) {
        data[i] = DHT11_ReadByte();
    }
    USART3_Print("5 bytes read\r\n");

    for (int i = 0; i < 5; ++i) {
        char datastr[100] = "Databyte: ";
        int s = mscanf_int(&datastr[10], data[i]);
        datastr[10 + s] = '\r';
        datastr[11 + s] = '\n';
        USART3_Print(datastr);
    }

    return 0;
}

int main(void) {

    //const char* data = "Hello\r\n";
    init_GPIO_output(GPIOC, LED_Pin);
    init_USART3_output();
    char test[100] = "NUM: ";
    int s = mscanf_int(&test[5], 100890);
    test[5 + s] = '\r';
    test[5 + s + 1] = '\n';

    USART3_Print(test);
    // main loop
    int temp, hum;
    while(1) {
        GPIO_SetBits(GPIOC, LED_Pin);    // turn the LED on
        delay(DELAY);
        DHT11_Read(&temp, &hum);
        GPIO_ResetBits(GPIOC, LED_Pin);  // turn the LED off
        delay(DELAY*10);
    }
}
