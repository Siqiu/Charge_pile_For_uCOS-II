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
uint8_t CrcCheck_MOD(uint8_t *buf, uint16_t len)
{
    uint8_t *ptr = buf;
    uint8_t CRC = 0;
    while(len--)
    {
        CRC += (*ptr++)%256;
    }
    return CRC;
}
/**
 * Params: crcCheck & crcCheck16
 *  @buf : data head addr
 *  @len : data len
 * Return:Parity data
 * Limitation:
 */
uint8_t	CrcCheck_8( uint8_t* ptr, uint16_t len)
{
	uint16_t i = 0;
	uint8_t data = 0, crc1 = 0;
	for( i = 0; i < len; i++, ptr++ )
	{
		data = *ptr;
		if( i == 0 )
			crc1 = data;
		else
			crc1 = crc1 ^ data;
	}
	return crc1;
}

uint16_t CrcCheck_16( uint8_t* ptr, uint16_t len )
{
	uint16_t i = 0;
	uint16_t data = 0, crc1 = 0;
	for( i = 0; i < len; i++, ptr++ )
	{
		data = *ptr;
		if( i == 0 )
			crc1 = data;
		else
			crc1 = crc1 ^ data;
	}
	return crc1;
}