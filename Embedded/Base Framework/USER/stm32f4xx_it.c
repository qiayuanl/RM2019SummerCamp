#include "User.h"
#include "stm32f4xx_it.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		HAL_IncTick();
	}
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}
void CAN2_TX_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan2);
}
void CAN2_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan2);
}
void CAN1_TX_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}
void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&hcan1);
}
void USART1_IRQHandler(void)
{
  uart_receive_handler(&huart1);
  HAL_UART_IRQHandler(&huart1);
}
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
}
void NMI_Handler(void)
{
}
void HardFault_Handler(void)
{
  while (1)
  {
  }
}
void MemManage_Handler(void)
{
  while (1)
  {
  }
}
void BusFault_Handler(void)
{
  while (1)
  {
  }
}
void UsageFault_Handler(void)
{
  while (1)
  {
  }
}
void DebugMon_Handler(void)
{
}
