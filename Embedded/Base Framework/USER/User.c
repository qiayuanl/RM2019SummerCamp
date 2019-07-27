#include "User.h"
TaskHandle_t LED_Task_Handle;			//LED闪烁句柄
TaskHandle_t PID_Calculate_Task;	//计算PID句柄
TaskHandle_t Remote_Control_Task;	//遥控数据处理句柄
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
							(UBaseType_t)2,
							(TaskHandle_t *)LED_Task_Handle);
	xTaskCreate((TaskFunction_t)PID_Calculate,    //PID运算进程
							(const char *)"PID_Calculate",
							(uint16_t)1024,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)PID_Calculate_Task);
	xTaskCreate((TaskFunction_t)Remote_Control,   //遥控器处理进程
							(const char *)"Remote_Control",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)4,
							(TaskHandle_t *)Remote_Control_Task);
}
