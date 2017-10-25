/*************************************************
  Copyright (C), RoadLighting Tech. Co., Ltd.
  File name:  main.c    
  Author:       Version:        Date:
  Description:   
  Others:         
  Function List:  
    1. 
  History:                         
    1. Date:
       Author:
       Modification:

*************************************************/
#include <string.h>
#include <stdlib.h>
#include "ftp.h"
#include "uart.h"
#include "flash.h"
#include "common.h"
#include "timer.h"
#include "bsp.h"


extern SerialBuffType  *rece_buff2;
extern uint8_t send_at_flag;

/*
*Function: ftp_size      
*Description:  获取FTP服务器中的文件大小  
*Calls: 无  
*Data Accessed:  send_at_flag
*Data Updated: send_at_flag   
*Input: 
	cmd：FTP SIZE 指令
	file：文件名
*Output: 无
*Return: 文件的大小   
*Others: 无    
*/
int ftp_size(uint8_t *cmd, uint8_t *file)
{
	int len = 0;
	uint8_t ret = 0;
	uint8_t tmp_str[512] = {0}, tmp[20] = {0};

	send_at_flag = 0;
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_ftp, 0);
	while (ret == 0)
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		receive_data(rece_buff2->data, tmp_str);
		if (strstr((char*)tmp_str, "OK") != NULL)
		{
			str_picked(tmp_str, "SIS: 0,4,213,", "\r\n", tmp);
			len = atoi((char*)tmp);
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, "\r\nAT^FTPCMD=1,%s,%s\r\n", cmd, file);
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_1ms(3000);
			if (IS_TIMEOUT_1MS(tim_ftp, 10000))
			{
				ret = 0;
				len = -1;
				break;
			}
		}
		if (IS_TIMEOUT_1MS(tim_at, 2000))
		{
			send_at_flag = 0;
		}
	}	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	
	return len;
}


/*
*Function: ftp_get      
*Description: 从FTP服务器中获取文件  
*Calls: 无  
*Data Accessed: send_at_flag  
*Data Updated: send_at_flag    
*Input: 
	cmd：FTP GET 指令
	file：文件名
*Output: 无
*Return: 无     
*Others: 无    
*/
uint8_t ftp_get(uint8_t *cmd, uint8_t *file)
{
	uint8_t ret = 0;
	uint8_t tmp_data[20] = {0}, tmp_str[512] = {0};
	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_ftp, 0);
	while (ret == 0)
	{
		memset(tmp_str, 0, sizeof(tmp_str));
		receive_data(rece_buff2->data, tmp_str);
		if (strstr((char*)tmp_str, "OK") != NULL)
		{
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, "\r\nAT^FTPCMD=1,%s,%s\r\n", cmd, file);
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_1ms(4000);
			if (IS_TIMEOUT_1MS(tim_ftp, 10000))
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
*Function: ftp_read_data      
*Description: 获取数据  
*Calls: 无  
*Data Accessed: send_at_flag  
*Data Updated: send_at_flag    
*Input: len：要获取数据字节的大小
*Output: data：获取到的数据
*Return: 失败返回-1  成功返回获取到数据的实际大小     
*Others: 无    
*/
int ftp_read_data(uint8_t *data, uint16_t data_len)
{
	uint8_t ret = 0;
	int real_len = 0;
	uint8_t tmp_str[512] = {0}, tmp[20] = {0}, str[30] = {0};
	
	memset(rece_buff2, 0, sizeof(SerialBuffType));
	IS_TIMEOUT_1MS(tim_ftp, 0);
	while (ret == 0)
	{	
		memset(tmp_str, 0, sizeof(tmp_str));
		receive_data(rece_buff2->data, tmp_str);
		if (mem_str((char*)tmp_str, 512, "OK") != NULL)
		{	
			str_picked(tmp_str, "^SISR: 1,", "\r\n", tmp);
			real_len = atoi((char*)tmp);
			if (real_len > 0)
			{	
				strcat((char*)str, "^SISR: 1,");
				strcat((char*)str, (char*)tmp);
				strcat((char*)str, "\r\n");
				mem_picked(tmp_str, str, real_len, data);
			}
			ret = 1;
		}
		else
		{
			memset(rece_buff2, 0, sizeof(SerialBuffType));
			if (send_at_flag == 0)
			{
				USART_OUT(USART2, "\r\nAT^SISR=1,256\r\n");
				send_at_flag = 1;
				IS_TIMEOUT_1MS(tim_at, 0);
			}
			delay_1ms(500);			
			if (IS_TIMEOUT_1MS(tim_ftp, 10000))
			{
				ret = 0;
				real_len = -1;
				break;
			}	
		}
		if (IS_TIMEOUT_1MS(tim_at, 2000))
		{
			send_at_flag = 0;
		}		
	}
	memset(rece_buff2, 0, sizeof(SerialBuffType));

	return real_len;
}


/*
*Function: ftp_download_app      
*Description: 从FTP服务器上下载APP程序 
*Calls: 
	ftp_size
	ftp_get
	ftp_read_data
	flash_write_data
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 成功TRUE 失败FALSH    
*Others: 无    
*/
bool ftp_download_app(uint8_t *file_name, uint8_t *check_sum)
{
	uint8_t ftp_get_status = 0;
	int packet_len = 0, app_size = 0, total_len = 0;
	uint8_t packet_data[256] = {0};
	uint32_t tmp_addr = STORAGE_ADDR;
	
	app_size = ftp_size("SIZE", file_name);
	if (app_size <= 0)
	{
		return FALSE;
	}
	
	ftp_get_status = ftp_get("GET", file_name);
	if (ftp_get_status != 1)
	{
		return FALSE;
	}
	
	while (1)
	{
		iwatchdog_clear();
		
		packet_len = ftp_read_data(packet_data, 256);		
		total_len += packet_len;
		USART_OUT(USART1, "\r\ntotal_len = %d\r\n", total_len);
		if (packet_len > 0)
		{
			uint16_t i = 0;
			for (i=0; i<packet_len; i++)
			{
				*check_sum += packet_data[i];
			}
			flash_write_data(tmp_addr, packet_data, packet_len);
			tmp_addr += packet_len;
			packet_len = 0;
		}
		else if ((packet_len == 0) && (app_size == total_len))
		{
			return TRUE;
		}
		else if ((packet_len == 0) && (app_size != total_len))
		{
			return FALSE;
		}
	}
}


/*
*Function: ftp_download_ini      
*Description: 从FTP服务器上下载配置文件 
*Calls: 
	ftp_size
	ftp_get
	ftp_read_data
	flash_write_data
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 成功TRUE 失败FALSH    
*Others: 无    
*/
bool ftp_download_ini(uint8_t *file_name)
{
	uint8_t ftp_get_status = 0;
	uint8_t packet_data[256] = {0};
	int packet_len = 0, ini_size = 0, total_len = 0;

	ini_size = ftp_size("SIZE", "updata.ini");
	if (ini_size == -1)
	{
		return FALSE;
	}
	
	ftp_get_status = ftp_get("GET", "updata.ini");
	if (ftp_get_status != 1)
	{
		return FALSE;
	}
			
	while (1)
	{	
		iwatchdog_clear();
		
		packet_len = ftp_read_data(packet_data, 256);
		total_len += packet_len;	
		if (packet_len > 0)
		{
			USART_OUT(USART1, "\r\nini_total_len = %d\r\n", total_len);
			str_picked(packet_data, ":file_name=", "\r\n", file_name);
			packet_len = 0;
		}
		else if ((packet_len == 0) && (ini_size == total_len))
		{
			return TRUE;
		}
		else if ((packet_len == 0) && (ini_size != total_len))
		{
			return FALSE;
		}
	}
}















