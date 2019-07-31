#include "SPI.h"
#include "User.h"

#if(USING_SPI == 1)

SPI_HandleTypeDef hspi1;				//SPI1句柄
SPI_HandleTypeDef SPI4_Handler;  //SPI4句柄
SPI_HandleTypeDef SPI5_Handler;  //SPI5句柄

void SPI4_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOE_CLK_ENABLE();       //使能GPIOF时钟
    __HAL_RCC_SPI4_CLK_ENABLE();        //使能SPI4时钟
    GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_12;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
    GPIO_Initure.Alternate=GPIO_AF5_SPI4;           //复用为SPI4
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		GPIO_Initure.Pin=GPIO_PIN_4;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;          //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;    
		HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    SPI4_Handler.Instance=SPI4;                         //SP4
    SPI4_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI4_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI4_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI4_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI4_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI4_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI4_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值为256
    SPI4_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI4_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI4_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI4_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI4_Handler);//初始化
    __HAL_SPI_ENABLE(&SPI4_Handler);                    //使能SPI4
    SPI4_ReadWriteByte(0Xff);                           //启动传输
		SPI4_SetSpeed(SPI_BAUDRATEPRESCALER_8);
}
void SPI4_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
    __HAL_SPI_DISABLE(&SPI4_Handler);            //关闭SPI
    SPI4_Handler.Instance->CR1&=0XFFC7;          //位3-5清零，用来设置波特率
    SPI4_Handler.Instance->CR1|=SPI_BaudRatePrescaler;//设置SPI速度
    __HAL_SPI_ENABLE(&SPI4_Handler);             //使能SPI
}
uint8_t SPI4_ReadWriteByte(uint8_t TxData)
{
    u8 Rxdata;
    HAL_SPI_TransmitReceive(&SPI4_Handler,&TxData,&Rxdata,1, 1000);       
		return Rxdata;          		    //返回收到的数据		
}
void MX_SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_SPI1_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
	HAL_SPI_Init(&hspi1);
}
void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
  if(spiHandle->Instance==SPI1)
  {
    __HAL_RCC_SPI1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4|GPIO_PIN_3);
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
  }
	if(spiHandle->Instance==SPI5)
  {
    __HAL_RCC_SPI5_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_8);
  }
} 


void SPI5_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();       //使能GPIOF时钟
    __HAL_RCC_SPI5_CLK_ENABLE();        //使能SPI4时钟
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;             //快速            
    GPIO_Initure.Alternate=GPIO_AF5_SPI5;           //复用为SPI5
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		GPIO_Initure.Pin=GPIO_PIN_6;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;          //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;                  //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;    
		HAL_GPIO_Init(GPIOF,&GPIO_Initure);
		PFout(6)=1;
    SPI5_Handler.Instance=SPI5;                         //SP4
    SPI5_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI5_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI5_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI5_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;    //串行同步时钟的空闲状态为高电平
    SPI5_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI5_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI5_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_8;//定义波特率预分频的值:波特率预分频值为256
    SPI5_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI5_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI5_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI5_Handler.Init.CRCPolynomial=10;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI5_Handler);//初始化
    __HAL_SPI_ENABLE(&SPI5_Handler);                    //使能SPI5

}



#endif	//#if(USING_SPI == 1)
