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


extern uint16_t timer_cnt[3];
extern uint16_t Uart_Inter[3];
extern UART_RING_BUFFER_T rb[3];
uint16_t timer_cnt_old[3];
uint16_t timer_cnt_new[3];
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
    uint8_t For_temp = 0;
    for(; For_temp<3; For_temp++){
        if(rb[For_temp].rx_head != 0){//收到数据
            timer_cnt_old[For_temp] = rb[For_temp].rx_head;
            if (TIM_GetIntStatus(LPC_TIM0, TIM_MR0_INT)== SET){//收到数据开始计数
                timer_cnt[For_temp]++;
            }
            if(timer_cnt[For_temp] >= 2){//间隔时间
                if(timer_cnt_old[For_temp] == rb[For_temp].rx_head){
                    timer_cnt_old[For_temp] = 0;
                    timer_cnt[For_temp] = 0;
                    Uart_Inter[For_temp] = 1;
                }
                timer_cnt[For_temp] = 0;
            }
        }
    }
    TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);
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
