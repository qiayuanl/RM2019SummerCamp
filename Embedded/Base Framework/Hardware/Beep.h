#ifndef _BEEP_H_
#define _BEEP_H_
void Beep_Complete(void);
void Beep_Startup(void);
void TIM_SetTIM12Compare1(float compare);
void Init_Beep(unsigned int Frequency);
void Beep_On(void);
void Beep_Off(void);
#endif
