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
#ifndef _FLASH_H
#define _FLASH_H
#include <stm32f10x.h>


// STM32F103C8 每个扇区 1k
#define PAGE_SIZE								0x400 			// 1k

// FTP区 sector7 ~ sector24
#define FTP_CLINET_ADDR							0x8001C00	// 
#define FTP_CLINET_SECTOR_SUM					18

// APP区 sector25 ~ sector75
#define APP_ADDR								0x8006400	 	//  
#define APP_SECTOR_SUM							50  		// 50k

//  存储区 sector76 ~ sector124
#define STORAGE_ADDR							0x8012C00		// 0x8012C00 ~ 0x801F3FF		
#define STORAGE_SECTOR_SUM						50			// 50k

// 标志位区 sector125
#define FLAG_BASE_ADDR							0x801F400		// sector 125 的地址
#define FLAG_SECTOR_SUM							1
#define IS_UPDATA_FLAG_OFFSET					0		
#define IS_UPDATA_FLAG_SIZE						2
#define DOWNLOAD_FINISH_FLAG_OFFSET				2
#define DOWNLOAD_FINISH_FLAG_SIZE				2

// APP信息描述区 sector126
#define APP_INFO_BASE_ADDR						0x801F800		// sector 126 的地址
#define	APP_INFO_SECTOR_SUM						1
#define APP_SIZE_INFO_OFFSET					0
#define APP_SIZE_INFO_SIZE						10
#define SOFTWARE_VERSION_INFO_ADDR				0x801F820



void flash_page_erase(uint32_t start_addr, uint32_t sector_sum);
bool flash_write_data(uint32_t addr, uint8_t *data, uint16_t size);
void flash_read_data(uint32_t addr, uint8_t *pdata, uint32_t size);
bool flash_write_byte(uint32_t addr, uint8_t ch);
uint8_t flash_read_byte(uint32_t addr);
#endif
