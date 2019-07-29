#ifndef __RANDOM_H_
#define __RANDOM_H_

#include "System_Config.h"
u8  Random_Init(void);			//RNG初始化 
u32 RNG_Get_RandomNum(void);//得到随机数
int Get_RandomRange(int min,int max);//生成[min,max]范围的随机数
#endif

