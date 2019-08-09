#include "User.h"

#if(USING_MACHINE_ARM == 1)

void Blanket_Open(void)		//物料盘打开
{
	PWM_SetDuty(&htim5,TIM_CHANNEL_3,1.5/20.0);
	vTaskDelay(1000);
	PWM_SetDuty(&htim5,TIM_CHANNEL_3,0.5/20.0);
	vTaskDelay(1000);
}

#endif	//#if(USING_MACHINE_ARM == 1)
