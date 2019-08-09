#include "Mecanum_Wheel.h"
#include "User_Config.h"

#if(USING_MECANUM_WHEEL == 1)

void Speed_To_Wheel(float vx_set, float vy_set,float wz_set,float wheel_speed[4])
{
	  wheel_speed[0] = (float)(+vx_set - vy_set + wz_set*(A+B))/(WHEEL_R * PI)*19*6;
    wheel_speed[1] = (float)(+vx_set + vy_set + wz_set*(A+B))/(WHEEL_R * PI)*19*6;
    wheel_speed[2] = (float)(-vx_set + vy_set + wz_set*(A+B))/(WHEEL_R * PI)*19*6;
    wheel_speed[3] = (float)(-vx_set - vy_set + wz_set*(A+B))/(WHEEL_R * PI)*19*6;
}

#endif	//#if(USING_MECANUM_WHEEL == 1)
