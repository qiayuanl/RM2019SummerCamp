#ifndef CRC8_CRC16_H
#define CRC8_CRC16_H

#include <algorithm>
#include <stdint.h>

uint8_t Get_CRC8_Check_Sum(uint8_t *pchMessage,int dwLength,uint8_t ucCRC8);
uint32_t Verify_CRC8_Check_Sum(uint8_t *pchMessage, int dwLength);
void Append_CRC8_Check_Sum(uint8_t *pchMessage, int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

#endif