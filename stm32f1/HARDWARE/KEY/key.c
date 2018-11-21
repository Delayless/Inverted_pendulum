#include "key.h"
 /**************************************************************************
作者：平衡小车之家 
淘宝店铺：http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
函数功能：按键初始化
入口参数：无
返回  值：无 
**************************************************************************/
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //使能PORTA时钟	   	 
	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X00088000;
	
	GPIOA->CRL&=0XFF0F00FF; 
	GPIOA->CRL|=0X00808800;
	
  GPIOA->ODR|=3<<2; //PA2 3 上拉	
  GPIOA->ODR|=1<<5; //PA5 上拉
	GPIOA->ODR|=3<<11; //PA11 12  上拉	
} 
/**************************************************************************
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态 0：无动作 1：单击 2：双击 
**************************************************************************/
u8 click_N_Double (u8 time)
{
		/***************当成5ms执行一次****************/
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(KEY2==0)  Forever_count++;   //按下为0  //长按标志位未置1
     else        Forever_count=0;
	
		//上一次是松开的flag_key才会等于，这次按下为1
		if(0==KEY2&&0==flag_key)		flag_key=1;		
		//必须是松开的或者上一次执行函数时时松开的才会执行
		//如果上一次是松开的话count_key=0，会执行if内部语句
		//而执行后count_key会等于1
		//如果不松开按键count_key一直为1，就不会再一次执行
		//执行效果是double_key++
		//如果double_key等于2的话说明是双击，返回2
	  if(0==count_key)
		{
				if(flag_key==1) 
				{
					double_key++;
					count_key=1;
				}
				if(double_key==2) 
				{
					double_key=0;
					count_single=0;
					return 2;//双击执行的指令
				}
		}
			
		//按键松开标志位flag_key为0，
		if(1==KEY2)			flag_key=0,count_key=0;
		
		
		if(1==double_key)
		{
			count_single++;
			//假设这是第一次按下KEY2
			//两个变量都会同步加1，直到KEY2松开时Forever_count会置零
			/*而count_single只有执行完一次完整的检测(耗时times次5ms的中断)
				或者完成双击或者单击才会置零*/
			//所以如果一直按着的话当count_single>time时,Forever_count=time会定为长按
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//单击执行的指令
			}
			if(Forever_count>=time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
		/***************当成5ms执行一次****************/
}
///**************************************************************************
//函数功能：按键扫描
//入口参数：无
//返回  值：按键状态 0：无动作 1：单击 
//**************************************************************************/
//u8 click(void)
//{
//			static u8 flag_key=1;//按键按松开标志
//			if(flag_key&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
//			{
//			flag_key=0;
//			if(KEY1==0)return 1;
//			if(KEY2==0)return 2;
//			if(KEY3==0)return 3;
//			if(KEY4==0)return 4;				
//			}
//			else if(1==KEY1&&1==KEY2&&1==KEY3&&1==KEY4)			flag_key=1;
//			return 0;//无按键按下
//}
///**************************************************************************
//函数功能：长按检测
//入口参数：无
//返回  值：按键状态 0：无动作 1：长按2s
//**************************************************************************/
//u8 Long_Press(void)
//{
//			static u16 Long_Press_count,Long_Press;
//	    if(Long_Press==0&&KEY==0)  Long_Press_count++;   //长按标志位未置1
//      else                       Long_Press_count=0; 
//		  if(Long_Press_count>200)		
//			{
//				Long_Press=1;	
//				Long_Press_count=0;
//				return 1;
//			}				
//			 if(Long_Press==1)     //长按标志位置1
//			{
//				  Long_Press=0;
//			}
//			return 0;
//}
