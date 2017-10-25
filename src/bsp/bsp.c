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
#include "bsp.h"
#include "common.h"
#include "timer.h"

/*
*Function: rcc_config      
*Description: 系统时钟初始化  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void rcc_config(void)
{
	ErrorStatus err_status;

	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);

	err_status = RCC_WaitForHSEStartUp();

	if (err_status == SUCCESS)
	{
		/* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1); 
        
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1); 
        
        /* PCLK1 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div1);
        
        /* PLLCLK = 12MHz * 6 = 72 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
        
        /* Enable PLL */ 
        RCC_PLLCmd(ENABLE);
	
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        
        /* Wait till PLL is used as system clock source */
        while(RCC_GetSYSCLKSource() != 0x08);
	}


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_USART2, ENABLE);
						   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
                           | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);
	
}



/*
*Function: iwatchdog_config      
*Description: 独立看门狗初始化 
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void iwatchdog_config(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0xFFE);
	IWDG_ReloadCounter();
	IWDG_Enable();		
}

/*
*Function: iwatchdog_clear      
*Description: 独立看门狗 清狗 
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void iwatchdog_clear(void)
{
	IWDG_ReloadCounter();
}

/*
*Function: gpio_config      
*Description: GPIO初始化
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void gpio_config(void)
{
	GPIO_InitTypeDef gpio_init_structure;
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_3;				                 //LED1
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &gpio_init_structure);	

	// UART2
	gpio_init_structure.GPIO_Pin = GPIO_Pin_2;				// UART2 TX				    
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_AF_PP;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;			
  	GPIO_Init(GPIOA, &gpio_init_structure);
	gpio_init_structure.GPIO_Pin = GPIO_Pin_3;				
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;			 
  	GPIO_Init(GPIOA, &gpio_init_structure);

	// GPRS  
	gpio_init_structure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;          
  	GPIO_Init(GPIOA, &gpio_init_structure);
	
	gpio_init_structure.GPIO_Pin = GPIO_Pin_13 ;
  	gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;
  	gpio_init_structure.GPIO_Mode = GPIO_Mode_Out_PP;          //
	GPIO_Init(GPIOC, &gpio_init_structure);	
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);			// GPRS_PWR
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);			// GPRS_PDN
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);			// GPRS_RST
	
}

/*
*Function: timer2_config      
*Description: 定时器2初始化  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void timer2_config(void)
{
	NVIC_InitTypeDef nvic_init_structure;
	TIM_TimeBaseInitTypeDef timer2_init_structure;
	uint16_t cnt = 1000;

	TIM_DeInit(TIM2); 
    timer2_init_structure.TIM_CounterMode = TIM_CounterMode_Up;
    timer2_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;
    timer2_init_structure.TIM_Period = cnt - 1;
    timer2_init_structure.TIM_Prescaler = 72 - 1; 	// 
    TIM_TimeBaseInit(TIM2, &timer2_init_structure);

	TIM_SetCounter(TIM2, 0);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}



/*
*Function: gprs_config      
*Description: GPRS启动 
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void gprs_config(void)
{
/*	
	GPIO_SetBits(GPIOC, GPIO_Pin_13); //上电 gprs_PWR PWR 
	GPIO_ResetBits(GPIOA, GPIO_Pin_1); //开关机引脚 PDN 
	GPIO_SetBits(GPIOA, GPIO_Pin_0); //复位脚 
	delay_1ms(500); 
	//RST 
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); //上电 gprs_PWR PWR 
	GPIO_ResetBits(GPIOA, GPIO_Pin_1); //开关机引脚 PDN 
	delay_1ms(250); 

	GPIO_SetBits(GPIOA, GPIO_Pin_0); //复位脚 
	delay_1ms(250); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_0); //复位脚 
	delay_1ms(200); 
	GPIO_SetBits(GPIOA, GPIO_Pin_1); //开关机引脚 PDN 
	delay_1ms(1100); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_1); //开关机引脚 PDN
*/
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13);				//GPRS_PWR
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);				//PND
	delay_1ms(250);
	GPIO_SetBits(GPIOA, GPIO_Pin_0);				//RST
	delay_1ms(200);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);				//	
	delay_1ms(200);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);				//
	delay_1ms(1100);
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);					
}



/*
*Function: uart2_config      
*Description: 串口2初始化  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void uart2_config(void)
{
	USART_InitTypeDef uart_init_structure;
	 
	USART_DeInit(USART2);

	uart_init_structure.USART_BaudRate = 115200;						//
  	uart_init_structure.USART_WordLength = USART_WordLength_8b;			//
  	uart_init_structure.USART_StopBits = USART_StopBits_1;				//
  	uart_init_structure.USART_Parity = USART_Parity_No;					//
  	uart_init_structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   	//
  	uart_init_structure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//
	/* Configure USART1 */
  	USART_Init(USART2, &uart_init_structure);							//

	// UART 
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

   	/* Enable the USART1 */
  	USART_Cmd(USART2, ENABLE);
}

/*
*Function: nvic_config      
*Description: 中断向量配置  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void nvic_config(void)
{
	NVIC_InitTypeDef nvic_init_structure;

	NVIC_DeInit();		// 自己写的函数
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, NVIC_VECTOR_ADDR);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	// 
	nvic_init_structure.NVIC_IRQChannel = TIM2_IRQn;
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 10;    //
    nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_init_structure);

	// 
	nvic_init_structure.NVIC_IRQChannel = USART2_IRQn;
    nvic_init_structure.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_init_structure.NVIC_IRQChannelSubPriority = 0;
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;	 
	NVIC_Init(&nvic_init_structure);	
}


/*
*Function: bsp_init      
*Description: 底层初始化主函数  
*Calls: 无  
*Data Accessed: 无  
*Data Updated: 无    
*Input: 无
*Output: 无
*Return: 无     
*Others: 无    
*/
void bsp_init(void)
{
	rcc_config();
	nvic_config();
	gpio_config();
	iwatchdog_config();
	timer2_config();
	uart2_config();
}





