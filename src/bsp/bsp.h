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
#ifndef _BSP_H
#define _BSP_H
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_iwdg.h>
#include <misc.h>


#define NVIC_VECTOR_ADDR			0x1C00



void iwatchdog_clear(void);
void gprs_config(void);
void bsp_init(void);

#endif



