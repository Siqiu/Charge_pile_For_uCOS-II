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
#include "lpc17xx_clkpwr.h"

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
	/*
	 * Initialize UART0 pin connect
	 */
    PINSEL_CFG_Type PinCfg;
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
     * 默认的设置
	 */
    UART_CFG_Type UARTConfigStruct;
	UART_ConfigStructInit(&UARTConfigStruct);

    
	// Re-configure baudrate to bps
	UARTConfigStruct.Baud_rate = BPS;
    
    // Set parity is ODD
    UARTConfigStruct.Parity = UART_PARITY_ODD;

	// Initialize DEBUG_UART_PORT peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);

	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);
    
    // UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
    
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
    
    NVIC_SetPriorityGrouping(4); //sets group priorities: 8 - subpriorities: 3
    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(UART0_IRQn, ((0x01<<3)|0x01));
	/* Enable Interrupt for UART0 channel */
    NVIC_EnableIRQ(UART0_IRQn);
#else
    CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCUART0, ENABLE);
    	uint16_t usFdiv;
    /* UART0 */
    LPC_PINCON->PINSEL0 |= (1 << 4);                /* Pin P0.2 used as TXD0 (Com0) */
    LPC_PINCON->PINSEL0 |= (1 << 6);                /* Pin P0.3 used as RXD0 (Com0) */
  
  	//LPC_UART0->LCR  = 0x83;                         /* 允许设置波特率   无校验*/
    LPC_UART0->LCR  = 0x9B;                         /* 允许设置波特率  偶校验(电能表为欧校验)*/
    usFdiv = (FPCLK / 16) / BPS;              /* 设置波特率 */
    LPC_UART0->DLM  = usFdiv / 256;//171
    LPC_UART0->DLL  = usFdiv % 256;//205 143585280
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
	NVIC_EnableIRQ(UART3_IRQn);                     /* 在NVIC中使能中断             */  
    LPC_UART3->IER = 0x01;                          /* 使能接收中断                 */

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
/************************** PRIVATE DEFINTIONS *************************/


/* Buf mask */
#define __BUF_MASK (UART_RING_BUFSIZE-1)
/* Check buf is full or not */
#define __BUF_IS_FULL(head, tail) ((tail&__BUF_MASK)==((head+1)&__BUF_MASK))
/* Check buf will be full in next receiving or not */
#define __BUF_WILL_FULL(head, tail) ((tail&__BUF_MASK)==((head+2)&__BUF_MASK))
/* Check buf is empty */
#define __BUF_IS_EMPTY(head, tail) ((head&__BUF_MASK)==(tail&__BUF_MASK))
/* Reset buf */
#define __BUF_RESET(bufidx)	(bufidx=0)
#define __BUF_INCR(bufidx)	(bufidx=(bufidx+1)&__BUF_MASK)


/************************** PRIVATE TYPES *************************/




/************************** PRIVATE VARIABLES *************************/
// UART Ring buffer
UART_RING_BUFFER_T rb;

// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;


/********************************************************************//**
 * @brief 		UART receive function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntReceive(void)
{
	uint8_t tmpc;
	uint32_t rLen;

	while(1){
		// Call UART read function in UART driver
		rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &tmpc, 1, NONE_BLOCKING);
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb.rx_head,rb.rx_tail)){
				rb.rx[rb.rx_head] = tmpc;
				__BUF_INCR(rb.rx_head);
			}
		}
		// no more data
		else {
			break;
		}
	}
}

/********************************************************************//**
 * @brief 		UART transmit function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntTransmit(void)
{
    // Disable THRE interrupt
    UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);

	/* Wait for FIFO buffer empty, transfer UART_TX_FIFO_SIZE bytes
	 * of data or break whenever ring buffers are empty */
	/* Wait until THR empty */
    while (UART_CheckBusy((LPC_UART_TypeDef *)LPC_UART0) == SET);

	while (!__BUF_IS_EMPTY(rb.tx_head,rb.tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
    	if (UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&rb.tx[rb.tx_tail], 1, NONE_BLOCKING)){
        /* Update transmit ring FIFO tail pointer */
        __BUF_INCR(rb.tx_tail);
    	} else {
    		break;
    	}
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
	if (__BUF_IS_EMPTY(rb.tx_head, rb.tx_tail)) {
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, DISABLE);
    	// Reset Tx Interrupt state
    	TxIntStat = RESET;
    }
    else{
      	// Set Tx Interrupt state
		TxIntStat = SET;
    	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_THRE, ENABLE);
    }
}


/*********************************************************************//**
 * @brief		UART Line Status Error
 * @param[in]	bLSErrType	UART Line Status Error Type
 * @return		None
 **********************************************************************/
void UART_IntErr(uint8_t bLSErrType)
{
	while(1);
}
/*********************************************************************//**
 * @brief		UART0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART0_IRQHandler(void)
{
#if 0
    uint32_t flag;
    uint8_t byte;
    flag  = LPC_UART0->IIR;                                 /* 读取(清除)中断中断状态    */
    flag &= 0x0F;
    if((0X04 == flag)&&(LPC_UART0->LSR & 0X01) )           /* 如果是接收中断，并且有数据 */
	{
        byte=LPC_UART0->RBR;			                    /* 读取数据  */
        uart0_buf[uart0_flag++] = byte;
    }
#else
    	uint32_t intsrc, tmp, tmp1;

	/* Determine the interrupt source */
	intsrc = UART_GetIntId(LPC_UART0);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(LPC_UART0);
		// Mask out the Receive Ready and Transmit Holding empty status
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
				| UART_LSR_BI | UART_LSR_RXFE);
		// If any error exist
		if (tmp1) {
				UART_IntErr(tmp1);
		}
	}

	// Receive Data Available or Character time-out
	if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
			UART_IntReceive();
	}

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
			UART_IntTransmit();
	}
#endif
}
