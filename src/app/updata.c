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
#include <string.h>
#include <stdlib.h>
#include "updata.h"
#include "flash.h"


/*
*Function: get_app_size      
*Description: 从flash中获取app的大小 
*Calls: flash_read_data：
*Data Accessed: 无 
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
uint32_t get_app_size(void)
{
	uint32_t app_size = 0;
	uint8_t tmp[10] = {0};
	
	flash_read_data(APP_INFO_BASE_ADDR+APP_SIZE_INFO_OFFSET, tmp, APP_SIZE_INFO_SIZE);
	app_size = atoi((char*)tmp);
	
	return app_size;
}

/*
*Function: get_app_check_sum      
*Description: 校验APP程序
*Calls: 
	get_app_size
	flash_read_byte
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 成功TRUE 失败FALSH     
*Others: 无    
*/
uint8_t get_app_check_sum(uint32_t addr, uint32_t data_len)
{
	uint32_t i = 0;
	uint8_t tmp_data[2] = {0};
	uint8_t check_sum = 0;
	
	for (i=0; i<data_len; i+=2)
	{
		flash_read_data(addr+i, tmp_data, 2);
		check_sum += tmp_data[0];
		check_sum += tmp_data[1];
	}
	
	return check_sum;
}



