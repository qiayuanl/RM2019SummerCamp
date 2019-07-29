#include "User.h"

#if(USING_BEEP == 1)

TIM_HandleTypeDef TIM12_Handler;
TIM_OC_InitTypeDef TIM12_CH1Handler;
u32 Compare1=0;
void Beep_Complete(void)
{
	Init_Beep(512);
	Beep_On();
	Delayms(250);
	Init_Beep(384*2);
	Beep_On();
	Delayms(250);
	Beep_Off();
	Init_Beep(2000);
}
void Beep_Startup(void)
{
	Init_Beep(512);
	Beep_On();
	Delayms(250);
	Init_Beep(288*2);
	Beep_On();
	Delayms(250);
	Init_Beep(384*2);
	Beep_On();
	Delayms(250);
	Beep_Off();
	Init_Beep(2000);
}
void TIM_SetTIM12Compare1(float compare)
{
	TIM12->CCR1=(u32)(Compare1*compare); 
}
void Init_Beep(unsigned int Frequency)
{
		GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM12_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOH_CLK_ENABLE();			//开启GPIOB时钟
    GPIO_Initure.Pin=GPIO_PIN_6;           	//PB4
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	  GPIO_Initure.Alternate= GPIO_AF9_TIM12;	//PB1复用为TIM3_CH4
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
	  Compare1=1000000/Frequency;
		TIM12_Handler.Instance=TIM12;
    TIM12_Handler.Init.Prescaler=89;
    TIM12_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;
    TIM12_Handler.Init.Period=(1000000/Frequency)-1;
    TIM12_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM12_Handler);
    TIM12_CH1Handler.OCMode=TIM_OCMODE_PWM1;
    TIM12_CH1Handler.Pulse=100;
    TIM12_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW;
    HAL_TIM_PWM_ConfigChannel(&TIM12_Handler,&TIM12_CH1Handler,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_1);
		Beep_Off();
}
void Beep_On(void)
{
	HAL_TIM_PWM_Start(&TIM12_Handler,TIM_CHANNEL_1);
	TIM_SetTIM12Compare1(0.95);
}
void Beep_Off(void)
{
	HAL_TIM_PWM_Stop(&TIM12_Handler,TIM_CHANNEL_1);
}

#endif //#if(USING_BEEP == 1)
