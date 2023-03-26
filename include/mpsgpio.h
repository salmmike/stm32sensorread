#ifndef __MPSGPIO_H
#define __MPSGPIO_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void init_GPIO_output(GPIO_TypeDef* PORT, uint32_t PIN);
void init_GPIO_input(GPIO_TypeDef* PORT, uint32_t PIN);

#endif