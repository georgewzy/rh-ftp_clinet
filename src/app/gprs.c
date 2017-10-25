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
#include "gprs.h"
#include "bsp.h"
#include "uart.h"
#include "common.h"
#include "timer.h"


extern SerialBuffType  *rece_buff2;

uint8_t gprs_status = 0;
uint8_t send_at_flag = 0;	

/*
*Function: send_at_cmd      
*Description: 发送AT指令 
*Calls: 无  
*Data Accessed: send_at_flag
*Data Updated: send_at_flag    
*Input:
	cmd：AT指令
	str：是否存在str
	timeo：超时
*Output: 无
*Return: 无     
*Others: 无    
*/
uint8_t send_at_cmd(uint8_t *cmd, uint8_t *str, uint32_t timeout)
{	
	uint8_t ret = 0;
	uint8_t tmp_data[512] = {0};

	send_at_flag = 0;
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_gprs, 0);
	while (ret == 0)
	{	
		memset(tmp_data, 0, sizeof(tmp_data));
		receive_data(rece_buff2->data, tmp_data);	
		if (strstr((char*)tmp_data, (char*)str) != NULL)
		{
			ret = 1;
		}
		else
		{		
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, cmd);
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_1ms(800);
			if (IS_TIMEOUT_1MS(tim_gprs, timeout))
			{
				ret = 0;
				break;
			}
		}
		if (IS_TIMEOUT_1MS(tim_at, 2000))
		{
			send_at_flag = 0;
		}
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return ret;
}

/*
*Function: gprs_init      
*Description: 把GPRS初始化成FTP模式
*Calls: 
	gprs_config
	send_at_cmd
*Data Accessed: gprs_status
*Data Updated: gprs_status 
*Input: 无
*Output: 无
*Return: GPRS的状态     
*Others: 无    
*/
uint8_t gprs_init(void)
{
	uint8_t ret = 0;
	
	switch (gprs_status)
	{
		case GPRS_STATUS_0:
			gprs_config();		
			delay_1ms(10000);
			gprs_status = GPRS_STATUS_1;
		break;
	
		case GPRS_STATUS_1:
			ret = send_at_cmd("\r\nAT+CSQ\r\n", "OK", 10000);	
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_2;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case GPRS_STATUS_2:
			ret = send_at_cmd("\r\nAT+CREG=1\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_3;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}	
		break;
		
		case GPRS_STATUS_3:
			ret = send_at_cmd("\r\nAT+CREG?\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_4;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}	
		break;
		
		case GPRS_STATUS_4:
			ret = send_at_cmd("\r\nAT^SICS=0,conType,GPRS0\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_5;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case GPRS_STATUS_5:
			ret = send_at_cmd("\r\nAT^SICS=0,apn,CMNET\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_6;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case GPRS_STATUS_6:
			ret = send_at_cmd("\r\nAT^IPCFL=14,1\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_7;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case GPRS_STATUS_7:
			ret = send_at_cmd("\r\nAT^SISS=1,srvType,ftp\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_8;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case GPRS_STATUS_8:
			ret = send_at_cmd("\r\nAT^SISS=1,conId,0\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_9;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
				
		case GPRS_STATUS_9:
			ret = send_at_cmd("\r\nAT^SISS=1,address,\"ftp://wzy8943:520xiao@010.3vftp.com\"\r\n", "OK", 10000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_10;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
		
		case 10:
			ret = send_at_cmd("\r\nAT^SISO=1\r\n", "OK", 20000);
			if(ret == 1)
			{			
				gprs_status = GPRS_STATUS_11;
			}
			else
			{
				gprs_status = GPRS_STATUS_0;
			}
		break;
	}
	
	return gprs_status;
}










