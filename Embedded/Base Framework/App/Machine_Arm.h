#ifndef _MACHINE_ARM_H_
#define _MACHINE_ARM_H_
void SkyTable_Back(void);			//云台转向后方
void SkyTable_Front(void);  //云台转向前方
void Arm_Lift(void);  //机械臂抬升
void Arm_Drop(void);	//机械臂下降
void Hand_Grab(void);	//抓手抓取
void Hand_Open(void);	//抓手张开
void Hand_Drop(void);	//抓手放物块
void Blanket_Open(void);//物料盘打开
void Blanket_Close(void);//物料盘关闭
#endif
