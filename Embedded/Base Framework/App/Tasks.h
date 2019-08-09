#ifndef _TASKS_H_
#define _TASKS_H_
#define Servo_ReMap(val,I_Min,I_Max,O_Min,O_Max) ((val)/((I_Max)-(I_Min))*((O_Max)-(O_Min)) + (O_Min))
void PID_Init(void);
void Remote_Control(void* pvParameters);
void Renew_PID(void);
#endif
