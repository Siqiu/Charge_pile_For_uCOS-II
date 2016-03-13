;/*
;********************************************************************************************************
;                                    EXCEPTION VECTORS & STARTUP CODE
;
; File      : cstartup.s
; For       : ARMv7M Cortex-M3
; Mode      : Thumb2
; Toolchain : RealView Development Suite
;             RealView Microcontroller Development Kit (MDK)
;             ARM Developer Suite (ADS)
;             Keil uVision
;********************************************************************************************************
;*/

;/*
;********************************************************************************************************
;*                           <<< Use Configuration Wizard in Context Menu >>>
;*
;* Note(s) : (1) The µVision4 Configuration Wizard enables menu driven configuration of assembler, 
;*               C/C++, or debugger initialization files. The Configuration Wizard uses control items 
;*               that are embedded into the comments of the configuration file.
;*
;********************************************************************************************************
;*/ 

;/*
;********************************************************************************************************
;*                                              STACK DEFINITIONS
;*
;* Configuration Wizard Menu:
; //<h> Stack Configuration
; //  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; //</h>
;*********************************************************************************************************
;*/

Stack_Size      EQU     0x00000200
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


;/*
;********************************************************************************************************
;*                                              STACK DEFINITIONS
; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
;*********************************************************************************************************
;*/

Heap_Size       EQU     0x00000000
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB

                                                  ; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                                IMPORT OS_CPU_IntHandler 
                                
                IMPORT  OS_CPU_PendSVHandler                                        
                IMPORT  OS_CPU_SysTickHandler                                       


__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     App_NMI_ISR               ; /*  2, NMI.                                           */
                DCD     App_Fault_ISR             ; /*  3, Hard Fault.                                    */
                DCD     App_MemFault_ISR          ; /*  4, Memory Management.                             */
                DCD     App_BusFault_ISR          ; /*  5, Bus Fault.                                     */
                DCD     App_UsageFault_ISR        ; /*  6, Usage Fault      .                             */
                DCD     App_Spurious_ISR          ; /*  7, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /*  8, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /*  9, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /* 10, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /* 11, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /* 12, Reserved.                                      */
                DCD     App_Spurious_ISR          ; /* 13, Reserved.                                      */
                DCD     OS_CPU_PendSVHandler      ; /* 14, PendSV Handler.                                */
                DCD     OS_CPU_SysTickHandler     ; /* 15, uC/OS-III Tick ISR Handler.                    */

                                         
                ; External Interrupts
                DCD     OS_CPU_IntHandler         ; /* 16, INTISR[  0]  WatchDog Timer                    */
                DCD     OS_CPU_IntHandler         ; /* 17, INTISR[  1]  Timer 0 (Match 0-3 Capture 0-3).  */
                DCD     OS_CPU_IntHandler         ; /* 18, INTISR[  2]  Timer 1 (Match 0-3 Capture 0-3).  */
                DCD     OS_CPU_IntHandler         ; /* 19, INTISR[  3]  Timer 2 (Match 0-3 Capture 0-3).  */
                DCD     OS_CPU_IntHandler         ; /* 20, INTISR[  4]  Timer 3 (Match 0-3 Capture 0-3).  */
                DCD     OS_CPU_IntHandler         ; /* 21, INTISR[  5]  UART 0  (RLS, THRE, RDA, CTI)     */
                DCD     OS_CPU_IntHandler         ; /* 22, INTISR[  6]  UART 1  (RLS, THRE, RDA, CTI)     */
                DCD     OS_CPU_IntHandler         ; /* 23, INTISR[  7]  UART 2  (RLS, THRE, RDA, CTI)     */
                DCD     OS_CPU_IntHandler         ; /* 24, INTISR[  8]  UART 3  (RLS, THRE, RDA, CTI)     */
                DCD     OS_CPU_IntHandler         ; /* 25, INTISR[  9]  PWM Interrupt                     */
                DCD     OS_CPU_IntHandler         ; /* 26, INTISR[ 10]  I2C0   (RLS, THRE, RDA, CTI)      */
                DCD     OS_CPU_IntHandler         ; /* 27, INTISR[ 11]  I2C1   (RLS, THRE, RDA, CTI)      */
                DCD     OS_CPU_IntHandler         ; /* 28, INTISR[ 12]  I2C2   (RLS, THRE, RDA, CTI)      */
                DCD     OS_CPU_IntHandler         ; /* 29, INTISR[ 13]  SPI0    (SPIF, MODF)              */
                DCD     OS_CPU_IntHandler         ; /* 30, INTISR[ 14]  SSP0                              */
                DCD     OS_CPU_IntHandler         ; /* 31, INTISR[ 15]  SSP1                              */
                DCD     OS_CPU_IntHandler         ; /* 32, INTISR[ 15]  PLL lock (PLOCK)                  */
                DCD     OS_CPU_IntHandler         ; /* 33, INTISR[ 15]  Real Time Clock Interrupt         */
                DCD     OS_CPU_IntHandler         ; /* 34, INTISR[  7]  External interrupt 0 (EINT0)      */
                DCD     OS_CPU_IntHandler         ; /* 35, INTISR[  8]  External interrupt 0 (EINT0)      */
                DCD     OS_CPU_IntHandler         ; /* 36, INTISR[  9]  External interrupt 0 (EINT0)      */
                DCD     OS_CPU_IntHandler         ; /* 37, INTISR[ 10]  External interrupt 0 (EINT0).     */
                DCD     OS_CPU_IntHandler         ; /* 38, INTISR[ 11]  A/D Converter 0 end of conversion */
                DCD     OS_CPU_IntHandler         ; /* 39, INTISR[ 12]  Brown out detect                  */
                DCD     OS_CPU_IntHandler         ; /* 40, INTISR[ 12]  USB Interrupts, DMA Interrupt     */
                DCD     OS_CPU_IntHandler         ; /* 41, INTISR[ 12]  CAN Interrupt                     */
                DCD     OS_CPU_IntHandler         ; /* 42, INTISR[ 12]  IntSattus of DMA channel 0/1      */
                DCD     OS_CPU_IntHandler         ; /* 23, INTISR[ 12]  SI (state change)                 */
                DCD     OS_CPU_IntHandler         ; /* 44, INTISR[ 12]  Ethernet Interrupt                */
                DCD     OS_CPU_IntHandler         ; /* 45, INTISR[ 12]  Repetitive interrupt timer        */
                DCD     OS_CPU_IntHandler         ; /* 46, INTISR[ 12]  Motor Control PWM                 */
                DCD     OS_CPU_IntHandler         ; /* 47, INTISR[ 12]  Quadrature Encoder                */
                DCD     OS_CPU_IntHandler         ; /* 48, INTISR[ 12]  PLL1 lock (PLOCK)                 */
                DCD     OS_CPU_IntHandler         ; /* 49, INTISR[ 12]  USB Activity                      */
                DCD     OS_CPU_IntHandler         ; /* 50, INTISR[ 12]  CAN Activity                      */


                IF      :LNOT::DEF:NO_CRP
                AREA    |.ARM.__at_0x02FC|, CODE, READONLY
CRP_Key         DCD     0xFFFFFFFF
                ENDIF


                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                
                LDR     R0, =__main
                BX      R0
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)                

App_NMI_ISR      PROC
                EXPORT  App_NMI_ISR                   [WEAK]
                B       .
                ENDP
App_Fault_ISR\
                PROC
                EXPORT  App_Fault_ISR                 [WEAK]
                B       .
                ENDP
App_MemFault_ISR\
                PROC
                EXPORT  App_MemFault_ISR             [WEAK]
                B       .
                ENDP
App_BusFault_ISR\
                PROC
                EXPORT  App_BusFault_ISR             [WEAK]
                B       .
                ENDP
App_UsageFault_ISR\
                PROC
                EXPORT  App_UsageFault_ISR            [WEAK]
                B       .
                ENDP
App_Spurious_ISR\
                PROC
                EXPORT  App_Spurious_ISR                [WEAK]
                B       .
                ENDP
App_Reserved_ISR\
                PROC
                EXPORT  App_Reserved_ISR                [WEAK]
                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
