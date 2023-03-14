#include "declar.h"
#include "fms.h"
#include "config.h"

//static unsigned char dis_speaker=0;

Port_TypeDef TimerPort={.Port=GPIOF,.Pin=GPIO_Pin_6};

static Event event = STEP1;                                  //событие по-которому происходит переход к следующему состоянию
static State nextState = STATE1;                         //текущее состояние
uint8_t PodogrevIsHot=0;

uint8_t globalState=0;
uint8_t end_stopping=0;

static State getState(void)
{
    return nextState;
}

static void setNextState(void)
{
    nextState= (State)(getState()+1);
}

static void waitStart(uint8_t start)
{
    if(start)
        {
            VsOpenClearFlagsTims();                               //предстартовая очистка таймеров устройст, учавствующих в алгоритме
            EngineClearFlagsTims();
            nextState = STATE1;
        }
}

void xTimerON(void)
{
    if(!GPIO_ReadOutputDataBit(TimerPort.Port,TimerPort.Pin))
        GPIO_SetBits(TimerPort.Port,TimerPort.Pin);
}

void xTimerOFF(void)
{
    GPIO_ResetBits(TimerPort.Port,TimerPort.Pin);
}

/*
функция определяет дальнейшее поведение по нажатию кнопоки стоп
*/
static void waitStop(uint8_t stop)                                  
{
    if(stop)
        {
            VsCloseClearFlagsTims();
            
						if((getState()==STATE1) || (getState()==STATE2) || (getState()==STATE3))
							{nextState=STATE16;}
						
						if(getState()==STATE4){nextState=STATE18;}
						
						if((getState()==STATE5) || (getState()==STATE6)){nextState=STATE19;}
						
						if(getState()==STATE7){nextState=STATE20;}
						
						if(getState()==STATE8){nextState=STATE19;}
						
						if(getState()==STATE9){nextState=STATE13;}
							
            if(getState()==STATE10){nextState=STATE11;}
        }
}

/*******************************************************************************
* Function Name  : preStartCheck
* Description    : предстартовая проверка можно ли запустить двигатель
* Input          : аварийный останов от ПУ;
									 состояние СЭП в норме (см. HowDoSepFeel);
									 обороты двигателя СЭП более 300 (discret signal); 
* Output         : nextState = STATE2;
									 состояние статуса - Авт. запуск двигателя СЭП невозможен
* Return         : 0
*******************************************************************************/
static signed char preStartCheck(void)
{
    Sep_typeDef *pSep = &Sep;
    Schusep_TypeDef *pSchusep = &pSep->SchuSep;
		Disel_TypeDef *pDis = &pSep->Disel;

		if(!pSchusep->Alarm_STOP_eng_PU || !pSchusep->SEP_not_norm || !pDis->io.isopen)
		{
			setNextState();						//nextState = STATE2;
		}
		else pSep->status.auto_start_eng_impossible=1; 						//Авт. запуск двигателя СЭП невозможен
		
		return 0;
}

/*******************************************************************************
* Function Name  : onPodogrev
* Description    : включает подогреватель, проверяет температуру ОЖ>0
* Input          : температура ОЖ;
* Output         : nextState = STATE3;
* Return         : 0
*******************************************************************************/
static signed char enPodogrev(void)
{
    Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;
		static uint16_t cnt_pgd=0;
  
		if(pHot->temp>(TEMP_COOLLIQ_FROM+6)*VAL_OF_DIV)
		{
				setNextState();																				//nextState = STATE3;
				cnt_pgd=0;
		}
		if(pHot->temp<=(TEMP_COOLLIQ_FROM+6)*VAL_OF_DIV)
		{
				if(cnt_pgd++>=_300MS)
					GPIO_ResetBits(GPIOE,GPIO_Pin_14);
				else pHot->On();                                            //включаем подогреватель
		}                           
    
    return 0;
}

/*******************************************************************************
* Function Name  : checkValves
* Description    : открывает заслонки
* Input          : нет
* Output         : event=STEP2;
* Return         : 0
*******************************************************************************/
static signed char opValves(void)                           //открыть заслонки
{   
		Sep_typeDef *pSep = &Sep;
    Dev_ofValveType *pValves = Sep.Valve+FIRST;
    
		pValves->io.isclose=0;																	//
    VsOpenClearFlagsTims();
//    pValves->Open();
    pValves->io.open=1;
    event=STEP2;
  
		pSep->status.cmd_valves_open=1;
	
    return 0;
}
static signed char checkValves(void)
{
		Sep_typeDef *pSep = &Sep;
    Dev_ofValveType *pValves = Sep.Valve+FIRST;
	
		{
						pSep->status.eng_start_not_to_load=1;
						pSep->status.cmd_valves_open=0;
						pValves->io.open=0;
            setNextState();																				//nextState = STATE4;
						event=STEP1;
		}
		
    return 0;
}

/*******************************************************************************
* Function Name  : permitEng
* Description    : включает ЭБУ двигателя
* Input          : авария ЭБУ
* Output         : nextState = STATE5;
* Return         : 0
*******************************************************************************/
static signed char permitEng(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    
    if(!pEng->Ebu.io1.in0)                                  //авария ЭБУ
        {pEng->start_hh();setNextState();}									//nextState=STATE5}
    else {nextState=STATE16;}
    
    return 0;
}

/*******************************************************************************
* Function Name  : onSvechi
* Description    : включает: ЭБУ и свечи
* Input          : индикация свечи
* Output         : nextState = STATE6;
* Return         : 0
*******************************************************************************/
static signed char onSvechi(void)
{
		Sep_typeDef *pSep = &Sep;
    Dev_ofStarterType *pSvechi = &Sep.Disel.Svechi;
		static uint8_t cnt=0;
    
    pSvechi->On();                                          //включаем свечи
 
		if((cnt++>=_4SEC) && (pSvechi->io.isopen))                  //индикация свечи ожидание 4 сек.
		{	
			setNextState();																				//nextState = STATE6;
			cnt=0;
			pSep->status.on_off_bu_eng_sep=1;
		}
		
		if((cnt>_4SEC) && (!pSvechi->io.isopen))
		{
			nextState = STATE18;
			cnt=0;			
		}
    
    return 0;
}

/*******************************************************************************
* Function Name  : startStarter
* Description    : запускает алгоритм стартера
* Input          : 14-ый таймер
* Output         : nextState = STATE7;
* Return         : 0
*******************************************************************************/
static signed char startStarter(void)
{
		Sep_typeDef *pSep = &Sep;
    //Tim20msUser_TypeDef *pTim = Tim20msUser+FOURTEENTH;
    
    Tim20msUser[FOURTEENTH].FlagSetTimer=1;
    
    setNextState();								//nextState = STATE7;
	
		pSep->status.eng_start_auto=1;
    
    return 0;
}

/*******************************************************************************
* Function Name  : startEng
* Description    : запуск двигателя по стартеру на ХХ
* Input          : нет
* Output         : nextState = STATE8;
* Return         : 0
*******************************************************************************/
static signed char startEng(void)
{
		Sep_typeDef *pSep = &Sep;
		Dev_ofEngineType *pEng = &pSep->Disel.Engine;
    Dev_ofStarterType *pStarter = &pSep->Disel.Starter;
//    Dev_ofValveType *pV1 = pSep->Valve+FIRST;
//    Dev_ofValveType *pV2 = pSep->Valve+SECOND;
    int8_t temp=pStarter->Algorithm();
	
//    pV1->io.open=pV2->io.open=IOSET;
//    pV1->io.close=pV2->io.close=IORESET;
    
    if(temp == 1){
        setNextState();																//nextState = STATE8;
				pSep->status.eng_start_auto=0;
    }
    else{
			if(temp == -1) 
			{
				pEng->stop_hh();
				pSep->status.eng_didnt_start_manual=1;
				pSep->status.on_off_bu_eng_sep=0;
				nextState=STATE19;
			}
		}
    
    return 0;
}

/*******************************************************************************
* Function Name  : runningEngHH
* Description    : наработка ХХ 30SEC
* Input          : нет
* Output         : nextState = STATE9;
* Return         : 0
*******************************************************************************/
static signed char runningEngHH(void)                       
{
    Tim10msUser_TypeDef *pTim = Tim10msUser+FIFTEENTH;
		Sep_typeDef *pSep = &Sep;
		AnSensors_TypeDef *pAnSensors = &pSep->Disel.Engine.AnSensors;
		Dev_ofStarterType *pSvechi = &pSep->Disel.Svechi;
		Dev_ofEngineType *pEng = &pSep->Disel.Engine;
		static uint32_t temp=0;
  
		temp += pAnSensors->oil_press;
		temp >>= 1;
   
    Tim10ms(EN,FIFTEENTH,_30SEC);
    
    if(pTim->FlagSetTimer){
        pTim->FlagSetTimer=0;
				if(temp>=OIL_PRESS_02_MPa)
				{
					status_pki_need_clear=1;																//флаг очистки ПКИ статусов
					setNextState();																					//nextState = STATE9;
					pSep->status.eng_start_work=1;												//статус - перевод на рабочие обороты
				}
				else
				{
					nextState = STATE18;
					pEng->stop_hh();																			//выключить ЭБУ
					pSep->status.eng_didnt_start_oil=1; 									//статус СЭП не запустился
					pSep->status.on_off_bu_eng_sep=0;
				}
				pSvechi->Off();
    }
    
    return 0;
}

/*******************************************************************************
* Function Name  : startEngWork
* Description    : наработка 30SEC рабочих оборотов
* Input          : нет
* Output         : nextState = STATE10;
* Return         : 0
*******************************************************************************/
static signed char startEngWork(void)
{
		Sep_typeDef *pSep = &Sep;
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
		Tim10msUser_TypeDef *pTim = Tim10msUser+FIFTEENTH;
	
		pEng->start_1800();
	
		Tim10ms(EN,FIFTEENTH,_30SEC);
    
    if(pTim->FlagSetTimer)
		{
			pTim->FlagSetTimer=0;
			pSep->status.eng_start_not_to_load=0;
			pSep->status.eng_wait_load=1;														//двигатель СЭП к нагрузке готов
			setNextState();																					//nextState = STATE10;		
		}
    
    return 0;
}

/*******************************************************************************
* Function Name  : preStopCheck
* Description    : предстоповая проверка можно ли остановить двигатель
* Input          : нет Работы ВУ;
* Output         : nextState = STATE12;
									 nextState = STATE13;
* Return         : 0
*******************************************************************************/
static signed char preStopCheck(void)
{
    Sep_typeDef *ptr = &Sep;
    Schusep_TypeDef *pSchusep = &ptr->SchuSep;
		static uint8_t cnt=0;

		if(!pSchusep->vu1)
		{
			nextState = STATE13;
		}
		else 
		{
			if(!GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_9 ))								// отключить двишатель СЭП
				GPIO_SetBits(GPIOD,GPIO_Pin_9);
				ptr->status.cmd_stop_eng_sep=1;
			if(cnt++>=_1SEC) 
			{
				GPIO_ResetBits(GPIOD,GPIO_Pin_9);
				setNextState();																								//nextState = STATE12;
			}
		}
		
		return 0;
}

/*******************************************************************************
* Function Name  : checkVU
* Description    : проверка работы ВУ
* Input          : Работа ВУ;
* Output         : nextState = STATE13;
									 состояние статуса - Авария отключения нагрузки
* Return         : 0
*******************************************************************************/
static signed char checkVU(void)
{
	Sep_typeDef *pSep = &Sep;
	Schusep_TypeDef *pSchusep = &Sep.SchuSep;
	if(pSchusep->vu1)
	{
		pSep->status.alarm_shutdown_load=1;											 	//статус - Авария отключения нагрузки
	}
	setNextState();																								//nextState = STATE13;
	
	return 0;
}

/*******************************************************************************
* Function Name  : runEngWSpeed_for30sec
* Description    : наработка на рабочих оборотах 30-ть sec, перед остановом
* Input          : Работа ВУ;
* Output         : nextState = STATE14;
* Return         : 0
*******************************************************************************/
static signed char runEngWSpeed_for30sec(void)
{
		Sep_typeDef *pSep = &Sep;
    Tim10msUser_TypeDef *pTim = Tim10msUser+FIFTEENTH;
		Schusep_TypeDef *pSchusep = &Sep.SchuSep;
		Dev_ofEngineType *pEng = &pSep->Disel.Engine;
	
		if(!pSchusep->vu1)
		{
			xTimerON();                                                                 //включаем индикатор таймера    
			
			Tim10ms(EN,FIFTEENTH,_30SEC);
			pSep->status.stop_eng_5min=1;
			if(pTim->FlagSetTimer){
					pTim->FlagSetTimer=0;
					pEng->stop_1800();
					setNextState();																													//nextState = STATE14;
			}			
		}
    
    return 0;
}

/*******************************************************************************
* Function Name  : runEngHH_for5min
* Description    : наработка на рабочих оборотах 5-ть, перед остановом
* Input          : 
* Output         : nextState = STATE15;
* Return         : 0
*******************************************************************************/
static signed char runEngHH_for5min(void)
{
    Tim1minUser_TypeDef *pTim = Tim1minUser+FIRST;

		Tim1min(EN,FIRST,_5MIN);
		if(pTim->FlagSetTimer){
				{
					pTim->FlagSetTimer=0;
					xTimerOFF();
					event = STEP1;
					setNextState();																													//nextState = STATE15;
				}
		}
    
    return 0;
}

/*******************************************************************************
* Function Name  : stopEngWork
* Description    : останов двигателя СЭП
* Input          : нет
* Output         : nextState = STATE16;
* Return         : 0
*******************************************************************************/
static signed char stopEngWork(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    
		pEng->stop_hh();
    
    setNextState();														    //nextState = STATE16;
    
    return 0;
}

/*******************************************************************************
* Function Name  : endStop
* Description    : 
* Input          : нет
* Output         : nextState = STATE17;
* Return         : 0
*******************************************************************************/
static signed char endStop(void)
{
	Sep_typeDef *pSep = &Sep;
	StopCtrl_TypeDef *pStop = &StopCtrl;
	pStop->EndStopping=1;
	
	pSep->status.stop_eng_5min=0;
	pSep->status.cmd_stop_eng_sep=0;
	pSep->status.on_off_bu_eng_sep=0;
	pSep->status.eng_start_work=0;
	
	setNextState();														    //nextState = STATE17;
	
	return 0;
}

/*******************************************************************************
* Function Name  : unpermitEng
* Description    : выключить ЭБУ
* Input          : состояние стартера
* Output         : nextState = STATE16;
* Return         : 0
*******************************************************************************/
static signed char unpermitEng(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Dev_ofStarterType *pStarter = &Sep.Disel.Starter;
    
    pEng->stop_hh();
    
    if(pStarter->io.isopen)pStarter->Off();
    
    nextState = STATE16;
    
    return 0;
}

/*******************************************************************************
* Function Name  : offSvechi
* Description    : выключить свечи
* Input          : нет
* Output         : nextState = STATE18;
* Return         : 0
*******************************************************************************/
static signed char offSvechi(void)
{
    Dev_ofStarterType *pSvechi = &Sep.Disel.Svechi;
    
    pSvechi->Off();
    
    nextState = STATE18;                              
    
    return 0;    
}

/*******************************************************************************
* Function Name  : offStarter
* Description    : выключить свечи
* Input          : нет
* Output         : nextState = STATE19;
* Return         : 0
*******************************************************************************/
static signed char offStarter(void)
{
	Dev_ofStarterType *pStarter = &Sep.Disel.Starter;
	
	pStarter->Off();                                    //перестраховка - выключить стартер
	
	nextState = STATE19;
	
	return 0;
}

void ProtectSwitchToManual(void)
{
	Dev_ofStarterType *pStarter = &Sep.Disel.Starter;
  Dev_ofStarterType *pSvechi = &Sep.Disel.Svechi;	
	Dev_ofEngineType *pEng = &Sep.Disel.Engine;	
	
	pStarter->Off();
    
  pSvechi->Off();	
    
  pEng->stop_hh();
	
	pEng->stop_1800();
	
	nextState = STATE15;	
}

static Transition transitionTableStart[18]={
    {STATE1,STEP1,preStartCheck},
    {STATE2,STEP1,enPodogrev},
		{STATE3,STEP1,opValves},
    {STATE3,STEP2,checkValves},
    {STATE4,STEP1,permitEng},
    {STATE5,STEP1,onSvechi},
    {STATE6,STEP1,startStarter},
    {STATE7,STEP1,startEng},
    {STATE8,STEP1,runningEngHH},
    {STATE9,STEP1,startEngWork},
    {STATE11,STEP1,preStopCheck},
    {STATE12,STEP1,checkVU},
    {STATE13,STEP1,runEngHH_for5min},
    {STATE14,STEP1,stopEngWork},
		{STATE15,STEP1,endStop},
		{STATE17,STEP1,unpermitEng},
		{STATE18,STEP1,offSvechi},
		{STATE19,STEP1,offStarter}
 };

 signed char FMSFunc_Disel(int elementsCount, Transition *transitionTable, State currentState, Event eventToFire)
{
		switch((uint8_t)currentState)
		{
			case STATE1:
				return preStartCheck();
			case STATE2:
				return enPodogrev();
			case STATE3:
				setNextState();
			break;
			case STATE4:
				return permitEng();
			case STATE5:
				return onSvechi();
			case STATE6:
				return startStarter();
			case STATE7:
				return startEng();
			case STATE8:
				return runningEngHH();
			case STATE9:
				return startEngWork();	
			case STATE11:
				return preStopCheck();		
			case STATE12:
				return checkVU();		
			case STATE13:
				return runEngWSpeed_for30sec();				
			case STATE14:
				return runEngHH_for5min();	
			case STATE15:
				return stopEngWork();			
			case STATE16:
				return endStop();				
			case STATE18:
				return unpermitEng();			
			case STATE19:
				return offSvechi();
			case STATE20:
				return offStarter();	
		}
    return 0;    
}

void AutoStart(uint8_t en)
{   
    waitStart(en);
    
    int elementsCount = sizeof(transitionTableStart) / sizeof(transitionTableStart[0]);
    FMSFunc_Disel(elementsCount,transitionTableStart,nextState,event);
    
    globalState = nextState;
}

void AutoStop(uint8_t en)
{       
    waitStop(en);

    int elementsCount = sizeof(transitionTableStart) / sizeof(transitionTableStart[0]);
    FMSFunc_Disel(elementsCount,transitionTableStart,nextState,event);
    
    globalState = nextState;
}

void DiselStatusController(void)
{
		HowDoSepFeel();
	
    ValveController();
    
    StarterController();                                                            //смотрит за работой стартера
    
    SvechiController();                                                             //смотрит за работой свечей
    
    VentController();                                                               //смотрит за работой вентилятора
    
    PodogrevController();                                                           //смотрит за работой подогрева
    
    EngineController();                                                             //смотрит за работой двигателя
    
}
