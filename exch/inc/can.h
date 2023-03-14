#ifndef CAN_H
#define CAN_H

#include "stm32f4xx_can.h"
#include "config.h"

typedef struct						        //структура где размещены все получаемые данные
{
	CanTxMsg Tx_Message;
	CanRxMsg Rx_Message;                    //копия отправляемых данных
  CanTxMsg SepTx_Message[2];
}input_data;

struct DataItem {
   unsigned char data[8];   
   int key;
};
extern struct DataItem hashArr[QCANDEV];

typedef struct{
    uint8_t NewRxMsg;
    CanRxMsg Rx_Message;                    //принятые данные
}CANRxBasket_TypeDef;
extern CANRxBasket_TypeDef CAN1RxBasket;

typedef struct{
    uint8_t NewTxMsg;
    CanTxMsg Tx_Message;                    //копия отправляемых данных
}CANTxBasket_TypeDef;
extern CANTxBasket_TypeDef CAN1TxBasket;

extern input_data pr_data;

#endif
