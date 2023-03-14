#include "stm32f2xx.h"
#include "timer.h"

static volatile union Tim20ms_un Tim20MSecond_un;
static volatile union Tim10ms_un Tim10MSecond_un;
static volatile union Tim1min_un Tim1Minut_un;
Tim10msUser_TypeDef Tim10msUser[16];

static volatile unsigned int tim6_cnt=0;
Tim20msUser_TypeDef Tim20msUser[24];
static volatile unsigned int tim7_cnt=0;
Tim1minUser_TypeDef Tim1minUser[16];

void TIM6_DAC_IRQHandler(void)
{
//    volatile struct Tim_ms *p_sec = &Tim20MSecond_un.Tim20msFlags;
    //volatile struct Tim10ms_st *p_msec = &Tim10MSecond_un.Tim10msFlags;
    if(TIM6->SR & TIM_SR_UIF){
        Tim10MSecond_un.build=(unsigned int)0xFFFF;            //10-ти миллисекундный таймер
        tim6_cnt++;
    }
	if((tim6_cnt%2)==0) Tim20MSecond_un.build=(unsigned int)0xFFFFFFFF;
    TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}

void TIM7_IRQHandler(void)
{
//    volatile struct Tim_ms *p_min = &Tim1Minut_un.Tim1minFlags;
    
    if(TIM7->SR & TIM_SR_UIF){tim7_cnt++;}
    if((tim7_cnt%60)==0) Tim1Minut_un.build=(unsigned int)0xFFFF;
    TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
}

/*
формула: 2*Fapb1/(TIM_Period * TIM_Prescaler) = Hz (герцы)
т.е. для данного случая: 2*30*10^6/(600*(1000-1))=100Hz т.е T=1/100=0.01 сек=10мс
Fapb1 - частота шины APB1 (куда подключен таймер) = 30MHz
2 - коэффициент см. мануал cd00237391 стр.20
*/
void TIM6_init(void)
{
    TIM_TimeBaseInitTypeDef tim6;
    
    RCC_APB1PeriphResetCmd(RCC_APB1RSTR_TIM6RST,ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1RSTR_TIM6RST,DISABLE);
    
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6EN,ENABLE);
    
    TIM_TimeBaseStructInit(&tim6);                          //10мс
    tim6.TIM_Period = 600;
    tim6.TIM_Prescaler = 1000-1;
    TIM_TimeBaseInit(TIM6,&tim6);
    
		TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM6,ENABLE);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void TIM7_init(void)
{
    TIM_TimeBaseInitTypeDef tim7;
    
    RCC_APB1PeriphResetCmd(RCC_APB1RSTR_TIM7RST,ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1RSTR_TIM7RST,DISABLE);
    
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7EN,ENABLE);
    
    TIM_TimeBaseStructInit(&tim7);                          //1сек
    tim7.TIM_Period = 6000;
    tim7.TIM_Prescaler = 10000-1;
    TIM_TimeBaseInit(TIM7,&tim7);
    
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM7,ENABLE);
    NVIC_EnableIRQ(TIM7_IRQn);
}

void Tim20ms(unsigned char EN,unsigned char num, unsigned int setting)
{
    Tim20msUser_TypeDef *p_sec = Tim20msUser+num;
    if(!EN) return;
    if(Tim20MSecond_un.build & (1<<num)){
			Tim20MSecond_un.build &= ~(1<<num); 
			p_sec->InsideCntTimer++;
		}
    if(p_sec->InsideCntTimer>=setting) {
        p_sec->FlagSetTimer=1;
        p_sec->InsideCntTimer=0;
        Tim20MSecond_un.build &= ~(1<<num);
    }
}

void Tim10ms(unsigned char EN,unsigned char num, unsigned int setting)
{
    Tim10msUser_TypeDef *p_msec = Tim10msUser+num;
    if(!EN) return;
    if(Tim10MSecond_un.build & (1<<num))
		{
        Tim10MSecond_un.build &= ~(1<<num); 
        p_msec->InsideCntTimer++;
    }
    if(p_msec->InsideCntTimer>=setting) 
		{
        p_msec->FlagSetTimer=1;
        p_msec->InsideCntTimer=0;
        Tim10MSecond_un.build &= ~(1<<num);
    }
}

void Tim1min(unsigned char EN, unsigned char num, unsigned char setting)
{
    Tim1minUser_TypeDef *p_min = Tim1minUser+num;
    if(!EN) return;
    if(Tim1Minut_un.build & (1<<num)){
        Tim1Minut_un.build &= ~(1<<num); 
        p_min->InsideCntTimer++;
    }
    if(p_min->InsideCntTimer>=setting) {
        p_min->FlagSetTimer=1;
        p_min->InsideCntTimer=0;
        Tim1Minut_un.build &= ~(1<<num);
    }    
}
