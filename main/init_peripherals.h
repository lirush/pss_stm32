#ifndef INIT_PERITH_H
#define INIT_PERITH_H

#define size_masiv_vish 10

void GPIO_Init_TE(void);
void CAN2_Config(void);
void CAN1_Config(void);
void RCC_Configuration(void);
void NVIC_Config(void);
void USART_Init_TE(void);
void ADC13_Init(void);
void SPI3_Init(void);
void SPI1_Init(void);
void USART1_Init_TE(void);

#endif //INIT_PERITH_H
