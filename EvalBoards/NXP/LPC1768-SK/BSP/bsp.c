/*
*********************************************************************************************************
*                                              uC/OS-III
*                                        The Real-Time Kernel
*
*                          (c) Copyright 2004-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/OS-III is provided in source form for FREE evaluation, for educational
*               use or peaceful research.  If you plan on using uC/OS-III in a commercial
*               product you need to contact Micrium to properly license its use in your
*               product.  We provide ALL the source code for your convenience and to
*               help you experience uC/OS-III.  The fact that the source code is provided
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
*                                       BOARD SUPPORT PACKAGE (BSP)
*
*                                        IAR Development Kits
*                                               on the
*
*                                            NXP LPC1768
*                                      LPC1766-SK KICKSTART KIT
*
* Filename      :bsp.c
* Version       :V1.00
* Programmer(s) :FT
*                DC
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_MODULE
#include <bsp.h>
#include <csp.h>
#include <os_cpu.h>

/*
*********************************************************************************************************
*                                             REGISTER & BIT DEFINES
*********************************************************************************************************
*/

                                                                /* - SYSTEM CONTROL AND STATUS REGISTER & BIT DEFINES  -*/
#define  BSP_REG_SCS                             (*(CPU_REG32 *)(0x400FC1A0))
#define  BSP_BIT_SCS_OSCRANGE                    DEF_BIT_04
#define  BSP_BIT_SCS_OSCEN                       DEF_BIT_05
#define  BSP_BIT_SCS_OSCSTAT                     DEF_BIT_06

                                                                /* ------- CLOCK DIVIDERS REGISTER & BIT DEFINES ------ */
#define  BSP_REG_CCLKCFG                         (*(CPU_REG32 *)(0x400FC104))
#define  BSP_REG_CLKSRCSEL                       (*(CPU_REG32 *)(0x400FC10C))
#define  BSP_REG_PCLKSEL0                        (*(CPU_REG32 *)(0x400FC1A8))
#define  BSP_REG_PCLKSEL1                        (*(CPU_REG32 *)(0x400FC1AC))

                                                                /* ------------ PLL REGISTER & BIT DEFINES ------------ */
#define  BSP_ADDR_PLL                            (CPU_INT32U )0x400FC080u

#define  BSP_REG_PLLCTRL(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x00u))
#define  BSP_REG_PLLCFG(pll_id)                  (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x04u))
#define  BSP_REG_PLLSTAT(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x08u))
#define  BSP_REG_PLLFEED(pll_id)                 (*(CPU_REG32 *)(BSP_ADDR_PLL + (0x20u * (pll_id)) + 0x0Cu))

#define  BSP_MSK_PLLCFG0_MSEL                    0x00007FFFu
#define  BSP_MSK_PLLCFG0_NSEL                    0x00FF0000u

#define  BSP_MSK_PLLCFG1_MSEL                    0x0000001Fu
#define  BSP_MSK_PLLCFG1_NSEL                    0x00000060u

#define  BSP_BIT_PLLCTRL_PLLE                    DEF_BIT_00     /* PLL enable                                           */
#define  BSP_BIT_PLLCTRL_PLLC                    DEF_BIT_01     /* PLL connect                                          */

#define  BSP_BIT_PLLSTAT_PLLE0_STAT              DEF_BIT_24     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC0_STAT              DEF_BIT_25     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK0                  DEF_BIT_26     /* Refkect the PLL lock status                          */

#define  BSP_BIT_PLLSTAT_PLLE1_STAT              DEF_BIT_08     /* Read-back for the PLL enable bit                     */
#define  BSP_BIT_PLLSTAT_PLLC1_STAT              DEF_BIT_09     /* Read-back for the PLL connect bit                    */
#define  BSP_BIT_PLLSTAT_PLOCK1                  DEF_BIT_10     /* Refkect the PLL lock status                          */

#define  BSP_BIT_PLLFEED_VAL0                    0x000000AAu
#define  BSP_BIT_PLLFEED_VAL1                    0x00000055u

#define  BSP_BIT_CLKSRCSEL_RC                    DEF_BIT_NONE
#define  BSP_BIT_CLKSRCSEL_MAIN                  DEF_BIT_00
#define  BSP_BIT_CLKSRCSEL_RTC                   DEF_BIT_01

                                                                /* --- FLASH ACCELERATOR CFG REGISTER & BIT DEFINES --- */
#define  BSP_REG_FLASHCFG                        (*(CPU_REG32 *)(0x400FC000u))

#define  BSP_MSK_FLASHCFG_CLK_1                  DEF_BIT_MASK(1u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_2                  DEF_BIT_MASK(2u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_3                  DEF_BIT_MASK(3u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_4                  DEF_BIT_MASK(4u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_5                  DEF_BIT_MASK(5u, 12u)
#define  BSP_MSK_FLASHCFG_CLK_6                  DEF_BIT_MASK(6u, 12u)
#define  BSP_MSK_FLASHCFG_RST_VAL                0x0000003Au

/*
*********************************************************************************************************
*                                             GPIO DEFINES
*********************************************************************************************************
*/
                                                               /* ----------------- GPIO PIN DEFINITION ------------- */
#define  BSP_GPIO0_BUT1                          DEF_BIT_23    /* P0.23 Push Button 1                                 */
#define  BSP_GPIO0_JOY_CENTER                    DEF_BIT_05    /* P0.5  Joystick Center  switch contact               */
#define  BSP_GPIO0_LED2                          DEF_BIT_04    /* P0.4  LED1                                          */

#define  BSP_GPIO1_LED1                          DEF_BIT_25    /* P1.25 LED1                                          */

#define  BSP_GPIO2_JOY_UP                        DEF_BIT_00    /* P2.0  Joystick Up      switch contact               */
#define  BSP_GPIO2_JOY_DOWN                      DEF_BIT_01    /* P2.1  Joystick Down    switch contact               */
#define  BSP_GPIO2_JOY_LEFT                      DEF_BIT_07    /* P2.7  Joystick Left    switch contact               */
#define  BSP_GPIO2_JOY_RIGHT                     DEF_BIT_08    /* P2.8  Joystick Right   switch contact               */

#define  BSP_GPIO2_BUT2                          DEF_BIT_13    /* P2.13 Push Button 2                                 */

#define  BSP_REG_DEMCR             (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR            (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT        (*(CPU_REG32 *)0xE0001004)


/*
*********************************************************************************************************
*                                             BSP CONSTANS VALUES
*********************************************************************************************************
*/
                                                               /* Maximum register timeout                            */
#define  BSP_VAL_MAX_TO                         (CPU_INT16U)(0xFFFF)




/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


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
*                                               MACRO'S
*********************************************************************************************************
*/

#define  BSP_PLL_FEED_SEQ(pll_nbr)              {    CPU_CRITICAL_ENTER();                             \
                                                                                                       \
                                                     BSP_REG_PLLFEED(pll_nbr) = BSP_BIT_PLLFEED_VAL0;  \
                                                     BSP_REG_PLLFEED(pll_nbr) = BSP_BIT_PLLFEED_VAL1;  \
                                                                                                       \
                                                     CPU_CRITICAL_EXIT();                              \
                                                }


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The PLL0 Frequency is determined by:
*
*                       Fcco = (2 x M x Fin) / N
*
*                       where M   = PLL0 Multipler
*                             N   = PLL0 Pre-dividier
*                            Fin  = PLL0 Input Frequency (Main oscillator).
*
*               (2) PLL0 settings must meet the following:
*                       Fin is in the range of 32Khz to 50 Mhz.
*                       Fcco is in the range of 275 Mhz to 550 Mhz
*
*               (3) The LPC17xx CPU frequency is determined by:
*
*                       CPU_freq = Fcc0 / CPU_Div
*
*               (4) The USB clock frequency is determined by:
*
*                       USB_Clk = M x Fosc x P, or USB_clk = Fcco / (2 x P)
*
*                       Fcco    =  Fosc x 2 x M x 2 x P
*
*                       where Fcco = PLL1 output frequency.
*                             M    = PLL1 multiplier.
*                             P    = PLL1 pre-divider.
*                             Fosc = Oscialltor  frequency.
*
*               (5) The PLL1 inputs and settings must meet the following criteria:
*                   Fosc    is in the range of 10 Mhz to 25 Mhz.
*                   USBClk  is 48 Mhz
*                   Fcco    is in the range of 156 Mhz to 320 Mhz
*
*               (6) In this example the LPC1768 operates:
*
*                       PLL0_Fcco = 2 x 25 x 12 / 2
*                       PLL0_Fcco = 300mhz
*
*                       CPU_freq  =  300 Mhz  / 3
*                                 =  100 Mhz
*
*                       PLL1_Fcc0 = 12 x 4 x 2 x 2 = 192 Mhz.
*                       USB_Clk   = 12 x 4         =  48 Mhz.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    CPU_INT16U    reg_to;
    CPU_INT32U    reg_val;
    CPU_SR_ALLOC();

                                                                /* ---------------- CLOCK INITIALIZATION -------------- */
    BSP_REG_FLASHCFG = BSP_MSK_FLASHCFG_CLK_6                   /* Set 6 cycles to acces the Flash memory.              */
                     | BSP_MSK_FLASHCFG_RST_VAL;
                                                                /* ----------- MAIN OSCILLATOR INITIALIZATION --------- */
    DEF_BIT_CLR(BSP_REG_SCS, BSP_BIT_SCS_OSCRANGE);             /* Set the main oscillator range                        */


    reg_to = BSP_VAL_MAX_TO;

    DEF_BIT_SET(BSP_REG_SCS, BSP_BIT_SCS_OSCEN);                /* Enable the Main Oscillator                           */

                                                                /* Wait until the main oscillator is enabled.           */
    while (DEF_BIT_IS_CLR(BSP_REG_SCS, BSP_BIT_SCS_OSCSTAT) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {                                         /* Configuration fail                                   */
        return;
    }

    BSP_REG_PCLKSEL0 = DEF_BIT_NONE;                            /* All peripheral clock runrs at CPU_Clk / 4 = 25 Mhz   */
    BSP_REG_PCLKSEL1 = DEF_BIT_NONE;

                                                                /* ------------------ PLL0 CONFIGURATION -------------- */

    reg_val  = (((25u - 1u) <<  0u) & BSP_MSK_PLLCFG0_MSEL)     /* PLL0 values M = 25 & N = 2 (see note #6)             */
             | ((( 2u - 1u) << 16u) & BSP_MSK_PLLCFG0_NSEL);
                                                                /* 1. Disconnect PLL0 with one feed sequence if PLL ... */
                                                                /* ... already connected.                               */
    if (DEF_BIT_IS_SET(BSP_REG_PLLSTAT(0u), BSP_BIT_PLLSTAT_PLLC0_STAT)) {
        DEF_BIT_CLR(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLC);
        BSP_PLL_FEED_SEQ(0u);
    }

    DEF_BIT_CLR(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLE);     /* 2. Disable PLL0 with one feed sequence               */
    BSP_PLL_FEED_SEQ(0u);

    BSP_REG_CCLKCFG   = (1u - 1u);                              /* 3. Change the CPU clock divider setting to speed ... */
                                                                /* ... operation without PLL0                           */

    BSP_REG_CLKSRCSEL = BSP_BIT_CLKSRCSEL_MAIN;                 /* 4. Select the main osc. as the PLL0 clock source     */

    BSP_REG_PLLCFG(0u) = reg_val;                               /* 5. Write to the PLLCFG and make it effective with... */
    BSP_PLL_FEED_SEQ(0u)                                        /* ... one one feed sequence                            */

    DEF_BIT_SET(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLE);     /* 6. Enable PLL0 with one feed sequence                */
    BSP_PLL_FEED_SEQ(0u);

    BSP_REG_CCLKCFG   = (3u - 1u);                              /* 7. Change the CPU clock divider setting for ...      */
                                                                /* ... operation with PLL0                              */

    reg_to = BSP_VAL_MAX_TO;                                    /* 8. Wait for PLL0 to achieve lock by monitoring ...   */
                                                                /* ... the PLOCK0 bit in the PLL0STAT                   */
    while (DEF_BIT_IS_CLR(BSP_REG_PLLSTAT(0u), BSP_BIT_PLLSTAT_PLOCK0) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {
        return;
    }

    DEF_BIT_SET(BSP_REG_PLLCTRL(0u), BSP_BIT_PLLCTRL_PLLC);     /* 9. Connect PLL0 with one feed sequence               */
    BSP_PLL_FEED_SEQ(0u);
                                                                /* ------------------ PLL1 CONFIGURATION -------------- */
    reg_val  = (((4u - 1u)  <<  0u) & BSP_MSK_PLLCFG1_MSEL)     /* PLL1 values M = 4; P = 2 coded as '01' (see note #6) */
             | (((0x01u   ) <<  5u) & BSP_MSK_PLLCFG1_NSEL);

    DEF_BIT_CLR(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLC);     /* 1. Disconnect PLL1 with one feed sequence            */
    BSP_PLL_FEED_SEQ(1u);

    DEF_BIT_CLR(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLE);     /* 2. Disable PLL1 with one feed sequence               */
    BSP_PLL_FEED_SEQ(1u);

    BSP_REG_PLLCFG(1u) = reg_val;                               /* 3. Write to the PLLCFG and make it effective with... */
    BSP_PLL_FEED_SEQ(1u);                                       /* ... one one feed sequence                            */

    DEF_BIT_SET(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLE);     /* 4. Enable PLL1 with one feed sequence                */
    BSP_PLL_FEED_SEQ(1u);

    reg_to = BSP_VAL_MAX_TO;                                    /* 5. Wait for PLL1 to achieve lock by monitoring ...   */
                                                                /* ... the PLOCK1 bit in the PLL1STAT                   */
    while (DEF_BIT_IS_CLR(BSP_REG_PLLSTAT(1u), BSP_BIT_PLLSTAT_PLOCK1) &&
          (reg_to > 0u)) {
        reg_to--;
    }

    if (reg_to == 0u) {
        return;
    }

    DEF_BIT_SET(BSP_REG_PLLCTRL(1u), BSP_BIT_PLLCTRL_PLLC);     /* 6. Connect PLL1 with one feed sequence               */
    BSP_PLL_FEED_SEQ(1u);


    BSP_REG_FLASHCFG = BSP_MSK_FLASHCFG_CLK_5                   /* Set 5 cycles to acces the Flash memory.              */
                     | BSP_MSK_FLASHCFG_RST_VAL;

    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_00,
                 BSP_GPIO0_LED2,
                 CSP_GPIO_DIR_OUT,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 0u,
                 CSP_GPIO_FNCT_00);

    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_01,
                 BSP_GPIO1_LED1,
                 CSP_GPIO_DIR_OUT,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 0u,
                 CSP_GPIO_FNCT_00);

    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_00,
                 BSP_GPIO0_BUT1,
                 CSP_GPIO_DIR_IN,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 0u,
                 CSP_GPIO_FNCT_00);

    CSP_GPIO_Cfg(CSP_GPIO_PORT_NBR_02,
                 BSP_GPIO2_BUT2,
                 CSP_GPIO_DIR_IN,
                 CSP_GPIO_FLAG_MODE_NONE,
                 DEF_NO,
                 0u,
                 CSP_GPIO_FNCT_00);

    BSP_LED_Off(0);

    CSP_GPIO_Cfg( CSP_GPIO_PORT_NBR_02,
                 (BSP_GPIO2_JOY_RIGHT |
                  BSP_GPIO2_JOY_DOWN  |
                  BSP_GPIO2_JOY_LEFT  |
                  BSP_GPIO2_JOY_RIGHT),
                  CSP_GPIO_DIR_IN,
                  CSP_GPIO_FLAG_MODE_NONE,
                  DEF_NO,
                  0u,
                  CSP_GPIO_FNCT_00);

    CSP_GPIO_Cfg( CSP_GPIO_PORT_NBR_00,
                  BSP_GPIO0_JOY_CENTER,
                  CSP_GPIO_DIR_IN,
                  CSP_GPIO_FLAG_MODE_NONE,
                  DEF_NO,
                  0u,
                  CSP_GPIO_FNCT_00);

    CSP_IntInit();
    CSP_IntDisAll(CSP_INT_CTRL_NBR_MAIN);
}

/*
*********************************************************************************************************
*                                              BSP_Start()
*
* Description : Initialize all the peripherals that required OS services (OS initialized)
*
* Argument(s) : tick_rate : The kernel tick rate in Hz.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Start (void)
{
    CPU_INT32U  cnts;
    CPU_INT32U  cpu_freq;


    cpu_freq = CSP_PM_CPU_ClkFreqGet();                         /* Get the CPU clock                                    */
#if (OS_VERSION >= 30000u)
    cnts     = (cpu_freq / OSCfg_TickRate_Hz);                  /* Calculate the number of SysTick counts               */
#else
    cnts     = (cpu_freq / OS_TICKS_PER_SEC);
#endif
    OS_CPU_SysTickInit(cnts);                                   /* Call the Generic OS Systick initialization           */

}


/*
*********************************************************************************************************
*                                             BSP_Joy_GetPos()
*
* Description : Get the position of the Joystick on the board.
*
* Argument(s) : none.
*
* Return(s)   : The current position of the Joystick.
*
*                           BSP_JOY_NONE     If the Joystick is not being pressed.
*                           BSP_JOY_UP       If the Joystick is toggled up.
*                           BSP_JOY_DOWN     If the Joystick is toggled down.
*                           BSP_JOY_RIGHT    If the Joystick is toggled right.
*                           BSP_JOY_LEFT     If the Joystick is toggled left.
*                           BSP_JOY_CENTER   If the Joystick is being pressed.
*
* Caller(s)  : Application.
*
* Note(s)    : none.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Joy_GetPos (void)
{
    CPU_INT32U  port_val;


    port_val = CSP_GPIO_Rd(CSP_GPIO_PORT_NBR_02);

    if (DEF_BIT_IS_CLR(port_val, BSP_GPIO2_JOY_UP)) {
        return (BSP_JOY_UP);
    }

    if (DEF_BIT_IS_CLR(port_val, BSP_GPIO2_JOY_DOWN)) {
        return (BSP_JOY_DOWN);
    }

    if (DEF_BIT_IS_CLR(port_val, BSP_GPIO2_JOY_LEFT)) {
        return (BSP_JOY_LEFT);
    }

    if (DEF_BIT_IS_CLR(port_val, BSP_GPIO2_JOY_RIGHT)) {
        return (BSP_JOY_RIGHT);
    }

    port_val = CSP_GPIO_Rd(CSP_GPIO_PORT_NBR_00);

    if (DEF_BIT_IS_CLR(port_val, BSP_GPIO0_JOY_CENTER)) {
        return (BSP_JOY_CENTER);
    }

    return (BSP_JOY_NONE);
}


/*
*********************************************************************************************************
*                                             BSP_Joy_GetStatus()
*
* Description : Get the status of the Joystick at a given position.
*
* Argument(s) :   joy_pos.   Position of the Joystick.
*
*                             BSP_JOY_UP       If the Joystick is toggled up.
*                             BSP_JOY_DOWN     If the Joystick is toggled down.
*                             BSP_JOY_RIGHT    If the Joystick is toggled right.
*                             BSP_JOY_LEFT     If the Joystick is toggled left.
*                             BSP_JOY_CENTER   If the Joystick is being pressed.
*
* Return(s)   :  The current Joystick status at a given position.
*
*                             DEF_ON           If the Joystick is     in the position specifies by joy_position
*                             DEF_OFF          If the Joystick is not in the position specifies by joy_position
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Joy_GetStatus (CPU_INT08U  joy_pos)
{
    CPU_INT08U joy_status;


    joy_status = BSP_Joy_GetPos();

    if (joy_status == joy_pos) {
        return (DEF_ON);
    }

    return (DEF_OFF);
}



/*
*********************************************************************************************************
*                                                 BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led_id   The ID of the LED to control:
*
*                       0    turn OFF all LEDs on the board
*                       1    turn OFF USB_UP_LED1
*                       2    turn OFF USB_UP_LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U  led)
{
    switch (led)  {
        case 0u:
             CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        case 1u:
             CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             break;

        case 2u:
             CSP_GPIO_BitSet(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        default:
            break;
    }
}


/*
*********************************************************************************************************
*                                                 BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led_id   The ID of the LED to control:
*
*                       0    turn ON all LEDs on the board.
*                       1    turn ON USB_UP_LED1.
*                       2    turn ON USB_UP_LED2.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U  led)
{
    switch (led)  {
        case 0u:
             CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        case 1u:
             CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             break;

        case 2u:
             CSP_GPIO_BitClr(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        default:
            break;
    }
}


/*
*********************************************************************************************************
*                                                 BSP_LED_Toggle()
*
* Description : Toggles any or all the LEDs on the board.
*
* Argument(s) : led_id   The ID of the LED to control:
*
*                       0    toggle all LEDs on the board
*                       1    toggle USB_UP_LED1
*                       2    toggle USB_UP_LED2
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    switch (led)  {
        case 0u:
             CSP_GPIO_BitToggle(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             CSP_GPIO_BitToggle(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        case 1u:
             CSP_GPIO_BitToggle(CSP_GPIO_PORT_NBR_01, BSP_GPIO1_LED1);
             break;

        case 2u:
             CSP_GPIO_BitToggle(CSP_GPIO_PORT_NBR_00, BSP_GPIO0_LED2);
             break;

        default:
            break;
    }
}


/*
*********************************************************************************************************
*                                           BSP_PB_GetStatus()
*
* Description : Get the status of a push button on the board.
*
* Argument(s) : pb_id   The ID of the push button to probe
*
*                       1    probe the push button B1
*                       2    probe the push button B2
*
* Return(s)   : DEF_OFF  if the push button is not pressed
*               DEF_ONN    if the push button is     pressed
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_PB_GetStatus (CPU_INT08U  pb_id)
{
    CPU_INT32U   port_val;
    CPU_BOOLEAN  pb_status;


    pb_status = DEF_OFF;

    if (pb_id == 1u) {
        port_val = CSP_GPIO_Rd(CSP_GPIO_PORT_NBR_00);
        if (DEF_BIT_IS_CLR(port_val, BSP_GPIO0_BUT1)) {
            pb_status = DEF_ON;
        }
    } else if (pb_id == 2u) {
        port_val =  CSP_GPIO_Rd(CSP_GPIO_PORT_NBR_02);
        if (DEF_BIT_IS_CLR(port_val, BSP_GPIO2_BUT2)) {
            pb_status = DEF_ON;
        }
    } else {
        ;
    }

    return (pb_status);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;

    fclk_freq = CSP_PM_CPU_ClkFreqGet();

    BSP_REG_DEMCR  |= DEF_BIT_24;
    BSP_REG_DWT_CR |= DEF_BIT_00;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;


    ts_tmr_cnts  = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application interface (API) function which
*               MAY be implemented by application/BSP function(s) [see Note #1] & MAY be called by
*               application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be
*                   optionally defined by the developer when either of the following CPU features is
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured,
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less
*                       than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
    CPU_ERR     err;


    fclk_freq = CPU_TS_TmrFreqGet(&err);
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{

    return (0u);

}
#endif

