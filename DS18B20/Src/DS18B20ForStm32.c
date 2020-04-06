#include <stdio.h>
#include "gpio.h"
#include "DS18B20ForStm32.h"

//用于实现us的定时时间
void delay_us(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9*i;         //??????, 72MHZ?
    SysTick->CTRL=0X01;         //??,???????,???????
    SysTick->VAL=0;                //?????
    do
    {
        temp=SysTick->CTRL;           //????????
    }
    while((temp&0x01)&&(!(temp&(1<<16))));     //??????
    SysTick->CTRL=0;    //?????
    SysTick->VAL=0;        //?????
}

//用于实现ms的定时时间
void delay_ms(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9000*i;      //??????, 72MHZ?
    SysTick->CTRL=0X01;        //??,???????,???????
    SysTick->VAL=0;            //?????
    do
    {
        temp=SysTick->CTRL;       //????????
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //??????
    SysTick->CTRL=0;    //?????
    SysTick->VAL=0;        //?????
}

GPIO_InitTypeDef GPIO_InitDsStruct;

void gpio_set_input(void)
{
	GPIO_InitDsStruct.Pin = DS18B20_DQ_PIN;
	GPIO_InitDsStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitDsStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DS18B20_DQ_PORT, &GPIO_InitDsStruct);
}

void gpio_set_output(void)
{
	GPIO_InitDsStruct.Pin = DS18B20_DQ_PIN;
	GPIO_InitDsStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitDsStruct.Pull = GPIO_NOPULL;
	GPIO_InitDsStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DS18B20_DQ_PORT, &GPIO_InitDsStruct);
}

uint8_t DS10B20Init(void)
{
	gpio_set_output();
	HAL_GPIO_WritePin (DS18B20_DQ_PORT, DS18B20_DQ_PIN, 0);
	delay_us(480);
	
	gpio_set_input();
	delay_us(80);
	
	if(!(HAL_GPIO_ReadPin(DS18B20_DQ_PORT, DS18B20_DQ_PIN)))
	{
		delay_us(400);
		return 0;
	}
	else
	{
		delay_us(400);
		return 1;
	}
}

void writeByte(uint8_t data)
{
	gpio_set_output();
	for(int i=0; i<8; i++)
	{
		if((data & (1 << i)) != 0)
		{
			gpio_set_output();
			HAL_GPIO_WritePin (DS18B20_DQ_PORT, DS18B20_DQ_PIN, 0);
			delay_us(1);
			gpio_set_input();
			delay_us(60);
		}
		else
		{
			gpio_set_output();
			HAL_GPIO_WritePin (DS18B20_DQ_PORT, DS18B20_DQ_PIN, 0);
			delay_us(60);
			gpio_set_input();
		}
	}
}

uint8_t readByte(void)
{
	uint8_t value = 0;
	gpio_set_input();
	
	for(int i=0; i<8; i++)
	{
		gpio_set_output();
		
		HAL_GPIO_WritePin (DS18B20_DQ_PORT, DS18B20_DQ_PIN, 0);
		delay_us(2);
		
		gpio_set_input();
		if(HAL_GPIO_ReadPin(DS18B20_DQ_PORT, DS18B20_DQ_PIN))
		{
			value |= 1 << i;
		}
		delay_us(60);
	}
	return value;
}

void outputTemperature(void)
{
	printf("Begin get temperature from DS10B20 sensor !!!\n");
	uint8_t tempL, tempH;
	uint16_t temp;
	float temperature;
	
	uint8_t test1 = DS10B20Init(); //初始化DS18B20芯片
	//printf("DS10B20Init first = %d \n",test1);
	writeByte(0xcc); //skip rom
	writeByte(0x44); //
	delay_ms(800);
	
	test1 = DS10B20Init();
	//printf("DS10B20Init second = %d \n",test1);
	writeByte(0xcc); //skip rom
	writeByte(0xbe); 
	
	tempL = readByte();
	tempH = readByte();
	temp = (tempH << 8)|tempL	;
	temperature = (float) temp/16;
	
	printf("temperature = %f\n",temperature);
}