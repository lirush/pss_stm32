#include "schusep.h"
#include "valve.h"
#include "timer.h"
#include "trigger.h"
#include "fms.h"
#include "ebu.h"
#include "podogrev.h"

//enum{OPEN,CLOSE}CMD;

Port_TypeDef VsOpenPort={.Port = GPIOD,.Pin = GPIO_Pin_14,};
Port_TypeDef VsClosePort={.Port = GPIOD,.Pin = GPIO_Pin_13,};
Port_TypeDef V2OpenPort={.Port = GPIOD,.Pin = GPIO_Pin_12,};
Port_TypeDef V2ClosePort={.Port = GPIOD,.Pin = GPIO_Pin_11,};

pValveRstOpen pVRstOpen[2];
pValveRstClose pVRstClose[2];
pValveStatus pVStatus[2];

static State StateAlgValve = STATE1;
static Event eventToFire=STEP1;

void ValvesOpen(void)
{
//    Dev_ofValveType *pV1 = Sep.Valve+FIRST;
    
//    pV1->io.close = IORESET;
//    pV1->io.open=IOSET;

    GPIO_SetBits(VsOpenPort.Port,VsOpenPort.Pin);										//открываем
}

static void ValvesRstOpen(void)
{
    GPIO_ResetBits(VsOpenPort.Port,VsOpenPort.Pin);
}

void ValvesClose(void)
{
    Dev_ofValveType *pV1 = Sep.Valve+FIRST;
    
//    pV1->io.open = IORESET;
//    pV1->io.close=IOSET;
    
		if(!pV1->io.isclose)
		{
			GPIO_SetBits(VsClosePort.Port,VsClosePort.Pin);												//закрываем
			pV1->io.isclose=1;
		}
}

static void ValvesRstClose(void)
{
    GPIO_ResetBits(VsClosePort.Port,VsClosePort.Pin);
}

void Valve2Open(void)
{
    Dev_ofValveType *ptr = Sep.Valve+SECOND;
    ptr->io.close = IORESET;
    ptr->io.open=IOSET;
		
		GPIO_SetBits(V2OpenPort.Port,V2OpenPort.Pin);											//открываем
}

static void Valve2RstOpen(void)
{
    GPIO_ResetBits(V2OpenPort.Port,V2OpenPort.Pin);
}

void Valve2Close(void)
{
    Dev_ofValveType *ptr = Sep.Valve+SECOND;
    ptr->io.open = IORESET;
    ptr->io.close=IOSET;  

    GPIO_SetBits(V2ClosePort.Port,V2ClosePort.Pin);									//закрываем
}

static void Valve2RstClose(void)
{
    GPIO_ResetBits(V2ClosePort.Port,V2ClosePort.Pin);
}

unsigned char VsKeyOpenState(void)
{
    return GPIO_ReadOutputDataBit(VsOpenPort.Port,VsOpenPort.Pin);
}

unsigned char VsKeyCloseState(void)
{
    return GPIO_ReadOutputDataBit(VsClosePort.Port,VsClosePort.Pin);
}

unsigned char V2KeyOpenState(void)
{
    return GPIO_ReadOutputDataBit(V2OpenPort.Port,V2OpenPort.Pin);
}

unsigned char V2KeyCloseState(void)
{
    return GPIO_ReadOutputDataBit(V2ClosePort.Port,V2ClosePort.Pin);
}

//static unsigned char Valve1ErrOpen(void)
//{
//    Dev_ofValveType *ptr = Sep.Valve+FIRST;
//    Tim10ms(ptr->io.open,FIRST,_40SEC);                          //40 секунд уставка
//    if((Tim10msUser[FIRST].FlagSetTimer) & 
//        ptr->io.open &                                         //подана команда на открытие заслонки
//        ((!ptr->io.isopen) | ptr->io.isclose)){                  //если секундомер протикал а состояние заслонки открыта не появилось -> это авария заслонки
//            //Tim10msUser[FIRST].FlagSetTimer=0;                   //освобождаем таймер
//            return 1;
//        }
//     return 0;
//}

//static unsigned char Valve1ErrClose(void)
//{
//    Dev_ofValveType *ptr = Sep.Valve+FIRST;
//    Tim10ms(ptr->io.close,SECOND,_40SEC);                          //40 секунд уставка
//    if((Tim10msUser[SECOND].FlagSetTimer) & 
//        ptr->io.close &                                         //подана команда на закрытие заслонки
//        ((!ptr->io.isclose) | ptr->io.isopen)){                   //если секундомер протикал а состояние заслонки закрыта не появилось -> это авария заслонки
//            //Tim10msUser[SECOND].FlagSetTimer=0;                   //освобождаем таймер
//            return 1;
//        }
//     return 0;    
//}

//static unsigned char Valve2ErrOpen(void)
//{
//    Dev_ofValveType *ptr = Sep.Valve+SECOND;
//    Tim10ms(ptr->io.open,THIRD,_40SEC);                          //40 секунд уставка
//    if((Tim10msUser[THIRD].FlagSetTimer) & 
//        ptr->io.open &                                         //подана команда на открытие заслонки
//        ((!ptr->io.isopen) | ptr->io.isclose)){                  //если секундомер протикал а состояние заслонки открыта не появилось -> это авария заслонки
//            //Tim10msUser[THIRD].FlagSetTimer=0;                   //освобождаем таймер
//            return 1;
//        }
//     return 0;
//}

//static unsigned char Valve2ErrClose(void)
//{
//    Dev_ofValveType *ptr = Sep.Valve+SECOND;
//    Tim10ms(ptr->io.close,FOURTH,_40SEC);                          //40 секунд уставка
//    if((Tim10msUser[FOURTH].FlagSetTimer) & 
//        ptr->io.close &                                         //подана команда на закрытие заслонки
//        ((!ptr->io.isclose) | ptr->io.isopen)){                   //если секундомер протикал а состояние заслонки закрыта не появилось -> это авария заслонки
//            //Tim10msUser[FOURTH].FlagSetTimer=0;                   //освобождаем таймер
//            return 1;
//        }
//     return 0;    
//}

static unsigned char Valve1Status(void)
{
    Dev_ofValveType *ptr = Sep.Valve+FIRST;
    unsigned char *old_state = (unsigned char *)&ptr->io;
    
    return (*old_state);
}

unsigned char Valve1OpenByTRG(void)
{
    Dev_ofValveType *ptr_v = Sep.Valve+FIRST;
    Tim20msUser_TypeDef *ptr_tim = Tim20msUser+FIRST;
    TRG_TypeDef *ptr = TRGUser+FIRST;
    RStrigger(FIRST);
    if(ptr->out) {
        Tim20ms(EN,FIRST,_2SEC);                 //запускаем таймер 2-1 сек.
        ptr_v->io.close=IORESET;
        ptr_v->io.open=IOSET;
    }else ptr_v->io.open=IORESET;
    if(ptr_tim->FlagSetTimer){
        ptr_tim->FlagSetTimer=0;
        ptr->reset=IOSET;
        ptr_v->io.open=IORESET;
    }
    return ptr_v->io.open;
}

unsigned char Valve1CloseByTRG(void)
{
    Dev_ofValveType *ptr_v = Sep.Valve+FIRST;
    Tim20msUser_TypeDef *ptr_tim = Tim20msUser+SECOND;
    TRG_TypeDef *ptr = TRGUser+SECOND;
    RStrigger(SECOND);
    if(ptr->out) {
        Tim20ms(EN,SECOND,_2SEC);                 //запускаем таймер 2-1 сек.
        ptr_v->io.open=IORESET;
        ptr_v->io.close=IOSET;
    }else ptr_v->io.close=IORESET;
    if(ptr_tim->FlagSetTimer){
        ptr_tim->FlagSetTimer=0;
        ptr->reset=IOSET;
        ptr_v->io.close=IORESET;
    }
    return ptr_v->io.close;
}

unsigned char Valve2OpenByTRG(void)
{
    Dev_ofValveType *ptr_v = Sep.Valve+SECOND;
    Tim20msUser_TypeDef *ptr_tim = Tim20msUser+THIRD;
    TRG_TypeDef *ptr = TRGUser+THIRD;
    RStrigger(THIRD);
    if(ptr->out) {
        Tim20ms(EN,THIRD,_2SEC);                 //запускаем таймер 3-1 сек.
        ptr_v->io.close=IORESET;
        ptr_v->io.open=IOSET;
    }else ptr_v->io.open=IORESET;
    if(ptr_tim->FlagSetTimer){
        ptr_tim->FlagSetTimer=0;
        ptr->reset=IOSET;
        ptr_v->io.open=IORESET;
    }
    return ptr_v->io.open;
}

unsigned char Valve2CloseByTRG(void)
{
    Dev_ofValveType *ptr_v = Sep.Valve+SECOND;
    Tim20msUser_TypeDef *ptr_tim = Tim20msUser+FOURTH;
    TRG_TypeDef *ptr = TRGUser+FOURTH;
    RStrigger(FOURTH);
    if(ptr->out) {
        Tim20ms(EN,FOURTH,_2SEC);                 //запускаем таймер 2-1 сек.
        ptr_v->io.open=IORESET;
        ptr_v->io.close=IOSET;
    }else ptr_v->io.close=IORESET;
    if(ptr_tim->FlagSetTimer){
        ptr_tim->FlagSetTimer=0;
        ptr->reset=IOSET;
        ptr_v->io.close=IORESET;
    }
    return ptr_v->io.close;
}

static uint8_t ImplValveController(uint8_t n_val, uint8_t n_tim1, uint8_t n_tim2)     //!!!
{    
    Dev_ofValveType *ptr_v = Sep.Valve+n_val;
    Tim20msUser_TypeDef *ptr_tim1 = Tim20msUser+n_tim1;
    Tim20msUser_TypeDef *ptr_tim2 = Tim20msUser+n_tim2;
		uint8_t rev=0;
    
    //Tim20ms(ptr_v->io.open,n_tim1,_1SEC);           //1 сек.
    Tim20ms(ptr_v->KeyOpenState(),n_tim1,_1SEC);           //1 сек.
    if(ptr_tim1->FlagSetTimer)
        {
            ptr_tim1->FlagSetTimer=0;
            pVRstOpen[n_val]();
						rev++;
        }
    
    //Tim20ms(ptr_v->io.close,n_tim2,_1SEC);          //1 сек.
    Tim20ms(ptr_v->KeyCloseState(),n_tim2,_1SEC);          //1 сек.
    if(ptr_tim2->FlagSetTimer)
        {
            ptr_tim2->FlagSetTimer=0;
            pVRstClose[n_val]();
						rev++;
        }
    
    return rev;
}

void ValveController(void)
{
    Dev_ofValveType *pV1 = Sep.Valve+FIRST;
	
    pV1->status = Valve1Status();
}

void ValveResetKeys(void)
{
		Dev_ofValveType *pV2 = Sep.Valve+SECOND;
    ImplValveController(FIRST,FIFTH,SIXTH);
	
		if(pV2->status==ISOPEN)Valve2RstOpen();
		if(pV2->status==ERROROPEN1)Valve2RstOpen();
	
		if(pV2->status==ISCLOSE)Valve2RstClose();
		if(pV2->status==ERRORCLOSE1)Valve2RstClose();	
	
}

void InitLevelValve(void)
{
    pVRstOpen[FIRST] = ValvesRstOpen;
    pVRstOpen[SECOND] = Valve2RstOpen;
    pVRstClose[FIRST] = ValvesRstClose;
    pVRstClose[SECOND] = Valve2RstClose;
    pVStatus[FIRST] = Valve1Status;
}

static signed char StepValveOpen(void)
{
    {ValvesOpen();}																	//по новой документации - это обе заслонки
		StateAlgValve = STATE3;
    return 0;
}

static signed char StepValveCtrlOpen(void)
{
    return ImplValveController(FIRST,FIFTH,SIXTH);
}

static signed char StepValveClose(void)
{
	ValvesClose();																	//по новой документации - это обе заслонки
  StateAlgValve = STATE5; 
  return 0;
}

static signed char StepValveCtrlClose(void)
{
	return ImplValveController(FIRST,FIFTH,SIXTH);
}

signed char StartValve(void)
{
	Sep_typeDef *pSep = &Sep;
	Ebu_TypeDef *pEbu = &pSep->Disel.Engine.Ebu;
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
	Schusep_TypeDef *pSchu = &pSep->SchuSep;
	Pki_TypeDef *pPki = pSep->Gens.Pki+0;
  Dev_ofValveType *pV1 = pSep->Valve+FIRST;
	
	static uint8_t cnt=0;
	
	uint8_t state_pki =0;
	
	for(int dev=0;dev<=5;dev++)
	{
		state_pki |= ((pPki->freq < 10)?1:0)<<dev;		 //freq pki #1-6 <10Hz
		pPki++;
	}
	
	uint8_t state = ((pEbu->eng_speed<ENGINESPEED300) && (!pSchu->Ind_podogrev) && (state_pki==0x3F))?CLOSE:0;
	
	if(state && (cnt++>=150) && (pV1->io.isopen) && (!pV1->io.isclose))		//15 seconds TSYS=100ms 150000ms/TSYS=15sec
		{
			pV1->io.isopen=0; 
			StateAlgValve = STATE4; cnt=0;							//to close
		}								
	if(!state)cnt=0;
		
//	if(pV1->io.open)																//&& (state==0)
		if((pHot->temp>=TempLiquidCalc(85)) && (!pV1->io.isopen))
		{
			pV1->io.isopen=1;
			pV1->io.isclose=0;
			StateAlgValve = STATE2;												//to open
		}
	
	return 1;
}

static Transition transitionTable[]={
		{STATE1,STEP1,StartValve},
		{STATE2,STEP1,StepValveOpen},
		{STATE3,STEP1,StepValveCtrlOpen},
    {STATE4,STEP1,StepValveClose},
    {STATE5,STEP2,StepValveCtrlClose}
};

signed char FMSFunc_Valve(int elementsCount, Transition *transitionTable, State currentState, Event eventToFire)
{
	switch((uint8_t)currentState)
	{
		case STATE1:
				StartValve();
		break;
		case STATE2:
			StepValveOpen();
		break;
		case STATE3:
			if(StepValveCtrlOpen())StateAlgValve=STATE1;
		break;
		case STATE4:
			StepValveClose();
		break;
		case STATE5:
			if(StepValveCtrlClose())StateAlgValve=STATE1;
		break;
	}
    return 0;
}

signed char AlgValve(void)
{
    int elementsCount = sizeof(transitionTable) / sizeof(transitionTable[0]);
    return FMSFunc_Valve(elementsCount,transitionTable,StateAlgValve,eventToFire);
    
	//return 0;
}
