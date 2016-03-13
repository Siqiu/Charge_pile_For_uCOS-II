/**
  ******************************************************************************
  * @file    Module_uart.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */
#ifndef __MODULE_UART_H__
#define __MODULE_UART_H__
#include "lpc17xx_uart.h"


#define ENABLE_485      LPC_GPIO0->FIOCLR =  (1<<26);         /* 485_DIR = 1 enable send*/
#define DISENABLE_485   LPC_GPIO0->FIOSET =  (1<<26);          /* 485_DIR = 1 enable Rec*/

void UART0_Init(uint32_t BPS);
void UART2_Init(uint32_t BPS);
void UART3_Init(uint32_t BPS);


#endif
