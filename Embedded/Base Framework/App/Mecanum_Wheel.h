#ifndef _MECANUM_WHEEL_H_
#define _MECANUM_WHEEL_H_
#define M3508_MOTOR_RPM_TO_VECTOR 0.000415809748903494517209f
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR
#define CHASSIS_WZ_SET_SCALE 0.1f
#define MOTOR_DISTANCE_TO_CENTER 0.2f
void Set_Speed(float speed_x,float speed_y,float speed_w);
void Speed_To_Wheel(float vx_set, float vy_set,float wz_set,float wheel_speed[4]);
#endif
