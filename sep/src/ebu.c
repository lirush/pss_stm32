#include "schusep.h"
#include "ebu.h"
#include "timer.h"

unsigned char SpeedIsNot(void)
{
    Ebu_TypeDef *pEbu = &Sep.Disel.Engine.Ebu;
    
    if(pEbu->eng_speed==0)
        return 1;
    return 0;
}

unsigned char SpeedIs(void)
{
    Ebu_TypeDef *pEbu = &Sep.Disel.Engine.Ebu;
    
    if((pEbu->eng_speed>=ENGINESPEED100) && (pEbu->eng_speed<ENGINESPEED400))
        return 1;
    return 0;
}

unsigned char SpeedIs400(void)
{
    Ebu_TypeDef *pEbu = &Sep.Disel.Engine.Ebu;
    
    if(pEbu->eng_speed>=ENGINESPEED400)
        return 1;
    return 0;    
}

unsigned char SpeedIsHH(void)
{
    Ebu_TypeDef *pEbu = &Sep.Disel.Engine.Ebu;
    
    if((pEbu->eng_speed>=ENGINESPEED900) && (pEbu->eng_speed<ENGINESPEED1700))
        return 1;
    return 0;
}

unsigned char SpeedIs1800(void)
{
    Ebu_TypeDef *pEbu = &Sep.Disel.Engine.Ebu;
    
    if(pEbu->eng_speed>=ENGINESPEED1800)
        return 1;
    return 0;
}
