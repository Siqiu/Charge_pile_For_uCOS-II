/*
*********************************************************************************************************
*                                              uC/OS-II
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-II is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-II in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-II.  The fact that the source code is provided
*               does NOT mean that you can use it without paying a licensing fee.
*
*               Knowledge of the source code may NOT be used to develop a similar product.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       APPLICATION CONFIGURATION
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT


/*
*********************************************************************************************************
*                                       ADDITIONAL uC/MODULE ENABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

//#define  APP_CFG_TASK_START_PRIO                           2u

//#define  APP_CFG_TASK_JOY_PRIO                             3u
//#define  APP_CFG_TASK_LCD_PRIO                             4u

#define  OS_TASK_TMR_PRIO                         (OS_LOWEST_PRIO - 2)


#define PRIO_START                  (4)                                         /* 开始任务 */
#define PRIO_APP_01                 (8)
#define PRIO_APP_02                 (9)                                        /* SHELL terminal 除空闲任务，优先级最低 */
#define PRIO_APP_03                 (10)
#define PRIO_APP_04                 (11)


/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/
#define TASK_STK_SIZE               (128)                                       /* 所有任务统一使用128字节堆栈地址 */
#define TASK_0_STK_SIZE             (TASK_STK_SIZE)
#define TASK_1_STK_SIZE             (TASK_STK_SIZE)
#define TASK_2_STK_SIZE             (TASK_STK_SIZE)
#define TASK_3_STK_SIZE             (TASK_STK_SIZE)
#define TASK_4_STK_SIZE             (TASK_STK_SIZE)

/*
*********************************************************************************************************
*                                             uC/LIB CONFIGURATION
*********************************************************************************************************
*/

#define  LIB_MEM_CFG_OPTIMIZE_ASM_EN                    DEF_ENABLED
#define  LIB_MEM_CFG_ARG_CHK_EXT_EN                     DEF_DISABLED
#define  LIB_MEM_CFG_ALLOC_EN                           DEF_ENABLED
#define  LIB_MEM_CFG_HEAP_SIZE                          20*1024u


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#define  TRACE_LEVEL_OFF                                0u
#define  TRACE_LEVEL_INFO                               1u
#define  TRACE_LEVEL_DBG                                2u


#define  APP_CFG_TRACE_LEVEL                            TRACE_LEVEL_DBG
#define  APP_CFG_TRACE                                  printf

#define  APP_TRACE_INFO(x)                     ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                      ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#endif
