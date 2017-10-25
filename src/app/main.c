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
#include <stm32f10x.h>
#include "bsp.h"
#include "common.h"
#include "flash.h"
#include "updata.h"
#include "gprs.h"
#include "ftp.h"
#include "timer.h"


extern uint8_t gprs_status;


int main(void)
{
	uint8_t ret = 0, check_sum = 0;
	uint8_t status = 0;
	uint8_t file_name[20] = {0};
	int app_size = 0;
	
	bsp_init();	
	
	delay_1ms(1000);
	
	while (1)
  	{			
		iwatchdog_clear();
		
		switch (status)
		{
			case 0:			
				ret = gprs_init();
				if (ret == 11)
				{
					gprs_status = 0;
					check_sum = 0;
					status = 1;
				}
			break;
				
			case 1:		
				ret = ftp_download_ini(file_name);
				if (ret == TRUE)		//下载成功
				{
					status = 2;
				}
				else
				{
					status = 0;
				}				
			break;
				
			case 2:
				flash_page_erase(APP_INFO_BASE_ADDR, APP_INFO_SECTOR_SUM);
				flash_page_erase(STORAGE_ADDR, STORAGE_SECTOR_SUM);
				status = 3;
			break;

			case 3:				
				ret = ftp_download_app(file_name, &check_sum);
				if (ret == TRUE)
				{
					status = 4;
				}
				else	//下载出错 
				{				
					status = 0;
				}							
			break;
				
			case 4:	
			{	
				uint8_t tmp[10] = {0};
				app_size = ftp_size("SIZE", file_name);
				if (check_sum == get_app_check_sum(STORAGE_ADDR, app_size))	// 
				{			
					itoa(app_size, (char*)tmp, 10);//转换成字符串，以供写入flash
					flash_write_data(APP_INFO_BASE_ADDR+APP_SIZE_INFO_OFFSET, tmp, APP_SIZE_INFO_SIZE);//写入文件大小					
			
					flash_page_erase(FLAG_BASE_ADDR, FLAG_SECTOR_SUM);
					flash_write_byte(FLAG_BASE_ADDR+IS_UPDATA_FLAG_OFFSET, 0x01);
					flash_write_byte(FLAG_BASE_ADDR+DOWNLOAD_FINISH_FLAG_OFFSET, 0x01);
					status = 255;
				}
				else	 
				{
					status = 0;
				}
			}
			break;
				
			case 255:
				system_reset();
			break;
				
			default:
			break;
		}
	
	}
}






