#ifndef __MPSUART_H
#define __MPSUART_H

#include <stm32f10x.h>
#include "stm32f10x_usart.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

void init_USART3_output();
void USART3_Print(const char* msg);
int mscanf_int(char* buff, int num);

#endif