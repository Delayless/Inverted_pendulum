#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
#include "show.h"

  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 3085
extern	int Balance_Pwm,Velocity_Pwm;
int TIM1_UP_IRQHandler(void);
int balance(float angle);
int Position(int Encoder);
void Set_Pwm(int moto);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off(int voltage);
int myabs(int a);
#endif