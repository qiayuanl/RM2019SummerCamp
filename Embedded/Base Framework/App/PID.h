#ifndef _PID_H_
#define _PID_H_
#define M2006_SPEED 0
#define M2006_ANGLE 1
#define M3508_SPEED 2
#define M3508_ANGLE 3
#define M3510_SPEED 4
#define M3510_ANGLE 5
#define CASCADE_ANGLE 6
typedef struct
{
	float Kp;   //比例系数
	float Ki;   //积分系数
	float Kd;   //微分系数
	float Offset;  //偏移量
	float Collect[2];  //前两次采集到的数据,Collect[0]为最近一次采集的数据
	float I_Sum;       //积分项累加值
	float User;        //用户设定值
	float Result;      //运算结果
	float Maxinum;     //输出最大值
	float Minium;      //输出最小值
	float I_Sum_Max;   //I项最大值
}PID_Structure;
void PID_Renew(PID_Structure* PID_Handler,float new_data);
void PID_Calculate(void* pvParameters);
void Base_Speed_Task(void* pvParameters);
float PID_Get_Result(PID_Structure* PID_Handler);
void Set_PID(PID_Structure* PID_Handler,unsigned char Device_Mode);
extern PID_Structure PID1;
extern PID_Structure PID2;
extern PID_Structure PID3;
extern PID_Structure PID4;
extern PID_Structure PID5;
extern PID_Structure PID6;
extern PID_Structure PID7;
extern PID_Structure PID8;
extern PID_Structure Chassis_Wz;
extern PID_Structure Chassis_Vx;
extern PID_Structure Chassis_Vy;
extern PID_Structure Chassis_Victor_y;
extern PID_Structure Chassis_Victor_x;
extern PID_Structure Ball2_Angle;
extern PID_Structure Ball1_Angle;
#endif
