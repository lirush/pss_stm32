#include "sensors.h"
#include "can.h"

void Read_from_sensor(void)
{
	uint16_t i;
	for (i = 0; i <= 4; i++)	pr_data.Tx_Message.Data[i] &= 0x00;
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1) 	<< 	( InA1 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_0)	<< 	( InB1 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) 	<< 	( InA2 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) 	<< 	( InB2 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) 	<<	( InA3 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) 	<< 	( InB3 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) 	<< 	( InA4 ) );
	pr_data.Tx_Message.Data[0] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) 	<< 	( InB4 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) 	<< 	( InA5 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) 	<< 	( InB5 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) 	<< 	( InA6 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) 	<< 	( InB6 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15)	<< 	( InA7 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_14) 	<< 	( InB7 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) 	<< 	( InA8 ) );
	pr_data.Tx_Message.Data[1] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12) 	<< 	( InB8 ) );      
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) 	<< 	( InA9 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10) 	<< 	( InB9 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) 	<< 	( InA10 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) 	<< 	( InB10 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) 	<< 	( InA11 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) 	<< 	( InB11 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) 	<< 	( InA12 ) );
	pr_data.Tx_Message.Data[2] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) 	<< 	( InB12 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) 	<< 	( InA13 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1) 	<< 	( InB13 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_0) 	<< 	( InA14 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12)	<< 	( InB14 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11) 	<< 	( InA15 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10) 	<< 	( InB15 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) 	<< 	( InA16 ) );
	pr_data.Tx_Message.Data[3] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) 	<< 	( InB16 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7) 	<< 	( InA17 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) 	<< 	( InB17 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_8) 	<< 	( InA18 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) 	<< 	( InB18 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) 	<< 	( InA19 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_5) 	<< 	( InB19 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_4) 	<< 	( InA20 ) );
	pr_data.Tx_Message.Data[4] |=  (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_3) 	<< 	( InB20 ) );  
    
//    pr_data.Tx_Message.Data[5] = 0x00;
//    pr_data.Tx_Message.Data[5] |= (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_11) 	<< 	( InA1 ) );
//    pr_data.Tx_Message.Data[5] |= (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_12) 	<< 	( InB1 ) );
//    pr_data.Tx_Message.Data[5] |= (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_13) 	<< 	( InA2 ) );
//    pr_data.Tx_Message.Data[5] |= (GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_14) 	<< 	( InB2 ) );
//    pr_data.Tx_Message.Data[5] |= (GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_6) 	<< 	( InA3 ) );
    
    
}
