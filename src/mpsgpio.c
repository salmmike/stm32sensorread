#include "mpsgpio.h"
#include <stm32f10x_rcc.h>

void init_GPIO_output(GPIO_TypeDef* PORT, uint32_t PIN)
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

void init_GPIO_input(GPIO_TypeDef* PORT, uint32_t PIN)
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