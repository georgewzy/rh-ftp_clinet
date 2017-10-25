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
#ifndef _GPRS_H
#define _GPRS_H
#include <stm32f10x.h>







#define GPRS_STATUS_0						0
#define GPRS_STATUS_1						1
#define GPRS_STATUS_2						2
#define GPRS_STATUS_3						3
#define GPRS_STATUS_4						4
#define GPRS_STATUS_5						5
#define GPRS_STATUS_6						6
#define GPRS_STATUS_7						7
#define GPRS_STATUS_8						8
#define GPRS_STATUS_9						9
#define GPRS_STATUS_10						10
#define GPRS_STATUS_11						11



uint8_t send_at_cmd(uint8_t *cmd, uint8_t *str, uint32_t timeout);
uint8_t gprs_init(void);

#endif

