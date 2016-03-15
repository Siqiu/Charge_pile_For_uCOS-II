/**
  ******************************************************************************
  * @file    Module_LED.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */
#ifndef   __MODULE_LED_H__
#define   __MODULE_LED_H__


#define LED_GREEN           (1<<3)/* p0.19 */
#define LED_BLUE            (1<<4)/* p0.20 */
#define LED_RED             (1<<5)/* p0.21 */
#define CON_485             (1<<2)/* p0.26 */

#define P02_06              (1<<6)/* p2.06 */
#define P02_07              (1<<7)/* p2.07 */
#define P00_18              (1<<2)/* p0.18 relays*/
/* Exported valable -------------------------------------------------------- */
/* Public functions ---------------------------------------------------------*/

void GPIO_Config(void);

void LED_Clear(void);

void LED_Red(void);

void LED_Green(void);

void LED_Blue(void);

void Relays_Open(void);

void Relays_Close(void);
/* private functions---------------------------------------------------------*/

#endif	/* __MODULE_LED_H__ */

