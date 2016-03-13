/**
  ******************************************************************************
  * @file    Module_LED.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */

#include "lpc17xx.h"
#include "Module_LED.h"
void LED_config(void)
{
    LPC_GPIO0->FIODIR |= 0x04380000;                    /* p0.19、p0.20、p0.21引脚选择为输出*/
    //LPC_GPIO0->FIOPIN |= 0x0;                       /* p0.19、p0.20、p0.21输出0*/
    LPC_GPIO0->FIOSET |= 0x0;                       /* p0.19、p0.20、p0.21输出0*/
}
void LED_CLEAR(void)
{
    LPC_GPIO0->FIOPIN &= (~0x00380000);                /* p0.19、p0.20、p0.21输出0*/
}
void LED_RED(void)
{
    LED_CLEAR();
    LPC_GPIO0->FIOPIN |= 0x00200000;                /* p0.21=1*/
}

void LED_GREEN(void)
{
    LED_CLEAR();
    LPC_GPIO0->FIOPIN |= 0x00080000;                /* p0.19=1*/
}

void LED_BLUE(void)
{
    LED_CLEAR();
    LPC_GPIO0->FIOPIN |= 0x00100000;                /* p0.20=1*/
}
