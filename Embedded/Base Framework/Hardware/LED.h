#ifndef _LED_H_
#define _LED_H_
#define LED_R PEout(11)
#define LED_G PFout(14)
#define LED0 PGout(1)
#define LED1 PGout(2)
#define LED2 PGout(3)
#define LED3 PGout(4)
#define LED4 PGout(5)
#define LED5 PGout(6)
#define LED6 PGout(7)
#define LED7 PGout(8)
#define LED_ON 0
#define LED_OFF 1
#define LED_ALL_OFF() 	LED0=LED_OFF;\
											LED1=LED_OFF;\
											LED2=LED_OFF;\
											LED3=LED_OFF;\
											LED4=LED_OFF;\
											LED5=LED_OFF;\
											LED6=LED_OFF;\
											LED7=LED_OFF
void LED_Init(void);
void LED_Flash(void* pvParameters);
#endif
