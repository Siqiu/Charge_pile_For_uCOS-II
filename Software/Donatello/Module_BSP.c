/**
  ******************************************************************************
  * @file    Module_BSP.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */
#include "lpc17xx.h"
#include "Module_BSP.h"
#include "Module_LED.h"
#include "Module_can.h"
#include "Module_uart.h"
#include "Module_timer.h"
#include "lpc17xx_pinsel.h"



/* SysTick Counter */
volatile unsigned long SysTickCnt;


/*********************************************************************//**
 * @brief		SysTick handler sub-routine (1ms)
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SysTick_Handler (void) {
  SysTickCnt++;
}
/*********************************************************************//**
 * @brief		Delay function
 * @param[in]	tick - number milisecond of delay time
 * @return 		None
 **********************************************************************/
void Delay (unsigned long tick) {
  unsigned long systickcnt;

  systickcnt = SysTickCnt;
  while ((SysTickCnt - systickcnt) < tick);
}


void bsp_init(void)
{
    //SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */
//    RTCInit();                                          /* RTC初始化 */	

    CAN1_Init();
    
    TIMER_Init(100);
    UART0_Init(9600);                                    /* wzx 485 */
    UART2_Init(9600);                                    /* wzx DWIN */
    UART3_Init(9600);                                    /* wzx CARD */
    LED_config();                                        /* LED contorl */
}
