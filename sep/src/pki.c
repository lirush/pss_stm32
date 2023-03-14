#include "schusep.h"
#include "pki.h"
#include "timer.h"

enum {PHASE_A,PHASE_B,PHASE_C,FREQ_P} en_ListParamPki;
enum {FLVU1=0,FLBGDISEP,FLBGDIS,FLEPW} filter_on_pki_alg;

uint8_t uiPkiControlPhase(uint8_t phase, uint8_t num_tim)
{
	uint32_t start_tims=0;
	Tim20msUser_TypeDef *ptr_tim = Tim20msUser+num_tim;
	
	if((phase<=LO_LIMIT_PHASE) || (phase>=UP_LIMIT_PHASE))
		start_tims |= (1<<num_tim);
	else {start_tims &= ~(1<<num_tim);}
	
	Tim20ms(((start_tims & (1<<num_tim))?1:0),num_tim,_3SEC);
	
	if(!(start_tims & (1<<num_tim)))ptr_tim->InsideCntTimer=0;
	
	return ptr_tim->FlagSetTimer;
}

uint8_t uiPkiControlFreq(uint16_t freq, uint8_t num_tim)
{
	uint32_t start_tims=0;
	Tim20msUser_TypeDef *ptr_tim = Tim20msUser+num_tim;
	
	if((freq<=LO_LIMIT_FREQ) || (freq>=UP_LIMIT_FREQ))
		start_tims |= (1<<num_tim);
	else {start_tims &= ~(1<<num_tim);}
	
	Tim20ms(((start_tims & (1<<num_tim))?1:0),num_tim,_3SEC);
	
	if(!(start_tims & (1<<num_tim)))ptr_tim->InsideCntTimer=0;
	
	return ptr_tim->FlagSetTimer;
}

void vImplPhasePki(uint8_t num_pki, uint8_t phase, uint8_t num_tim, uint8_t num_pa, uint8_t (*func)(uint8_t, uint8_t))
{
	Tim20msUser_TypeDef *ptr_tim = Tim20msUser+num_tim;
	Gens_TypeDef *pGens = &Sep.Gens;
	if((*func)(phase,num_tim))
	{
		ptr_tim->FlagSetTimer=0;
		pGens->status |= (1<<(num_pki*4+num_pa));
		pGens->cmd |= (1<<(num_pki*4+num_pa));
	}
}

void vImplFreqPki(uint8_t num_pki, uint16_t freq, uint8_t num_tim, uint8_t num_pa, uint8_t (*func)(uint16_t, uint8_t))
{
	Tim20msUser_TypeDef *ptr_tim = Tim20msUser+num_tim;
	Gens_TypeDef *pGens = &Sep.Gens;
	if((*func)(freq,num_tim))
	{
		ptr_tim->FlagSetTimer=0;
		pGens->status |= (1<<(num_pki*4+num_pa));
		pGens->cmd |= (1<<(num_pki*4+num_pa));
	}
}

void vCheckParametersPki(uint8_t dev1,uint8_t dev2)
{
	Sep_typeDef *pSep = &Sep;
	Gens_TypeDef *pGens = &pSep->Gens;
	
	vImplPhasePki(dev1,pGens->Pki[dev1].phase_a,SIXTEENTH,PHASE_A,uiPkiControlPhase);
	vImplPhasePki(dev1,pGens->Pki[dev1].phase_b,SEVENTEENTH,PHASE_B,uiPkiControlPhase);
	vImplPhasePki(dev1,pGens->Pki[dev1].phase_c,EIGHTEENTH,PHASE_C,uiPkiControlPhase);
	vImplFreqPki(dev1,pGens->Pki[dev1].freq,NINETEENTH,FREQ_P,uiPkiControlFreq);
	
	vImplPhasePki(dev2,pGens->Pki[dev2].phase_a,TWENTIETH,PHASE_A,uiPkiControlPhase);
	vImplPhasePki(dev2,pGens->Pki[dev2].phase_b,TWENTYFIRST,PHASE_B,uiPkiControlPhase);
	vImplPhasePki(dev2,pGens->Pki[dev2].phase_c,TWENTYSECOND,PHASE_C,uiPkiControlPhase);
	vImplFreqPki(dev2,pGens->Pki[dev2].freq,TWENTYTHIRD,FREQ_P,uiPkiControlFreq);		
}

/*******************************************************************************
* Function Name  : vParametersPkiController
* Description    : Алгорим контроляпараметров ПКИ
* Input          : параметры ПКИ (фазы A,B,C и частоты)
* Output         : pGens->status
									 pGens->cmd - отключает СЭП
* Return         : нет
*******************************************************************************/

void vParametersPkiController(void)
{
	
	Sep_typeDef *pSep = &Sep;
	Gens_TypeDef *pGens = &pSep->Gens;

	static uint8_t cnt_filter_on_alg[4]={0};
	static uint8_t flag_filter_on_alg[4]={0};		
	

	if(pSep->SchuSep.SB4_on_BG_dis_sep)flag_filter_on_alg[FLBGDISEP]=1;//pGens->status |= (1<<29);									//Включение БГ-ДИЗ. СЭП
	if(pSep->SchuSep.SB3_on_BG_dis)flag_filter_on_alg[FLBGDIS]=1;//pGens->status |= (1<<30);											//Включение БГ-ДИЗ
//	if(pSep->SchuSep.SB2_on_extpower)flag_filter_on_alg[FLEPW]=1;//pGens->status |= (1<<31);										//включение ВИП
	
	if(flag_filter_on_alg[FLBGDISEP] && (cnt_filter_on_alg[FLBGDISEP]++>=_10SEC))
	{
		pGens->status &= ~((1<<30) | (1<<31));
		pGens->status |= (1<<29);
		cnt_filter_on_alg[FLBGDISEP]=0;	
		flag_filter_on_alg[FLBGDISEP]	=0;
	}	
	
	if(flag_filter_on_alg[FLBGDIS] && (cnt_filter_on_alg[FLBGDIS]++>=_10SEC))
	{
		pGens->status &= ~((1<<29) | (1<<31));
		pGens->status |= (1<<30);
		cnt_filter_on_alg[FLBGDIS]=0;	
		flag_filter_on_alg[FLBGDIS]	=0;
	}		

//	if(flag_filter_on_alg[FLEPW] && (cnt_filter_on_alg[FLEPW]++>=_10SEC))
//	{
//		pGens->status &= ~((1<<29) | (1<<30));
//		pGens->status |= (1<<31);
//		cnt_filter_on_alg[FLEPW]=0;	
//		flag_filter_on_alg[FLEPW]	=0;
//	}			
	
	if(status_pki_need_clear)																											//флаг очистки ПКИ статусов
	{
		status_pki_need_clear=0;
		pGens->status &= 0xF0000000;
	}
	
//	if((pGens->status & (1<<28)))																									//Работа ВУ1
//	{
//		vCheckParametersPki(PKI1n-2,PKI2n-2);
//	}
	
	if((pGens->status & (1<<29)))																									//Включение БГ-ДИЗ. СЭП
	{
		vCheckParametersPki(PKI1n-2,PKI6n-2);
	}	
	
	if((pGens->status & (1<<30)))																									//Включение БГ-ДИЗ
	{
		vCheckParametersPki(PKI2n-2,PKI3n-2);
	}		
	
//	if((pGens->status & (1<<31)))																									//включение ВИП
//	{
//		vCheckParametersPki(PKI4n-2,PKI5n-2);
//	}
	
}
