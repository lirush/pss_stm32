#ifndef TRIGGER_H
#define TRIGGER_H

typedef struct{
    unsigned char set;
    unsigned char reset;
    unsigned char out;
}TRG_TypeDef;
extern TRG_TypeDef TRGUser[4];

void RStrigger(unsigned char num);

#endif
