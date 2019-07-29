#include "User.h"

#if(USING_LED == 1)

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pin=GPIO_PIN_11;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_InitStructure.Pin=GPIO_PIN_14;
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure);
	GPIO_InitStructure.Pin=GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
	GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
	LED_G=LED_ON;
	LED_R=LED_OFF;
	LED_ALL_OFF();
}
void LED_Flash(void* pvParameters)
{
	while(1)
	{
		LED_R=1;
		LED_G=0;
		vTaskDelay(500);
		LED_R=0;
		LED_G=1;
		vTaskDelay(500);
	}
}

#endif	//#if(USING_LED == 1)
