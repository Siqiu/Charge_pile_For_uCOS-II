@/*
@*********************************************************************************************************
@*                                              EXAMPLE CODE
@*
@*                             (c) Copyright 2011; Micrium, Inc.; Weston, FL
@*
@*                   All rights reserved.  Protected by international copyright laws.
@*                   Knowledge of the source code may not be used to write a similar
@*                   product.  This file may only be used in accordance with a license
@*                   and should not be redistributed in any way.
@*********************************************************************************************************
@*/


@/*
@*********************************************************************************************************
@*
@*                                         EXCEPTION VECTORS
@*
@*                                        IAR Development Kits
@*                                               on the
@*
@*                                            NXP LPC1768
@*                                      LPC1766-SK KICKSTART KIT
@*
@* Filename      : cstartup.s
@* Version       : V1.00
@* Programmer(s) : DC
@*********************************************************************************************************
@*/

@/*
@********************************************************************************************************
@*                                              STACK DEFINITIONS
@*********************************************************************************************************
@*/

  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler

@/*
@********************************************************************************************************
@*                                           LINKER SCRIPT DEFINITIONS
@*********************************************************************************************************
@*/

@/* start address for the initialization values of the .data section */
.word _sidata

@/* start address for the .data section */
.word _sdata

@/* end address for the .data section */
.word _edata

@/* start address for the .bss section */
.word _sbss

@/* end address for the .bss section */
.word _ebss

@/*
@**********************************************************************************************************
@ Description:      This is the code that gets called when the processor first
@                   starts execution following a reset event. Only the absolutely
@                   necessary set is performed, after which the application
@                   supplied main() routine is called.
@ Parameters: None
@
@ Return Value: None
@
@ Notes: The process would be the following:
@                       1. Copy .data section from ROM to RAM
@                       2. Clear .bss section (Zero init)
@                       3. Call system initialzation routine
@                       4. Run static constructors
@                       5. Enter main
@                       6. Loop forever if returning from main
@**********************************************************************************************************
@*/

@ Reset Handler
  .section  .text.Reset_Handler
  .weak Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr   sp, =_estack    /* set stack pointer */

@/* 1. copy .data section (Copy from ROM to RAM) */
  movs r1, #0
  b LoopCopyDataInit

CopyDataInit:
  ldr r3, =_sidata
  ldr r3, [r3, r1]
  str r3, [r0, r1]
  adds r1, r1, #4

LoopCopyDataInit:
  ldr r0, =_sdata
  ldr r3, =_edata
  adds r2, r0, r1
  cmp r2, r3
  bcc CopyDataInit
  ldr r2, =_sbss
  b LoopFillZerobss

@/* 2. Clear .bss section (Zero init) */
FillZerobss:                                           @ Zero fill the bss segment.
  movs r3, #0
  str r3, [r2], #4

LoopFillZerobss:
  ldr r3, = _ebss
  cmp r2, r3
  bcc FillZerobss

@/* 3. Call system initialzation routine */
  @bl SystemInit

@/* 4. Run static constructors  */
  bl __libc_init_array                                 @ Call static constructors

@/* 5. Enter main  */
  bl main

@/* 6. Loop forever if returning from main */
LoopForever:
  b LoopForever


.size Reset_Handler, .-Reset_Handler


@/*
@*********************************************************************************************************
@*                                  EXCEPTION / INTERRUPT VECTOR TABLE
@*
@* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
@*               vector table.  The STM32L1xxx has 81 external interrupt vectors.
@*
@*           (2) Interrupts vector 2-13 are implemented in this file as infinite loop for debuging
@*               purposes only. The application might implement a recover procedure if it is needed.
@*
@*           (3) OS_CPU_PendSVHandler() and OS_CPU_SysTickHandler() are implemented in the generic OS
@*               port.
@*********************************************************************************************************
@*/

@  .section  .text.Default_Handler,"ax",%progbits
@Default_Handler:
@InfiniteLoop:
@  b InfiniteLoop
@  .size Default_Handler, .-Default_Handler

/**
**===========================================================================
**  Reset, Exception, and Interrupt vectors
**===========================================================================
*/
  .section  .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
  .size g_pfnVectors, .-g_pfnVectors

    
                                                                @ Vector Table Mapped to Address 0 at Reset
                    .extern  OS_CPU_IntHandler
                    
                    .extern  OS_CPU_PendSVHandler
                    .extern  OS_CPU_SysTickHandler
                    
                    
g_pfnVectors:                                            @/* --------------- External Interrupts -------------- */
                      .word  _estack                     @/* 0: Stack pointer                                   */
                      .word  Reset_Handler               @/* 1: Reset                                           */
                      .word  App_NMI_ISR                 @/* 2: NMI                                             */
                      .word  App_Fault_ISR               @/* 3: HardFault                                       */
                      .word  App_MemFault_ISR            @/* 4: MemManage                                       */
                      .word  App_BusFault_ISR            @/* 5: BusFault                                        */
                      .word  App_UsageFault_ISR          @/* 6: Usage Fault                                     */
                      .word  App_Spurious_ISR            @/* 7: Reserved                                        */
                      .word  App_Spurious_ISR            @/* 8: Reserved                                        */
                      .word  App_Spurious_ISR            @/* 9: Reserved                                        */
                      .word  App_Spurious_ISR            @/* 10: Reserved                                       */
                      .word  App_Spurious_ISR            @/* 11: SVC                                            */
                      .word  App_Spurious_ISR            @/* 12: Reserved for Debug                             */
                      .word  App_Spurious_ISR            @/* 13: Reserved                                       */
                      .word  OS_CPU_PendSVHandler        @/* 14: PendSV Handler                                 */
                      .word  OS_CPU_SysTickHandler       @/* 15: uC/OS-III Tick ISR Handler                     */

                                                         @/* ------------- NXP LPC 1768 Peripherals ----------- */
                      .word  OS_CPU_IntHandler           @/* 16, INTISR[  0]  WatchDog Timer                    */
                      .word  OS_CPU_IntHandler           @/* 17, INTISR[  1]  Timer 0 (Match 0-3 Capture 0-3).  */
                      .word  OS_CPU_IntHandler           @/* 18, INTISR[  2]  Timer 1 (Match 0-3 Capture 0-3).  */
                      .word  OS_CPU_IntHandler           @/* 19, INTISR[  3]  Timer 2 (Match 0-3 Capture 0-3).  */
                      .word  OS_CPU_IntHandler           @/* 20, INTISR[  4]  Timer 3 (Match 0-3 Capture 0-3).  */
                      .word  OS_CPU_IntHandler           @/* 21, INTISR[  5]  UART 0  (RLS, THRE, RDA, CTI)     */
                      .word  OS_CPU_IntHandler           @/* 22, INTISR[  6]  UART 1  (RLS, THRE, RDA, CTI)     */
                      .word  OS_CPU_IntHandler           @/* 23, INTISR[  7]  UART 2  (RLS, THRE, RDA, CTI)     */
                      .word  OS_CPU_IntHandler           @/* 24, INTISR[  8]  UART 3  (RLS, THRE, RDA, CTI)     */
                      .word  OS_CPU_IntHandler           @/* 25, INTISR[  9]  PWM Interrupt                     */
                      .word  OS_CPU_IntHandler           @/* 26, INTISR[ 10]  I2C0   (RLS, THRE, RDA, CTI)      */
                      .word  OS_CPU_IntHandler           @/* 27, INTISR[ 11]  I2C1   (RLS, THRE, RDA, CTI)      */
                      .word  OS_CPU_IntHandler           @/* 28, INTISR[ 12]  I2C2   (RLS, THRE, RDA, CTI)      */
                      .word  OS_CPU_IntHandler           @/* 29, INTISR[ 13]  SPI0    (SPIF, MODF)              */
                      .word  OS_CPU_IntHandler           @/* 30, INTISR[ 14]  SSP0                              */
                      .word  OS_CPU_IntHandler           @/* 31, INTISR[ 15]  SSP1                              */
                      .word  OS_CPU_IntHandler           @/* 32, INTISR[ 15]  PLL lock (PLOCK)                  */
                      .word  OS_CPU_IntHandler           @/* 33, INTISR[ 15]  Real Time Clock Interrupt         */
                      .word  OS_CPU_IntHandler           @/* 34, INTISR[  7]  External interrupt 0 (EINT0)      */
                      .word  OS_CPU_IntHandler           @/* 35, INTISR[  8]  External interrupt 0 (EINT0)      */
                      .word  OS_CPU_IntHandler           @/* 36, INTISR[  9]  External interrupt 0 (EINT0)      */
                      .word  OS_CPU_IntHandler           @/* 37, INTISR[ 10]  External interrupt 0 (EINT0).     */
                      .word  OS_CPU_IntHandler           @/* 38, INTISR[ 11]  A/D Converter 0 end of conversion */
                      .word  OS_CPU_IntHandler           @/* 39, INTISR[ 12]  Brown out detect                  */
                      .word  OS_CPU_IntHandler           @/* 40, INTISR[ 12]  USB Interrupts, DMA Interrupt     */
                      .word  OS_CPU_IntHandler           @/* 41, INTISR[ 12]  CAN Interrupt                     */
                      .word  OS_CPU_IntHandler           @/* 42, INTISR[ 12]  IntSattus of DMA channel 0/1      */
                      .word  OS_CPU_IntHandler           @/* 23, INTISR[ 12]  SI (state change)                 */
                      .word  OS_CPU_IntHandler           @/* 44, INTISR[ 12]  Ethernet Interrupt                */
                      .word  OS_CPU_IntHandler           @/* 45, INTISR[ 12]  Repetitive interrupt timer        */
                      .word  OS_CPU_IntHandler           @/* 46, INTISR[ 12]  Motor Control PWM                 */
                      .word  OS_CPU_IntHandler           @/* 47, INTISR[ 12]  Quadrature Encoder                */
                      .word  OS_CPU_IntHandler           @/* 48, INTISR[ 12]  PLL1 lock (PLOCK)                 */
                      .word  OS_CPU_IntHandler           @/* 49, INTISR[ 12]  USB Activity                      */
                      .word  OS_CPU_IntHandler           @/* 50, INTISR[ 12]  CAN Activity                      */



@/*
@*********************************************************************************************************
@ Provide weak aliases for each Exception handler to the Default_Handler.
@ As they are weak aliases, any function with the same name will override
@ this definition.
@
@*********************************************************************************************************
@*/

    .section    .text.App_NMI_ISR
App_NMI_ISR:
    b   App_NMI_ISR
    .size   App_NMI_ISR, .-App_NMI_ISR


    .section    .text.App_Fault_ISR
App_Fault_ISR:
    b   App_Fault_ISR
    .size   App_Fault_ISR, .-App_Fault_ISR


    .section    .text.App_MemFault_ISR
App_MemFault_ISR:
    b   App_MemFault_ISR
    .size   App_MemFault_ISR, .-App_MemFault_ISR


    .section    .text.App_BusFault_ISR
App_BusFault_ISR:
    b   App_BusFault_ISR
    .size   App_BusFault_ISR, .-App_BusFault_ISR


    .section    .text.App_UsageFault_ISR
App_UsageFault_ISR:
    b   App_UsageFault_ISR
    .size   App_UsageFault_ISR, .-App_UsageFault_ISR

    .section    .text.App_Spurious_ISR
App_Spurious_ISR:
    b   App_Spurious_ISR
    .size   App_Spurious_ISR, .-App_Spurious_ISR


    .section    .text.App_Reserved_ISR
App_Reserved_ISR:
    b   App_Reserved_ISR
    .size   App_Reserved_ISR, .-App_Reserved_ISR

.end
