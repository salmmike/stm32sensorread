#ifndef __MPSDHT11_H
#define __MPSDHT11_H

#include <stm32f10x.h>

#define ZERO_BIT_MAX_T 120
#define DHT11_PIN GPIO_Pin_5
#define DHT11_PORT GPIOA

int DHT11_Read(int* temperature, int* humidity);

#endif