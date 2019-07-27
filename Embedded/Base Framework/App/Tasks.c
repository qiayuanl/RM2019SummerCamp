#include "User.h"
void Renew_PID(void)         //更新PID值
{
	PID1.Collect[1]=PID1.Collect[0];
	PID1.Collect[0]=moto_chassis[0].speed_rpm;
	PID2.Collect[1]=PID2.Collect[0];
	PID2.Collect[0]=moto_chassis[1].speed_rpm;
	PID3.Collect[1]=PID3.Collect[0];
	PID3.Collect[0]=moto_chassis[2].speed_rpm;
	PID4.Collect[1]=PID4.Collect[0];
	PID4.Collect[0]=moto_chassis[3].speed_rpm;
}
void PID_Init(void)         //初始化PID值
{
	PID1.Kp=100;
	PID1.Ki=5;
	PID1.Kd=2;
}
void Remote_Control(void* pvParameters)              //周期处理遥控器接收值
{
	float Wheel_Speed[4];
	while(1)
	{
		Speed_To_Wheel(Remote.ch2*10,-1*Remote.ch1*10,-1*Remote.ch3*20,Wheel_Speed);
		PID1.User=Wheel_Speed[0];
		PID2.User=Wheel_Speed[1];
		PID3.User=Wheel_Speed[2];
		PID4.User=Wheel_Speed[3];
		vTaskDelay(5);
	}
}
