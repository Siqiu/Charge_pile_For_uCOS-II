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
#include "includes.h"

/* buffer size definition */
#define UART_RING_BUFSIZE 256
/** @brief UART Ring buffer structure */
typedef struct
{
    /*__IO*/ uint32_t tx_head;                /*!< UART Tx ring buffer head index */
    /*__IO*/ uint32_t tx_tail;                /*!< UART Tx ring buffer tail index */
    /*__IO*/ uint32_t rx_head;                /*!< UART Rx ring buffer head index */
    /*__IO*/ uint32_t rx_tail;                /*!< UART Rx ring buffer tail index */
    /*__IO*/ uint8_t  tx[UART_RING_BUFSIZE];  /*!< UART Tx data ring buffer */
    /*__IO*/ uint8_t  rx[UART_RING_BUFSIZE];  /*!< UART Rx data ring buffer */
} UART_RING_BUFFER_T;

/*!< UART 硬件模块号 */
#define HW_UART0  (0x00U)  /* 芯片的UART0端口 */
#define HW_UART1  (0x01U)
#define HW_UART2  (0x02U)
#define HW_UART3  (0x03U)

#define SEND_485    FIO_ByteClearValue(0, 3, CON_485);//LPC_GPIO0->FIOCLR =  (1<<26);   /* 485_DIR = 1 enable send*/
#define RECV_485    FIO_ByteSetValue(0, 3,   CON_485);//LPC_GPIO0->FIOSET =  (1<<26);   /* 485_DIR = 1 enable Rec*/

void UART0_Init(uint32_t BPS);
void UART2_Init(uint32_t BPS);
void UART3_Init(uint32_t BPS);


#endif
