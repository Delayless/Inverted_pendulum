#include "control.h"		
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
int Balance_Pwm,Position_Pwm;
u8 Flag_Target,Position_Target;
/**************************************************************************
函数功能：所有的控制代码都在这里面
          TIM1控制的5ms定时中断 
**************************************************************************/
int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);                                       //===清除定时器1中断标志位	                     
	     if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;          //===给主函数提供50ms的精准延时
			 }
			//返回TIM2 -> CNT的值
    	Encoder=Read_Encoder(2);             	                   //===更新编码器位置信息
      //返回角度传感器的值
			 Angle_Balance=Get_Adc_Average(7,15);                     //===更新姿态	
     	
			/**********************python写***************************/
			/*********************************************************
			 外部中断有分优先级
			 通过改变Position_Zero来改变摆杆位置
			 *********************************************************/
			//商家采用的方案是两个单独的闭环，只不过直立环的控制频率更高，从而实现控制效果
			DataScope();	            //===python上位机
			/*在上位机写 Balance_Pwm = balance(Angle_Balance);                                          //===角度PD控制	

	    if(++Position_Target>4)	Position_Pwm=Position(Encoder),Position_Target=0;     //===位置PD控制 25ms进行一次位置控制
      Moto=Balance_Pwm-Position_Pwm;        //===计算电机最终PWM
		  Xianfu_Pwm();                         //===PWM限幅 反正占空比100%带来的系统不稳定因素
			
			*/
	  	Led_Flash(100);                       //===LED闪烁指示系统正常运行 
	    Voltage=Get_battery_volt();           //===获取电池电压	      
			//Key();       //按键可以控制正反旋转             //===扫描按键变化
	}       	
	 return 0;	  
} 


/*************************************************************************
python上位机将数据处理好发送回单片机
直接就是PWM波了
**************************************************************************/
void USART1_IRQHandler(void)
{
		u8 res;	
		if(USART1->SR&(1<<5))//接收到数据
		{	 
				res=USART1->DR;		//自然可以把中断标志位清零，如果发生数据异常可以检查这里
				Moto = res;		 		
				if(Turn_Off(Voltage)==0)              //===低压和倾角过大保护
						Set_Pwm(Moto);                        //===赋值给PWM寄存器			
		} 
} 

/**************************************************************************
函数功能：倾角PD控制
入口参数：角度
返回  值：倾角控制PWM
作    者：平衡小车之家
**************************************************************************/
int balance(float Angle)
{  
   float Bias;                       //倾角偏差
	 static float Last_Bias,D_Bias;    //PID相关变量
	 int balance;                      //PWM返回值 
	 Bias=Angle-ZHONGZHI;              //求出平衡的角度中值 和机械相关
	 D_Bias=Bias-Last_Bias;            //求出偏差的微分 进行微分控制
	 balance=-Balance_KP*Bias-D_Bias*Balance_KD;   //===计算倾角控制的电机PWM  PD控制
   Last_Bias=Bias;                   //保持上一次的偏差
	 return balance;
}

/**************************************************************************
函数功能：位置PD控制 
入口参数：编码器
返回  值：位置控制PWM
作    者：平衡小车之家

int Position(int Encoder)
{  
   static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
	 static float Position_Least;
  	Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;		   
    Position_Bias += Position_Least*0.2;	             //===一阶低通滤波器  
	  Position_Differential=Position_Bias-Last_Position;
	  Last_Position=Position_Bias;
		Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===速度控制	
	  return Position_PWM;
}
**************************************************************************/


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto)
{
    	if(moto<0)			AIN2=1,			AIN1=0;
			else 	          AIN2=0,			AIN1=1;
			PWMA=myabs(moto);
}

/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM满幅是7200 限制在6900
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}
**************************************************************************/


/**************************************************************************
函数功能：按键修改小车运行状态  控制摆杆的位置
入口参数：无
返回  值：无
**************************************************************************/
//key()放在5ms的中断里面
void Key(void)
{	
	int Position=2080; //目标位置 电机原始位置是10000  转一圈是1040 和编码器精度有关，默认是摆杆Z轴转一圈，输出1040个跳变沿
	static int tmp,flag,count;
	tmp=click_N_Double(100); //key()放在5ms的中断里面
	
	if(tmp==1)flag=1;//++
  if(tmp==2)flag=2;//--
	
	if(flag==1) //摆杆顺时针运动			//没分析错的话单击一次顺时针转两圈，双击一次顺时针转两圈
	{
		Position_Zero++;
		count++;
		if(count==Position) 	flag=0,count=0;
	}	
	if(flag==2) //摆杆逆时针运动
	{
		Position_Zero--;
		count++;	
		if(count==Position) 	flag=0,count=0;
	}
}

/**************************************************************************
函数功能：异常关闭电机
入口参数：电压
返回  值：1：异常  0：正常
**************************************************************************/
u8 Turn_Off(int voltage)
{
	    u8 temp; 
			//电池电压过低或者倾角过大，关闭电机
			if(1==Flag_Stop||Angle_Balance<(ZHONGZHI-800)||Angle_Balance>(ZHONGZHI+800)||voltage<1110) 
			{	      
      Flag_Stop=1;				
      temp=1;                                            
			AIN1=0;                                            
			AIN2=0;
      }
			else
      temp=0;
      return temp;			
}
	

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
