#include "mpsuart.h"

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

    int start = 0;
    while(buff[start + 1] != 0) {
        ++start;
    }

    //buff[start + i + 1] = 0;
    for (int k = i - 1; k >= 0; --k) {
        buff[start + i - k] = temp[k];
    }
    return i + 1;
}

void init_USART3_output()
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

void USART3_Print(const char* msg)
{
    for (unsigned int i = 0; i < 4098; ++i) {
        while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
        USART_SendData(USART3, msg[i]);
        if (msg[i] == 0) {
            break;
        }
    }
    while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
    USART_SendData(USART3, '\r');
    while (!USART_GetFlagStatus(USART3, USART_FLAG_TXE));
    USART_SendData(USART3, '\n');
}