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

#ifndef _UART_H
#define _UART_H
#include <stm32f10x.h>



#define UART_BUFF_LENGHT				   	512

typedef struct
{
	uint16_t index;	
	uint8_t	 data[UART_BUFF_LENGHT];	
} SerialBuffType;

#define SerialBuffDefault() {0,0,{0}}




void receive_data(uint8_t *rece_data, uint8_t *data);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif




