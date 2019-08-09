#ifndef _VAR_H_
#define _VAR_H_
#define PID_Time 5               //PID运算时间5ms/次 200Hz
#define USART3_Priority 1
#define USART3_Receve_Length 2  //USART3接收缓冲区大小
#define UART_RX_DMA_SIZE (1024)		//串口1，DMA2长度
#define DBUS_MAX_LEN     (50)			//遥控器最大长度
#define DBUS_BUFLEN      (18)    //遥控器字符长度
#define USART2_Priority 0	     //串口2接收中断优先级
#define USART2_Receve_Length 2 //USART2接收缓冲区大小
#define CAN1_Priority 0				//CAN1接收中断优先级
#define CAN2_Priority 0				//CAN2接收中断优先级
#define Flash_Base_Address 0x8180000  //用户可写Flash基地址
#define PWM_FREQUENCE 50						//PWM频率
#define IN_CCM __attribute__((at(0x10000000)))  //在CCM内存中申请
#endif
