#include "User.h"

#if(USING_KEY == 1)

void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin=GPIO_PIN_2;
	GPIO_InitStructure.Pull=GPIO_PULLDOWN;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
}
unsigned char Read_Key(void)
{
	if(PBin(2)==1)
	{
		Delayms(20);
		if(PBin(2)==1)
		{
			while(PBin(2));
			return 'K';
		}
		else
			return 'N';
	}
	unsigned int ADC_Back=Get_ADC(6);
	Delayms(20);
	if(ADC_Back!=Get_ADC(6))
		ADC_Back=Get_ADC(6);
	if(ADC_Back<100)
		return 'M';
	if(ADC_Back<900&&ADC_Back>800)
		return 'L';
	if(ADC_Back<1800&&ADC_Back>1700)
		return 'R';
	if(ADC_Back<2500&&ADC_Back>2400)
		return 'U';
	if(ADC_Back<3300&&ADC_Back>3200)
		return 'D';
	return 'N';
}

#endif	//USING_KEY
