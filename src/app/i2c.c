
#include "i2c.h"
#include "common.h"
#include "bsp.h"
#include "sys_tick.h"


/*
** 
** ÃèÊö£ºi2c ¿ªÊ¼ĞÅºÅ
**
**
*/ 
bool i2c_start(void)
{
	SDA_HIGH();
	SCL_HIGH();
	delay(5);
	if(!SDA_READ())
	{
		return FALSE;		//SDAÏßÎªµÍµçÆ½Ôò×ÜÏßÃ¦,ÍË³ö
	}
	SDA_LOW();
	if(SDA_READ()) 
	{
		return FALSE; 		//SDAÏßÎª¸ßµçÆ½Ôò×ÜÏß³ö´í,ÍË³ö
	}
	SDA_LOW();
	delay(5);
	return TRUE;
}

/*
** 
** ÃèÊö£ºi2c Í£Ö¹ĞÅºÅ
**
**
*/ 
void i2c_stop(void)
{
	SCL_LOW();
	delay(5);
	SDA_LOW();
	delay(5);
	SCL_HIGH();
	SDA_HIGH();
	delay(5);
}

/*
** 
** ÃèÊö£ºi2c µÈ´ıÓ¦´ğ
**
**
*/ 
bool i2c_wait_ack(void) //·µ»ØÎª:=1ÓĞACK,=0ÎŞACK
{
	SCL_LOW();
	delay(5);
	SDA_HIGH();
	delay(5);
	SCL_HIGH();
	delay(5);
	if(SDA_READ())
	{
		SCL_LOW();
		return FALSE;
	}
	SCL_LOW();
	return TRUE;
}

/*
** 
** ÃèÊö£ºi2c ·¢ËÍÓ¦´ğÎ»
**
**
*/ 
void i2c_ack(void)
{
	SCL_LOW();
	SDA_LOW();
	delay(5);
	SCL_HIGH();
	delay(5);
	SCL_LOW();
	delay(5);
}

/*
** 
** ÃèÊö£ºi2c ÎŞÓ¦´ğ
**
**
*/ 
void i2c_no_ack(void)
{ 
	SCL_LOW();
	SDA_HIGH();
	delay(5);
	SCL_HIGH();
	delay(5);
	SCL_LOW();
	delay(5);
}

/*
** 
** ÃèÊö£ºi2c ·¢ËÍÒ»¸ö×Ö½Ú
**
**
*/ 
void i2c_send_byte(uint8_t byte) //Êı¾İ´Ó¸ßÎ»µ½µÍÎ»//
{
	uint8_t i = 8;
	while (i--)
	{
		SCL_LOW();
		delay(5);
		if (byte&0x80)
		{
			SDA_HIGH();
		}
		else 
		{
			SDA_LOW();
		}
		byte<<=1;
		SCL_HIGH();
		delay(5);
	}
	
	SCL_LOW();
}


/*
** 
** ÃèÊö£ºi2c ½ÓÊÕÒ»¸ö×Ö½Ú
**
**
*/ 
uint8_t i2c_rece_byte(void)    //Êı¾İ´Ó¸ßÎ»µ½µÍÎ»//
{ 
	uint8_t i=8;
	uint8_t byte=0;

	SDA_HIGH();
	while (i--)
	{
		byte <<= 1;
		SCL_LOW();
		delay(5);
		SCL_HIGH();
		if (SDA_READ())
		{
			byte |= 0x01;
		}
	}
	SCL_LOW();
	return byte;
}


/////////////////////////////////////////////////////////////////////////

/*
** º¯ÊıÃû£ºee_byte_write
** ÃèÊö£ºĞ´Ò»¸ö×Ö½Úµ½EEPROMµÄÄ³¸öµØÖ·
** ²ÎÊı£º
	dev_addr£ºÉè±¸µØÖ·
	ee_addr£ºÉè±¸EEPROMµØÖ·
	ch£ºÒªĞ´ÈëµÄÊı¾İ
**
*/ 
bool ee_byte_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t ch)
{
	uint8_t addr = 0;
	
	addr = dev_addr<<1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr);		//ÉèÖÃÆ÷¼şµØÖ·+¶ÎµØÖ· 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}
	
	i2c_send_byte(ee_addr>>8);	//ÉèÖÃ¶ÎÄÚµØÖ·
	i2c_wait_ack();
	
	i2c_send_byte(ee_addr&0xFF);	//ÉèÖÃ¶ÎÄÚµØÖ·
	i2c_wait_ack();
	
	i2c_send_byte(ch);
	i2c_wait_ack();
	
	i2c_stop();
	
	delay(50000);//delay_1ms(5);
	return TRUE;
}

/*
** º¯ÊıÃû£ºee_random_read
** ÃèÊö: ´ÓEEPROMÄ³¸öµØÖ·¶ÁÒ»¸ö×Ö½ÚµÄÊı¾İ
** ²ÎÊı£º
	dev_addr£ºÉè±¸µØÖ·
	ee_addr£ºÉè±¸EEPROMµØÖ·
	ch£ºÒªĞ´ÈëµÄÊı¾İ
**
*/ 
uint8_t ee_random_read(uint8_t dev_addr, uint16_t ee_addr)
{
	uint8_t ch = 0;
	uint8_t addr_write = 0, addr_read = 0;
		
	addr_write = dev_addr<<1;
	addr_read = (dev_addr<<1) + 1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr_write);		//ÉèÖÃÆ÷¼şµØÖ·+¶ÎµØÖ· 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}

	i2c_send_byte(ee_addr>>8);  //ÉèÖÃ¸ßµØÖ·
	i2c_wait_ack();
	i2c_send_byte(ee_addr&0xFF);  //ÉèÖÃµÍµØÖ·
	i2c_wait_ack();
	
	i2c_start();
	i2c_send_byte(addr_read);
	i2c_wait_ack();
		
	ch = i2c_rece_byte();
	i2c_no_ack();
				
	i2c_stop();
	
	return ch;	
}


/*
** º¯ÊıÃû£ºee_random_read
** ÃèÊö: ´ÓEEPROMÄ³¸öµØÖ·¿ªÊ¼Ğ´Èëlen¸ö³¤¶ÈµÄÊı¾İdata
** ²ÎÊı£º
	dev_addr£ºÉè±¸µØÖ·
	ee_addr£ºÉè±¸EEPROMµØÖ·
	data£ºÒªĞ´ÈëµÄÊı¾
	len£ºĞ´ÈëÊı¾İµÄ³¤¶È
**
*/ 
bool ee_page_write(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len)
{
	uint8_t addr = 0;
	
	addr = dev_addr<<1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr);		//ÉèÖÃÆ÷¼şµØÖ·+¶ÎµØÖ· 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}
	
	i2c_send_byte(ee_addr>>8);	//ÉèÖÃ¶ÎÄÚµØÖ·
	i2c_wait_ack();
	
	i2c_send_byte(ee_addr&0xFF);	//ÉèÖÃ¶ÎÄÚµØÖ·
	i2c_wait_ack();
	
	while(len--)
	{
		i2c_send_byte(*data);
		i2c_wait_ack();
		data++;
	}
	i2c_stop();
	
	delay(100000);//delay_1ms(20);
	return TRUE;
}

/*
** º¯ÊıÃû£ºee_sequential_read
** ÃèÊö: ´ÓEEPROMÄ³¸öµØÖ·¿ªÊ¼¶ÁÈ¡len³¤¶ÈµÄÊı¾İ
** ²ÎÊı£º
	dev_addr£ºÉè±¸µØÖ·
	ee_addr£ºÉè±¸EEPROMµØÖ·
	data£º¶Á³öµÄÊı¾İ
	len£ºÒª¶ÁÈ¡µÄÊı¾İ³¤¶È
**
*/ 
bool ee_sequential_read(uint8_t dev_addr, uint16_t ee_addr, uint8_t *data, uint16_t len)
{	
	uint8_t addr_write = 0, addr_read = 0;
		
	addr_write = dev_addr<<1;
	addr_read = (dev_addr<<1) + 1;
	
	if (!i2c_start()) 
		return FALSE;
	
	i2c_send_byte(addr_write);		//ÉèÖÃÆ÷¼şµØÖ·+¶ÎµØÖ· 
	if (!i2c_wait_ack())
	{
		i2c_stop(); 
		return FALSE;
	}

	i2c_send_byte(ee_addr>>8);  //ÉèÖÃ¸ßµØÖ·
	i2c_wait_ack();
	i2c_send_byte(ee_addr&0xFF);  //ÉèÖÃµÍµØÖ·
	i2c_wait_ack();
	
	i2c_start();
	i2c_send_byte(addr_read);
	i2c_wait_ack();
	
	while(len)
	{
		*data = i2c_rece_byte();
		if(len == 1)
		{		
			i2c_no_ack();
		}
		else
		{
			i2c_ack();
		}
		data++;
		len--;
	}
	i2c_stop();
	
	return TRUE;
}


