#include "User.h"
Manifold_Receve_Struct Manifold;
extern UART_HandleTypeDef USART3_Handler;
unsigned char Last_Possition_X,Last_Possition_Y;
void Manifold_Analyze(void)
{
	if((USART3_Receve_Handler.Buffer[0]&0xc0)==0x00&&(USART3_Receve_Handler.Buffer[1]&0xc0)==0x40)
	{
		u8 Temp=0x00; 
		Manifold.Possition_Row = (USART3_Receve_Handler.Buffer[0]&0x3c)>>2;
		Temp = USART3_Receve_Handler.Buffer[0]&0x03;
		Manifold.Possition_Col = (Temp<<2)|((USART3_Receve_Handler.Buffer[1]&0x30)>>4);
		Manifold.Camera=(USART3_Receve_Handler.Buffer[1]&0x0f)>>2;
		Manifold.Delay=(USART3_Receve_Handler.Buffer[1]&0x02)>>1;
	}
}
void Manifold_Send(Manifold_Send_Struct send)
{
	unsigned char Manifold_Buffer[2];
	Manifold_Buffer[0]=0x00|send.Possition_Row<<2|send.Possition_Col>>2;
	Manifold_Buffer[1]=0x40|((send.Possition_Col&0x03)<<4)|send.Set_Disable<<3|send.Enemy<<2|send.Get<<1|send.Teminal;
	HAL_UART_Transmit(&USART3_Handler,Manifold_Buffer,sizeof(Manifold_Buffer),1000);
}
