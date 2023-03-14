#include "schusep.h"
#include "svechi.h"
#include "timer.h"

Port_TypeDef SvechiPort = {.Port = GPIOD,.Pin = GPIO_Pin_8};

void SvechiON(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    ptr->io.close=IORESET;
    ptr->io.open=IOSET;
		if(!GPIO_ReadOutputDataBit(SvechiPort.Port,SvechiPort.Pin))
			GPIO_SetBits(SvechiPort.Port,SvechiPort.Pin);
}

void SvechiOFF(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    ptr->io.open=IORESET;
    ptr->io.close=IOSET;
    GPIO_ResetBits(SvechiPort.Port,SvechiPort.Pin);
}

static unsigned char SvechiErrOn(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    Tim10ms(ptr->io.open,SEVENTH,_300MS);                                 //уставка 300 мсек
    
    if(Tim10msUser[SEVENTH].FlagSetTimer & (!ptr->io.isopen)){
        Tim10msUser[SEVENTH].FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char SvechiErrOff(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    Tim10ms(ptr->io.close,EIGHT,_300MS);                              //уставка 300 мсек
    
    if(Tim10msUser[EIGHT].FlagSetTimer & ptr->io.isopen){
        Tim10msUser[EIGHT].FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char SvechiStatus(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    unsigned char *old_state = (unsigned char *)&ptr->io;
    
    if(SvechiErrOn()) return ERROROPEN1;                                //ошибка включения свечей
    
    if(SvechiErrOff()) return ERRORCLOSE1;                              //ошибка выключения свечей
    
    if(ptr->io.isopen) return ISOPENVALVE;                      //свечи включены
    
    return (*old_state);
}

void SvechiController(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Svechi;
    
    ptr->status = SvechiStatus();
}
