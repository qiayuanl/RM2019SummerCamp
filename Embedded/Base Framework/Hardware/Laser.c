#include "User.h"
void Laser_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=GPIO_PIN_13;
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
	LASER=1;  //´ò¿ª¼¤¹â
}
