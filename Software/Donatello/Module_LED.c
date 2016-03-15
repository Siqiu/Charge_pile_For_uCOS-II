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
#include "lpc17xx_gpio.h"

void GPIO_Config(void)
{
    FIO_ByteSetDir(0, 2, LED_GREEN|LED_BLUE|LED_RED|P00_18, 1);
    FIO_ByteClearValue(0, 2, LED_GREEN|LED_BLUE|LED_RED|P00_18);
    
    FIO_ByteSetDir(0, 3, CON_485, 1);
    FIO_ByteClearValue(0, 3, CON_485);
    
    FIO_ByteSetDir(2, 0, P02_06, 1);
    FIO_ByteClearValue(2, 0, P02_06);
}
void LED_Clear(void)
{
    /* p0.19、p0.20、p0.21输出0*/
    FIO_ByteClearValue(0, 2, LED_GREEN);
    FIO_ByteClearValue(0, 2, LED_BLUE);
    FIO_ByteClearValue(0, 2, LED_RED);
}

void LED_Green(void)
{
    LED_Clear();
    
    FIO_ByteSetValue(0, 2, LED_GREEN);/* p0.19 */
}

void LED_Blue(void)
{
    LED_Clear();
    FIO_ByteSetValue(0, 2, LED_BLUE);/* p0.20 */
}

void LED_Red(void)
{
    LED_Clear();               
    FIO_ByteSetValue(0, 2, LED_RED);/* p0.21 */
}

/**
 * Params:Relays open
 *  @
 * Return:
 * Limitation:
 */
void Relays_Open(void)
{
    FIO_ByteSetValue(0, 2, P00_18);
}

/**
 * Params:Relays close
 *  @
 * Return:
 * Limitation:
 */
void Relays_Close(void)
{
    FIO_ByteClearValue(0, 2, P00_18);
}
/**
 * Params:
 *  @
 * Return:
 * Limitation:
 */
