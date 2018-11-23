/*
GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 | GPIO_PinSource10, GPIO_AF_USART1);
最后会
9|10 = 1001 | 1010 = 1011 = 11
最后会将11引脚复用而不是将9和10号引脚复用
*/
//2017年7月7日16:25:03

# include "usart3.h"
# include "usart.h"

int main(void)
{
    usart3_init_128000();
		uart_init(115200);
    while(1);
}

