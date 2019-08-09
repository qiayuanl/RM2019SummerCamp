#ifndef __MECANUM_WHEEL_H__
#define __MECANUM_WHEEL_H__
#define A 0.0211f
#define B 0.0160f
#define WHEEL_R 0.0152f
#define PI 3.14159265358979f
void Set_Speed(float speed_x,float speed_y,float speed_w);
void Speed_To_Wheel(float vx_set, float vy_set,float wz_set,float wheel_speed[4]);
#endif
