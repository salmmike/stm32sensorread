#include "main.h"
#include "mpsgpio.h"
#include "mpsdht11.h"
#include "mpsdelay.h"
#include "mpsuart.h"

#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

#define LED_Pin GPIO_Pin_13
#define LCD_I2C_ADDRESS 0x4E

static void I2C1_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C1);
    I2C_Cmd(I2C1, ENABLE);

    I2C_InitTypeDef I2C_InitStruct;
    I2C_StructInit(&I2C_InitStruct);
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = LCD_I2C_ADDRESS;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C1, &I2C_InitStruct);
}


void I2C1_Send(uint8_t data, uint8_t address)
{
    I2C_GenerateSTART(I2C1, ENABLE);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, address);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
}


void I2C1_SendBuffer(uint8_t* data, unsigned int size, uint8_t address)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    for (unsigned int i = 0; i < size; ++i) {
        I2C_SendData(I2C1, data[i]);
            while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
        while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
}

void lcd_send_cmd (char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = cmd & 0xf0;
    data_l = (cmd << 4) & 0xf0;
    data_t[0] = data_u | 0x0C;
    data_t[1] = data_u | 0x08;
    data_t[2] = data_l | 0x0C;
    data_t[3] = data_l | 0x08;
    I2C1_SendBuffer(data_t, 4, LCD_I2C_ADDRESS);
}

void lcd_send_data (char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = data & 0xf0;
    data_l = (data << 4) & 0xf0;
    data_t[0] = data_u | 0x0D;
    data_t[1] = data_u | 0x09;
    data_t[2] = data_l | 0x0D;
    data_t[3] = data_l | 0x09;
    I2C1_SendBuffer(data_t, 4, LCD_I2C_ADDRESS);
}

void lcd_clear()
{
    lcd_send_cmd(0x1);
}

void lcd_set_cursor(int row, int column)
{
    lcd_send_cmd(column | (row == 0) ? 0x80 : 0xC0);
}

void lcd_init()
{
    mpsdelay(50);
    lcd_send_cmd(0x30);
    mpsdelay(5);
    lcd_send_cmd(0x30);
    mpsdelay(1);
    lcd_send_cmd(0x30);
    mpsdelay(10);
    lcd_send_cmd(0x20);
    mpsdelay(10);

    lcd_send_cmd(0x28);
    mpsdelay(1);
    lcd_send_cmd(0x08);
    mpsdelay(1);
    lcd_send_cmd(0x01);
    mpsdelay(2);
    lcd_send_cmd(0x06);
    mpsdelay(1);
    lcd_send_cmd(0x0C);
}

void lcd_send_string(char* string)
{
    while (*string) {
        lcd_send_data(*string);
        ++string;
    }
}

int main(void) {

    init_GPIO_output(GPIOC, LED_Pin);
    init_USART3_output();
    int temp, hum;
    I2C1_Init();
    USART3_Print("I2C Init done");
    lcd_init();
    USART3_Print("LCD Init done");
    lcd_clear();

    lcd_send_cmd(0x80);

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
