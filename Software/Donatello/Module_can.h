/**
  ******************************************************************************
  * @file    Module_can.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-2-29
  * @brief
  ******************************************************************************
  */
#ifndef __MODULE_CAN_H__
#define __MODULE_CAN_H__

#include <lpc17xx.h>


void CAN1_Init(uint32_t BPS);
void CAN2_Init(uint32_t BPS);

#endif /* __MODULE_CAN_H__ */
