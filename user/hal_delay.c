#include "tim.h"
#include "hal_delay.h"



//Î¢ÃëÑÓÊ±
void delay_us(uint32_t delay_time)
{
	__HAL_TIM_SET_COUNTER(&htim3,1);	
	HAL_TIM_Base_Start(&htim3);
	uint32_t temp = delay_time;
	while(temp >= htim3.Instance->CNT);
	HAL_TIM_Base_Stop(&htim3);
}

