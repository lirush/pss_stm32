#include "schusep.h"
#include "ventilator.h"
#include "timer.h"

Port_TypeDef VentPort ={.Port=GPIOE,.Pin=GPIO_Pin_12};

void VentOn(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    
    ptr->io.open = IOSET;
    ptr->io.close = IORESET;
    
    GPIO_SetBits(VentPort.Port,VentPort.Pin);
}

void VentOff(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    
    ptr->io.close = IOSET;
    ptr->io.open = IORESET;
    
    GPIO_ResetBits(VentPort.Port,VentPort.Pin);    
}

static unsigned char VentErrOn(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    Tim10msUser_TypeDef *ptim = Tim10msUser+NINTH;
    Tim10ms(ptr->io.open,NINTH,_400MS);                                      //400 мсек.
    
    if(ptim->FlagSetTimer & (!ptr->io.isopen)){
        ptim->FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char VentErrOff(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    Tim10msUser_TypeDef *ptim = Tim10msUser+TENTH;
    Tim10ms(ptr->io.open,TENTH,_400MS);                                      //400 мсек.
    
    if(ptim->FlagSetTimer & ptr->io.isopen){
        ptim->FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char VentStatus(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    unsigned char *old_state = (unsigned char *)&ptr->io;
    
    if(VentErrOn()) return ERROROPEN1;                                   //ошибка включения вентилятора
    
    if(VentErrOff()) return ERRORCLOSE1;                                 //ошибка выключения вентилятора
    
    if(ptr->io.isopen) return ISOPENVALVE;                               //вентилятора включен
    
    if(!ptr->io.isopen) return ISCLOSEVALVE;                               //вентилятора включен
    
    return (*old_state);
}

void VentController(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Ventilator;
    
    ptr->status = VentStatus();
}

void AlgVentilator(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    static uint8_t state=0;
    
    switch(state){
				case 0:
					if(pEng->DSensors.B105_peregrev_ohlag) state=1;																		//если сработал датчик перегрева ОЖ
				break;
        case 1:
            VentOn();
            state=3;
        break;
        case 2:
            VentOff();
            state=0;              
        break;
				case 3:
					if(!pEng->DSensors.B105_peregrev_ohlag) state=2;
				break;
        default:break;        
    }
}
