#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_can.h"

#include "stm32f4xx_adc.h"
#include "stm32f4xx_dma.h"
#include "init_peripherals.h"
#include "config.h"

//extern __IO uint16_t      ADCValues[size_masiv_vish*4]; //приходит из main

//void ADC13_Init(void)
//{
//	/*
//  ADC_InitTypeDef       ADC_InitStructure;
//  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//  DMA_InitTypeDef       DMA_InitStructure;
//  GPIO_InitTypeDef      GPIO_InitStructure;

//  // Enable ADC, DMA2 and GPIO clocks ***************************************
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);

// 

//  // Configure ADC pins as analog input ******************************

////	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_CAN1); 
//	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	

//  // ADC Init ***************************************************************
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_NbrOfConversion = 10;
//	
//  ADC_Init(ADC2, &ADC_InitStructure);
//	

//	  // Enable ADC 
//  ADC_Cmd(ADC2, ENABLE);


// // ADC_Init(ADC3, &ADC_InitStructure);

//ADC_InjectedChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_15Cycles);
//ADC_InjectedSequencerLengthConfig(ADC2, 1);
//ADC_AutoInjectedConvCmd(ADC2, ENABLE);

//ADC_ContinuousModeCmd(ADC2, ENABLE);

//*/
//	NVIC_InitTypeDef NVIC_InitStructure;
//  ADC_InitTypeDef       ADC_InitStructure;
//  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//  DMA_InitTypeDef       DMA_InitStructure;
//  GPIO_InitTypeDef      GPIO_InitStructure;

//  /* Enable ADC, DMA2 and GPIO clocks ****************************************/
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC3, ENABLE);

//  /* DMA2 configuration **************************************/
//  DMA_InitStructure.DMA_Channel = DMA_Channel_0;
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
//  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADCValues;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//  DMA_InitStructure.DMA_BufferSize = size_masiv_vish * 4;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA2_Stream4, &DMA_InitStructure);
//	DMA_ITConfig(DMA2_Stream4, DMA_IT_TC, ENABLE);
//  DMA_Cmd(DMA2_Stream4, ENABLE);

//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  //chanal 4 - дут , 6 - двс 
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

////chanal 10 актуатор
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);

//  /* ADC Common Init **********************************************************/
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
//  ADC_CommonInit(&ADC_CommonInitStructure);

//  /* ADC Init ****************************************************************/
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//  ADC_InitStructure.ADC_NbrOfConversion = 4;
//  ADC_Init(ADC1, &ADC_InitStructure);

//  
// // ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_480Cycles);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_480Cycles);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_480Cycles);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 4, ADC_SampleTime_480Cycles);

///* Enable DMA request after last transfer (Single-ADC mode) */
//  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);    
//  
//  /* Enable ADC DMA */
//  ADC_DMACmd(ADC1, ENABLE);

//  /* Enable ADC */
//  ADC_Cmd(ADC1, ENABLE);
// 
//  /* Start ADC Software Conversion */
//  ADC_SoftwareStartConv(ADC1);   

//}

//void SPI3_Init(void)
//{
//	SPI_InitTypeDef  SPI_InitStructure;	
//	GPIO_InitTypeDef GPIO_InitStructure;	
//  NVIC_InitTypeDef NVIC_InitStructure;
//  /* Peripheral Clock Enable -------------------------------------------------*/
//  /* Enable the SPI clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);// for SPI1
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);// for SPI2
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); // for SPI3.
//  
//  /* Enable GPIO clocks */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

//  /* SPI GPIO Configuration --------------------------------------------------*/
//  /* Connect SPI pins to AF5 */  
//  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

//	//miso, mosi, slk
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//	//NSS
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	 
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
// 
//  /* SPI configuration -------------------------------------------------------*/
//  SPI_I2S_DeInit(SPI3);
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	
//	SPI_Init(SPI3, &SPI_InitStructure);
//  
//	SPI_Cmd(SPI3, ENABLE);
//  /* Configure the Priority Group to 1 bit */    
//	
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  // Configure the SPI interrupt priority 
//  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//}
//void SPI1_Init(void)
//{
//	SPI_InitTypeDef  SPI_InitStructure;	
//	GPIO_InitTypeDef GPIO_InitStructure;	
//  NVIC_InitTypeDef NVIC_InitStructure;
//  /* Peripheral Clock Enable -------------------------------------------------*/
//  /* Enable the SPI clock */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);// for SPI1

//  
//  /* Enable GPIO clocks */
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);

//  /* SPI GPIO Configuration --------------------------------------------------*/
//  /* Connect SPI pins to AF5 */  
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
//	//miso, mosi, slk
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL ;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	//NSS
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	 
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
//  /* SPI configuration -------------------------------------------------------*/
//  SPI_I2S_DeInit(SPI1);
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//	
//	SPI_Init(SPI1, &SPI_InitStructure);
//  
//	SPI_Cmd(SPI1, ENABLE);
//  /* Configure the Priority Group to 1 bit */    
//	
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  // Configure the SPI interrupt priority 
//  NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//}
void NVIC_Config(void) 
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Enable the USARTx Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
    
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void GPIO_Init_TE(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd((RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOC) , ENABLE);
		/*Configure GPIO pin Output Level */
  GPIO_WriteBit(GPIOF, GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6, Bit_RESET);

  /*Configure GPIO pin Output Level */
  GPIO_WriteBit(GPIOE, GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12 
                          |GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);

  /*Configure GPIO pin Output Level */
  GPIO_WriteBit(GPIOD, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 
                          |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, Bit_RESET);

  /*Configure GPIO pin Output Level */
  GPIO_WriteBit(GPIOG, GPIO_Pin_2, Bit_RESET);

  /*Configure GPIO pins : PE2 PE3 PE4 PE5 
                           PE6 PE0 PE1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 
                          |GPIO_Pin_6|GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /*Configure GPIO pins : PC13 PC14 PC15 PC6 
                           PC7 PC8 PC9 PC10 
                           PC11 PC12 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_6 
                          |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 
                          |GPIO_Pin_11|GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /*Configure GPIO pins : PF0 PF1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /*Configure GPIO pins : PF2 PF3 PF4 PF5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /*Configure GPIO pins : PE9 PE10 PE11 PE12 
                           PE13 PE14 PE15 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12 
                          |GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11 
                           PD12 PD13 PD14 PD15 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11 
                          |GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /*Configure GPIO pin : PG2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /*Configure GPIO pins : PG3 PG4 PG5 PG6 
                           PG7 PG8 PG9 PG10 
                           PG11 PG12 PG13 PG14 
                           PG15 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 
                          |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 
                          |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14 
                          |GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  /*Configure GPIO pins : PD0 PD1 PD2 PD3 
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 
                          |GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	/*-------------------------------------------*/
	///РЕЗЕРВ!!!
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9                  //GPIO_Pin_6| - добавили к выводам
                          |GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13
													|GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 
                          |GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
	/*-------------------------------------------*/
}

void CAN1_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	//Инициализация CAN2 для STM32F207
///**	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

		
		//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 

	 // Connect PD0 to CAN1_Rx pin  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); 
	// Connect PD1 to CAN1_Tx pin  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); 
	
	// Configure CAN RX and TX pins 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);



////        // Connect PD0 to CAN1_Rx pin 
////        GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_CAN1);
////        // Connect PD1 to CAN1_Tx pin 
////        GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_CAN1);
////        
////        //config RX TX pins
////        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
////        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
////        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
////        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
////        GPIO_Init(GPIOD, &GPIO_InitStructure);



	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,  ENABLE);
	// CAN configuration 
	// Enable CAN clock

	// CAN register init 
	CAN_DeInit(CAN1);
	
	CAN_InitStructure.CAN_TTCM = DISABLE;			//Включение или отключение времени срабатывает режим связи
	CAN_InitStructure.CAN_ABOM = ENABLE;			//Включить или отключить автоматическое управление bus-off    //тут кажется нужно будет включить
	CAN_InitStructure.CAN_AWUM = DISABLE;			//Включить или выключить режим автоматического пробуждению
	CAN_InitStructure.CAN_NART = DISABLE;			//Включить или выключить режим без автоматического повтора передачи.
	CAN_InitStructure.CAN_RFLM = DISABLE;			//Включение или отключение Locked режим приема FIFO.
	CAN_InitStructure.CAN_TXFP = DISABLE;			//Включить или выключить приоритет передачи FIFO
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //Определяет режим работы CAN
		 
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
// CAN Baudrate = 125 (CAN clocked at 30 MHz)  
 //BaudRate=1/[ t_pclk(BRP +1)(1+Ts1+1+Ts2+1) ]
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;      //4----это TS1+1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq; 			//5----это TS2+1
	CAN_InitStructure.CAN_Prescaler = 24; 				//24----это BRP-1
	CAN_Init(CAN1, &CAN_InitStructure);
	
 // CAN Baudrate = 250 (CAN clocked at 30 MHz)  
 //BaudRate=1/[ t_pclk(BRP +1)(1+Ts1+1+Ts2+1) ]
	/*CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;      //3----это TS1+1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq; 			//2----это TS2+1
	CAN_InitStructure.CAN_Prescaler = 20; 				//20----это BRP-1
	CAN_Init(CAN1, &CAN_InitStructure); */
    
	CAN_FilterInitStructure.CAN_FilterNumber = 1; // от 14 до 27 только для CAN2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)((ID_PMV<<3)>>16); 
	CAN_FilterInitStructure.CAN_FilterIdLow = (uint16_t) ((ID_PMV<<3) | 0x04);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)((ID_EBU<<3)>>16);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(uint16_t)((ID_EBU<<3) | 0x04);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; 
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);        

	CAN_FilterInitStructure.CAN_FilterNumber = 2; // от 14 до 27 только для CAN2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)((ID_PKI1<<3)>>16); 
	CAN_FilterInitStructure.CAN_FilterIdLow = (uint16_t)((ID_PKI1<<3) | 0x04);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)((0xFFFFF000<<3)>>16);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(uint16_t)((0xFFFFF000<<3));
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1; 
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure); 
	
	CAN_FilterInitStructure.CAN_FilterNumber = 3; // от 14 до 27 только для CAN2
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)((ID_BRI1PN1AN1<<3)>>16); 
	CAN_FilterInitStructure.CAN_FilterIdLow = (uint16_t)((ID_BRI1PN1AN1<<3));
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)((0xFFFFF000<<3)>>16);
	CAN_FilterInitStructure.CAN_FilterMaskIdLow =(uint16_t)((0xFFFFF000<<3));
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO1; 
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);   

	CAN_FilterInitStructure.CAN_FilterNumber = 4;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)((ID_BRI2PN2AN2<<3)>>16); 
	CAN_FilterInitStructure.CAN_FilterIdLow = (uint16_t)((ID_BRI2PN2AN2<<3) | 0x04);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)((ID_PMV_PKI<<3)>>16); 
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = (uint16_t)((ID_PMV_PKI<<3) | 0x04);
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);    
    
}

void CAN2_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	 // Connect PB5 to CAN2_Rx pin  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1); 
	// Connect PB6 to CAN2_Tx pin  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1); 
	
	// Configure CAN RX and TX pins 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
////         // Connect PB5 to CAN2_Rx pin  
////        GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN1); 
////        // Connect PB6 to CAN2_Tx pin  
////        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN1); 
////        
////        // Configure CAN RX and TX pins 
////        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6; 
////        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
////        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
////        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
////        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
////        GPIO_Init(GPIOB, &GPIO_InitStructure); 

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2,  ENABLE);
		
	// CAN configuration 
	// Enable CAN clock

	// CAN register init 
	CAN_DeInit(CAN2);

	// CAN cell init 
	CAN_InitStructure.CAN_TTCM = DISABLE;			//Включение или отключение времени срабатывает режим связи
	CAN_InitStructure.CAN_ABOM = ENABLE;			//Включить или отключить автоматическое управление bus-off    //тут кажется нужно будет включить
	CAN_InitStructure.CAN_AWUM = DISABLE;			//Включить или выключить режим автоматического пробуждению
	CAN_InitStructure.CAN_NART = DISABLE;			//Включить или выключить режим без автоматического повтора передачи.
	CAN_InitStructure.CAN_RFLM = DISABLE;			//Включение или отключение Locked режим приема FIFO.
	CAN_InitStructure.CAN_TXFP = DISABLE;			//Включить или выключить приоритет передачи FIFO
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //Определяет режим работы CAN
		 
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;  
 //CAN Baudrate = 125 (CAN clocked at 30 MHz)  
 //BaudRate=1/[ t_pclk(BRP +1)(1+Ts1+1+Ts2+1) ]
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;      //4----это TS1+1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq; 			//5----это TS2+1
	CAN_InitStructure.CAN_Prescaler = 24; 				//24----это BRP-1
	
	
 // CAN Baudrate = 250 (CAN clocked at 30 MHz)  
 //BaudRate=1/[ t_pclk(BRP +1)(1+Ts1+1+Ts2+1) ]
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;      //3----это TS1+1
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq; 			//2----это TS2+1
//	CAN_InitStructure.CAN_Prescaler = 512; 				//20----это BRP-1
	CAN_Init(CAN2, &CAN_InitStructure); 
    
//	//это фильтрация  для запрета всех входных пакетов
//	CAN_FilterInitStructure.CAN_FilterNumber = 15; // от 14 до 27 только для CAN2
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList; 
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit ; 	 
//	CAN_FilterInitStructure.CAN_FilterIdHigh = (uint16_t)((0x7FFFF<<3)>>16); 
//	CAN_FilterInitStructure.CAN_FilterIdLow =  (uint16_t) ((0x7FFFF<<3) | 0x04);		
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0; 
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 
//	//CAN_FilterInit(&CAN_FilterInitStructure);

}

void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;		
	RCC_ClocksTypeDef RCC_ClockFreq;
	// Сбрасываем клокинг в "0"
	RCC_DeInit();
	// Включаем внешний кварц
	RCC_HSEConfig(RCC_HSE_ON);//RCC_HSE_Bypass);
	// Ждём пока устанавливается внешний ВЧ тактовый сигнал
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus != ERROR)  
	{
		
    // Внешний тактовый сигнал появился, стабильный
    // разрешаем использование буфера команд
		FLASH_PrefetchBufferCmd(ENABLE);
    //-------------------------------------------------------------------
 		// итак
		//Внешний кварц 25МГц
		//Тактовая частота ядра: 120МГц		
		//-------------------------------------------------------------------
		/* Flash 2 wait state  двойная задержка*/
		FLASH_SetLatency(FLASH_Latency_2);

		RCC_HCLKConfig(RCC_SYSCLK_Div1); /* HCLK = SYSCLK    AHB clock = SYSCLK=120*/

		RCC_PCLK1Config(RCC_HCLK_Div4);  //APB1 clock = HCLK/4 120/4=30 для CAN
		RCC_PCLK2Config(RCC_HCLK_Div4);  //APB2 clock = HCLK/2 120/4=30 
		
		//RCC_PLLSource_HSE=25 - источник для PLL, PLLM=25,  PLLN=240, PLLP=2, PLLQ=5
		//  HSE/PLLM=VCO, VCO*PLLN/PLLP=PLLCLK, VCO*PLLN/PLLQ=PLL48CK см. 48 стр мануала 
		RCC_PLLConfig(RCC_PLLSource_HSE,25, 240, 2, 4);
		//Разрешаем PLL
		RCC_PLLCmd(ENABLE);

		// Ждём пока устаканится PLL
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY ) == RESET)
		{}
		// PLL - устаканился
		// Выбираем PLL как источник тактирования системы
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		// Ждём пока PLL не станет клокингом системы
		while (RCC_GetSYSCLKSource() != 0x08)
		{}
		
	}
	RCC_GetClocksFreq(&RCC_ClockFreq);
	
}

void USART_Init_TE(void)
{
	//инициализация из примера HyperTerminal_Interrupt
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3 , ENABLE);
	
	//D8 - TX USART3 - вывод 5 черный провод
	// Connect PXx to USARTx_Tx
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);

	//D9 - RX USART3 - вывод 3 розовый провод
  // Connect PXx to USARTx_Rx/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);


  // Configure USART Tx as alternate function  /
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // Configure USART Rx as alternate function  /
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	// USARTx configuration ------------------------------------------------------/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode=USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure);
	
	USART_Cmd(USART3, ENABLE); //включаем USART3
	
}

void USART1_Init_TE(void)
{
	//инициализация из примера HyperTerminal_Interrupt
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	
	// Connect PXx to USARTx_Tx
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

  // Connect PXx to USARTx_Rx/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);


  // Configure USART Tx as alternate function  /
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART Rx as alternate function  /
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	// USARTx configuration ------------------------------------------------------/
  /* USARTx configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - Two Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	
  USART_InitStructure.USART_BaudRate = 128000;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode=USART_Mode_Tx;	//USART_Mode_Rx | 

  USART_Init(USART1, &USART_InitStructure);
	
	USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;// | USART_CR1_TXEIE;
	
	USART_Cmd(USART1, ENABLE); //включаем USART1
	
	NVIC_EnableIRQ(USART1_IRQn);	
}

