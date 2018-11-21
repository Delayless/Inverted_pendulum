#include "control.h"		
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
int Balance_Pwm,Position_Pwm;
u8 Flag_Target,Position_Target;
/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
          TIM1���Ƶ�5ms��ʱ�ж� 
**************************************************************************/
int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms��ʱ�ж�
	{   
		  TIM1->SR&=~(1<<0);                                       //===�����ʱ��1�жϱ�־λ	                     
	     if(delay_flag==1)
			 {
				 if(++delay_50==10)	 delay_50=0,delay_flag=0;          //===���������ṩ50ms�ľ�׼��ʱ
			 }
			//����TIM2 -> CNT��ֵ
    	Encoder=Read_Encoder(2);             	                   //===���±�����λ����Ϣ
      //���ؽǶȴ�������ֵ
			 Angle_Balance=Get_Adc_Average(7,15);                     //===������̬	
     	
			/**********************pythonд***************************/
			/*********************************************************
			 �ⲿ�ж��з����ȼ�
			 ͨ���ı�Position_Zero���ı�ڸ�λ��
			 *********************************************************/
			//�̼Ҳ��õķ��������������ıջ���ֻ����ֱ�����Ŀ���Ƶ�ʸ��ߣ��Ӷ�ʵ�ֿ���Ч��
			DataScope();	            //===python��λ��
			/*����λ��д Balance_Pwm = balance(Angle_Balance);                                          //===�Ƕ�PD����	

	    if(++Position_Target>4)	Position_Pwm=Position(Encoder),Position_Target=0;     //===λ��PD���� 25ms����һ��λ�ÿ���
      Moto=Balance_Pwm-Position_Pwm;        //===����������PWM
		  Xianfu_Pwm();                         //===PWM�޷� ����ռ�ձ�100%������ϵͳ���ȶ�����
			
			*/
	  	Led_Flash(100);                       //===LED��˸ָʾϵͳ�������� 
	    Voltage=Get_battery_volt();           //===��ȡ��ص�ѹ	      
			//Key();       //�������Կ���������ת             //===ɨ�谴���仯
	}       	
	 return 0;	  
} 


/*************************************************************************
python��λ�������ݴ���÷��ͻص�Ƭ��
ֱ�Ӿ���PWM����
**************************************************************************/
void USART1_IRQHandler(void)
{
		u8 res;	
		if(USART1->SR&(1<<5))//���յ�����
		{	 
				res=USART1->DR;		//��Ȼ���԰��жϱ�־λ���㣬������������쳣���Լ������
				Moto = res;		 		
				if(Turn_Off(Voltage)==0)              //===��ѹ����ǹ��󱣻�
						Set_Pwm(Moto);                        //===��ֵ��PWM�Ĵ���			
		} 
} 

/**************************************************************************
�������ܣ����PD����
��ڲ������Ƕ�
����  ֵ����ǿ���PWM
��    �ߣ�ƽ��С��֮��
**************************************************************************/
int balance(float Angle)
{  
   float Bias;                       //���ƫ��
	 static float Last_Bias,D_Bias;    //PID��ر���
	 int balance;                      //PWM����ֵ 
	 Bias=Angle-ZHONGZHI;              //���ƽ��ĽǶ���ֵ �ͻ�е���
	 D_Bias=Bias-Last_Bias;            //���ƫ���΢�� ����΢�ֿ���
	 balance=-Balance_KP*Bias-D_Bias*Balance_KD;   //===������ǿ��Ƶĵ��PWM  PD����
   Last_Bias=Bias;                   //������һ�ε�ƫ��
	 return balance;
}

/**************************************************************************
�������ܣ�λ��PD���� 
��ڲ�����������
����  ֵ��λ�ÿ���PWM
��    �ߣ�ƽ��С��֮��

int Position(int Encoder)
{  
   static float Position_PWM,Last_Position,Position_Bias,Position_Differential;
	 static float Position_Least;
  	Position_Least =Encoder-Position_Zero;             //===
    Position_Bias *=0.8;		   
    Position_Bias += Position_Least*0.2;	             //===һ�׵�ͨ�˲���  
	  Position_Differential=Position_Bias-Last_Position;
	  Last_Position=Position_Bias;
		Position_PWM=Position_Bias*Position_KP+Position_Differential*Position_KD; //===�ٶȿ���	
	  return Position_PWM;
}
**************************************************************************/


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto)
{
    	if(moto<0)			AIN2=1,			AIN1=0;
			else 	          AIN2=0,			AIN1=1;
			PWMA=myabs(moto);
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM������7200 ������6900
	  if(Moto<-Amplitude) Moto=-Amplitude;	
		if(Moto>Amplitude)  Moto=Amplitude;		
}
**************************************************************************/


/**************************************************************************
�������ܣ������޸�С������״̬  ���ưڸ˵�λ��
��ڲ�������
����  ֵ����
**************************************************************************/
//key()����5ms���ж�����
void Key(void)
{	
	int Position=2080; //Ŀ��λ�� ���ԭʼλ����10000  תһȦ��1040 �ͱ����������йأ�Ĭ���ǰڸ�Z��תһȦ�����1040��������
	static int tmp,flag,count;
	tmp=click_N_Double(100); //key()����5ms���ж�����
	
	if(tmp==1)flag=1;//++
  if(tmp==2)flag=2;//--
	
	if(flag==1) //�ڸ�˳ʱ���˶�			//û������Ļ�����һ��˳ʱ��ת��Ȧ��˫��һ��˳ʱ��ת��Ȧ
	{
		Position_Zero++;
		count++;
		if(count==Position) 	flag=0,count=0;
	}	
	if(flag==2) //�ڸ���ʱ���˶�
	{
		Position_Zero--;
		count++;	
		if(count==Position) 	flag=0,count=0;
	}
}

/**************************************************************************
�������ܣ��쳣�رյ��
��ڲ�������ѹ
����  ֵ��1���쳣  0������
**************************************************************************/
u8 Turn_Off(int voltage)
{
	    u8 temp; 
			//��ص�ѹ���ͻ�����ǹ��󣬹رյ��
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
�������ܣ�����ֵ����
��ڲ�����int
����  ֵ��unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
