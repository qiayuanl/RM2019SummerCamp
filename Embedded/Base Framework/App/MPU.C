#include "User.h"
float Filter_Output = 0;
void MPU_task(void *pvParameters)
{
	float Filter_Parameter = 0.001;
	while (1)
	{
		mpu_get_data();
		imu_ahrs_update();
		imu_attitude_update();
		Filter_Output = (1 - Filter_Parameter * imu.wz) + Filter_Parameter * Filter_Output;
		if (imu.temp < 50)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); //陀螺仪温度小于49度开始加热
		if (imu.temp > 50)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); //陀螺仪温度大于51度停止加热
		vTaskDelay(10);
	}
}
