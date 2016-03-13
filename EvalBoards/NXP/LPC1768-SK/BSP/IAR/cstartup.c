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
*                                         EXCEPTION VECTORS
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
* Filename      : app_vect.c
* Version       : V1.00
* Programmer(s) : FT
*                 DC
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <os_cpu.h>
#include  <os_csp.h>
#include  <lib_def.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;

/*
*********************************************************************************************************
*                                            LOCAL TABLES
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

#pragma language=extended
#pragma segment="CSTACK"

static  void  App_NMI_ISR        (void);

static  void  App_Fault_ISR      (void);

static  void  App_BusFault_ISR   (void);

static  void  App_UsageFault_ISR (void);

static  void  App_MemFault_ISR   (void);

static  void  App_Spurious_ISR   (void);

extern  void  __iar_program_start(void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
*               vector table.  The LPC17xx has 34 external interrupt vectors.
*********************************************************************************************************
*/

__root  const  APP_INTVECT_ELEM  __vector_table[] @ ".intvec" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /*  0, SP start value.                                */
    __iar_program_start,                                        /*  1, PC start value.                                */
    App_NMI_ISR,                                                /*  2, NMI.                                           */
    App_Fault_ISR,                                              /*  3, Hard Fault.                                    */
    App_MemFault_ISR,                                           /*  4, Memory Management.                             */
    App_BusFault_ISR,                                           /*  5, Bus Fault.                                     */
    App_UsageFault_ISR,                                         /*  6, Usage Fault.                                   */
    App_Spurious_ISR,                                           /*  7, Reserved.                                      */
    App_Spurious_ISR,                                           /*  8, Reserved.                                      */
    App_Spurious_ISR,                                           /*  9, Reserved.                                      */
    App_Spurious_ISR,                                           /* 10, Reserved.                                      */
    App_Spurious_ISR,                                           /* 11, SVCall.                                        */
    App_Spurious_ISR,                                           /* 12, Debug Monitor.                                 */
    App_Spurious_ISR,                                           /* 13, Reserved.                                      */
    OS_CPU_PendSVHandler,                                       /* 14, PendSV Handler.                                */
    OS_CPU_SysTickHandler,                                      /* 15, uC/OS-III Tick ISR Handler.                    */

    OS_CPU_IntHandler,                                          /* 16, INTISR[  0]  WatchDog Timer                    */
    OS_CPU_IntHandler,                                          /* 17, INTISR[  1]  Timer 0 (Match 0-3 Capture 0-3).  */
    OS_CPU_IntHandler,                                          /* 18, INTISR[  2]  Timer 1 (Match 0-3 Capture 0-3).  */
    OS_CPU_IntHandler,                                          /* 19, INTISR[  3]  Timer 2 (Match 0-3 Capture 0-3).  */
    OS_CPU_IntHandler,                                          /* 20, INTISR[  4]  Timer 3 (Match 0-3 Capture 0-3).  */
    OS_CPU_IntHandler,                                          /* 21, INTISR[  5]  UART 0  (RLS, THRE, RDA, CTI)     */
    OS_CPU_IntHandler,                                          /* 22, INTISR[  6]  UART 1  (RLS, THRE, RDA, CTI)     */
    OS_CPU_IntHandler,                                          /* 23, INTISR[  7]  UART 2  (RLS, THRE, RDA, CTI)     */
    OS_CPU_IntHandler,                                          /* 24, INTISR[  8]  UART 3  (RLS, THRE, RDA, CTI)     */
    OS_CPU_IntHandler,                                          /* 25, INTISR[  9]  PWM Interrupt                     */
    OS_CPU_IntHandler,                                          /* 26, INTISR[ 10]  I2C0   (RLS, THRE, RDA, CTI)      */
    OS_CPU_IntHandler,                                          /* 27, INTISR[ 11]  I2C1   (RLS, THRE, RDA, CTI)      */
    OS_CPU_IntHandler,                                          /* 28, INTISR[ 12]  I2C2   (RLS, THRE, RDA, CTI)      */
    OS_CPU_IntHandler,                                          /* 29, INTISR[ 13]  SPI0    (SPIF, MODF)              */
    OS_CPU_IntHandler,                                          /* 30, INTISR[ 14]  SSP0                              */
    OS_CPU_IntHandler,                                          /* 31, INTISR[ 15]  SSP1                              */
    OS_CPU_IntHandler,                                          /* 32, INTISR[ 15]  PLL lock (PLOCK)                  */
    OS_CPU_IntHandler,                                          /* 33, INTISR[ 15]  Real Time Clock Interrupt         */
    OS_CPU_IntHandler,                                          /* 34, INTISR[  7]  External interrupt 0 (EINT0)      */
    OS_CPU_IntHandler,                                          /* 35, INTISR[  8]  External interrupt 0 (EINT0)      */
    OS_CPU_IntHandler,                                          /* 36, INTISR[  9]  External interrupt 0 (EINT0)      */
    OS_CPU_IntHandler,                                          /* 37, INTISR[ 10]  External interrupt 0 (EINT0).     */
    OS_CPU_IntHandler,                                          /* 38, INTISR[ 11]  A/D Converter 0 end of conversion */
    OS_CPU_IntHandler,                                          /* 39, INTISR[ 12]  Brown out detect                  */
    OS_CPU_IntHandler,                                          /* 40, INTISR[ 12]  USB Interrupts, DMA Interrupt     */
    OS_CPU_IntHandler,                                          /* 41, INTISR[ 12]  CAN Interrupt                     */
    OS_CPU_IntHandler,                                          /* 42, INTISR[ 12]  IntSattus of DMA channel 0/1      */
    OS_CPU_IntHandler,                                          /* 23, INTISR[ 12]  SI (state change)                 */
    OS_CPU_IntHandler,                                          /* 44, INTISR[ 12]  Ethernet Interrupt                */
    OS_CPU_IntHandler,                                          /* 45, INTISR[ 12]  Repetitive interrupt timer        */
    OS_CPU_IntHandler,                                          /* 46, INTISR[ 12]  Motor Control PWM                 */
    OS_CPU_IntHandler,                                          /* 47, INTISR[ 12]  Quadrature Encoder                */
    OS_CPU_IntHandler,                                          /* 48, INTISR[ 12]  PLL1 lock (PLOCK)                 */
    OS_CPU_IntHandler,                                          /* 49, INTISR[ 12]  USB Activity                      */
    OS_CPU_IntHandler,                                          /* 50, INTISR[ 12]  CAN Activity                      */
};


/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

static  void  App_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_BusFault_ISR()
*
* Description : Handle bus fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_BusFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                          App_UsageFault_ISR()
*
* Description : Handle usage fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_UsageFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_MemFault_ISR()
*
* Description : Handle memory fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_MemFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}
