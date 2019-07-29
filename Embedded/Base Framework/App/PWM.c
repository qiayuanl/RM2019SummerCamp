#include "User.h"

#if(USING_PWM == 1)

void PWM_Init(void)
{
	MX_TIM2_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM8_Init();   
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
}
void PWM_SetDuty(TIM_HandleTypeDef *tim,uint32_t tim_channel,float duty)
{
	switch(tim_channel)
	{
		case TIM_CHANNEL_1: 
			tim->Instance->CCR1 = (PWM_RESOLUTION*duty) - 1;
		break;
		case TIM_CHANNEL_2: 
			tim->Instance->CCR2 = (PWM_RESOLUTION*duty) - 1;
		break;
		case TIM_CHANNEL_3: 
			tim->Instance->CCR3 = (PWM_RESOLUTION*duty) - 1;
		break;
		case TIM_CHANNEL_4:
			tim->Instance->CCR4 = (PWM_RESOLUTION*duty) - 1;
		break;
	}
}

#endif	//#if(USING_PWM == 1)
