#include "User.h"

#if(USING_PID == 1)

PID_Structure PID1;
PID_Structure PID2;
PID_Structure PID3;
PID_Structure PID4;
PID_Structure PID5;
PID_Structure PID6;
PID_Structure PID7;
PID_Structure PID8;
PID_Structure Chassis_Wz;
PID_Structure Chassis_Vx;
PID_Structure Chassis_Vy;
PID_Structure Chassis_Victor_y;
PID_Structure Chassis_Victor_x;
PID_Structure Ball2_Angle;
PID_Structure Ball1_Angle;
void PID_Renew(PID_Structure* PID_Handler,float new_data)
{
	PID_Handler->Collect[1] = PID_Handler->Collect[0];
	PID_Handler->Collect[0] = new_data;
}
void Set_PID(PID_Structure* PID_Handler,unsigned char Device_Mode)
{
	switch(Device_Mode)
	{
		case M2006_SPEED:
			PID_Handler->Kp = 3;
			PID_Handler->Ki = 0.1;
			PID_Handler->Kd = 0.1;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 10000;
			PID_Handler->Minium = -10000;
			break;
		case M2006_ANGLE:
			PID_Handler->Kp = 0.05;
			PID_Handler->Ki = 0.01;
			PID_Handler->Kd = 0;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 10000;
			PID_Handler->Minium = -10000;
			break;
		case M3508_SPEED:
			PID_Handler->Kp = 10;
			PID_Handler->Ki = 0.1;
			PID_Handler->Kd = 0;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 16384;
			PID_Handler->Minium = -16384;
			break;
		case M3508_ANGLE:
			PID_Handler->Kp = 0.5;
			PID_Handler->Ki = 0.01;
			PID_Handler->Kd = 0.02;
			PID_Handler->I_Sum_Max = 15000;
			PID_Handler->Maxinum = 16384;
			PID_Handler->Minium = -16384;
			break;
		case M3510_ANGLE:
			PID_Handler->Kp = 2;
			PID_Handler->Ki = 0.01;
			PID_Handler->Kd = 0;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 29000;
			PID_Handler->Minium = -29000;
		case M3510_SPEED:
			PID_Handler->Kp = 1;
			PID_Handler->Ki = 0;
			PID_Handler->Kd = 0;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 29000;
			PID_Handler->Minium = -29000;
			break;
		case CASCADE_ANGLE:
			PID_Handler->Kp = 0.05;
			PID_Handler->Ki = 0;
			PID_Handler->Kd = 0;
			PID_Handler->I_Sum_Max = 10000;
			PID_Handler->Maxinum = 30000;
			PID_Handler->Minium = -30000;
			break;
	}
}
float PID_Get_Result(PID_Structure* PID_Handler)
{
			float Error=(PID_Handler->User-PID_Handler->Collect[0]);    //计算误差
			PID_Handler->I_Sum = PID_Handler->I_Sum+(float)0.5*(float)((PID_Handler->User-PID_Handler->Collect[1])+Error);  //梯形积分
			if(fabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum > 0)
				PID_Handler->I_Sum = PID_Handler->I_Sum_Max;
			else if(fabs(PID_Handler->I_Sum) > PID_Handler->I_Sum_Max && PID_Handler->I_Sum < 0)
				PID_Handler->I_Sum = (-1.0f * PID_Handler->I_Sum_Max) ;
			PID_Handler->Result = (PID_Handler->Kp)*Error
														+ (PID_Handler->Ki * PID_Handler->I_Sum)
														+ (PID_Handler->Kd * ((PID_Handler->User-PID_Handler->Collect[1])-Error))
														+ (PID_Handler->Offset);
			if(PID_Handler->Result > PID_Handler->Maxinum)
				PID_Handler->Result = PID_Handler->Maxinum;
			if(PID_Handler->Result < PID_Handler->Minium)
				PID_Handler->Result = PID_Handler->Minium;
			return PID_Handler->Result;
}
void PID_Calculate(void* pvParameters)
{
	while(1)
	{
		Renew_PID();
		taskENTER_CRITICAL();
		PID8.User=PID_Get_Result(&Ball2_Angle);
		PID7.User=PID_Get_Result(&Ball1_Angle);
		set_moto_current(&hcan1,PID_Get_Result(&PID5),PID_Get_Result(&PID6),PID_Get_Result(&PID7),PID_Get_Result(&PID8),ID_5_8);  //ID为5~8的电机
		set_moto_current(&hcan1,PID_Get_Result(&PID1),PID_Get_Result(&PID2),PID_Get_Result(&PID3),PID_Get_Result(&PID4),ID_1_4);  //ID为1~4的电机
		taskEXIT_CRITICAL();
		vTaskDelay(PID_Time);	
	}
}
void Base_Speed_Task(void* pvParameters)   //底盘闭环PID
{
	while(1)
	{
		float Wheel_Speed[4];
		//Chassis_Vy.User = PID_Get_Result(&Chassis_Victor_y);
		//Chassis_Vx.User = PID_Get_Result(&Chassis_Victor_x);
		Speed_To_Wheel(PID_Get_Result(&Chassis_Vx),PID_Get_Result(&Chassis_Vy),PID_Get_Result(&Chassis_Wz),Wheel_Speed);
		PID1.User=Wheel_Speed[0];
		PID2.User=Wheel_Speed[1];
		PID3.User=Wheel_Speed[2];
		PID4.User=Wheel_Speed[3];
		vTaskDelay(5);
	}
}
#endif	//#if(USING_PID == 1)
