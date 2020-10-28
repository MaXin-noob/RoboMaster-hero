/*
 * @Description:
 * @Version: new
 * @Autor: maxin
 * @Date: 2020-07-30 22:00:51
 * @LastEditors: maxin
 * @LastEditTime: 2020-07-31 10:09:24
 */
#ifndef __CRC_H
#define __CRC_H
#include "main.h"

#define JudgeBufferLength 255
#define JudgeFrameHeader 0xA5  //帧头
#define FrameHeader_Len 5
#define client_custom_Length 28

unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,
                                 unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage,
                                   unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength,
                             uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
#endif
