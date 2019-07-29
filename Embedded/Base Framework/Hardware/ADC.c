#include "adc.h"
#include "User.h"

#if(USING_ADC == 1)

ADC_HandleTypeDef ADC1_Handler;//ADC句柄
void ADC1_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //使能ADC1时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStructure.Mode=GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pin=GPIO_PIN_0|GPIO_PIN_1;
		GPIO_InitStructure.Pull=GPIO_NOPULL;
		GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_InitStructure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
		HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_InitStructure.Pin=GPIO_PIN_6;
		HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         //1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 //初始化 
}
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_3CYCLES;        //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
	  return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //返回最近一次ADC1规则组的转换结果
}
//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_ADC(u32 ch)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<50;t++)
	{
		temp_val+=Get_Adc(ch);
		//Delayus(100);
	}
	return temp_val/50;
} 
//得到温度值
//返回值:温度值(扩大了100倍,单位:℃.)
static u32 adcx;
static short result=0;
static double temperate=0;
short Get_Temprate(void)
{
	adcx=Get_ADC(ADC_CHANNEL_TEMPSENSOR);//读取内部温度传感器通道,10次取平均
	temperate=(float)adcx*(3.3/4096);		//电压值
	temperate=(temperate-0.76)/0.0025 + 25; //转换为温度值 
	result=temperate*=100;					//扩大100倍.
	return result;
}

#endif	//#if(USING_ADC == 1)
