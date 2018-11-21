#include "exti.h"
#include "key.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void EXTI_Init(void)
{
	KEY_Init();	
	Ex_NVIC_Config(GPIO_A,3,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_A,5,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_A,11,FTIR);		//下降沿触发
	Ex_NVIC_Config(GPIO_A,12,FTIR);		//下降沿触发
	

	MY_NVIC_Init(2,2,EXTI9_5_IRQn,2);  	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI15_10_IRQn,2);	//抢占2，子优先级2，组2
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);    	//抢占2，子优先级2，组2
}

void EXTI3_IRQHandler(void)  //Menu
{
	delay_ms(5);	//消抖
  if(KEY3==1)	///Menu 
	{
		if(Menu++==4) Menu=1;
		EXTI->PR=1<<3;  //清除LINE2上的中断标志位
	}		
}

//外部中断9~5服务程序
void EXTI9_5_IRQHandler(void)
{			
	delay_ms(5);   //消抖			 
   if(KEY5==1)	// 
	{
		Flag_Stop=!Flag_Stop;
		EXTI->PR=1<<5;     //清除LINE5上的中断标志位  
	}		
 	
}
//外部中断15~10服务程序
void EXTI15_10_IRQHandler(void)
{			
	delay_ms(5);   //消抖			 
  if(KEY11==1)	//PID-
	{
		if(Menu==1)        Balance_KP-=Amplitude1;
	  else	if(Menu==2)  Balance_KD-=Amplitude2;
		else  if(Menu==3)  Position_KP-=Amplitude3;
		else  if(Menu==4)  Position_KD-=Amplitude4;
	}		
	 if(KEY12==1)	//PID+ 
	{
			    if(Menu==1)  Balance_KP+=Amplitude1;
	  else	if(Menu==2)  Balance_KD+=Amplitude2;
		else  if(Menu==3)  Position_KP+=Amplitude3;
		else  if(Menu==4)  Position_KD+=Amplitude4;
	}		
	if(Balance_KP<=0) Balance_KP=0;
	if(Balance_KD<=0) Balance_KD=0;
	if(Position_KP<=0) Position_KP=0;
	if(Position_KD<=0) Position_KD=0;
  EXTI->PR=1<<11; //清除LINE15上的中断标志位  
	EXTI->PR=1<<12; //清除LINE15上的中断标志位 
}





