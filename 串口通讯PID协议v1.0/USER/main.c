/*
GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 | GPIO_PinSource10, GPIO_AF_USART1);
����
9|10 = 1001 | 1010 = 1011 = 11
���Ὣ11���Ÿ��ö����ǽ�9��10�����Ÿ���
*/
//2017��7��7��16:25:03

# include "usart3.h"
# include "usart.h"

int main(void)
{
    usart3_init_128000();
		uart_init(115200);
    while(1);
}

