#include "schusep.h"
#include "podogrev.h"
#include "timer.h"
#include "fms.h"
#include "valve.h"
#include "alarm.h"
#include "engine.h"

Port_TypeDef PodogrevPort = {.Port=GPIOE,.Pin=GPIO_Pin_14};
Port_TypeDef PodogrevPortOFF = {.Port=GPIOE,.Pin=GPIO_Pin_13};

uint8_t Flag_Off_Podogrev=0;

void PodogrevON(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    
    pHot->io.open = IORESET;
    pHot->io.open = IOSET;
	
		GPIO_ResetBits(PodogrevPortOFF.Port,PodogrevPortOFF.Pin);
    
	  if(!GPIO_ReadOutputDataBit(PodogrevPort.Port,PodogrevPort.Pin))
    GPIO_SetBits(PodogrevPort.Port,PodogrevPort.Pin);
}

static void PodogrevRstOn(void)
{
    GPIO_ResetBits(PodogrevPort.Port,PodogrevPort.Pin);
}

void PodogrevOFF(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    
    pHot->io.open = IORESET;
    pHot->io.close = IOSET;
  
		GPIO_ResetBits(PodogrevPort.Port,PodogrevPort.Pin);
	
		if(!GPIO_ReadOutputDataBit(PodogrevPortOFF.Port,PodogrevPortOFF.Pin))
			GPIO_SetBits(PodogrevPortOFF.Port,PodogrevPortOFF.Pin);
}

//static void PodogrevRstOff(void)
//{
//    GPIO_ResetBits(PodogrevPortOFF.Port,PodogrevPortOFF.Pin);
//}

static unsigned char PodogrevErrOn(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    Tim10msUser_TypeDef *ptim = Tim10msUser+ELEVENTH;
    
    Tim10ms(pHot->io.open,ELEVENTH,_400MS);                               //400 мсек
    if(ptim->FlagSetTimer & (!pHot->io.isopen)){
        ptim->FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char PodogrevErrOff(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    Tim10msUser_TypeDef *ptim = Tim10msUser+TWELFTH;
    
    Tim10ms(pHot->io.close,TWELFTH,_400MS);                                //400мсек
    if(ptim->FlagSetTimer & pHot->io.isopen){
        ptim->FlagSetTimer=0;
        return 1;
    }
    return 0;
}

unsigned char TempLiquidIsMinus(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;

    unsigned int temp = (TEMP_COOLLIQ_FROM-1)*VAL_OF_DIV;//((-1)*ADC_MAX)/TEMP_COOLLIQ_FROM;
    
    if(pHot->temp<=temp) return 1;
    
    return 0;    
}

unsigned char TempLiquidIs10(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;

    unsigned int temp = (TEMP_COOLLIQ_FROM+10)*VAL_OF_DIV;//(10 *ADC_MAX)/TEMP_COOLLIQ_TO;
    
    if(pHot->temp<=temp) return 1;
    
    return 0;    
}

unsigned char TempLiquidIs20(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;

    unsigned int temp = (TEMP_COOLLIQ_FROM+20)*VAL_OF_DIV;//(20 *ADC_MAX)/TEMP_COOLLIQ_TO;
    
    if(pHot->temp<=temp) return 1;
    
    return 0;    
}

unsigned char TempLiquidIs30(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;

    unsigned int temp = (TEMP_COOLLIQ_FROM+30)*VAL_OF_DIV;//(30 *ADC_MAX)/TEMP_COOLLIQ_TO;
    
    if(pHot->temp<=temp) return 1;
    
    return 0;    
}

unsigned char TempLiquidIs40(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;

    signed int temp = (TEMP_COOLLIQ_FROM+40)*VAL_OF_DIV;//(40 *ADC_MAX)/TEMP_COOLLIQ_TO;
    
    if(pHot->temp<=temp) return 1;
    
    return 0;    
}

signed int TempLiquidCalc(signed char temp)
{
	return (temp-(float)TEMP_COEFF_C)/(float)TEMP_COEFF_B;
}

/*
функция вычисляет температуры подогрева ОЖ
параметры выходные: 1- ОЖ подогрета до тепературы уставки SET_TEMP_COOLIQ
                    0- ОЖ не подогрета
*/
unsigned char LiquidIsHot(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    
    if(pHot->temp>=TEMP_COOLIQ_ISHOT) return 1;
    
    return 0;
}

static unsigned char PodogrevStatus(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    unsigned char *old_state = (unsigned char *)&pHot->io;
    
    if(PodogrevErrOn()) {return ERROROPEN1;}
    
    if(PodogrevErrOff()) {return ERRORCLOSE1;}
    
    if(pHot->io.isopen) {return ISOPENVALVE;}
    
    if(!pHot->io.isopen) {return ISCLOSEVALVE;}
    
    return (*old_state);
}

void PodogrevController(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;      
    
    pHot->status = PodogrevStatus();
}

static Event event = STEP1;
static State nextState = STATE1;
unsigned char podogrevState=0;

/*******************************************************************************
* Function Name  : chooseStartStop
* Description    : определяет состояние СЭП
* Input          : Авт.старт СЭП
									 Температура ОЖ
									 Аварийный стоп СЭП от ПУ
									 ОТКЛ. СЭП
									 Перегрев ОЖ
									 Обороты двигателя 300
* Output         : nextState = STATE1;
									 nextState = STATE4;
* Return         : нет
*******************************************************************************/
void chooseStartStop(void)
{
	Schusep_TypeDef *pSchusep = &Sep.SchuSep;
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
	DSensors_TypeDef *pSensors = &Sep.Disel.Engine.DSensors;
	
	if((pHot->io.in0) && (pHot->temp<(TEMP_COOLLIQ_FROM+65)*VAL_OF_DIV) && ((nextState==STATE1) || (nextState==STATE6)))
		nextState = STATE2;
	
	if((pSchusep->SB6_auto_start_sep) && (pHot->temp<(TEMP_COOLLIQ_FROM+65)*VAL_OF_DIV))
	{
		nextState = STATE2;
		pHot->io.in0=1;
	}
	
	if(((pSensors->B105_peregrev_ohlag) | (pSchusep->SB1_off_sep) | (pSchusep->Alarm_STOP_eng_PU)))
	{
		nextState = STATE5;
	}
	if((uiControlSpeed300()) || (pHot->temp>(TEMP_COOLLIQ_FROM+85)*VAL_OF_DIV))
	{
		nextState = STATE5;
	}

}

static signed char StartAlgPodogrev(void)
{
//	Schusep_TypeDef *pSchusep = &Sep.SchuSep;
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
	
	if((pHot->temp<(TEMP_COOLLIQ_FROM+66)*VAL_OF_DIV))		//(pSchusep->SB6_auto_start_sep) && 
	{
		nextState = STATE2;
		pHot->io.in0=1;
	}	
	return 0;
}

static signed char StopAlgPodogrev(void)
{
	Schusep_TypeDef *pSchusep = &Sep.SchuSep;
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
	DSensors_TypeDef *pSensors = &Sep.Disel.Engine.DSensors;
	
	if(((pSchusep->SB1_off_sep) | (pSchusep->Alarm_STOP_eng_PU)))
	{
		nextState = STATE5;
		Flag_Off_Podogrev=1;
	}
	if(pSensors->B105_peregrev_ohlag)	
	{
		nextState = STATE5;		
	}
	if(uiControlSpeed300())
	{
		nextState = STATE5;	
		Flag_Off_Podogrev=1;
	}
	if(pHot->temp>(TEMP_COOLLIQ_FROM+86)*VAL_OF_DIV)
		nextState = STATE5;	
	
	return 0;
}

/*******************************************************************************
* Function Name  : openValveRight
* Description    : открывает заслонку правого борта
* Input          : нет
* Output         : nextState = STATE2;
									 nextState = STATE5;
* Return         : нет
*******************************************************************************/
static signed char openValveRight(void)
{
		Dev_ofValveType *pValve2 = Sep.Valve+SECOND;
		
		VsCloseClearFlagsTims();
		if((pValve2->status != OPENING) || (pValve2->status != ISOPEN))
		pValve2->Open();
	
		event = STEP2;
	
    return 0;
}
static signed char checkValve2(void)
{
    Dev_ofValveType *pV2 = Sep.Valve+SECOND;
	
    if(pV2->status==ISOPEN)
        {
            nextState = STATE3;
						event=STEP1;
        }
    if(pV2->status==ERROROPEN1)
        {
            nextState = STATE1;
						event=STEP1;
        }
                
    return 0;
}

/*******************************************************************************
* Function Name  : onPodogrev
* Description    : включает подогреватель
* Input          : подогреватель включен
* Output         : nextState = STATE3;
									 nextState = STATE4;
* Return         : нет
*******************************************************************************/
static signed char onPodogrev(void)
{
		Sep_typeDef *pSep = &Sep;
		Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
		static uint8_t cnt=0;
	
		pHot->On();
	
		if(pHot->io.isopen)
			nextState = STATE4;
		
		if((cnt++>=_300MS) || (pHot->io.isopen))
		{
			PodogrevRstOn();
		}
		
		if((cnt++>=_300MS) && (!pHot->io.isopen))
		{
			PodogrevRstOn();
			pSep->status.heater_didnt_start_manual=1;
			nextState = STATE1;
		}
			
    return 0;
}

/*******************************************************************************
* Function Name  : offPodogrev
* Description    : выключает подогреватель
* Input          : нет
* Output         : nextState = STATE3;
									 nextState = STATE4;
* Return         : нет
*******************************************************************************/
static signed char offPodogrev(void)
{
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
	
	pHot->Off();
	
	nextState = STATE1;
	
	if(Flag_Off_Podogrev){Flag_Off_Podogrev=0; pHot->io.in0=0;}
	
	return 0;
}

static Transition transitionTablePgd[5]={
		{STATE1,STEP1,StartAlgPodogrev},
    {STATE2,STEP1,openValveRight},
		{STATE2,STEP2,checkValve2},
    {STATE3,STEP1,onPodogrev},
		{STATE5,STEP1,offPodogrev}
};

signed char FMSFunc_Hot(int elementsCount, Transition *transitionTable, State currentState, Event eventToFire)
{
	switch((uint8_t)currentState)
	{
		case STATE1:
			return StartAlgPodogrev();
		case STATE2:
//			if(event==STEP1){return openValveRight();}
//			if(event==STEP2){return checkValve2();}
		nextState = STATE3;
		break;
		case STATE3:
			return onPodogrev();
		case STATE4:
			return StopAlgPodogrev();
		case STATE5:
			return offPodogrev();
	}
    return 0;    
}

/*
функция алгоритм подогрева ОЖ
параметры выходные: 1 - алгоритм прошел успешно
*/
signed char AlgPodogrev(void)
{
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
//		chooseStartStop();
		if(!pHot->io.in0) return 0;
		int elementsCount = sizeof(transitionTablePgd) / sizeof(transitionTablePgd[0]);
		FMSFunc_Hot(elementsCount,transitionTablePgd,nextState,event);
    podogrevState=nextState;
    return 0;
}
