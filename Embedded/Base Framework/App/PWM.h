#ifndef _PWM_H_
#define _PWM_H_
void PWM_Init(void);
void PWM_SetDuty(TIM_HandleTypeDef *tim,uint32_t tim_channel,float duty);
#endif
