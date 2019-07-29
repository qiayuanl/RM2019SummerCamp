#ifndef __ADC_H
#define __ADC_H
#include "System_Config.h"

void ADC1_Init(void); 				//ADC通道初始化
u16 Get_ADC(u32 ch);//得到某个通道给定次数采样的平均值
short Get_Temprate(void);    //获取CPU内核温度
#endif 
