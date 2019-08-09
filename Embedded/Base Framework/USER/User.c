#include "User.h"
TaskHandle_t LED_Task_Handle;					//LED闪烁句柄
TaskHandle_t PID_Calculate_Task;			//计算PID句柄
TaskHandle_t Remote_Control_Task;			//遥控数据处理句柄
TaskHandle_t Base_Speed_Task_Handler; //底盘控制句柄
TaskHandle_t Communicate_Task_Handler;//上位机通信句柄
TaskHandle_t Odometer_Task_Handler;		//里程计累加句柄
TaskHandle_t Control_Task_Handler;    //控制算法句柄
void User_Code(void)
{
	Power1_CTRL=1;   //打开1号电源开关
	Power2_CTRL=1;   //打开2号电源开关
	Power3_CTRL=1;   //打开3号电源开关
	Power4_CTRL=1;   //打开4号电源开关
	xTaskCreate((TaskFunction_t)LED_Flash,        //LED闪烁进程
							(const char *)"LED_TASK",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)1,
							(TaskHandle_t *)LED_Task_Handle);
	xTaskCreate((TaskFunction_t)PID_Calculate,    //底盘电机PID闭环
							(const char *)"PID_Calculate",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)PID_Calculate_Task);
	xTaskCreate((TaskFunction_t)Remote_Control,   //遥控器周期处理
							(const char *)"Remote_Control",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Remote_Control_Task);
	xTaskCreate((TaskFunction_t)Base_Speed_Task,	//底盘PID闭环
							(const char *)"Base_Speed_Task",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Base_Speed_Task_Handler);
	xTaskCreate((TaskFunction_t)Communicate_Task,
							(const char *)"Communicate_Task",   //周期通信
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Communicate_Task_Handler);
	xTaskCreate((TaskFunction_t)Odometer_Summary,
							(const char *)"Odometer_Task",   //里程计累计
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)Odometer_Task_Handler);
	xTaskCreate((TaskFunction_t)Control_Task,
							(const char *)"Control_Task",   //控制算法
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)Control_Task_Handler);
}
