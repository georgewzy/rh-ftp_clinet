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
#ifndef _UPDATA_H
#define _UPDATA_H
#include <stm32f10x.h>








uint32_t get_app_size(void);
uint8_t get_app_check_sum(uint32_t addr, uint32_t data_len);
#endif
