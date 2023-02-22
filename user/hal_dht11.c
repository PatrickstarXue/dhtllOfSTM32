#include "hal_dht11.h"
#include <string.h>
#include "tim.h"
#include "hal_delay.h"

thTypedef_t temphumTypedef;


/****************************************
配置DHT的Dat引脚为输出
****************************************/
void DHT11_IO_OUT(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/****************************************
配置DHT的Dat引脚为输入
****************************************/
void DHT11_IO_IN(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = DHT11_Pin;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/****************************************
复位DHT11
****************************************/
static void dht11Rst(void)
{
    DHT11_IO_OUT(); 			            //SET OUTPUT
    DHT11_DQ_OUT_0; 			            //GPIOA.0=0
    HAL_Delay(20);    			            //Pull down Least 18ms
    DHT11_DQ_OUT_1; 			            //GPIOA.0=1
    delay_us(30);     			            //Pull up 20~40us
}

/****************************************
查询DHT11
返回1 代表出错
****************************************/
static uint8_t dht11Check(void)
{
    uint8_t retry=0;
    DHT11_IO_IN();                                              //SET INPUT
    while (DHT11_DQ_IN && (retry<100))                          //DHT11 Pull down 40~80us
    {
        retry++;
        delay_us(1);
    }

    if(retry >= 100)
    {
        return 1;
    }
    else
    {
        retry=0;
    }

    while (!DHT11_DQ_IN&& (retry < 100))		    //DHT11 Pull up 40~80us
    {
        retry++;
        delay_us(1);
    }

    if(retry >= 100)
    {
        return 1;	                        //check error
    }        

    return 0;
}

/****************************************
读取DHT11的一位数据
****************************************/
static uint8_t dht11ReadBit(void)
{
    uint8_t retry=0;
    while(DHT11_DQ_IN && (retry<100))                           //wait become Low level
    {
        retry++;
        delay_us(1);
    }

    retry = 0;
    while(!DHT11_DQ_IN && (retry < 100))		    //wait become High level
    {
        retry++;
        delay_us(1);
    }

    delay_us(30);//wait 40us

    if(DHT11_DQ_IN)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/****************************************
读取DHT11的一个字节数据
****************************************/
static uint8_t dht11ReadByte(void)
{
    uint8_t i,dat;
    dat=0;
    for (i=0; i<8; i++)
    {
        dat<<=1;
        dat |= dht11ReadBit();
    }

    return dat;
}

/****************************************
读取DHT11数据并校验，错误返回1
****************************************/
static uint8_t dht11ReadData(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t buf[5];
    uint8_t i;
    dht11Rst(); 
    if(0 == dht11Check()) 
    {
        for(i=0; i<5; i++) //读取5个字节的数据
        {
            buf[i] = dht11ReadByte();
        }
        if(buf[4] == (buf[0]+buf[1]+buf[2]+buf[3]))//检验数据是否正确
        {
            *humidity = buf[0];
            *temperature = buf[2];
        }
    }
    else
    {
        return 1;
    }

    return 0;
}

uint8_t dht11Read(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t curTem = 0, curHum = 0;
    uint8_t ret = 0; 
    ret = dht11ReadData(&curTem, &curHum);

		if(ret != 1)
		{
			*temperature=curTem;
			*humidity=curHum;
		}
    return 0;
}
/****************************************
DHT11初始化
****************************************/
uint8_t dht11Init(void)
{
    /* Migrate your driver code */
    dht11Rst(); 
    memset((uint8_t *)&temphumTypedef, 0, sizeof(thTypedef_t)); //把指针的前多少位设置为0
    printf("DHT11 Init Success\r\n"); 
    return dht11Check(); 
}
