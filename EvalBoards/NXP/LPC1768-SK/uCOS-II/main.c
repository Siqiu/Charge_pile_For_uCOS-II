/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "Module_BSP.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/* Example group ----------------------------------------------------------- */
/** @defgroup TIMER_Polling_Match	Polling_Match
 * @ingroup TIMER_Examples
 * @{
 */
static void Task_Start(void *pdata);
extern uint8_t     uart0_buf[64];      //接收缓冲区
extern CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
/************************** PRIVATE VARIABLES *************************/
uint8_t menu1[] =
"********************************************************************************\n\r"
"Hello NXP Semiconductors \n\r"
"Timer delay demo \n\r"
"\t - MCU: LPC17xx \n\r"
"\t - Core: ARM Cortex-M3 \n\r"
"\t - Communicate via: UART0 - 9600 bps \n\r"
" Use timer 0 in polling mode \n\r"
" Toggle MAT0.0 at frequency 10Hz \n\r"
"********************************************************************************\n\r";
uint16_t timer_cnt = 0;
    int num = 0;


/*
*********************************************************************************************************
*                                       声明任务堆栈
*********************************************************************************************************
*/
OS_STK  STK_START[TASK_0_STK_SIZE];

OS_STK  STK_APP_01[TASK_1_STK_SIZE];
OS_STK  STK_APP_02[TASK_2_STK_SIZE];
OS_STK  STK_APP_03[TASK_3_STK_SIZE];
OS_STK  STK_APP_04[TASK_4_STK_SIZE];
/*******************************************************************************
  * @函数名称		Task_MBOX
  * @函数说明		喂看门狗和解析数据
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void Task_APP_01(void *pdata)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
#endif
    OS_ENTER_CRITICAL();
    OS_EXIT_CRITICAL();
    pdata = pdata;
    for(;;)
    {
        //OS_ENTER_CRITICAL();

        //WDOG_Refresh();

        //OS_EXIT_CRITICAL();

        UardDmaFlow();
        

        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}
/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/
/*******************************************************************************
  * @函数名称		Task_APP_02
  * @函数说明		SHELL
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void Task_APP_02(void *pdata)
{

    pdata=pdata;
    for(;;)
    {
//         SEND_485;
//         UART_Send(LPC_UART0, (uint8_t *)ReadVol, sizeof(ReadVol), BLOCKING);
//         //OSTimeDlyHMSM(0, 0, 0, 50);
//         RECV_485;
        if(num)
        {
            num--;
            ReadData(ReadVol,16);
        }
//        ReadData(ReadVol,16);
//        OSTimeDlyHMSM(0, 0, 2, 0);
//        ReadData(ReadCur,16);
//        OSTimeDlyHMSM(0, 0, 3, 0);
//        ReadData(ReadEng,16);
//        OSTimeDlyHMSM(0, 0, 4, 0);
//        printf("%d\n",timer_cnt);
        
        FIO_ByteSetValue(2, 0, P02_06);
        OSTimeDlyHMSM(0, 0, 0, 500);
        FIO_ByteClearValue(2, 0, P02_06);
        OSTimeDlyHMSM(0, 0, 0, 500);
        

        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/*******************************************************************************
  * @函数名称		Task_APP_03
  * @函数说明		充电机充电桩发送任务
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void Task_APP_03(void *pdata)
{
    pdata = pdata;

    OSStatInit();                   											/* 统计任务初始化 */

    for(;;)
    {
        OSTimeDlyHMSM(0, 0, 2, 0);
    }
}

/*******************************************************************************
  * @函数名称		Task_APP_04
  * @函数说明		无
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void Task_APP_04(void *pdata)
{
    pdata = pdata;
    for(;;)
    {
        CAN_SendMsg(LPC_CAN1, &TXMsg);
//        UARTPuts (LPC_UART0,"Match interrupt occur..\r\naaa");
//        UARTPuts (LPC_UART2, menu1);
//        UARTPuts (LPC_UART3, menu1);

        LED_Red();
        printf("%d\n",timer_cnt);
        OSTimeDlyHMSM(0, 0, 1, 0);
        printf("%d\n",timer_cnt);
        LED_Green();
        OSTimeDlyHMSM(0, 0, 1, 0);
        LED_Blue();
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

/*******************************************************************************
  * @函数名称		Task_Family_Energy_Storage
  * @函数说明		家庭储能2.5°电项目
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
void Task_Family_Energy_Storage(void *pdata)
{
    pdata = pdata;
    for(;;)
    {
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}


uint16_t Uart0_Inter = 0;
int  main (void)
{
    //CPU_Init();

    //BSP_Init();                                                 /* Initialize BSP functions                             */
    bsp_init();

    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreate(Task_Start,(void *)0,
                 &STK_START[TASK_0_STK_SIZE-1],
                 PRIO_START);

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return(1);
}
/*******************************************************************************
  * @函数名称		AppStartTask
  * @函数说明		起始任务，创建完其他任务后，自身挂起
  * @输入参数		无
  * @输出参数		无
  * @返回参数		无
*******************************************************************************/
static void Task_Start(void *pdata)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
#endif
    OS_ENTER_CRITICAL();

    pdata = pdata;
    BSP_Start();                                                /* Start BSP and tick initialization                    */

    uint8_t	err;                                                                //错误信息
#if 1
    OSTaskCreate(Task_APP_01,(void *)0,
                 &STK_APP_01[TASK_1_STK_SIZE-1],
                 PRIO_APP_01);
    OSTaskNameSet(PRIO_APP_01, (uint8_t*)"UardDmaFlow",&err);
#endif
#if 1
    OSTaskCreate(Task_APP_02,(void *)0,
                 &STK_APP_02[TASK_2_STK_SIZE-1],
                 PRIO_APP_02);
    OSTaskNameSet(PRIO_APP_02, (uint8_t*)"SHEEL",&err);
#endif
#if 1
    //充电机充电桩发送任务
    OSTaskCreate(Task_APP_03,(void *)0,
                 &STK_APP_03[TASK_3_STK_SIZE-1],
                 PRIO_APP_03);
    OSTaskNameSet(PRIO_APP_03, (uint8_t*)"Can_Post",&err);
#endif
#if 1
    OSTaskCreate(Task_APP_04,(void *)0,
                 &STK_APP_04[TASK_4_STK_SIZE-1],
                 PRIO_APP_04);
    OSTaskNameSet(PRIO_APP_04, (uint8_t*)"Time_to_one",&err);
#endif
    //挂起起始任务.
    OSTaskSuspend(PRIO_START);
    OS_EXIT_CRITICAL();
}
#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif
