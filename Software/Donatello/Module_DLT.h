/**
  ******************************************************************************
  * @file    Module_DLT.h
  * @author  Donatello
  * @version V1.0
  * @date    2015.9.16
  * @brief
  ******************************************************************************
  */
#ifndef   __MODULE_DLT_H__
#define   __MODULE_DLT_H__
#include "includes.h"

/* Exported valable -------------------------------------------------------- */
typedef struct       // ±£´æµçÄÜ±íµÄÊý¾Ý
{
	uint8_t Val[2];         
	uint8_t Cur[2];          
	uint8_t Elenergy[4];
}DLTDat;

/* Public functions ---------------------------------------------------------*/
void ReadData(const uint8_t * ptr,uint16_t len);

/* private functions---------------------------------------------------------*/


#endif    /* __MODULE_DLT_H__ */
