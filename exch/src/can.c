#include <string.h>
#include "can.h"

input_data pr_data;
CANTxBasket_TypeDef CAN1TxBasket;
CANRxBasket_TypeDef CAN1RxBasket;

struct DataItem hashArr[QCANDEV]={
	{.key=ID_PMV},
	{.key=ID_EBU},
	{.key=ID_PKI1},
	{.key=ID_PKI2},
	{.key=ID_PKI3},
	{.key=ID_PKI4},
	{.key=ID_PKI5},
	{.key=ID_PKI6},
	{.key=ID_BRI1PN1AN1},
	{.key=ID_BRI1PN1AN2},
  {.key=ID_BRI1PN2AN1},
  {.key=ID_BRI1PN2AN2},
	{.key=ID_BRI2PN1AN1},
	{.key=ID_BRI2PN1AN2},
  {.key=ID_BRI2PN2AN1},
  {.key=ID_BRI2PN2AN2},
	{.key=ID_PMV_PKI}
};

int sizeArr=sizeof(hashArr)/sizeof(hashArr[0]);
int getHashIndex(unsigned int key);
void CAN1CopyData(CANRxBasket_TypeDef *rx);

void CAN1_RX1_IRQHandler (void)
{
    CANRxBasket_TypeDef *ptr = &CAN1RxBasket;
//    GPIO_SetBits(GPIOE,GPIO_Pin_15);
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)){
        ptr->NewRxMsg=1;
        CAN_Receive(CAN1, CAN_FIFO0, &ptr->Rx_Message);
    }
    else if(CAN_GetITStatus(CAN1,CAN_IT_FMP1)){
        ptr->NewRxMsg=2;
        CAN_Receive(CAN1, CAN_FIFO1, &ptr->Rx_Message);        
    }
    
    CAN1CopyData(ptr);	
    
//    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
}

void CAN1_RX0_IRQHandler (void)	
{   
    CANRxBasket_TypeDef *ptr = &CAN1RxBasket;
//    GPIO_SetBits(GPIOE,GPIO_Pin_15);
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)){
        ptr->NewRxMsg=1;
        CAN_Receive(CAN1, CAN_FIFO0, &ptr->Rx_Message);
    }
    else if(CAN_GetITStatus(CAN1,CAN_IT_FMP1)){
        ptr->NewRxMsg=2;
        CAN_Receive(CAN1, CAN_FIFO1, &ptr->Rx_Message);        
    }
    
    CAN1CopyData(ptr);
	
//    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
}

int getHashIndex(unsigned int key)
{
	struct DataItem *ptr = &hashArr[0];
	for(int i=0;i<=QCANDEV;i++){
		if(ptr->key==key) return i;
		else ptr++;
	}
	return -1;
}

//struct DataItem *_search(int key) {
//   //get the hash 
//   int hashIndex = getHashIndex(key);  
//	 struct DataItem *p = &hashArr[hashIndex];
//   //move in array until an empty 
////   while(p != 0) {
////	
////      if(p->key == key)
////         return p; 
////			
////      //go to next cell
////      ++hashIndex;
////   }        
//	
//   return 0;        
//}

void CAN1CopyData(CANRxBasket_TypeDef *rx)
{
    if((rx->NewRxMsg==1) || (rx->NewRxMsg==2))
		{        
        
      uint32_t id=0;
        
			if(rx->Rx_Message.IDE==4) id=rx->Rx_Message.ExtId;									//пакет с расширенным или стандартным id
        else id=rx->Rx_Message.StdId;
        
      int hashIndex = getHashIndex(id);
        
			if(hashIndex>-1)
      memcpy(&hashArr[hashIndex].data,rx->Rx_Message.Data,8);
			
			rx->NewRxMsg = 0;
    }    
}
