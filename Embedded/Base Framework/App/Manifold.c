#include "User.h"
#if ( USING_MANIFOLD == 1 )
Summer_Camp_Info_t Summer_Camp_Info;
Map_Data_t Map_Data;
Chassis_Data_t Chassis_Data;
void Wait_For_HandShake(void)
{
	static uint8_t Hand_Shake_Data[8] = {0x5a,0xa5,0x5a,0xa5,0x5a,0xa5,0x5a,0xa5};
	Send_Data(PID_HAND_SHAKE, Hand_Shake_Data);
	
}
void Communicate_Task(void * pvParameters)
{
	//Wait_For_HandShake();		//µ»¥˝Œ’ ÷
	while(1)
	{
//		Cvt_Map_Data(&Summer_Camp_Info, &Map_Data);
//		Send_Map_Data(&Map_Data);
		Send_Chassis_Data(&Chassis_Data);
		vTaskDelay(100);
	}
}

#endif	//#if(USING_MANIFOLD == 1)
