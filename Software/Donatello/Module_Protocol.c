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
extern DLTDat std;
extern UART_RING_BUFFER_T rb;
/* Public  functions ---------------------------------------------------------*/

PWRESULT CheckPack_DTL(void)
{
    if(!((rb.rx[4] == 0x68)&&(rb.rx[11] == 0x68)))return PW_HEAD_1_ERR;
    switch(rb.rx[13])
    {
        case 0x06:
            {
                /**************************获取电能表的电压*****************************/
                if((rb.rx[14] == 0x33)&&(rb.rx[15] == 0x34)&&(rb.rx[16] == 0x34)&&(rb.rx[17] == 0x35))
                {
                    (std.Val)[1] = CONVERT_0X_10(rb.rx[18]-0x33);
                    (std.Val)[0] = CONVERT_0X_10(rb.rx[19]-0x33);
                }
                break;
            }
        case 0x07:
            {
                /**************************获取电能表的电流*********************************/
                if((rb.rx[14] == 0x33)&&(rb.rx[15] == 0x34)&&(rb.rx[16] == 0x35)&&(rb.rx[17] == 0x35))
                {
                    (std.Cur)[1] = CONVERT_0X_10(rb.rx[18]-0x33);
                    (std.Cur)[0] = CONVERT_0X_10(rb.rx[19]-0x33);
                }
                /********************************获取hhmmss*********************************/
                if((rb.rx[14] == 0x35)&&(rb.rx[15] == 0x34)&&(rb.rx[16] == 0x33)&&(rb.rx[17] == 0x37))
                {
                    (std.hhmmss)[2] = CONVERT_0X_10(rb.rx[18]-0x33);
                    (std.hhmmss)[1] = CONVERT_0X_10(rb.rx[19]-0x33);
                    (std.hhmmss)[0] = CONVERT_0X_10(rb.rx[20]-0x33);
                }
                break;
            }
        case 0x08:
            {
                /*******************获取电能表正向有功总功率的电流**************************/
                if((rb.rx[14] == 0x33)&&(rb.rx[15] == 0x33)&&(rb.rx[16] == 0x34)&&(rb.rx[17] == 0x33))
                {
                    (std.Elenergy)[3] = CONVERT_0X_10(rb.rx[18]-0x33);
                    (std.Elenergy)[2] = CONVERT_0X_10(rb.rx[19]-0x33);
                    (std.Elenergy)[1] = CONVERT_0X_10(rb.rx[20]-0x33);
                    (std.Elenergy)[0] = CONVERT_0X_10(rb.rx[21]-0x33);
                }
                /********************************获取YYMMDDWW*******************************/
                if((rb.rx[14] == 0x34)&&(rb.rx[15] == 0x34)&&(rb.rx[16] == 0x33)&&(rb.rx[17] == 0x37))
                {
                    (std.YYMMDDWW)[3] = CONVERT_0X_10(rb.rx[18]-0x33);
                    (std.YYMMDDWW)[2] = CONVERT_0X_10(rb.rx[19]-0x33);
                    (std.YYMMDDWW)[1] = CONVERT_0X_10(rb.rx[20]-0x33);
                    (std.YYMMDDWW)[0] = CONVERT_0X_10(rb.rx[21]-0x33);
                }
                break;
            }
        default:
            break;
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

