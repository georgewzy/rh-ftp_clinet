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
*************************************************/
#include <stm32f10x_usart.h>
#include <string.h>
#include <stdarg.h>
#include "uart.h"
#include "bsp.h"
#include "common.h"
#include "timer.h"


extern uint8_t send_at_flag;

static SerialBuffType sb = SerialBuffDefault();
SerialBuffType  *rece_buff2 = &sb;
uint8_t rece_over_flag = 0;


/*
*Function: USART2_IRQHandler      
*Description: 串口2中断服务程序
*Calls: 无  
*Data Accessed: 
	rece_buff2
	send_at_flag
	rece_over_flag
*Data Updated: 
	rece_buff2
	rece_over_flag
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void USART2_IRQHandler(void)
{
	u8 ch = 0;	

	send_at_flag = 0;
	
   	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {   
	    USART_ClearITPendingBit(USART2, USART_IT_RXNE);	
        ch = USART_ReceiveData(USART2);	
		rece_over_flag = 0;
		IS_TIMEOUT_1MS(tim_uart2, 0);
        
		if (rece_buff2->index < UART_BUFF_LENGHT)
		{
			rece_buff2->data[rece_buff2->index++] = ch;
		}
		else
		{
			rece_over_flag = 1;
			rece_buff2->index = 0;
		}
	}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)                  
  	{ 
     	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);					   
  	}		
}


/*
*Function: receive_data      
*Description: 接收串口数据  
*Calls: IS_TIMEOUT_1MS 
*Data Accessed: rece_over_flag  
*Data Updated: rece_over_flag 
*Input: rece_data：串口缓冲区数据
*Output: data：接收到的数据
*Return: 无     
*Others: 无    
*/
void receive_data(uint8_t *rece_data, uint8_t *data)
{
	if (IS_TIMEOUT_1MS(tim_uart2, 40))
	{
		rece_over_flag = 1;
	}
	
	if (rece_over_flag == 1)
	{
		memcpy(data, rece_data, UART_BUFF_LENGHT);
		rece_over_flag = 0;
	}
	
}



/*
*Function: system_reset      
*Description: system reset  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
{ 
	const char *s;
    int d;  
    char buf[16];
    
	va_list ap;
    __va_start(ap, Data);

	while(*Data != '\0')
	{				                         
		if(*Data==0x5c)
		{									
			switch (*++Data)
			{
				case 'r':							          
					USART_SendData(USARTx, 0x0d);	   
					Data++;
				break;
				case 'n':							          
					USART_SendData(USARTx, 0x0a);	
					Data++;
				break;
				
				default:
					Data++;
			    break;
			}						 
		}
		else if(*Data=='%')
		{									  //
			switch (*++Data){				
				case 's':										  
                	s = __va_arg(ap, const char *);
                	for ( ; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
            	case 'd':										 
                	d = __va_arg(ap, int);
                	itoa(d, buf, 10);
                	for (s = buf; *s; s++) 
					{
                    	USART_SendData(USARTx,*s);
						while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
                	}
					Data++;
                	break;
				default:
					Data++;
				    break;
			}		 
		}
		else 
			USART_SendData(USARTx, *Data++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
	}
}


















