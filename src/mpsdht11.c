#include "mpsdht11.h"
#include "mpsuart.h"
#include "mpsdelay.h"
#include "mpsgpio.h"

static uint8_t DHT11_ReadBit()
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

int DHT11_Read(int* temperature, int* humidity)
{
    uint8_t data[5] = {0};

    init_GPIO_output(DHT11_PORT, DHT11_PIN);
    GPIO_ResetBits(DHT11_PORT, DHT11_PIN);
    mpsdelay(20);
    GPIO_SetBits(DHT11_PORT, DHT11_PIN);
    mpsudelay(24);
    init_GPIO_input(DHT11_PORT, DHT11_PIN);
    while (!GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}
    while (GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)) {}

    for (int i = 0; i < 5; ++i) {
        data[i] = DHT11_ReadByte();
    }

    int sum = 0;

    for (int i = 0; i < 5; ++i) {
        if (i != 4) {
            sum += data[i];
        }
    }
    *humidity = data[0];
    *temperature = data[2];

    return sum == data[4];
}
