#ifndef __DS18B20FORSTM32_H__
#define __DS18B20FORSTM32_H__

#include <stdint.h>

#define DS18B20_DQ_PIN GPIO_PIN_11
#define DS18B20_DQ_PORT GPIOA

void delay_us(uint32_t i); //us��ʱ��
void delay_ms(uint32_t i); //ms��ʱ��

uint8_t DS10B20Init(void); //DS10B20�¶ȴ�����ģ���ʼ�� temperature sensor initilize
void writeByte(uint8_t data);
uint8_t readByte(void);

void outputTemperature(void); //��DS10B20ģ��ӿ��ж�����Ӧ���¶�



#endif /* DS18B20FORSTM32_H */
