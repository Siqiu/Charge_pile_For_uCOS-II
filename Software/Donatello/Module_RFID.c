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


extern UART_RING_BUFFER_T rb[3];
RFID_STRUCT Rfid;
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
void RFID_Send(uint8_t cmd, uint8_t* buf1, uint8_t* buf2, uint8_t data)
{
    if(buf1 == NULL){
        uint8_t Send_data[28] = {0};
        buf1 = Send_data;
    }
    uint8_t header = 0xAA;

    uint8_t *ptr = rb[2].tx;
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
            {
                *ptr++ = 18;
                len += 18;
                *ptr++ = 0x00;//_Mode 0

                *ptr++ = data;//_Secnr 扇区号

                *ptr++ = *buf1;*buf1++;//TKey Data[2~9] 传输密码
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;

                *ptr++ = *buf2;*buf2++;//Nkey Data[10~17] 要下载到RAM中的卡密码
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;*buf2++;
                *ptr++ = *buf2;
                break;
            }
        case Cmd_Request://aa 41 01 01 eb
            {
                *ptr++ = 1;
                len += 1;
                *ptr++ = 0x01;//_Mode
                break;
            }
        case Cmd_Anticoll://aa 42 05 00 00 00 00 00 ed
            {
                *ptr++ = 5;
                len += 5;
                *ptr++ = 0;// 卡片系列号
                *ptr++ = 0;
                *ptr++ = 0;
                *ptr++ = 0;
                *ptr++ = 0;
                break;
            }
        case Cmd_Select://aa 43 04 82 8a 8d 5d 35
            {
                *ptr++ = 4;
                len += 4;
                *ptr++ = *buf1;*buf1++;// 卡片系列号
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;
                break;
            }
        case Cmd_Authentic://aa 44 02 00 01 ed
            {
                *ptr++ = 2;
                len += 2;
                *ptr++ = 0x00;// _Mode 0
                *ptr++ = data;// _SecNr 扇区号
                break;
            }
        case Cmd_Halt://aa 45 00 ef
            {
                *ptr++ = 0;
                len += 0;            
                break;
            }
        case Cmd_R_Block://aa 46 01 04 e9 //读 4 块  即1扇区0块的数据/////////////////
            {
                *ptr++ = 1;
                len += 1;
                *ptr++ = data;
                break;
            }
        case Cmd_W_Block://aa 47 11 04 00 11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff f8//写 16 字节数据
            {
                *ptr++ = 17;
                len += 17;
                *ptr++ = data;// 要写入数据的块地址
                *ptr++ = *buf1;*buf1++;// 数据块的第一个字节
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;
                break;
            }
        case Cmd_A_Money://aa 48 05 04 0a 00 00 00 e9//对 4 块加值 10
            {
                *ptr++ = 5;
                len += 5;
                *ptr++ = *buf1;*buf1++;// 值
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;
                break;
            }
        case Cmd_L_Money://aa 49 05 04 0a 00 00 00 e9//对 4 块减值 10
            {
                *ptr++ = 5;
                len += 5;
                *ptr++ = *buf1;*buf1++;// 值
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;*buf1++;
                *ptr++ = *buf1;
                break;
            }
        default:
            {
                break;
            }
    }
    *ptr = CrcCheck_8(rb[2].tx,len);
    len += 1;
    UART_Puts (LPC_UART3, rb[2].tx, len);
    //UART_Send(LPC_UART3, Send_data, len, BLOCKING);
}

RFID_ERRNUM CheckPack_RFID(void)
{
    uint8_t * ptr = rb[2].rx;
    if((*ptr++) != 0xBB) return OP_CODE;
    switch(*ptr++)//error num
    {
        case OP_OK:
            {
                switch(*ptr++)//len
                {
                    case 0x01:
                        {
                            Rfid.Card_SC = *ptr++;
                            break;
                        }
                    case 0x02://Request
                        {
                            Rfid.Card_type = *ptr++;
                            Rfid.Card_type += ((*ptr++)<<8);
                            break;
                        }
                    case 0x04://Anticoll
                        {
                            if(Rfid.Snr1[0]){//第一次刷写入到Snr1，第二次刷写入到Snr2
                                Rfid.Snr2[0] = *ptr++;//_Snr(LL)低字节低位
                                Rfid.Snr2[1] = *ptr++;//_Snr(LH)
                                Rfid.Snr2[2] = *ptr++;//_Snr(HL)
                                Rfid.Snr2[3] = *ptr++;//_Snr(HH)
                            } else {
                                Rfid.Snr1[0] = *ptr++;//_Snr(LL)低字节低位
                                Rfid.Snr1[1] = *ptr++;//_Snr(LH)
                                Rfid.Snr1[2] = *ptr++;//_Snr(HL)
                                Rfid.Snr1[3] = *ptr++;//_Snr(HH)
                            }
                            
                            break;
                        }
                    case 0x10:
                        {
                            if(*ptr == 0x24){
                                Rfid.Data_block2[0]  = *ptr++;//数据块的第一字节
                                Rfid.Data_block2[1]  = *ptr++;
                                Rfid.Data_block2[2]  = *ptr++;
                                Rfid.Data_block2[3]  = *ptr++;
                                Rfid.Data_block2[4]  = *ptr++;
                                Rfid.Data_block2[5]  = *ptr++;
                                Rfid.Data_block2[6]  = *ptr++;
                                Rfid.Data_block2[7]  = *ptr++;
                                Rfid.Data_block2[8]  = *ptr++;
                                Rfid.Data_block2[9]  = *ptr++;
                                Rfid.Data_block2[10] = *ptr++;
                                Rfid.Data_block2[11] = *ptr++;
                                Rfid.Data_block2[12] = *ptr++;
                                Rfid.Data_block2[13] = *ptr++;
                                Rfid.Data_block2[14] = *ptr++;
                                Rfid.Data_block2[15] = *ptr++;
                            }else{
                                Rfid.Data_block1[0]  = *ptr++;//数据块的第一字节
                                Rfid.Data_block1[1]  = *ptr++;
                                Rfid.Data_block1[2]  = *ptr++;
                                Rfid.Data_block1[3]  = *ptr++;
                                Rfid.Data_block1[4]  = *ptr++;
                                Rfid.Data_block1[5]  = *ptr++;
                                Rfid.Data_block1[6]  = *ptr++;
                                Rfid.Data_block1[7]  = *ptr++;
                                Rfid.Data_block1[8]  = *ptr++;
                                Rfid.Data_block1[9]  = *ptr++;
                                Rfid.Data_block1[10] = *ptr++;
                                Rfid.Data_block1[11] = *ptr++;
                                Rfid.Data_block1[12] = *ptr++;
                                Rfid.Data_block1[13] = *ptr++;
                                Rfid.Data_block1[14] = *ptr++;
                                Rfid.Data_block1[15] = *ptr++;
                            }
                            
                            break;
                        }
                    case 0x12:
                        {
                            Rfid.HWVersion[0]  = *ptr++;
                            Rfid.HWVersion[1]  = *ptr++;
                            Rfid.HWVersion[2]  = *ptr++;
                            Rfid.HWVersion[3]  = *ptr++;
                            Rfid.HWVersion[4]  = *ptr++;
                            Rfid.HWVersion[5]  = *ptr++;
                            Rfid.HWVersion[6]  = *ptr++;
                            Rfid.HWVersion[7]  = *ptr++;
                            Rfid.HWVersion[8]  = *ptr++;
                            Rfid.HWVersion[9]  = *ptr++;
                            Rfid.HWVersion[10] = *ptr++;
                            Rfid.HWVersion[11] = *ptr++;
                            Rfid.HWVersion[12] = *ptr++;
                            Rfid.HWVersion[13] = *ptr++;
                            Rfid.HWVersion[14] = *ptr++;
                            Rfid.HWVersion[15] = *ptr++;
                            Rfid.HWVersion[16] = *ptr++;
                            Rfid.HWVersion[17] = *ptr++;
                            break;
                        }
                    default:
                        {
                            break;
                        }
                }
                return OP_OK;
            }
        case OP_NOTAG:
            {
                return OP_NOTAG;
                break;
            }
        case OP_CRC:
            {
                return OP_CRC;
                break;
            }
        case OP_EMPTY:
            {
                return OP_EMPTY;
                break;
            }
        case OP_AUTH:
            {
                return OP_AUTH;
                break;
            }
        case OP_PARITY:
            {
                return OP_PARITY;
                break;
            }
        case OP_SERNR:
            {
                return OP_SERNR;
                break;
            }
        case OP_NOTAUTH:
            {
                return OP_NOTAUTH;
                break;
            }
        case OP_BITCNT:
            {
                return OP_BITCNT;
                break;
            }
        case OP_BYTECNT:
            {
                return OP_BYTECNT;
                break;
            }
        case OP_TRANS:
            {
                return OP_TRANS;
                break;
            }
        case OP_WRITE:
            {
                return OP_WRITE;
                break;
            }
        case OP_INCR:
            {
                return OP_INCR;
                break;
            }
        case OP_DECR:
            {
                return OP_DECR;
                break;
            }
        case OP_READ:
            {
                return OP_READ;
                break;
            }
        case OP_HL_COMPERR:
            {
                return OP_HL_COMPERR;
                break;
            }
        case OP_HL_STRUCTERR:
            {
                return OP_HL_STRUCTERR;
                break;
            }
        case OP_CMDERR:
            {
                return OP_CMDERR;
                break;
            }
        default:
            {
                break;
            }
    }
    return OP_OK;
}
