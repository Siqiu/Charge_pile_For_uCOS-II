/**
  ******************************************************************************
  * @file    Module_uart.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */
#include "lpc17xx.h"                                                  /* LPC17xx definitions    */
#include "Module_uart.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"

uint8_t     uart0_buf[64] = {0};      //接收缓冲区
uint32_t    uart0_flag = 0;           //接收数据标志
extern uint16_t timer_cnt;

#define FOSC                        12000000                          /*  振荡器频率                  */

#define FCCLK                      (FOSC  * 8)                        /*  主时钟频率<=100Mhz          */
                                                                      /*  FOSC的整数倍                */
#define FCCO                       (FCCLK * 3)                        /*  PLL频率(275Mhz~550Mhz)      */
                                                                      /*  与FCCLK相同，或是其的偶数倍 */
#define FPCLK                      (FCCLK / 4)                        /*  外设时钟频率,FCCLK的1/2、1/4*/
                                                                      /*  或与FCCLK相同               */
void UART0_Init(uint32_t BPS)
{
#if 0
    // UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    UART_CFG_Type UARTConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize UART0 pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 1;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Re-configure baudrate to bps
	UARTConfigStruct.Baud_rate = BPS;
    
    // Set parity is ODD
    UARTConfigStruct.Parity = UART_PARITY_ODD;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
    /* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);

    /* Enable UART Rx interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RLS, ENABLE);
//	/*
//	 * Do not enable transmit interrupt here, since it is handled by
//	 * UART_Send() function, just to reset Tx Interrupt state for the
//	 * first time
//	 */
//	TxIntStat = RESET;
//
//	// Reset ring buf head and tail idx
//	__BUF_RESET(rb.rx_head);
//	__BUF_RESET(rb.rx_tail);
//	__BUF_RESET(rb.tx_head);
//	__BUF_RESET(rb.tx_tail);

    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);
#else
    	uint16_t usFdiv;
    /* UART0 */
    LPC_PINCON->PINSEL0 |= (1 << 4);                /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);                /* Pin P0.3 used as RXD0 (Com0) */
  
  	//LPC_UART0->LCR  = 0x83;                         /* 允许设置波特率   无校验*/
    LPC_UART0->LCR  = 0x9B;                         /* 允许设置波特率  偶校验(电能表为欧校验)*/
    usFdiv = (FPCLK / 16) / BPS;              /* 设置波特率 */
    LPC_UART0->DLM  = usFdiv / 256;
    LPC_UART0->DLL  = usFdiv % 256; 
    LPC_UART0->LCR  &= (~0x80);                     /* 锁定波特率 */
    LPC_UART0->FCR  = 0x07; 				   
    NVIC_EnableIRQ(UART0_IRQn);                     /* 在NVIC中使能中断 */  
    LPC_UART0->IER = 0x01;                          /* 使能接收中断 */
#endif
}

void UART2_Init(uint32_t BPS)
{
    UART_CFG_Type UARTConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize UART0 pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 10;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 11;
	PINSEL_ConfigPin(&PinCfg);
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Re-configure baudrate to bps
	UARTConfigStruct.Baud_rate = BPS;
    
    // Set parity is UART_PARITY_NONE
    UARTConfigStruct.Parity = UART_PARITY_NONE;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART2, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART2, ENABLE);
}
#if 1

void UART3_Init (uint32_t UART3_BPS)
{
	uint16_t usFdiv;
   
    LPC_PINCON->PINSEL9 |= (3 << 24);               /* Pin P4.28 used as TXD3 (Com3) */
    LPC_PINCON->PINSEL9 |= (3 << 26);               /* Pin P4.29 used as RXD3 (Com3) */

   	LPC_SC->PCONP = LPC_SC->PCONP|(1<<25);	        /*使能UART3 */

    LPC_UART3->LCR  = 0x83;                         /* 允许设置波特率 */
    usFdiv = (FPCLK / 16) / UART3_BPS;              /* 设置波特率 */
    LPC_UART3->DLM  = usFdiv / 256;
    LPC_UART3->DLL  = usFdiv % 256; 
    //LPC_UART3->LCR  = 0x03;                       /* 锁定波特率 数据位8位 停止位1位，奇校验*/
    //LPC_UART3->LCR  = 0x1B;                         /*锁定波特率 数据位8位 停止位1位，偶校验*/ 
    LPC_UART3->LCR  ^= (1<<7);
    LPC_UART3->FCR  = 0x07;
	//NVIC_EnableIRQ(UART3_IRQn);                     /* 在NVIC中使能中断             */  
    //LPC_UART3->IER = 0x01;                          /* 使能接收中断                 */

}
#else
void UART3_Init(uint32_t BPS)
{
    UART_CFG_Type UARTConfigStruct;
	PINSEL_CFG_Type PinCfg;

	/*
	 * Initialize UART3 pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 28;
	PinCfg.Portnum = 4;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 29;
	PINSEL_ConfigPin(&PinCfg);
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	// Re-configure baudrate to bps
	UARTConfigStruct.Baud_rate = BPS;
    
    // Set parity is UART_PARITY_NONE
    UARTConfigStruct.Parity = UART_PARITY_NONE;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART3, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART3, ENABLE);
}
#endif
/*********************************************************************//**
 * @brief		UART0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART0_IRQHandler(void)
{
    uint32_t flag;
    uint8_t byte;
    flag  = LPC_UART0->IIR;                                 /* 读取(清除)中断中断状态    */
    flag &= 0x0F;
    if((0X04 == flag)&&(LPC_UART0->LSR & 0X01) )           /* 如果是接收中断，并且有数据 */
	{
        byte=LPC_UART0->RBR;			                    /* 读取数据  */
        uart0_buf[uart0_flag++] = byte;
    }
}
