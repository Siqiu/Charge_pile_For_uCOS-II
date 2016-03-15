#ifndef __STASTR_H
#define __STASTR_H
/*****************************������д���ݺ����������*************************/
                                                  /*   ��ַ    ����*/ 
static uint8_t DwinWrite[8] = {0x5A,0xa5,0x05,0x82,0x00,0x00,0x00,0x00};
static uint8_t DwinRead[7]  = {0x5A,0xa5,0x04,0x83,0x00,0x00,0x01};
static uint8_t DLT_wakeup[4] ={0xfe,0xfe,0xfe,0xfe};

/*****************************************���ܱ���������***********************************/
const static char DReadAdd[12] = {0X68,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X68,0X13,0X00,0XDF,0X16};    // �����ȡ���ܱ��ͨ�ŵ�ַ
const static char ReadVol[16]  = {0X68,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X68,0X11,0X04,0X33,0X34,0x34,0x35,0xB1,0x16};  //�����ȡͨ�����ܱ�ĵ�ѹ
const static char ReadCur[16]  = {0X68,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X68,0X11,0X04,0X33,0X34,0x35,0x35,0xB2,0x16};  //�����ȡͨ���������ܱ�ĵ���
const static char ReadEng[16]  = {0X68,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X68,0X11,0X04,0X33,0X33,0x34,0x33,0xAE,0x16};  //�����ȡͨ�����ܱ�ĵ���

/**************************************����������������***************************************/
static uint8_t ReadAdd[6] = {0x5a,0xa5,0x03,0x81,0x03,0x02};            // ��ȡ��ǰͼƬ�ĵ�ַ
static uint8_t ReadCash[7] = {0x5a,0xa5,0x04,0x83,0x00,0x5a,01};         //�����ȡ���õ�Ѻ��
static uint8_t ReadPri[7] = {0x5a,0xa5,0x04,0x83,0x00,0x59,01};         //�����ȡ��ʱ��絥��
static uint8_t Exchang[6] = {0x5a,0xa5,0x04,0x80,0x00,0x00};            // �ӵ�ǰ�����л���ָ����ַ
static uint8_t ShowPileAdd[7] = {0x5a,0xa5,0x04,0x83,0x00,0x59,0x01};   //�����ȡ׮��ַ
                                                     //��ַ  1�������ݳ���
static uint8_t ShowPri[8] = {0x5a,0xa5,0x05,0x82,0x00,0x00,0x00,0x00};  //��ʾ��ǰ��ĵ��ۣ���ַ00 00  ���� 00 00
                                                    /*    ��ַ        ����     */
static uint8_t ShowBal[8] = {0x5a,0xa5,0x05,0x82,0x00,0x01,0x00,0x00};   //��ʾIC������ַ00 01  ��� 00 00
                                                    /*    ��ַ        ���     */
static uint8_t ShowTron[8] = {0x5a,0xa5,0x05,0x82,0x00,0x03,0x00,0x00};  //��ʾ�ɳ��������ַ00 03   ���� 00 00
                                                    /*    ��ַ        ����     */
static uint8_t ShowVal[8] = {0x5a,0xa5,0x05,0x82,0x00,0x10,0x00,0x00};   //��ʾ����ѹ����ַ00 10    ��ѹ 00 00
                                                    /*    ��ַ        ��ѹ    */
static uint8_t ShowCur[8] = {0x5a,0xa5,0x05,0x82,0x00,0x11,0x00,0x00};   //��ʾ����������ַ00 11    ���� 00 00
                                                    /*    ��ַ        ����    */
static uint8_t ShowEle[8] = {0x5a,0xa5,0x05,0x82,0x00,0x12,0x00,0x00};   //��ʾ���ѵ�������ַ00 12    ���� 00 00
                                                    /*    ��ַ        ����    */
static uint8_t ShowCost[8] = {0x5a,0xa5,0x05,0x82,0x00,0x13,0x00,0x00};  //��ʾ���ѽ���ַ00 13    ��� 00 00
                                                    /*    ��ַ        ���    */
static uint8_t ShowChargHour[8] = {0x5a,0xa5,0x05,0x82,0x00,0x77,0x00,0x00};  //��ʾ���ʱ��   ��ַ00 0c    ���ʱ��   ʱ
                                                        /*    ��ַ          ʱ    */
static uint8_t ShowChargMinute[8] = {0x5a,0xa5,0x05,0x82,0x00,0x78,0x00,0x00};  //��ʾ���ʱ�䣬��ַ00 0b    ���ʱ��   ��
                                                        /*    ��ַ          ��   */
const static uint8_t AccordAmtCharg[8] = {0XB0,0XB4,0XBD,0XF0,0XB6,0XEE,0XB3,0XE4};         // "������"
const static uint8_t AccordTimCharg[8] = {0XB0,0XB4,0Xca,0Xb1,0XBc,0XE4,0XB3,0XE4};         // "��ʱ���"
const static uint8_t AccordEleCharg[8] = {0XB0,0XB4,0Xb5,0Xe7,0Xc1,0Xbf,0XB3,0XE4};         // "��������"
const static uint8_t FullBatteCharg[8] = {0XB3,0XB4,0XC2,0XFA,0XB5,0XE7,0XB3,0XD8};         // "�������"

/************************************************CANͨ�ŵ�����****************************************************/
static uint8_t CANReadPri[8] = {0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x02};        // ��׮����   ׮״̬  ���
static uint8_t CANReadCar[8] = {0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x05};        // ����״̬������
static uint8_t CANReadExp[8] = {0x01,0x05,0x00,0x00,0x00,0x00,0x00,0x04};        // ��ȡ���ѵ���  ���ѽ��
static uint8_t CANReadBal[8] = {0x01,0x06,0x00,0x00,0x00,0x00,0x00,0x07};        // ��ȡ���ڽ��
static uint8_t CANReadTim[8] = {0x01,0x07,0x00,0x00,0x00,0x00,0x00,0x06};        // ��ȡʱ��

static uint8_t CANConStar[8] = {0x01,0x02,0x00,0x00,0x00,0xff,0x00,0xfc};         //  ����׮����
static uint8_t CANConStop[8] = {0x01,0x02,0x00,0x00,0x00,0x00,0xff,0xfc};         //  ����׮ֹͣ

/***************************************************�����������******************************************/
static uint8_t const CardPassword[8] = {0x11,0x22,0x33,0x44,0x55,0x66};            // ����������
static uint8_t  LoadKey[22] = {0xaa,0x4c,0x12,0x00,0x01,0x14,0x8a,0xc5,0xe2,0x28,0x28,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x40,0xf6,0xfa};   // �������뵽������
static uint8_t  Request[5] = {0xaa,0x41,0x01,0x01,0xeb};                          // Ѱ��                 
static uint8_t  Anticoll[9] = {0xaa,0x42,0x05,0x00,0x00,0x00,0x00,0x00,0xed};     // ����ͻ
static uint8_t  Select[8] = {0xaa,0x43,0x04};             //ѡ��
static uint8_t  Authentication[6] = {0xaa,0x44,0x02,0x00,0x01,0xed};       //   ��֤����
static uint8_t  Halt[4] = {0xaa,0x45,0x00,0xef};           // ͣ��      
static uint8_t  ReadCardData[5] = {0xaa,0x46,0x01,0x04,0xe9};    //  �����Ŀ�����ݣ���1����0�������
static uint8_t  ReadCardFlag[5] = {0xaa,0x46,0x01,0x05,0xe8};    //  �����������ݣ���1����1�������       ����־λ 
static uint8_t  WriteNormalData[21] = {0xaa,0x47,0x11,0x04,0x00,0x00,0x00,0xaa,0xbb,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe9};       //�����Ŀ�����д��16�ֽڵ�����
static uint8_t  WriteAbnormalData[21] = {0xaa,0x47,0x11,0x04,0x00,0x00,0x00,0xbb,0xaa,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe9};     //�����Ŀ�����д��16�ֽڵ�����
#endif                                     /* 47��������  11������   04����ַ*/
