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

static  OS_STK  App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  App_ObjCreate          (void);
static  void  App_TaskCreate         (void);

static  void  App_TaskStart          (void       *p_arg);
/* Example group ----------------------------------------------------------- */
/** @defgroup TIMER_Polling_Match	Polling_Match
 * @ingroup TIMER_Examples
 * @{
 */
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


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This the main standard entry point.
*
* Note(s)     : none.
*********************************************************************************************************
*/
uint16_t timer_cnt = 0;

uint16_t Uart0_Inter = 0;
int  main (void)
{

    //CPU_Init();

    //BSP_Init();                                                 /* Initialize BSP functions                             */
    bsp_init();
    
    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreateExt((void (*)(void *))App_TaskStart,            /* Create the start task                                */
                    (void          *)0,
                    (OS_STK        *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U          )APP_CFG_TASK_START_PRIO,
                    (INT16U         )APP_CFG_TASK_START_PRIO,
                    (OS_STK        *)&App_TaskStartStk[0],
                    (INT32U         )APP_CFG_TASK_START_STK_SIZE,
                    (void          *)0,
                    (INT16U         )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));


    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    return(1);
}

/*
*********************************************************************************************************
*                                          App_TaskStart()
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Caller(s)   : This is a task.
*
* Notes       : (1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                   used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  App_TaskStart (void *p_arg)
{

    (void)p_arg;                                                /* See Note #1                                          */


    BSP_Start();                                                /* Start BSP and tick initialization                    */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif


    App_ObjCreate();                                            /* Create Applicaiton kernel objects                    */

    App_TaskCreate();                                           /* Create Application tasks                             */
    
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        CAN_SendMsg(LPC_CAN1, &TXMsg);
        UARTPuts (LPC_UART0,"Match interrupt occur..\r\naaa");
        UARTPuts (LPC_UART2, menu1);
        UARTPuts (LPC_UART3, menu1);
        LED_RED();
        printf("%d\n",timer_cnt);
        OSTimeDlyHMSM(0, 0, 1, 0);
        printf("%d\n",timer_cnt);
        LED_GREEN();
        OSTimeDlyHMSM(0, 0, 1, 0);
        LED_BLUE();
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}


/*
*********************************************************************************************************
*                                      App_ObjCreate()
*
* Description:  Creates the application kernel objects.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_ObjCreate (void)
{
}


/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description :  This function creates the application tasks.
*
* Argument(s) :  none.
*
* Return(s)   :  none.
*
* Caller(s)   :  App_TaskStart().
*
* Note(s)     :  none.
*********************************************************************************************************
*/

static  void  App_TaskCreate (void)
{
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
