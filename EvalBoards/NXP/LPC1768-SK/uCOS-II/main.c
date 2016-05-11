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
#include "Module_DLT.h"
#include "Module_RFID.h"

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
extern CAN_MSG_Type TXMsg, RXMsg; // messages for test Bypass mode
extern RFID_STRUCT Rfid;
extern uint8_t Uart2_Errnum;
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
*                                       ���������ջ
*********************************************************************************************************
*/
OS_STK  STK_START[TASK_0_STK_SIZE];

OS_STK  STK_APP_01[TASK_1_STK_SIZE];
OS_STK  STK_APP_02[TASK_2_STK_SIZE];
OS_STK  STK_APP_03[TASK_3_STK_SIZE];
OS_STK  STK_APP_04[TASK_4_STK_SIZE];
/*******************************************************************************
  * @��������		Task_MBOX
  * @����˵��		ι���Ź��ͽ�������
  * @�������		��
  * @�������		��
  * @���ز���		��
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
  * @��������		Task_APP_02
  * @����˵��		SHELL
  * @�������		��
  * @�������		��
  * @���ز���		��
*******************************************************************************/
void Task_APP_02(void *pdata)
{
    uint8_t buf1[22] = {0X8A, 0XE7, 0X91, 0X7C, 0X82, 0X9F, 0X00, 0X00, 0X8A, 0XE7, 0X91, 0X7C, 0X82, 0X9F};
    //uint8_t Master_Card[16]  = {0x24,0x23,0x22,0x21,0xdb,0xdc,0xdd,0xde,0x24,0x23,0x22,0x21,0x0d,0xf2,0x0d,0xf2};
    uint8_t Master_Card[16]  = {0xfe,0xff,0xff,0xff,0x01,0x00,0x00,0x00,0xfe,0xff,0xff,0xff,0x0c,0xf3,0x0c,0xf3};
    uint8_t lock[16] = {0X00, 0X00, 0X00, 0X00, 0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00,0x0e,0xf1,0x0e,0xf1};
    uint8_t unlock[16] = {0X01, 0X01, 0X01, 0X01, 0xfe,0xfe,0xfe,0xfe,0x01,0x01,0x01,0x01,0x0e,0xf1,0x0e,0xf1};
    RFID_Send(Cmd_Load_Key, buf1, buf1, 0x03);//ok
    pdata=pdata;
    num = 120;
    for(;;)
    {
        
        if(num == 6){
            num -= 6;
            UART_Puts (LPC_UART2, unlock, 16);
            RFID_Send(Cmd_R_HWVersion, NULL, NULL, NULL);//ok
        }
        RFID_Send(Cmd_Request, NULL, NULL, NULL);//Ѱ��ok
        OSTimeDlyHMSM(0, 0, 0, num) ;//�ӳ�release ʱ��Ҫ�ӳ�
        if(!Rfid.Card_type)continue;
        RFID_Send(Cmd_Anticoll, NULL, NULL, NULL);//����ͻok
        OSTimeDlyHMSM(0, 0, 0, num);
        RFID_Send(Cmd_Select, Rfid.Snr1, NULL, NULL);//ѡ��Ƭok
        OSTimeDlyHMSM(0, 0, 0, num);
        RFID_Send(Cmd_Authentic, NULL, NULL, 0x03);//���� У������ok
        OSTimeDlyHMSM(0, 0, 0, num);
        
        if((!memcmp(Rfid.Snr1,Rfid.Snr2,4)) && Rfid.Snr1[0] && Rfid.Snr2[0]){
            //�ڶ���ˢ��
            LED_Clear();
            if(!memcmp(Rfid.Data_block1,Master_Card,16)){//�ж��Ƿ��ǹ���Ա��
                RFID_Send(Cmd_W_Block, unlock, NULL, 3*4+2);
            }
            memset(&Rfid,0,RFID_STRUCT_SIZE);
            continue;
            //LED_Blue();
        }
            
        RFID_Send(Cmd_R_Block, NULL, NULL, 3*4+1);
        OSTimeDlyHMSM(0, 0, 0, num);

        if(Rfid.Data_block1[4] != 0){//�ж��ǵڼ���ˢ��
            if(memcmp(Rfid.Snr1,Rfid.Snr2,4) && (Rfid.Snr2[3] != 0xFF)){
                //��һ��ˢ��
                LED_Red();
                Rfid.Snr2[3] = 0xFF;
                if(!memcmp(Rfid.Data_block1,Master_Card,16)){//�ж��Ƿ��ǹ���Ա��
                }else if(!Rfid.Data_block1[0]){//�ж���δ����
                    //δ����
                    RFID_Send(Cmd_W_Block, lock, NULL, 3*4+2);
                }
            }
        }

//        if(num == 14){
//            num -= 14;            
//            RFID_Send(Cmd_A_Money, NULL, NULL, NULL);
//        }
//        if(num == 15){
//            num -= 15;
//            RFID_Send(Cmd_L_Money, NULL, NULL, NULL);
//        }
//        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

/*******************************************************************************
  * @��������		Task_APP_03
  * @����˵��		�������׮��������
  * @�������		��
  * @�������		��
  * @���ز���		��
*******************************************************************************/
//int sumi(int c, ...)
//{
//    va_list argptr;
//    va_start(argptr, c); //��ʼ����Ԫָ��
//    int sum = c;
//    c = va_arg(argptr, int); //��Ϊ��һ���������͵Ĳ������ͣ����ز�������
//    //c�����һ����������
//    printf("%d\n", c);
//    while(0 != c) //ĩβ�ض��ַ�ָʾ����
//    {
//        sum = sum + c;
//        c = va_arg(argptr, int);
//    }
//    va_end(argptr);
//    return sum;
//}
void Task_APP_03(void *pdata)
{
    pdata = pdata;

    OSStatInit();                   											/* ͳ�������ʼ�� */

    for(;;)
    {
//        sumi(1,2,3,4,5);
#if 1//test DLT
        if(num == 1){
            num -= 1;
            ReadData(DLT_Vol);
            OSTimeDlyHMSM(0, 0, 2, 0);
        }
        if(num == 2){
            num -= 2;
            ReadData(DLT_Cur);
            OSTimeDlyHMSM(0, 0, 2, 0);
        }
        if(num == 3){
            num -= 3;
            ReadData(DLT_Eng);
            OSTimeDlyHMSM(0, 0, 2, 0);
        }
        if(num == 4){
            num -= 4;
            ReadData(DLT_YYMMDDWW);
            OSTimeDlyHMSM(0, 0, 2, 0);
        }
        if(num == 5){
            num -= 5;
            ReadData(DLT_hhmmss);
            OSTimeDlyHMSM(0, 0, 2, 0);
        }
#endif
        OSTimeDlyHMSM(0, 0, 2, 0);
    }
}

/*******************************************************************************
  * @��������		Task_APP_04
  * @����˵��		��
  * @�������		��
  * @�������		��
  * @���ز���		��
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

//        LED_Red();
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        LED_Green();
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        LED_Blue();
//        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

/*******************************************************************************
  * @��������		Task_Family_Energy_Storage
  * @����˵��		��ͥ����2.5�����Ŀ
  * @�������		��
  * @�������		��
  * @���ز���		��
*******************************************************************************/
void Task_Family_Energy_Storage(void *pdata)
{
    pdata = pdata;
    for(;;)
    {
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}


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
  * @��������		AppStartTask
  * @����˵��		��ʼ���񣬴���������������������
  * @�������		��
  * @�������		��
  * @���ز���		��
*******************************************************************************/
static void Task_Start(void *pdata)
{
#if OS_CRITICAL_METHOD == 3
    OS_CPU_SR cpu_sr;
#endif
    OS_ENTER_CRITICAL();

    pdata = pdata;
    BSP_Start();                                                /* Start BSP and tick initialization                    */

    uint8_t	err;                                                                //������Ϣ
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
    //�������׮��������
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
    //������ʼ����.
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
