#ifndef __tim_H
#define __tim_H
#include "stm32f4xx_hal.h"
#include "System_Config.h"
#define TIM_PSC_APB1 ((APB1_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define PWM_RESOLUTION 9000
#define APB1_TIMER_CLOCKS 90000000
#define PWM_DEFAULT_DUTY 5000
#define TIM_PSC_APB2 ((APB2_TIMER_CLOCKS/PWM_FREQUENCE)/PWM_RESOLUTION -1)
#define APB2_TIMER_CLOCKS 192000000
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim8;
void MX_TIM2_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);
void MX_TIM8_Init(void);    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
#endif
