#include "key.h"
 /**************************************************************************
���ߣ�ƽ��С��֮�� 
�Ա����̣�http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ�������ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void KEY_Init(void)
{
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��	   	 
	GPIOA->CRH&=0XFFF00FFF; 
	GPIOA->CRH|=0X00088000;
	
	GPIOA->CRL&=0XFF0F00FF; 
	GPIOA->CRL|=0X00808800;
	
  GPIOA->ODR|=3<<2; //PA2 3 ����	
  GPIOA->ODR|=1<<5; //PA5 ����
	GPIOA->ODR|=3<<11; //PA11 12  ����	
} 
/**************************************************************************
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫�� 
**************************************************************************/
u8 click_N_Double (u8 time)
{
		/***************����5msִ��һ��****************/
		static	u8 flag_key,count_key,double_key;	
		static	u16 count_single,Forever_count;
	  if(KEY2==0)  Forever_count++;   //����Ϊ0  //������־λδ��1
     else        Forever_count=0;
	
		//��һ�����ɿ���flag_key�Ż���ڣ���ΰ���Ϊ1
		if(0==KEY2&&0==flag_key)		flag_key=1;		
		//�������ɿ��Ļ�����һ��ִ�к���ʱʱ�ɿ��ĲŻ�ִ��
		//�����һ�����ɿ��Ļ�count_key=0����ִ��if�ڲ����
		//��ִ�к�count_key�����1
		//������ɿ�����count_keyһֱΪ1���Ͳ�����һ��ִ��
		//ִ��Ч����double_key++
		//���double_key����2�Ļ�˵����˫��������2
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
					return 2;//˫��ִ�е�ָ��
				}
		}
			
		//�����ɿ���־λflag_keyΪ0��
		if(1==KEY2)			flag_key=0,count_key=0;
		
		
		if(1==double_key)
		{
			count_single++;
			//�������ǵ�һ�ΰ���KEY2
			//������������ͬ����1��ֱ��KEY2�ɿ�ʱForever_count������
			/*��count_singleֻ��ִ����һ�������ļ��(��ʱtimes��5ms���ж�)
				�������˫�����ߵ����Ż�����*/
			//�������һֱ���ŵĻ���count_single>timeʱ,Forever_count=time�ᶨΪ����
			if(count_single>time&&Forever_count<time)
			{
			double_key=0;
			count_single=0;	
			return 1;//����ִ�е�ָ��
			}
			if(Forever_count>=time)
			{
			double_key=0;
			count_single=0;	
			}
		}	
		return 0;
		/***************����5msִ��һ��****************/
}
///**************************************************************************
//�������ܣ�����ɨ��
//��ڲ�������
//����  ֵ������״̬ 0���޶��� 1������ 
//**************************************************************************/
//u8 click(void)
//{
//			static u8 flag_key=1;//�������ɿ���־
//			if(flag_key&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))
//			{
//			flag_key=0;
//			if(KEY1==0)return 1;
//			if(KEY2==0)return 2;
//			if(KEY3==0)return 3;
//			if(KEY4==0)return 4;				
//			}
//			else if(1==KEY1&&1==KEY2&&1==KEY3&&1==KEY4)			flag_key=1;
//			return 0;//�ް�������
//}
///**************************************************************************
//�������ܣ��������
//��ڲ�������
//����  ֵ������״̬ 0���޶��� 1������2s
//**************************************************************************/
//u8 Long_Press(void)
//{
//			static u16 Long_Press_count,Long_Press;
//	    if(Long_Press==0&&KEY==0)  Long_Press_count++;   //������־λδ��1
//      else                       Long_Press_count=0; 
//		  if(Long_Press_count>200)		
//			{
//				Long_Press=1;	
//				Long_Press_count=0;
//				return 1;
//			}				
//			 if(Long_Press==1)     //������־λ��1
//			{
//				  Long_Press=0;
//			}
//			return 0;
//}
