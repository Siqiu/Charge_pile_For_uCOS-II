/**
  ******************************************************************************
  * @file    Module_CRC.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */

#include "lpc17xx.h"
#include "Module_crc.h"

/**
 * Params: crcCheck_MOD,Even Parity
 *  @buf : data head addr
 *  @len : data len
 * Return:Parity data
 * Limitation:
 */
uint8_t crcCheck_MOD(uint8_t *buf, uint16_t len)
{
    uint8_t *ptr = buf;
    uint8_t CRC = 0;
    while(len--)
    {
        CRC += (*ptr++)%256;
    }
    return CRC;
}
