#ifndef __can_H
#define __can_H
#include "stm32f4xx_hal.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
void MX_CAN1_Init(void);
void MX_CAN2_Init(void);
#endif
