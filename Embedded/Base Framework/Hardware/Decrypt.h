#ifndef _DECRYPT_H_
#define _DECRYPT_H_

#define LOCATION 0x01

typedef struct
{
  float X_Possition;
	float Y_Possition;
}Receive_Data_Package;
void Received_CallBack(unsigned char Receive_Buffer[]);
#endif
