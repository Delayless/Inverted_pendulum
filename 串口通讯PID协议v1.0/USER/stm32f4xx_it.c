#include "stm32f4xx_it.h"
#include "usart.h"

u8 sign = 0;
int Moto = 0;
u8 end_flag = 0;
u8 RxCounter = 0;
u8 sign_flag = 0;
u8 start_flag = 0;
u8 complete_flag = 0;
int pwm_temp = 0;

void USART3_IRQHandler()
{
    u8 res;
	u8 clear = clear;;
    if(USART_GetITStatus(USART3, USART_IT_RXNE))
    {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        //res = USART_ReceiveData(USART3);
		res=USART3->DR;		//自然可以把中断标志位清零，如果发生数据异常可以检查这里

		if('$' == res)	//帧尾55  
			end_flag = 1;
		
		if(1 == start_flag && 0 == end_flag && 1 == sign_flag)
			pwm_temp =pwm_temp*10 +(res - '0');		//将字节类型数据转换成int型
		
		if(1 == start_flag && 0 == sign_flag)
		{
			sign = res;
			sign_flag = 1;
		}
		
		if('^' == res)	//帧首
			start_flag = 1;
		
		if(1 == end_flag)
		{
			if('+' == sign && 0 == complete_flag)
			{
				Moto = pwm_temp;
				//数据处理
				complete_flag = 1;
			}
			else
				if('-' == sign && 0 == complete_flag)
				{
					complete_flag = 1;
					Moto = -pwm_temp;
				}
			
			sign = 0;
			pwm_temp = 0;
			end_flag = 0;
			RxCounter = 0;
			sign_flag = 0;
			start_flag = 0;
			printf("%d", Moto);
		}	
	else
		if(USART_GetFlagStatus(USART3, USART_FLAG_IDLE) == RESET)
		{
			clear = USART3->SR;
			clear = USART3->DR; 
		}
    }
}
