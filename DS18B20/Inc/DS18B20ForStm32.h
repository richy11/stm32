#ifndef __DS18B20FORSTM32_H__
#define __DS18B20FORSTM32_H__

#include <stdint.h>

#define DS18B20_DQ_PIN GPIO_PIN_11
#define DS18B20_DQ_PORT GPIOA

void delay_us(uint32_t i); //us定时器
void delay_ms(uint32_t i); //ms定时器

uint8_t DS10B20Init(void); //DS10B20温度传感器模块初始化 temperature sensor initilize
void writeByte(uint8_t data);
uint8_t readByte(void);

void outputTemperature(void); //从DS10B20模块接口中读出对应的温度



#endif /* DS18B20FORSTM32_H */
