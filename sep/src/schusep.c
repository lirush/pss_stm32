/**
  ******************************************************************************
  * @file    schusep.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    07.04.2020
  * @brief   Запуск алгоритма автоматического запуска диз. СЭП 
  ******************************************************************************
  */
	
//#include "schusep.h"
#include "declar.h"
#include "sdata.h"
#include "trigger.h"

Sep_typeDef Sep;
StopCtrl_TypeDef StopCtrl;
uint8_t stateSep=0;
static ModeSep cnt_mode=DEFAULT;                                                        //счетчик режима - определяет режим работы автоматический старт или автоматический стоп
static int cnt_start=0;
static int cnt_stop=0;
uint8_t status_pki_need_clear=0;
volatile uint8_t Flag_BlockSepFromPU=0;

/*******************************************************************************
* Function Name  : HowDoSepFeel
* Description    : определяет состояние СЭП
* Input          : нет
* Output         : 1 - не норм; 0 - норм
* Return         : нет
*******************************************************************************/
void HowDoSepFeel(void)
{
	Sep_typeDef *pSep = &Sep;
	DSensors_TypeDef *pSensors = &pSep->Disel.Engine.DSensors;
	Schusep_TypeDef *pSchusep = &pSep->SchuSep;
	Dev_ofEngineType *pEng = &pSep->Disel.Engine;
	
	pSchusep->SEP_not_norm =(pSep->SchuSep.ppo 																						//pSensors->oil_press_dis
											| pEng->Ebu.io1.in0 																							//авария ЭБУ
											| pSensors->B105_peregrev_ohlag
											| pSensors->B107_oil_level_ohlag
											| pSensors->B109_filter_zasoren
											| pSensors->B106_oil_level_poddon);
}

uint8_t startDisSepAuto(void)
{
    Sep_typeDef *ptr = &Sep;
    Schusep_TypeDef *pSchusep = &ptr->SchuSep;
	
		if(pSchusep->SB6_auto_start_sep | pSchusep->START_eng_PU)
		{
			if((cnt_mode==DEFAULT) || (cnt_mode==STOP) || (cnt_mode==STOPPING))          //старт диз. в автомат. режиме  
					{return cnt_mode=START;}			
		}
    
    return 0;
}

uint8_t stopDisSepAuto(void)
{
    Sep_typeDef *ptr = &Sep;
    Schusep_TypeDef *pSchusep = &ptr->SchuSep;
    
    if(pSchusep->SB1_off_sep | pSchusep->Alarm_STOP_eng_PU)                          //ptr_schusep->SA1_auto_dis_sep & 
    {
			if((cnt_mode==START) || (cnt_mode==STARTING))                                 //стоп диз. в автомат. режиме 
					{return cnt_mode=STOP;}
		}
    return 0;
}

void SepMode(void)
{

    if(startDisSepAuto()==START){stateSep=1;}                                     //переход к старту
    
    if(stopDisSepAuto()==STOP)
    {stateSep=2;}                                        													//переход к останову
    
}

void BlockSepFromPU(void)
{
	Sep_typeDef *ptr = &Sep;
	Schusep_TypeDef *pSchusep = &ptr->SchuSep;
	
	if(pSchusep->Alarm_STOP_eng_PU)
		Flag_BlockSepFromPU |=(1<<4);
	else Flag_BlockSepFromPU &= ~(1<<4);
}

/*******************************************************************************
* Function Name  : StatusSepClear
* Description    : очищает флаги статусов СЭП, только те которые необходимо 
									 вне автоматического режима запуска двигателя
* Input          : Аврарийное отключения СЭП от ПУ
									 СЭП в норме
									 Обороты двигателя 300
									 Авт.Старт.СЭП
									 Работа ВУ
* Output         : нет
* Return         : нет
*******************************************************************************/
void StatusSepClear(void)
{
		Sep_typeDef *pSep = &Sep;
    Schusep_TypeDef *pSchusep = &pSep->SchuSep;
		Disel_TypeDef *pDis = &pSep->Disel;

		if(!pSchusep->Alarm_STOP_eng_PU || !pSchusep->SEP_not_norm || !pDis->io.isopen)
			pSep->status.auto_start_eng_impossible=0;
		
		if(pSchusep->SB6_auto_start_sep)
			pSep->status.eng_didnt_start_manual=0;
		
		if(pSchusep->SB6_auto_start_sep)
			pSep->status.eng_didnt_start_oil=0;
		
		if(!pSchusep->vu1)
			pSep->status.alarm_shutdown_load=0;
		
}

void StartStopDiselSep(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
    StopCtrl_TypeDef *pStop = &StopCtrl;
    uint8_t flTemp=0;                   //флаг - используется для "взвода" алгоритма старта или алгоритма останова
    
    SepMode();
	
		AlgValve();

    AlgVentilator();                                                                //алгоритм вентилятора должен работать независимо  

    pHot->io.out0=pHot->Algorithm();                                                //out0 - ОЖ нагрета; алгоритм подогрева должен работать независимо, в рамках автоматического режима    

    xStarterLocker();
    
//    ValveResetKeys();
	
		vSepAlarmDisable();																															//контроль параметров ПКИ
	
		StatusSepClear();
	
		BlockSepFromPU();

    switch(stateSep){
        case 0:
           
        break;
        case 1:
           flTemp=(cnt_mode==START)?1:0;                                            //флаг перехода в старт -  воздействие должно быть разовым -> смотреть следующую строчку
           cnt_mode=STARTING;
           Sep.Disel.AutoStart(flTemp);
           if(pStop->EndStopping){cnt_start=cnt_stop=0;pStop->EndStopping=0;cnt_mode=STOPPING;stateSep=0;}
					 if(!pHot->io.in0)pHot->io.in0=1;
        break;
        case 2:
           flTemp=(cnt_mode==STOP)?1:0;                                             //флаг перехода в старт - воздействие должно быть разовым -> смотреть следующую строчку
           cnt_mode=STOPPING;
           Sep.Disel.AutoStop(flTemp);
           if(pStop->EndStopping){cnt_start=cnt_stop=0;pStop->EndStopping=0;stateSep=0;}
        break;
    }
    
}
