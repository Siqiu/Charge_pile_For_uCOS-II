/**
  ******************************************************************************
  * @file    Module_DLT.c
  * @author  Donatello
  * @version V1.0
  * @date    2015.9.16
  * @brief
  ******************************************************************************
  */
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
#include "Module_DLT.h"
DLTDat std;
/* Public  functions ---------------------------------------------------------*/
/*******************************************************************************
  * @函数名称		DltStart
  * @函数说明		主站发送帧信息之前，唤醒从站
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void DltStart(void)
{
    UART_Puts (LPC_UART0,DLT_wakeup,4);
}
/*******************************************************************************
  * @函数名称		ReadData
  * @函数说明		读取电能表的数据
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void ReadData(const uint8_t * ptr,uint16_t len)
{
    DltStart();
    UART_Puts (LPC_UART0,ptr,len);
}


/* Private functions ---------------------------------------------------------*/

