#include "User.h"
TaskHandle_t Init_Task_Hanlder;
void Init_Task(void* pvParameters)
{
	taskENTER_CRITICAL();
	u8 num=1;
	oled_init();										//初始化OLED
	oled_LOGO();										//OLED显示RM的LOGO
	Delayms(300);
	oled_clear(Pen_Clear);
	oled_printf(num,Pen_Write,"OLED Init Success");  //1
	LED_Init();											//初始化LED
	num++;
	oled_printf(num,Pen_Write,"LED Init Success");   //2
	Random_Init();									//初始化硬件随机数
	num++;
	oled_printf(num,Pen_Write,"RNG Init Success");   //3
	USART2_init(115200);							//初始化串口2(蓝牙串口)，波特率115200
	num++;
	oled_printf(num,Pen_Write,"USART2 Init Success");//4
	ADC1_Init();										//初始化ADC
	num=1;
	oled_clear(Pen_Clear);
	oled_printf(num,Pen_Write,"ADC1 Init Success");  //1
	DAC1_Init();										//初始化DAC1
	num++;
	oled_printf(num,Pen_Write,"DAC1 Init Success");  //2
	DAC2_Init();										//初始化DAC2
	num++;
	oled_printf(num,Pen_Write,"DAC2 Init Success");  //3
	IIC2_Init();										//初始化IIC2
	num++;
	oled_printf(num,Pen_Write,"IIC2 Init Success");  //4
	num=1;
	oled_clear(Pen_Clear);
	Key_Init();											//初始化用户按键
	oled_printf(num,Pen_Write,"Key Init Success");   //1
	num++;
	oled_printf(num,Pen_Write,"Malloc Init Success"); //2
	Beep_Startup();									//初始化蜂鸣器
	num++;
	oled_printf(num,Pen_Write,"Beep Init Success");  //3
	num++;
	MX_CAN1_Init();									//初始化CAN1
	oled_printf(num,Pen_Write,"CAN1 Init Success");  //4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	MX_CAN2_Init();									//初始化CAN2
	oled_printf(num,Pen_Write,"CAN2 Init Success");  //1
	num++;
	PWM_Init();											//初始化PWM
	oled_printf(num,Pen_Write,"PWM Init Success");   //2
	num++;
	USART3_Init(115200);						//初始化妙算交互口
	oled_printf(num,Pen_Write,"USART3 Init Success");
	num++;
	Laser_Init();										//初始化激光
	oled_printf(num,Pen_Write,"Laser Init Success");//4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	Remote_Init();									//初始化遥控器
	oled_printf(num,Pen_Write,"Remote Init Success"); //1
	num++;
	Power_Init();										//初始化供电
	oled_printf(num,Pen_Write,"Power Init Success"); //2
	num=1;
	oled_clear(Pen_Clear);
	SPI4_Init();										//初始化SPI4
	oled_printf(num,Pen_Write,"SPI4 Init Success");  //3
	num++;
	Motor_Init();										//初始化马达
	oled_printf(num,Pen_Write,"Motor Init Success"); //4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	USART2_Receve_Handler.Buffer_Counter=-1;
	USART3_Receve_Handler.Buffer_Counter=-1;
	PID_Init();                     //初始化PID参数
	User_Code();										//进入用户代码
	taskEXIT_CRITICAL();
	vTaskDelete(Init_Task_Hanlder);
}
int main(void)
{
	HAL_Init();                     //初始化HAL库    
	Stm32_Clock_Init(360,12,2,8);   //设置时钟,180Mhz
	xTaskCreate((TaskFunction_t)Init_Task,
							(const char *)"Init_Task",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Init_Task_Hanlder);
	vTaskStartScheduler();
}
