#include "trigger.h"

TRG_TypeDef TRGUser[4];

void RStrigger(unsigned char num)
{
    TRG_TypeDef *p_trg = TRGUser+num;
    if(p_trg->set & (!p_trg->reset)){p_trg->out=1;}
    else {p_trg->out=0;}
}
