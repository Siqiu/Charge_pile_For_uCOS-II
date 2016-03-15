/**
  ******************************************************************************
  * @file    Module_can.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */

#include "Module_can.h"
#include "lpc17xx_can.h"
//#include "lpc17xx_libcfg.h"
#include "lpc17xx_pinsel.h"

/** CAN variable definition **/
CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
uint32_t CANRxCount, CANTxCount = 0;

PINSEL_CFG_Type PinCfg;
/*********************************************************************//**
 * @brief		CAN_IRQ Handler, control receive message operation
 * param[in]	none
 * @return 		none
 **********************************************************************/
void CAN_IRQHandler()
{
	uint8_t IntStatus;
	uint32_t data1;
	/* get interrupt status
	 * Note that: Interrupt register CANICR will be reset after read.
	 * So function "CAN_IntGetStatus" should be call only one time
	 */
	IntStatus = CAN_IntGetStatus(LPC_CAN1);
	//check receive interrupt
	if((IntStatus>>0)&0x01)
	{
		CAN_ReceiveMsg(LPC_CAN1,&RXMsg);
		//PrintMessage(&RXMsg);
		CANRxCount++; //count success received message
		//increase data for next TX Message
		TXMsg.id ++;
		data1 = (TXMsg.dataA[0])|(((TXMsg.dataA[1]))<<8)|((TXMsg.dataA[2])<<16)|((TXMsg.dataA[3])<<24);
		if(data1 == 0xFFFFFFFF) data1 = 0;
		else data1++;
		*((uint8_t *) &TXMsg.dataA[0])= *((uint8_t *) &TXMsg.dataB[0])= data1 & 0x000000FF;
		*((uint8_t *) &TXMsg.dataA[1])= *((uint8_t *) &TXMsg.dataB[1])=(data1 & 0x0000FF00)>>8;;
		*((uint8_t *) &TXMsg.dataA[2])= *((uint8_t *) &TXMsg.dataB[2])=(data1 & 0x00FF0000)>>16;
		*((uint8_t *) &TXMsg.dataA[3])= *((uint8_t *) &TXMsg.dataB[3])=(data1 & 0xFF000000)>>24;

		CAN_SendMsg(LPC_CAN1, &TXMsg);
	}
}
/*********************************************************************//**
 * @brief		Initialize transmit and receive message for Bypass operation
 * @param[in]	none
 * @return 		none
 **********************************************************************/
void CAN_InitMessage(void) {
	TXMsg.format = STD_ID_FORMAT;//EXT_ID_FORMAT;
	TXMsg.id = 0x12;
	TXMsg.len = 8;
	TXMsg.type = DATA_FRAME;
	TXMsg.dataA[0] = TXMsg.dataA[1] = TXMsg.dataA[2] = TXMsg.dataA[3] = 0x00000000;
	TXMsg.dataB[0] = TXMsg.dataB[1] = TXMsg.dataB[2] = TXMsg.dataB[3] = 0x00000000;

	RXMsg.format = 0x00;
	RXMsg.id = 0x00;
	RXMsg.len = 0x00;
	RXMsg.type = 0x00;
	RXMsg.dataA[0] = RXMsg.dataA[1] = RXMsg.dataA[2] = RXMsg.dataA[3] = 0x00000000;
	RXMsg.dataB[0] = RXMsg.dataB[1] = RXMsg.dataB[2] = RXMsg.dataB[3] = 0x00000000;
}
void CAN1_Init(uint32_t BPS)
{
	/* Pin configuration
	 * CAN1: select P0.0 as RD1. P0.1 as TD1
	 * CAN2: select P2.7 as RD2, P2.8 as RD2
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 0;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 1;
	PINSEL_ConfigPin(&PinCfg);

	//Initialize CAN1
	CAN_Init(LPC_CAN1, BPS);
    
	//Enable Interrupt
	CAN_IRQCmd(LPC_CAN1, CANINT_RIE, ENABLE);

	//Enable CAN Interrupt
	NVIC_EnableIRQ(CAN_IRQn);


	CAN_SetAFMode(LPC_CANAF,CAN_AccBP);
	CAN_InitMessage();
}
void CAN2_Init(uint32_t BPS)
{
	/* Pin configuration
	 * CAN1: select P0.0 as RD1. P0.1 as TD1
	 * CAN2: select P2.7 as RD2, P2.8 as RD2
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 7;
	PinCfg.Portnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 8;
	PINSEL_ConfigPin(&PinCfg);

	//Initialize CAN2
	CAN_Init(LPC_CAN2, BPS);

	//Enable Interrupt
	CAN_IRQCmd(LPC_CAN2, CANINT_RIE, ENABLE);

	//Enable CAN Interrupt
	NVIC_EnableIRQ(CAN_IRQn);

	CAN_SetAFMode(LPC_CANAF,CAN_AccBP);
	CAN_InitMessage();
}
