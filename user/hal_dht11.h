#ifndef _HAL_DHT11_H
#define _HAL_DHT11_H

#include <stdio.h>
#include "stm32f1xx_hal.h"

//Set GPIO Direction

#define DHT11_Pin GPIO_PIN_13
#define DHT11_GPIO_Port GPIOB
	
#define	DHT11_DQ_OUT_1	 HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define	DHT11_DQ_OUT_0 	 HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)

#define	DHT11_DQ_IN   	 HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)

#define MEAN_NUM            10

typedef struct
{
    uint8_t curI;
    uint8_t thAmount; 
    uint8_t thBufs[10][2];
}thTypedef_t; //����һ���ṹ��洢��εĶ�ȡ��ֵ

/* Function declaration */
uint8_t dht11Init(void); //Init DHT11
uint8_t dht11Read(uint8_t *temperature, uint8_t *humidity); //Read DHT11 Value
static uint8_t dht11ReadData(uint8_t *temperature, uint8_t *humidity);
static uint8_t dht11ReadByte(void);//��һ���ֽ�
static uint8_t dht11ReadBit(void);//��һλ
static uint8_t dht11Check(void);//��ѯDHT11
static void dht11Rst(void);//��λDHT11    
void dht11SensorTest(void);

#endif /*_HAL_HEMP_HUM_H*/

