#include "User.h"

#if(USING_IIC_2 == 1)

void IIC2_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    IIC2_SDA=1;
    IIC2_SCL=1;  
}
void SDA2_OUT()
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_0;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
  GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}
void SDA2_IN()
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_0;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
	GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}
//产生IIC起始信号
void IIC2_Start(void)
{
	SDA2_OUT();     //sda线输出
	IIC2_SDA=1;	  	  
	IIC2_SCL=1;
	Delayus(4);
 	IIC2_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delayus(4);
	IIC2_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC2_Stop(void)
{
	SDA2_OUT();//sda线输出
	IIC2_SCL=0;
	IIC2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delayus(4);
	IIC2_SCL=1; 
	Delayus(4);			
	IIC2_SDA=1;//发送I2C总线结束信号				   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA2_IN();      //SDA设置为输入  
	IIC2_SDA=1;Delayus(1);	   
	IIC2_SCL=1;Delayus(1);	 
	while(READ_SDA2)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	IIC2_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC2_Ack(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=0;
	Delayus(2);
	IIC2_SCL=1;
	Delayus(2);
	IIC2_SCL=0;
}
//不产生ACK应答		    
void IIC2_NAck(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=1;
	Delayus(2);
	IIC2_SCL=1;
	Delayus(2);
	IIC2_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC2_Send_Byte(u8 txd)
{              
    u8 t;   
		SDA2_OUT(); 	    
    IIC2_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC2_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				Delayus(2);   //对TEA5767这三个延时都是必须的
				IIC2_SCL=1;
				Delayus(2); 
				IIC2_SCL=0;	
				Delayus(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC2_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA2_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
		{
        IIC2_SCL=0; 
        Delayus(2);
				IIC2_SCL=1;
        receive<<=1;
        if(READ_SDA2)
					receive++;   
				Delayus(1); 
    }					 
    if (!ack)
        IIC2_NAck();//发送nACK
    else
        IIC2_Ack(); //发送ACK   
    return receive;
}

#endif	//#if(USING_IIC_2 == 1)
