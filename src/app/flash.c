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
#include <stm32f10x_flash.h>
#include "flash.h"
#include "common.h"


/*
*Function: flash_erase      
*Description: 擦除扇区操作  
*Calls:  无
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	start_addr：扇区的开始地址
	sector_sum：要擦除的扇区数量
*Output: 无 
*Return: void        
*Others:  start_addr必选是每个扇区的开始地址，STM32F103C8每个扇区为1k   
*/
void flash_page_erase(uint32_t start_addr, uint32_t sector_sum)
{

	FLASH_Status flash_status = FLASH_COMPLETE;
	uint32_t i = 0;	

	FLASH_Unlock();

	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

	for (i=0; (i<sector_sum) && (flash_status==FLASH_COMPLETE); i++)
	{
		flash_status = FLASH_ErasePage(start_addr + (PAGE_SIZE * i));
	}
	
	FLASH_Lock();
	
}

/*
*Function: flash_write_data      
*Description: 向flash中的某个地址连续写入size字节大小的数据 
*Calls:  无
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	addr：写入的开始地址
	data：写入的数据
	size：写入的数据大小
*Output: 无 
*Return: 成功TRUE  失败FLASH        
*Others: 无    
*/
bool flash_write_data(uint32_t addr, uint8_t *data, uint16_t size)
{
	uint16_t *tmp_data = (uint16_t *)data;
	uint32_t tmp_addr = addr;	
	
	FLASH_Unlock();
	for (tmp_addr=addr; tmp_addr<(addr + size); tmp_data++, tmp_addr += 2)
	{
		FLASH_ProgramHalfWord(tmp_addr, *tmp_data);	
		if (*tmp_data != *(uint16_t *)tmp_addr)	 		// 验证要写入的数据 和 已经写入的数据 是否相等
		{
			return FALSE;								// 
		}
	}
	FLASH_Lock();
		
	return TRUE;
}


/*
*Function: flash_read_data      
*Description: 从FLASH中某个地址开始连续读size个字节大小的数据 
*Calls: 无
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	start_addr：读取的开始地址
	size：要读取的数据大小	
*Output: pdata：写入的数据
*Return: void      0  
*Others: 无    
*/
void flash_read_data(uint32_t addr, uint8_t *pdata, uint32_t size)
{
	uint32_t end_addr = addr + size;
	uint32_t tmp_addr = addr;
	uint32_t i = 0;

	while (tmp_addr < end_addr) 
	{
		pdata[i++] = (*(uint32_t*)tmp_addr);
		tmp_addr++;
	}
}

/*
*Function: flash_write_byte      
*Description: 向FLASH中某个地址写入一个字节大小的数据
*Calls: 无
*Data Accessed: 无  
*Data Updated: 无    
*Input: 
	addr：写入的开始地址
	ch：写入的数据
*Output: 无 
*Return: 成功TRUE  失败FALSH       
*Others: 无    
*/
bool flash_write_byte(uint32_t addr, uint8_t ch)
{
	
	FLASH_Unlock();
	FLASH_ProgramHalfWord(addr, ch);
	if (ch != *(uint16_t *)addr)	 		
	{
		return FALSE;								
	}
	
	FLASH_Lock();
		
	return TRUE;
}


/*
*Function: flash_read_byte      
*Description: 从FLASH中某个地址读取一个字节大小的数据 
*Calls:   
*Data Accessed: 无  
*Data Updated: 无    
*Input: addr：读取的开始地址    
*Output: 无 
*Return: value：读取的数据        
*Others: 无    
*/
uint8_t flash_read_byte(uint32_t addr)
{
	uint8_t value;

	value = *(uint32_t*)addr;

	return value;
}





