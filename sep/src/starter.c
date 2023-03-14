/**
  ******************************************************************************
  * @file    starter.c
  * @author  Murzina Lily
  * @version V1.0.0
  * @date    07.04.2020
  * @brief   It is startet algorithm
  ******************************************************************************
  */

#include "schusep.h"
#include "starter.h"
#include "timer.h"
#include "ebu.h"

Port_TypeDef StarterPort = {.Port = GPIOE,.Pin =GPIO_Pin_15};
Port_TypeDef StarterLock = {.Port = GPIOD,.Pin = GPIO_Pin_10};

void StarterON(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
    ptr->io.close=IORESET;
    ptr->io.open=IOSET;
    GPIO_SetBits(StarterPort.Port,StarterPort.Pin);
}

void StarterOFF(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
    ptr->io.open=IORESET;
    ptr->io.close=IOSET;
    GPIO_ResetBits(StarterPort.Port,StarterPort.Pin);
}

unsigned char StarterKeyONState(void)
{
    return GPIO_ReadOutputDataBit(StarterPort.Port,StarterPort.Pin);
}

static unsigned char StarterErrOn(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
    Tim10ms(ptr->io.open,FIFTH,_300MS);                                //уставка таймера 300 мсек
    
    if(Tim10msUser[FIFTH].FlagSetTimer & (!ptr->io.isopen)){
        Tim10msUser[FIFTH].FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char StarterErrOff(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
    Tim10ms(ptr->io.close,SIXTH,_300MS);                                //уставка таймера 300 мсек
    
    if(Tim10msUser[SIXTH].FlagSetTimer & ptr->io.isopen){
        Tim10msUser[SIXTH].FlagSetTimer=0;
        return 1;
    }
    return 0;
}

static unsigned char StarterStatus(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
    unsigned char *old_state = (unsigned char *)&ptr->io;
    
    if(StarterErrOn()) return ERROROPEN1;                                   //ошибка включения стартера
    
    if(StarterErrOff()) return ERRORCLOSE1;                                 //ошибка выключения стартера
    
    if(ptr->io.isopen) {return ISOPENVALVE;}                          //стартер включен
    
    return (*old_state);
}

/*******************************************************************************
* Function Name  : LockerUnLocker
* Description    : алгоритм запуска  диз. двигателя по стартеру; для того чтобы 
									функция заработала ее нужно завести с помощью 
									флага таймера - Tim20msUser[FOURTEENTH].FlagSetTimer
* Input          : нет
* Output         : 1 - блокировка; 0 - разблокировка
* Return         : нет
*******************************************************************************/
signed char AlgStarter(void)
{
    Dev_ofEngineType *pEng = &Sep.Disel.Engine;
    Dev_ofStarterType *pStr = &Sep.Disel.Starter;
    
    if(Tim20msUser[FOURTEENTH].FlagSetTimer &&                        //если таймер еще не сработал
        (pStr->cnt_start<QUANTITY_TRY) &&                             //количество попыток запуска не превышает предела
        (!pStr->KeyState())){                                        //и ключ стартера ON не включен
            pStr->On();                                               //включаем стартер
            Tim20msUser[FOURTEENTH].FlagSetTimer=0;    
        }
        
    Tim20ms(pStr->io.open,THIRTEENTH,_10SEC);                            //таймер 10 сек. таймер - на удержания стартера
    
    if((Tim20msUser[THIRTEENTH].FlagSetTimer) &&                        //если 10-ти секундный таймер сработал а обороты не появились
        (SpeedIsNot())){                                                //выключаем стартер  || (!(SpeedIs400()))
            pStr->Off();
            pStr->cnt_start++;                                          //увеличиваем счетчик включений стартера
            Tim20msUser[THIRTEENTH].FlagSetTimer=0;                     //обнуляем таймер
        }
        
    Tim20ms(pStr->io.close,FOURTEENTH,_5SEC);                              //таймер 5 сек.
    
    if(SpeedIs400()){
        pStr->Off();
        Tim20msUser[THIRTEENTH].FlagSetTimer=0;
        Tim20msUser[FOURTEENTH].FlagSetTimer=0;
        pStr->cnt_start=0;
        pEng->io1.open=IOSET;                                           //начинаем отсчет (30 сек. для набора холостых оборотов)
        return 1;                                                       //двигатель запустился по стартеру
    }
    
    if((pStr->cnt_start>=QUANTITY_TRY) 
        //&& (SpeedIsNot())
    ){
            pStr->Off();
            Tim20msUser[THIRTEENTH].FlagSetTimer=0;
            Tim20msUser[FOURTEENTH].FlagSetTimer=0;      
            pStr->cnt_start=0;
            return -1;                                                   //авария запуска двигателя по стартеру
        }
    
    return 0;                                                           //двигатель еще не запустился
}

/*******************************************************************************
* Function Name  : LockerUnLocker
* Description    : разрешает блокировку
* Input          : SEP_not_norm - формируется в HowDoSepFeel
									 pDis->io.isopen - работа диз. СЭП
									 pV1->io.isopen
									 pV2->io.isopen
* Output         : 1 - блокировка; 0 - разблокировка
* Return         : uint8_t
*******************************************************************************/
uint8_t LockerUnLocker(void)
{
	Sep_typeDef *pSep = &Sep;
	Schusep_TypeDef *pSchusep = &pSep->SchuSep;
  Disel_TypeDef *pDis = &pSep->Disel;
//  Dev_ofValveType *pV1 = pSep->Valve+FIRST;
//  Dev_ofValveType *pV2 = pSep->Valve+SECOND;
	Dev_ofPodogrevType *pHot = &pDis->Podogrev;
	
	return ((pSchusep->SEP_not_norm | pDis->io.isopen) || (pHot->temp<=0));
}

/*******************************************************************************
* Function Name  : xStarterLock
* Description    : блокирует стартер
* Input          : нет
* Output         : нет
* Return         : нет
*******************************************************************************/
static void xStarterLock(void)
{
	if(LockerUnLocker())
	{
		if(!GPIO_ReadOutputDataBit(StarterLock.Port,StarterLock.Pin))
			GPIO_SetBits(StarterLock.Port,StarterLock.Pin);
	}
}

/*******************************************************************************
* Function Name  : xStarterUnlock
* Description    : разблокирует стартер
* Input          : нет
* Output         : нет
* Return         : нет
*******************************************************************************/
static void xStarterUnlock(void)
{
	if(!LockerUnLocker())
	{
		GPIO_ResetBits(StarterLock.Port,StarterLock.Pin);
	}
}

void xStarterLocker(void)
{
    xStarterLock();
    xStarterUnlock();    
}

void StarterController(void)
{
    Dev_ofStarterType *ptr = &Sep.Disel.Starter;
 
    ptr->status = StarterStatus();
}
