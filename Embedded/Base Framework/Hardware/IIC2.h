#ifndef _MYIIC_H
#define _MYIIC_H
#include "System_Config.h"
#define IIC2_SCL   PFout(1) //SCL
#define IIC2_SDA   PFout(0) //SDA
#define READ_SDA2  PFin(0)  //输入SDA
void IIC2_Init(void);                //初始化IIC2的IO口				 
void IIC2_Start(void);				//发送IIC2开始信号
void IIC2_Stop(void);	  			//发送IIC2停止信号
void IIC2_Send_Byte(u8 txd);			//IIC2发送一个字节
u8 IIC2_Read_Byte(unsigned char ack);//IIC2读取一个字节
u8 IIC2_Wait_Ack(void); 				//IIC2等待ACK信号
void IIC2_Ack(void);					//IIC2发送ACK信号
void IIC2_NAck(void);				//IIC2不发送ACK信号
void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

