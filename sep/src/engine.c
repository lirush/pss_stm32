#include "schusep.h"
#include "engine.h"
#include "ebu.h"
#include "timer.h"

Port_TypeDef EnginePort_hhON ={.Port=GPIOG,.Pin=GPIO_Pin_2};
Port_TypeDef EnginePort_1800ON ={.Port=GPIOD,.Pin=GPIO_Pin_15};

void Engine_start(void)
{
    Dev_ofEngineType *ptr = &Sep.Disel.Engine;
    
    ptr->io1.close=IORESET;
    //ptr->io1.open=IOSET;
    
		if(!GPIO_ReadOutputDataBit(EnginePort_hhON.Port,EnginePort_hhON.Pin))
    GPIO_SetBits(EnginePort_hhON.Port,EnginePort_hhON.Pin);
}

void Engine_hh_stop(void)
{
    Dev_ofEngineType *ptr = &Sep.Disel.Engine;
    
    ptr->io1.open=IORESET;
    ptr->io1.close=IOSET;
    
    GPIO_ResetBits(EnginePort_hhON.Port,EnginePort_hhON.Pin);    
}

void Engine_1800_start(void)
{
    Dev_ofEngineType *ptr = &Sep.Disel.Engine;
    
    ptr->io2.close=IORESET;
    ptr->io2.open=IOSET;
    
    GPIO_SetBits(EnginePort_1800ON.Port,EnginePort_1800ON.Pin);    
}

void Engine_1800_stop(void)
{
    Dev_ofEngineType *ptr = &Sep.Disel.Engine;
    
    ptr->io2.open=IORESET;
    ptr->io2.close=IOSET;
    
    GPIO_ResetBits(EnginePort_1800ON.Port,EnginePort_1800ON.Pin);
}

static unsigned char EngineHHErrStart(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Tim20msUser_TypeDef *pTim = Tim20msUser+NINTH;
    
    Tim20ms(pEng->io1.open,NINTH,_30SEC);                          //30 сек.
    if(pTim->FlagSetTimer & (!SpeedIsHH())){
        //pTim->FlagSetTimer =0;
        return 1;                                               //при возврате значения 1 -> ошибка старта ХХ
    }
    return 0;
}

static unsigned char EngineHHErrStop(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Tim20msUser_TypeDef *pTim = Tim20msUser+TENTH;

    Tim10ms(pEng->io1.close,TENTH,_3SEC);                          //3 сек.
    if(pTim->FlagSetTimer & SpeedIsHH()){
        pTim->FlagSetTimer=0;
        return 1;                                               //ошибка остановки ХХ
    }
    return 0;
}

static unsigned char Engine1800ErrStart(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Tim20msUser_TypeDef *pTim = Tim20msUser+ELEVENTH;
    
    Tim20ms(pEng->io2.open,ELEVENTH,_30SEC);                          //30 сек.
    if(pTim->FlagSetTimer & (!SpeedIs1800())){
        //pTim->FlagSetTimer =0;
        return 1;                                               //при возврате значения 1 -> ошибка старта ХХ
    }
    return 0;    
}

static unsigned char Engine1800ErrStop(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Tim20msUser_TypeDef *pTim = Tim20msUser+TWELFTH;

    Tim20ms(pEng->io2.close,TWELFTH,_3SEC);                          //3 сек.
    if(pTim->FlagSetTimer & SpeedIs1800()){
        pTim->FlagSetTimer=0;
        return 1;                                               //ошибка остановки ХХ
    }
    return 0;    
}

static unsigned char EngineStatus(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    unsigned char *old_status = (unsigned char *)&pEng->io1;
    //*old_status |= (uint8_t)&pEng->io1;
    
    if(EngineHHErrStart()) return ERRHHSTART;
    
    if(EngineHHErrStop()) return ERRHHSTOP;

    if(Engine1800ErrStart()) return ERR1800START;
    
    if(Engine1800ErrStop()) return ERR1800STOP;
    
    if(SpeedIsNot()) return ENG_ISSTOP;
    
    if(SpeedIsHH()) return ENGHH_ISSTART;
    
    if(SpeedIs1800()) return ENG1800_ISSTART;
    
    if(pEng->io1.open) return ENGHH_STARTING;
    
    if(pEng->io2.open) return ENG1800_STARTING;
    
    if(pEng->io1.close) return ENGHH_STOPPING;
    
    if(pEng->io2.close) return ENG1800_STOPPING;    
    
    return (*old_status);
}

unsigned char uiControlSpeed300(void)
{
	Disel_TypeDef *pDis = &Sep.Disel;
	Tim1minUser_TypeDef *pTim = Tim1minUser+SECOND;
	uint8_t temp=0;
	
	Tim1min(pDis->io.isopen,SECOND,_3MIN);
	
	if(pTim->FlagSetTimer)
	{
		pTim->FlagSetTimer=0;
		temp= 1;
	}
	
	if(!pDis->io.isopen)
	{ 
		pTim->FlagSetTimer=0;
		pTim->InsideCntTimer=0;
		temp=0;
	}
	
	return temp;
}

void EngineController(void)
{
    Dev_ofEngineType *ptr = &Sep.Disel.Engine;
    
    ptr->status = EngineStatus();
}

