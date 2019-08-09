#include "User.h"

#if(USING_MOTOR == 1)

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
single_gyro_t single_gyro;
moto_measure_t moto_chassis[8] = {0};//8 chassis moto
moto_measure_t moto_info;
void get_total_angle(moto_measure_t *p);
void get_moto_offset(moto_measure_t *ptr, CAN_HandleTypeDef* hcan);
void Motor_Init(void)
{
	my_can_filter_init_recv_all(&hcan1);
	my_can_filter_init_recv_all(&hcan2);
	HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0);
	HAL_CAN_Receive_IT(&hcan2, CAN_FIFO1);
}
void my_can_filter_init_recv_all(CAN_HandleTypeDef* _hcan)
{
	CAN_FilterConfTypeDef		CAN_FilterConfigStructure;
	static CanTxMsgTypeDef		Tx1Message;
	static CanRxMsgTypeDef 		Rx1Message;
	static CanTxMsgTypeDef		Tx2Message;
	static CanRxMsgTypeDef 		Rx2Message;
	CAN_FilterConfigStructure.FilterNumber = 0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.BankNumber = 14;
	CAN_FilterConfigStructure.FilterActivation = ENABLE;
	HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure);
	CAN_FilterConfigStructure.FilterNumber = 14;
	HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure);
	if(_hcan == &hcan1)
	{
		_hcan->pTxMsg = &Tx1Message;
		_hcan->pRxMsg = &Rx1Message;
	}
	if(_hcan == &hcan2)
	{
		_hcan->pTxMsg = &Tx2Message;
		_hcan->pRxMsg = &Rx2Message;
	}
}
void can_filter_recv_special(CAN_HandleTypeDef* hcan, uint8_t filter_number, uint16_t filtered_id)
{
	CAN_FilterConfTypeDef cf;
	cf.FilterNumber = filter_number;
	cf.FilterMode = CAN_FILTERMODE_IDMASK;
	cf.FilterScale = CAN_FILTERSCALE_32BIT;
	cf.FilterIdHigh = (filtered_id<<21)>>16;
	cf.FilterIdLow = filtered_id<<21;
	cf.FilterMaskIdHigh = 0xFFFF;
	cf.FilterMaskIdLow = 0xFFF8;
	cf.FilterFIFOAssignment = CAN_FilterFIFO0;
	cf.BankNumber = 14;
	cf.FilterActivation = ENABLE;
	HAL_CAN_ConfigFilter(hcan, &cf);
} 
HAL_StatusTypeDef can_send_msg()
{ 
	return HAL_OK;
}
float ZGyroModuleAngle;
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* _hcan)
{
	u8 i;
	switch(_hcan->pRxMsg->StdId)
	{
		case CAN_3510Moto1_ID:
		case CAN_3510Moto2_ID:
		case CAN_3510Moto3_ID:
		case CAN_3510Moto4_ID:
		case CAN_3510Moto5_ID:
		case CAN_3510Moto6_ID:
		case CAN_3510Moto7_ID:
		case CAN_3510Moto8_ID:
		{
				i = _hcan->pRxMsg->StdId - CAN_3510Moto1_ID;
				moto_chassis[i].msg_cnt++ <= 50	?	get_moto_offset(&moto_chassis[i], _hcan) : get_moto_measure(&moto_chassis[i], _hcan);
				get_moto_measure(&moto_info,_hcan);
				break;
		}
		case SINGLE_GYRO_ID:
		{
			single_gyro.angle = 0.001f * ((int32_t)( (_hcan->pRxMsg->Data[0] << 24) | (_hcan->pRxMsg->Data[1] << 16) | (_hcan->pRxMsg->Data[2] << 8) | (_hcan->pRxMsg->Data[3]) ) );
      single_gyro.gyro = 0.001f * ((int32_t)( (_hcan->pRxMsg->Data[4] << 24) | (_hcan->pRxMsg->Data[5] << 16) | (_hcan->pRxMsg->Data[6] << 8) | (_hcan->pRxMsg->Data[7]) ) );
      break;
		}
	}
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_FMP0);
	__HAL_CAN_ENABLE_IT(&hcan2, CAN_IT_FMP0);
}
void get_moto_measure(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
{
	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(hcan->pRxMsg->Data[0]<<8 | hcan->pRxMsg->Data[1]) ;
	ptr->real_current  = (int16_t)(hcan->pRxMsg->Data[2]<<8 | hcan->pRxMsg->Data[3]);
	ptr->speed_rpm = ptr->real_current;
	ptr->given_current = (int16_t)(hcan->pRxMsg->Data[4]<<8 | hcan->pRxMsg->Data[5])/-5;
	ptr->hall = hcan->pRxMsg->Data[6];
	if(ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt --;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt ++;
	ptr->total_angle = ptr->round_cnt * 8192 + ptr->angle - ptr->offset_angle;
}
void get_moto_offset(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
{
	ptr->angle = (uint16_t)(hcan->pRxMsg->Data[0]<<8 | hcan->pRxMsg->Data[1]) ;
	ptr->offset_angle = ptr->angle;
}

#define ABS(x)	( (x>0) ? (x) : (-x) )
void get_total_angle(moto_measure_t *p)
{
	int res1, res2, delta;
	if(p->angle < p->last_angle)
	{
		res1 = p->angle + 8192 - p->last_angle;
		res2 = p->angle - p->last_angle;
	}
	else
	{
		res1 = p->angle - 8192 - p->last_angle;
		res2 = p->angle - p->last_angle;
	}
	if(ABS(res1)<ABS(res2))
		delta = res1;
	else
		delta = res2;
	p->total_angle += delta;
	p->last_angle = p->angle;
}
void set_moto_current(CAN_HandleTypeDef* hcan, s16 iq1, s16 iq2, s16 iq3, s16 iq4,u32 Motor_ID)
{
	hcan->pTxMsg->StdId = Motor_ID;
	hcan->pTxMsg->IDE = CAN_ID_STD;
	hcan->pTxMsg->RTR = CAN_RTR_DATA;
	hcan->pTxMsg->DLC = 0x08;
	hcan->pTxMsg->Data[0] = iq1 >> 8;
	hcan->pTxMsg->Data[1] = iq1;
	hcan->pTxMsg->Data[2] = iq2 >> 8;
	hcan->pTxMsg->Data[3] = iq2;
	hcan->pTxMsg->Data[4] = iq3 >> 8;
	hcan->pTxMsg->Data[5] = iq3;
	hcan->pTxMsg->Data[6] = iq4 >> 8;
	hcan->pTxMsg->Data[7] = iq4;
	HAL_CAN_Transmit(hcan, 1000);
}	

#endif	//#if(USING_MOTOR == 1)
