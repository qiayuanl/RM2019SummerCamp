#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H
#ifdef __cplusplus
 extern "C" {
#endif 
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void DMA1_Stream1_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void CAN1_TX_IRQHandler(void);
void CAN1_RX0_IRQHandler(void);
void USART1_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void CAN2_TX_IRQHandler(void);
void CAN2_RX0_IRQHandler(void);
void TIM3_IRQHandler(void);
#ifdef __cplusplus
}
#endif
#endif
