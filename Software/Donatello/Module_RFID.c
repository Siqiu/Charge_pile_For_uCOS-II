/**
  ******************************************************************************
  * @file    Module_RFID.h
  * @author  Donatello
  * @version V1.0
  * @date    2016-3-17
  * @brief
  ******************************************************************************
  */

#include "lpc17xx.h"
#include "Module_RFID.h"

/**
 * Params:
 *  @AA/BB 数据包头（ 1 字节）
 *  @Command 命令码（ 1 字节）
 *  @Status 指令调用的结果（ 1 字节）
 *  @Len 数据长度（ 1 字节）
 *  @Data[I] 数据
 *  @Bcc 异或校验码
 * Return:
 * Limitation:
 */
void RFID_Send(uint8_t cmd, uint8_t* buf1, uint8_t** buf2)
{
    uint8_t Send_data[20] = {0};
    uint8_t header = 0xAA;
    uint8_t *ptr = Send_data;
    uint16_t len = 3;
    *ptr++ = header;
    *ptr++ = cmd;
    switch(cmd)
    {
        case Cmd_R_HWVersion://AA 4F 00 E5
            {
                *ptr++ = 0;
                len += 0;
                break;
            }
        case Cmd_Load_Key://aa 4c 12 00 01 14 8a c5 e2 28 28 00 00 ff ff ff ff ff ff 40 f6 fa
            uint8_t TKey[8];// 传输密码
            memcpy(TKey,buf1,sizeof(TKey));
            uint8_t NKey[8] = {0};// 要现在到RAM中的卡密码
            memcpy(NKey,buf2,sizeof(NKey));
            *ptr++ = 18;
            len += 18;
            *ptr++ = 0x00;//_Mode 0

            *ptr++ = 0x00;//_Secnr 扇区号

            *ptr++ = TKey[0];//TKey Data[2~9] 传输密码
            *ptr++ = TKey[1];
            *ptr++ = TKey[2];
            *ptr++ = TKey[3];
            *ptr++ = TKey[4];
            *ptr++ = TKey[5];
            *ptr++ = TKey[6];
            *ptr++ = TKey[7];

            *ptr++ = NKey[0];//Nkey Data[10~17] 要现在到RAM中的卡密码
            *ptr++ = NKey[1];
            *ptr++ = NKey[2];
            *ptr++ = NKey[3];
            *ptr++ = NKey[4];
            *ptr++ = NKey[5];
            *ptr++ = NKey[6];
            *ptr++ = NKey[7];
            break;
        case Cmd_Request://aa 41 01 01 eb
            *ptr++ = 1;
            len += 1;
            *ptr++ = 0x01;//_Mode
            break;
        case Cmd_Anticoll://aa 42 05 00 00 00 00 00 ed
            uint8_t Card_SetNUM1[5] = {0};
            *ptr++ = 5;
            len += 5;
            *ptr++ = Card_SetNUM1[0];// 卡片系列号
            *ptr++ = Card_SetNUM1[1];
            *ptr++ = Card_SetNUM1[2];
            *ptr++ = Card_SetNUM1[3];
            *ptr++ = Card_SetNUM1[4];
            break;
        case Cmd_Select://aa 43 04 82 8a 8d 5d 35
            uint8_t Card_SetNUM2[4] = {0};
            *ptr++ = 4;
            len += 4;
            *ptr++ = Card_SetNUM2[0];// 卡片系列号
            *ptr++ = Card_SetNUM2[1];
            *ptr++ = Card_SetNUM2[2];
            *ptr++ = Card_SetNUM2[3];
            break;
        case Cmd_Authentic://aa 44 02 00 01 ed
            *ptr++ = 2;
            len += 2;
            *ptr++ = 0x00;// _Mode 0
            *ptr++ = 0x00;// _SecNr 扇区号
            break;
        case Cmd_Halt://aa 45 00 ef
            *ptr++ = 0;
            len += 0;            
            break;
        case Cmd_R_Block://aa 46 01 04 e9 //读 4 块即 1 扇区 0 块的数据/////////////////
            uint8_t R_data_block[16] = {0};
            *ptr++ = 16;
            len += 16;
            *ptr++ = R_data_block[0];// 数据块的第一个字节
            *ptr++ = R_data_block[1];
            *ptr++ = R_data_block[2];
            *ptr++ = R_data_block[3];
            *ptr++ = R_data_block[4];
            *ptr++ = R_data_block[5];
            *ptr++ = R_data_block[6];
            *ptr++ = R_data_block[7];
            *ptr++ = R_data_block[8];
            *ptr++ = R_data_block[9];
            *ptr++ = R_data_block[10];
            *ptr++ = R_data_block[11];
            *ptr++ = R_data_block[12];
            *ptr++ = R_data_block[13];
            *ptr++ = R_data_block[14];
            *ptr++ = R_data_block[15];
            break;
        case Cmd_W_Block://aa 47 11 04 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff f8//写 16 字节数据
            uint8_t W_data_block[17] = {0};
            *ptr++ = 17;
            len += 17;
            *ptr++ = W_data_block[0];// 要写入数据的块地址
            *ptr++ = W_data_block[1];// 数据块的第一个字节
            *ptr++ = W_data_block[2];
            *ptr++ = W_data_block[3];
            *ptr++ = W_data_block[4];
            *ptr++ = W_data_block[5];
            *ptr++ = W_data_block[6];
            *ptr++ = W_data_block[7];
            *ptr++ = W_data_block[8];
            *ptr++ = W_data_block[9];
            *ptr++ = W_data_block[10];
            *ptr++ = W_data_block[11];
            *ptr++ = W_data_block[12];
            *ptr++ = W_data_block[13];
            *ptr++ = W_data_block[14];
            *ptr++ = W_data_block[15];
            *ptr++ = W_data_block[16];
            break;
        case Cmd_A_Money://aa 48 05 04 0a 00 00 00 e9//对 4 块加值 10
            uint8_t Money_add[4] = {0};
            *ptr++ = 5;
            len += 5;
            *ptr++ = Money_add[0];// 值
            *ptr++ = Money_add[1];
            *ptr++ = Money_add[2];
            *ptr++ = Money_add[3];
            break;
        case Cmd_L_Money://aa 49 05 04 0a 00 00 00 e9//对 4 块减值 10
            uint8_t Money_less[4] = {0};
            *ptr++ = 5;
            len += 5;
            *ptr++ = Money_less[0];// 值
            *ptr++ = Money_less[1];
            *ptr++ = Money_less[2];
            *ptr++ = Money_less[3];
            break;
        default:
            break;
    }
    len += 1;
    *ptr = CrcCheck_8(Send_data,len);
    UART_Puts (LPC_UART3, Send_data, len);
}
