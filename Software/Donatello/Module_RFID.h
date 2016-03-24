/**
  ******************************************************************************
  * @file    Module_RFID.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-3-17
  * @brief
  ******************************************************************************
  */
#ifndef   __MODULE_RFID_H__
#define   __MODULE_RFID_H__

#include "Module_crc.h"
#include "includes.h"

/* Exported valable -------------------------------------------------------- */
typedef enum {
	OP_OK = 0,            /* (0)  函数调用成功 */
	OP_NOTAG,             /* (1)  在操作区域内无卡 */
	OP_CRC,               /* (2)  卡片CRC error */
	OP_EMPTY,             /* (3)  数值溢出 */
	OP_AUTH,              /* (4)  验证不成功 */
	OP_PARITY,            /* (5)  卡片奇偶检验 error */
	OP_CODE,              /* (6)  通讯 error */
	OP_SERNR = 8,         /* (8)  防冲突过程中读系列号 error */
	OP_NOTAUTH = 10,      /* (10) 卡片没有通过验证 */
	OP_BITCNT,            /* (11) 从卡片收到的位数 error */
	OP_BYTECNT,           /* (12) 从卡片收到的字节数 error only read function valid */
	OP_TRANS = 14,        /* (14) Call transfer function error */
	OP_WRITE,             /* (15) Call write function error */
	OP_INCR,              /* (16) Call increment(增加) function error */
	OP_DECR,              /* (17) Call decrement(减少) function error */
	OP_READ,              /* (18) Call read function error */
	OP_HL_COMPERR = 40,   /* (40) 高级写比较 error */
	OP_HL_STRUCTERR,      /* (41) 高级加/减数据结构 error */
	OP_CMDERR             /* (42) Commend error */
} RFID_ERRNUM;

typedef struct {
    uint16_t Card_type;//卡类型
    uint8_t  Snr1[4];//第一次刷的卡片系列号
    uint8_t  Snr2[4];//第二次刷的卡片系列号
    uint8_t  Card_SC;//storage capacity
    uint8_t  Data_block1[16];//普通卡
    uint8_t  Data_block2[16];//管理员卡
    uint8_t  HWVersion[18];
} RFID_STRUCT;
#define RFID_STRUCT_SIZE    sizeof(RFID_STRUCT)

//typedef enum {
#define Cmd_R_HWVersion 0x4F /* (1) Read hardware version */
#define Cmd_Load_Key    0x4C /* (2) Download password to card reader */
#define Cmd_Request     0x41 /* (3) Find card request */
#define Cmd_Anticoll    0x42 /* (4) 防冲突 */
#define Cmd_Select      0x43 /* (5) Select card */
#define Cmd_Authentic   0x44 /* (6) Check password */
#define Cmd_Halt        0x45 /* (7) Stop card */
#define Cmd_R_Block     0x46 /* (8) Read a block */
#define Cmd_W_Block     0x47 /* (9) Write a block */
#define Cmd_A_Money     0x48 /* (10) Add money */
#define Cmd_L_Money     0x49 /* (11) Less money */
    //Cmd_Transfer    = 0x4B  /* (12) Money operation */
//} RFID_DATATYPE;
/* Public functions ---------------------------------------------------------*/
void RFID_Send(uint8_t cmd, uint8_t* buf1, uint8_t* buf2, uint8_t data);

RFID_ERRNUM CheckPack_RFID(void);
/* private functions---------------------------------------------------------*/

#endif	/* __MODULE_RFID_H__ */

