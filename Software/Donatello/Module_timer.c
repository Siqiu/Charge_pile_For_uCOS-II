/**
  ******************************************************************************
  * @file    Module_timer.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */

#include "lpc17xx.h"
#include "Module_timer.h"
#include "lpc17xx_timer.h"
#include "lpc17xx_pinsel.h"
#include "debug_frmwrk.h"


extern uint16_t timer_cnt;
extern uint16_t Uart0_Inter;
extern uint32_t uart0_flag;           //接收数据标志
extern UART_RING_BUFFER_T rb;
uint16_t timer_cnt_old = 0;
uint16_t timer_cnt_new = 0;
//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct;

/*********************************************************************//**
 * @brief		TIMER0 interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void TIMER0_IRQHandler(void)
{
#if 0
	if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)== SET){
        timer_cnt++;
	}
    if(rb.rx_head != 0 & timer_cnt_old == 0){//收到数据
        timer_cnt_old = timer_cnt;
    }
    if((timer_cnt>(timer_cnt_old+30)) & (timer_cnt_old != 0)){
        timer_cnt_old = 0;
        timer_cnt = 0;
        Uart0_Inter = 1;
    }
	TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
#else
    if(rb.rx_head != 0){//收到数据
        timer_cnt_old = rb.rx_head;
        if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)== SET){//收到数据开始计数
            timer_cnt++;
        }
        if(timer_cnt >= 30){
            if(timer_cnt_old == rb.rx_head){
                timer_cnt_old = 0;
                timer_cnt = 0;
                Uart0_Inter = 1;
            }
            timer_cnt = 0;
        }
    }
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
#endif
}

void TIMER_Init(uint32_t Hz)
{
    PINSEL_CFG_Type PinCfg;
	// Conifg P1.28 as MAT0.0
	PinCfg.Funcnum = 3;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Portnum = 1;
	PinCfg.Pinnum = 28;
	PINSEL_ConfigPin(&PinCfg);

	// Initialize timer 0, prescale count time of 100uS
	TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
	TIM_ConfigStruct.PrescaleValue	= 100;

	// use channel 0, MR0
	TIM_MatchConfigStruct.MatchChannel = 0;
	// Enable interrupt when MR0 matches the value in TC register
	TIM_MatchConfigStruct.IntOnMatch   = TRUE;
	//Enable reset on MR0: TIMER will reset if MR0 matches it
	TIM_MatchConfigStruct.ResetOnMatch = TRUE;
	//Stop on MR0 if MR0 matches it
	TIM_MatchConfigStruct.StopOnMatch  = FALSE;
	//Toggle MR0.0 pin if MR0 matches it
	TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_TOGGLE;
	// Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz)
	TIM_MatchConfigStruct.MatchValue   = Hz;//10000 * Hz;

	// Set configuration for Tim_config and Tim_MatchConfig
	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);
	TIM_ConfigMatch(LPC_TIM0,&TIM_MatchConfigStruct);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER0_IRQn);
    // To start timer 0
    TIM_Cmd(LPC_TIM0,ENABLE);
}
