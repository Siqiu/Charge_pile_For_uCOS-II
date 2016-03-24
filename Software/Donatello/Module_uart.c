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

/************************** PUBLOC VARIABLES *************************/
// UART Ring buffer
UART_RING_BUFFER_T rb[3];
extern uint16_t timer_cnt[3];
uint16_t Uart_Inter[3];
// Current Tx Interrupt enable state
__IO FlagStatus TxIntStat;
extern UART_RING_BUFFER_T rb[3];
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



#define FOSC                        12000000                          /*  振荡器频率                  */

#define FCCLK                      (FOSC  * 8)                        /*  主时钟频率<=100Mhz          */
                                                                      /*  FOSC的整数倍                */
#define FCCO                       (FCCLK * 3)                        /*  PLL频率(275Mhz~550Mhz)      */
                                                                      /*  与FCCLK相同，或是其的偶数倍 */
#define FPCLK                      (FCCLK / 4)                        /*  外设时钟频率,FCCLK的1/2、1/4*/
                                                                      /*  或与FCCLK相同               */

void UART0_Init(uint32_t BPS)
{
#if 1
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	// Pin configuration for UART0
	PINSEL_CFG_Type PinCfg;

	__IO FlagStatus exitflag;

	/*
	 * Initialize UART0 pin connect
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
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
    //Even parity
    UARTConfigStruct.Parity = UART_PARITY_EVEN;
	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);


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
	/*
	 * Do not enable transmit interrupt here, since it is handled by
	 * UART_Send() function, just to reset Tx Interrupt state for the
	 * first time
	 */
//	TxIntStat = RESET;


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

/********************************************************************//**
 * @brief 		UART receive function (ring buffer used)
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void UART_IntReceive(uint16_t instance)
{
	uint8_t tmpc;
	uint32_t rLen;
    uint8_t num;
    if(instance){
        num = instance - 1;
    } else {
        num = instance;
    }
	while(1){
		// Call UART read function in UART driver
        switch(instance)
        {
            case HW_UART0:
                {
                    rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART0, &tmpc, 1, NONE_BLOCKING);
                    break;
                }
            case HW_UART3:
                {
                    rLen = UART_Receive((LPC_UART_TypeDef *)LPC_UART3, &tmpc, 1, NONE_BLOCKING);
                    break;
                }
        }
		// If data received
		if (rLen){
			/* Check if buffer is more space
			 * If no more space, remaining character will be trimmed out
			 */
			if (!__BUF_IS_FULL(rb[num].rx_head,rb[num].rx_tail)){
				rb[num].rx[rb[num].rx_head] = tmpc;
				__BUF_INCR(rb[num].rx_head);
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

	while (!__BUF_IS_EMPTY(rb[0].tx_head,rb[0].tx_tail))
    {
        /* Move a piece of data into the transmit FIFO */
    	if (UART_Send((LPC_UART_TypeDef *)LPC_UART0, (uint8_t *)&rb[0].tx[rb[0].tx_tail], 1, NONE_BLOCKING)){
        /* Update transmit ring FIFO tail pointer */
        __BUF_INCR(rb[0].tx_tail);
    	} else {
    		break;
    	}
    }

    /* If there is no more data to send, disable the transmit
       interrupt - else enable it or keep it enabled */
	if (__BUF_IS_EMPTY(rb[0].tx_head, rb[0].tx_tail)) {
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
static void UART_IRQ_Handler(LPC_UART_TypeDef* instance)
{
	uint32_t intsrc, tmp, tmp1;
    
	/* Determine the interrupt source */
	intsrc = UART_GetIntId(instance);
	tmp = intsrc & UART_IIR_INTID_MASK;

	// Receive Line Status
	if (tmp == UART_IIR_INTID_RLS){
		// Check line status
		tmp1 = UART_GetLineStatus(instance);
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
        if(instance == LPC_UART0) UART_IntReceive(HW_UART0);
        if(instance == LPC_UART3) UART_IntReceive(HW_UART3);
    }

	// Transmit Holding Empty
	if (tmp == UART_IIR_INTID_THRE){
			UART_IntTransmit();
	}
}
/*********************************************************************//**
 * @brief		UART0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void UART0_IRQHandler(void)
{
    UART_IRQ_Handler(LPC_UART0);
}

void UART3_IRQHandler(void)
{
    UART_IRQ_Handler(LPC_UART3);
}