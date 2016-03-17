/**
  ******************************************************************************
  * @file    Module_CRC.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-3-16
  * @brief
  ******************************************************************************
  */
#ifndef   __MODULE_CRC_H__
#define   __MODULE_CRC_H__

/* Exported valable -------------------------------------------------------- */
/* Public functions ---------------------------------------------------------*/

uint8_t CrcCheck_MOD(uint8_t *buf, uint16_t len);

uint8_t	CrcCheck_8( uint8_t* ptr, uint16_t len);

uint16_t CrcCheck_16( uint8_t* ptr, uint16_t len );
/* private functions---------------------------------------------------------*/

#endif	/* __MODULE_CRC_H__ */

