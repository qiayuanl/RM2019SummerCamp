#include "User.h"

#if (USING_TASK == 1)

void Renew_PID(void) //更新PID值
{
	PID1.Collect[1] = PID1.Collect[0];
	PID1.Collect[0] = moto_chassis[0].speed_rpm;
	PID2.Collect[1] = PID2.Collect[0];
	PID2.Collect[0] = moto_chassis[1].speed_rpm;
	PID3.Collect[1] = PID3.Collect[0];
	PID3.Collect[0] = moto_chassis[2].speed_rpm;
	PID4.Collect[1] = PID4.Collect[0];
	PID4.Collect[0] = moto_chassis[3].speed_rpm;
}
void PID_Init(void) //初始化PID值
{
	PID1.Kp = 0.5;
	PID1.Ki = 0.005;
	PID1.Kd = 0;
	PID1.I_Limit = 3000;

	PID2.Kp = 0.5;
	PID2.Ki = 0.005;
	PID2.Kd = 0;
	PID2.I_Limit = 3000;

	PID3.Kp = 0.5;
	PID3.Ki = 0.005;
	PID3.Kd = 0;
	PID3.I_Limit = 3000;

	PID4.Kp = 0.5;
	PID4.Ki = 0.005;
	PID4.Kd = 0;
	PID4.I_Limit = 3000;
}
void Remote_Control(void *pvParameters) //周期处理遥控器接收值
{
	float Wheel_Speed[4];
	while (1)
	{
		if (Remote.sw1 == Left_L && Remote.sw2 == Right_L)
		{
			Speed_To_Wheel(Remote.ch1 * 10, Remote.ch2 * 10, Remote.ch3 * 10, Wheel_Speed);
			PID1.User = Wheel_Speed[0];
			PID2.User = Wheel_Speed[1];
			PID3.User = Wheel_Speed[2];
			PID4.User = Wheel_Speed[3];
			vTaskDelay(5);
		}
		else
			PID1.User = 0;
		vTaskDelay(1);
	}
}

#endif //#if(USING_TASK == 1)
