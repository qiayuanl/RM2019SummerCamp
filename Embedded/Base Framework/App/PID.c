#include "User.h"
PID_Structure PID1;
PID_Structure PID2;
PID_Structure PID3;
PID_Structure PID4;
PID_Structure PID5;
PID_Structure PID6;
PID_Structure PID7;
PID_Structure PID8;
float PID_Get_Result(PID_Structure* PID_Handler)
{
			float Error=(PID_Handler->User-PID_Handler->Collect[0]);    //计算误差
			if(fabs(Error) < PID_Handler->I_Limit)
				PID_Handler->I_Sum=PID_Handler->I_Sum+(float)0.5*(float)(Error+PID_Handler->User-PID_Handler->Collect[1]);  //梯形积分
			PID_Handler->Result = (PID_Handler->Kp)*Error
														+ (PID_Handler->Ki * PID_Handler->I_Sum)
														+ (PID_Handler->Kd * (Error + PID_Handler->User-PID_Handler->Collect[1]))
														+ (PID_Handler->Offset);
			return PID_Handler->Result;
}
void PID_Calculate(void* pvParameters)
{
	while(1)
	{
		Renew_PID();
		taskENTER_CRITICAL();
		set_moto_current(&hcan1,PID_Get_Result(&PID5),PID_Get_Result(&PID6),PID_Get_Result(&PID7),PID_Get_Result(&PID8),ID_5_8);  //ID为5~8的电机
		set_moto_current(&hcan1,PID_Get_Result(&PID1),PID_Get_Result(&PID2),PID_Get_Result(&PID3),PID_Get_Result(&PID4),ID_1_4);  //ID为1~4的电机
		taskEXIT_CRITICAL();
		vTaskDelay(PID_Time);
	}
}
