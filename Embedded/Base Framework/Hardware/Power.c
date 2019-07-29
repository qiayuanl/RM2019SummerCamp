#include "User.h"

#if(USING_POWER == 1)

void Power_Init(void)
{
	GPIO_InitTypeDef TypeDefPower;
	__HAL_RCC_GPIOH_CLK_ENABLE();
	TypeDefPower.Pin=GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	TypeDefPower.Mode=GPIO_MODE_OUTPUT_PP;
	TypeDefPower.Pull=GPIO_PULLUP;
	TypeDefPower.Speed=GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOH,&TypeDefPower);
	PHout(2)=0;
	PHout(3)=0;
	PHout(4)=0;
	PHout(5)=0;
}

#endif	//#if(POWER == 1)
