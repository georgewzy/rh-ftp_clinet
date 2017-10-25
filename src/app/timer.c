/*************************************************
  Copyright (C), 1988-1999,  Tech. Co., Ltd.
  File name:      
  Author:       Version:        Date:
  Description:   
  Others:         
  Function List:  
    1. ....
  History:         
                   
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#include <stm32f10x_tim.h>
#include "timer.h"


volatile uint32_t g_tim_cnt[(uint8_t)tim_max] = {0};
volatile uint32_t g_delay_cnt = 0;
uint32_t gt = 0;

/*
*Function: TIM2_IRQHandler      
*Description: 定时器2中断服务程序
*Calls: 
*Data Accessed:  
*Data Updated: g_tim_cnt   
*Input: 无
*Output: 无
*Return: 成功TRUE 失败FALSH    
*Others: 无    
*/
void TIM2_IRQHandler(void)
{
	uint16_t i = 0;

	if (SET == TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
		
		if (g_delay_cnt != 0x00)
		{
			g_delay_cnt--;
		}
		
		gt++;
		if(gt == 1000)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		}
		if(gt == 2000)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_3);
			gt = 0;
		}
        for (i = 0; i<(uint8_t)tim_max; i++)
        {
            g_tim_cnt[i]++;
        }					  
    }
}


void delay_1ms(uint32_t ms)
{
	g_delay_cnt = ms;		 
	while(g_delay_cnt != 0);					//
}

/*
*Function: is_timeout_1MS      
*Description: //如果index对应的时间超时，清零并返回1，未超时返回0
*Calls: 
*Data Accessed: g_tim_cnt 
*Data Updated: g_tim_cnt   
*Input: 无
*Output: 无
*Return: 定时到返回1 或者返回0  
*Others: 无    
*/
int is_timeout_1MS(uint8_t type, uint32_t count)
{
	int status = 0;
	
	if (g_tim_cnt[(uint8_t)(type)] >= count)
	{
		g_tim_cnt[(uint8_t)(type)] = 0;
		status = 1;
	}
	else
	{
		status = 0;
	}
	
	return 0;
}











