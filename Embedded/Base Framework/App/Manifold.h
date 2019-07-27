#ifndef _MANIFOLD_H_
#define _MANIFOLD_H_
#define Camera_UP 0
#define Camera_DOWN 1
#define Camera_LEFT 2
#define Camera_RIGHT 3
typedef struct
{
	unsigned char Possition_Row;
	unsigned char Possition_Col;
	unsigned char Camera;
	unsigned char Delay;
}Manifold_Receve_Struct;
extern Manifold_Receve_Struct Manifold;;
typedef struct
{
	unsigned char Possition_Row;
	unsigned char Possition_Col;
	unsigned char Set_Disable;
	unsigned char Enemy;
	unsigned char Get;
	unsigned char Teminal;
}Manifold_Send_Struct;
void Manifold_Analyze(void);
void Manifold_Send(Manifold_Send_Struct send);
#endif
