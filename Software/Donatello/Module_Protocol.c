/**
  ******************************************************************************
  * @file    Module_Protocol.c
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
#include "Module_Protocol.h"
#include "Module_DLT.h"


extern uint16_t Uart0_Inter;
extern uint32_t uart0_flag;           //接收数据标志
extern uint8_t     uart0_buf[64];      //接收缓冲区
extern DLTDat std;
extern UART_RING_BUFFER_T rb;
/* Public  functions ---------------------------------------------------------*/

PWRESULT CheckPack_DTL(void)
{
    if((uart0_buf[4] == 0x68)&&(uart0_buf[11] == 0x68)) return PW_HEAD_1_ERR;
        /**************************获取电能表的电压****************************/
    if((uart0_buf[12] == 0x91)&&(uart0_buf[14] == 0x33)&&(uart0_buf[15] == 0x34)&&(uart0_buf[16] == 0x34)&&(uart0_buf[17] == 0x35))
    {
        (std.Val)[0] = uart0_buf[18]-0x33;
        (std.Val)[1] = uart0_buf[19]-0x33;
    }
    /**************************获取电能表的电流****************************/
    if((uart0_buf[12] == 0x11)&&(uart0_buf[14] == 0x33)&&(uart0_buf[15] == 0x34)&&(uart0_buf[16] == 0x35)&&(uart0_buf[17] == 0x35))
    {
        (std.Cur)[0] = uart0_buf[18]-0x33;
        (std.Cur)[1] = uart0_buf[19]-0x33;
    }
    /**************************获取电能表正向有功总功率的电流****************************/
    if((uart0_buf[12] == 0x11)&&(uart0_buf[14] == 0x33)&&(uart0_buf[15] == 0x33)&&(uart0_buf[16] == 0x34)&&(uart0_buf[17] == 0x33))
    {
        (std.Elenergy)[0] = uart0_buf[18]-0x33;
        (std.Elenergy)[1] = uart0_buf[19]-0x33;
        (std.Elenergy)[2] = uart0_buf[20]-0x33;
        (std.Elenergy)[3] = uart0_buf[21]-0x33;
    }
    return PW_OK;
}
/*******************************************************************************
  * @函数名称		UardDmaFlow
  * @函数说明		接收线程，出来那个函数解析数据
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void UardDmaFlow(void)
{
    if(Uart0_Inter){
        CheckPack_DTL();
        Uart0_Inter = 0;
        rb.rx_head = 0;
    }
}


/* Private functions ---------------------------------------------------------*/

