#include "User.h"
float VectorO_X , VectorO_Y , RotateO ,VO_X,VO_Y;
float Des_Vx , Des_Vy , Des_Wz , L_Des_Vx , L_Des_Vy , L_Des_Wz;
void Odometer_Summary(void *pvParameters)
{
	while(1)
	{
		float Des_Wheel[4] = {PID1.Collect[0] ,PID2.Collect[0] ,PID3.Collect[0] ,PID4.Collect[0]};
		L_Des_Vx = Des_Vx; L_Des_Vy = Des_Vy; L_Des_Wz = Des_Wz;
		Mecanum_To_Chassis(Des_Wheel,&Des_Vx,&Des_Vy,&Des_Wz);
		VO_X = Des_Vx;
		VO_Y = Des_Vy;
		VectorO_X += (0.5f * (Des_Vx + L_Des_Vx))*(0.005f);
		VectorO_Y += (0.5f * (Des_Vy + L_Des_Vy))*(0.005f);
		RotateO  +=  (0.5f * (Des_Wz + L_Des_Wz))*(0.005f);
		vTaskDelay(5);
	}
}
void Mecanum_To_Chassis(float Wheel[4],float *Vx,float *Vy,float *Wz)
{
	float buf1,buf2,buf3;
	buf1 = (Wheel[0]+Wheel[1])/2.0f;//x+wz
	buf2 = (Wheel[0]+Wheel[2])/2.0f;//wz
	buf3 = (Wheel[1]+Wheel[2])/2.0f;//y+wz
	*Vx = (buf1-buf2)/(19.0/60.0/60*23.55f/1.6f);
	*Vy = (buf3-buf2)/(19.0/60.0/60*23.55f/1.6f);
	*Wz = (buf2)/(19.0/60.0/(A+B)/3.0/1.4f);
}
