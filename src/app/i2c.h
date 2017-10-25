
#ifndef _I2C_H
#define _I2C_H
#include <stm32f10x.h>


	

	

#define DEV1								0x50


#define DOWNLOAD_FLAG_ADDR						0x0208	//1 byte
#define UPDATA_FLAG_ADDR						0x0209	//1 byte
#define APP_SIZE_INFO_ADDR              		0x08D5	//10 bytes
#define APP_SIZE_INFO_ADDR_SIZE					10


bool i2c_start(void);
void i2c_stop(void);
bool i2c_wait_ack(void);
void i2c_ack(void);
void i2c_no_ack(void);
void i2c_send_byte(uint8_t byte);
uint8_t i2c_rece_byte(void);
bool ee_byte_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t ch);
uint8_t ee_random_read(uint8_t dev_addr, uint16_t ee_addr);
bool ee_page_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len);
bool ee_sequential_read(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len);



#endif
