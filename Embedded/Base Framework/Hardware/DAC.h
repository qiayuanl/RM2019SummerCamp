#ifndef __DAC_H
#define __DAC_H
#include "System_Config.h"
extern DAC_HandleTypeDef DAC1_Handler;//DAC¾ä±ú
void DAC1_Init(void);
void DAC2_Init(void);
void DAC1_Set(u16 vol);
void DAC2_Set(u16 vol);
#endif
