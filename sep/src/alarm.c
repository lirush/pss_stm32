#include "alarm.h"
#include "schusep.h"
#include "timer.h"
#include "pki.h"

void VsOpenClearFlagsTims(void)
{
    Tim10msUser[FIRST].InsideCntTimer=0;
    Tim10msUser[FIRST].FlagSetTimer=0;                                          //освобождаем таймер - флаг таймера ошибки открытия заслонок
    Tim10msUser[THIRD].InsideCntTimer=0;
    Tim10msUser[THIRD].FlagSetTimer=0;                                          //освобождаем таймер - флаг таймера ошибки открытия правой заслонки
}

void VsCloseClearFlagsTims(void)
{
    Tim10msUser[FOURTH].InsideCntTimer=0;
    Tim10msUser[FOURTH].FlagSetTimer=0;                                         //освобождаем таймер - флаг таймера ошибки закрытия правой заслонки
    Tim10msUser[SECOND].InsideCntTimer=0;
    Tim10msUser[SECOND].FlagSetTimer=0;                                         //освобождаем таймер - флаг таймера ошибки закрытия заслонок
}

void EngineClearFlagsTims(void)
{
    Tim20msUser[NINTH].InsideCntTimer=0;
    Tim20msUser[NINTH].FlagSetTimer=0;                                          //ошибка запуска ХХ
    Tim20msUser[ELEVENTH].InsideCntTimer=0;
    Tim20msUser[ELEVENTH].FlagSetTimer=0;                                       //ошибка запуска рабочих оборотов
}

void vSepAlarmDisable(void)
{
//	Dev_ofEngineType *pEng = &Sep.Disel.Engine;
//	StopCtrl_TypeDef *pStop = &StopCtrl;
	Gens_TypeDef *pGens = &Sep.Gens;
	Tim20msUser_TypeDef *ptr_tim = Tim20msUser+TWENTYFOUR;
	uint32_t mask=0x00FFFFFF;
	uint32_t diconnect_permit=0;
	
	vParametersPkiController();																										//контроль параметров ПКИ
	
	if((pGens->status & (1<<29)) | (pGens->status & (1<<30)) | (pGens->status & (1<<31)))
		diconnect_permit =(pGens->cmd & mask);														//отключение разрешено
	
	if(diconnect_permit)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_9);
		pGens->cmd = 0x00000000;
		pGens->status &= ~((1<<29) | (1<<30) | (1<<31));
	}											

	Tim20ms(((GPIOD->ODR & GPIO_Pin_9)?1:0),TWENTYFOUR,_2SEC);
	if(ptr_tim->FlagSetTimer)
	{
		ptr_tim->FlagSetTimer=0;
		GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	}
	
}
