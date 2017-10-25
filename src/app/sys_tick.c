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
#include "sys_tick.h"


volatile uint32_t sys_tick_time;


/*
*Function: delay_1ms      
*Description: 毫秒级延时函数 
*Calls: 无  
*Data Accessed: sys_tick_time  
*Data Updated: sys_tick_time    
*Input: ms：延时计数器
*Output: 无
*Return: 无     
*Others: 无    
*/
void delay_1ms(uint32_t ms)
{
	sys_tick_time = ms;		 
	while(sys_tick_time != 0);					
}

/*
*Function: SysTick_Handler      
*Description: 系统滴答中断服务程序 
*Calls: 无  
*Data Accessed: sys_tick_time  
*Data Updated: sys_tick_time  
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void SysTick_Handler(void)
{
	SysTick->VAL = 0;
	if (sys_tick_time != 0x00)
	{
		sys_tick_time--;
	}
}
