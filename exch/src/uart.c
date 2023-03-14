#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "stm32f4xx_usart.h"

uint8_t U1_Flag_TX=0;
uint8_t U1_TX_size=0;
uint8_t *pU1_TX_Buffer;


//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//  struct __FILE {int handle;};
//  FILE __stdout;
//#endif /* __GNUC__ */
	
void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_TXE)
	{
		USART1->SR &= ~(1<<7);
		if(U1_TX_size--)
		{
			USART1->DR=*pU1_TX_Buffer++;
		}
		else
		{
			U1_Flag_TX=0;
			USART1->CR1 &= ~USART_CR1_TXEIE;
		}
	}
}

/*
	функция интересна тем, что она не блокирующая
*/
void UART1_send(unsigned char *data, unsigned char size)
{
	if(U1_Flag_TX)return;
	U1_Flag_TX=1;
	pU1_TX_Buffer = data;
	U1_TX_size=size;
	USART1->CR1 |= USART_CR1_TXEIE;
}
