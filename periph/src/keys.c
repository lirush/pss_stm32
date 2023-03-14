#include "keys.h"
#include "can.h"
#include "schusep.h"
#include "valve.h"
#include "timer.h"
#include "trigger.h"

void Write_to_key (void)
{
	if (( hashArr[0].data[1] >> Out15 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_10 );
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_10 );
	if (( hashArr[0].data[1] >> Out16 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_9 );
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_9 );
	if (( hashArr[0].data[2] >> Out17 ) & 1 ) GPIO_SetBits(GPIOF,GPIO_Pin_2);
	else	GPIO_ResetBits(GPIOF,GPIO_Pin_2);
	if (( hashArr[0].data[2] >> Out18 ) & 1 ) GPIO_SetBits(GPIOF,GPIO_Pin_3 );
	else	GPIO_ResetBits(GPIOF,GPIO_Pin_3 );
	if (( hashArr[0].data[2] >> Out19 ) & 1 ) GPIO_SetBits(GPIOF,GPIO_Pin_4);
	else	GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	if (( hashArr[0].data[2] >> Out20 ) & 1 ) GPIO_SetBits(GPIOF,GPIO_Pin_5);
	else	GPIO_ResetBits(GPIOF,GPIO_Pin_5);
}

void Write_to_keys_by_if(void)
{
    TRG_TypeDef *ptr_trg_vo1 = TRGUser+0;
    TRG_TypeDef *ptr_trg_vc1 = TRGUser+1;
//    TRG_TypeDef *ptr_trg_vo2 = TRGUser+2;
//    TRG_TypeDef *ptr_trg_vc2 = TRGUser+3;
    
	if (( hashArr[0].data[0] >> Out1 ) & 1 ) GPIO_SetBits(GPIOG,GPIO_Pin_2);                    
	else GPIO_ResetBits(GPIOG,GPIO_Pin_2 );    
  if (( hashArr[0].data[0] >> Out2 ) & 1 ) GPIO_SetBits(GPIOD,GPIO_Pin_15 );
	else GPIO_ResetBits(GPIOD,GPIO_Pin_15);
    
	if (( hashArr[0].data[0] >> Out3 ) & 1 ) ptr_trg_vo1->set=1;                       //управление заслонками через триггер 0-ой 
    else {ptr_trg_vo1->reset=ptr_trg_vo1->set=0;}
    if(Sep.Valve[FIRST].io.open){//if(Valve1OpenByTRG()) {
				Sep.Valve[FIRST].io.isclose=0;
        GPIO_SetBits(GPIOD,GPIO_Pin_14);                                                    //ключ открыть заслонки
        //ptr_trg_vo1->set=ptr_trg_vo1->reset=0;                                                      //сбрасываем триггер управления заслонкой
    }
    else GPIO_ResetBits(GPIOD,GPIO_Pin_14);                                                 //снять U с ключа - открыть заслонки
	
    if (( hashArr[0].data[0] >> Out4 ) & 1 )                                    //закрыть
		{                                     										//управление заслонкой 1,2 через триггер 1-ый         (( hashArr[0].data[0] >> Out5 ) & 1 ))         ptr_trg_vc2->set=1;        ptr_trg_vc2->set=ptr_trg_vc2->reset=0;
        ptr_trg_vc1->set=1;
    }else {
       ptr_trg_vc1->set=ptr_trg_vc1->reset=0;
    }
    if(Sep.Valve[FIRST].io.close){//if(Valve1CloseByTRG()){
        GPIO_SetBits(GPIOD,GPIO_Pin_13);
//        GPIO_SetBits(GPIOD,GPIO_Pin_12);
        //ptr_trg_vc1->set=0;
        //ptr_trg_vc2->set=0;
    }
	else {
        GPIO_ResetBits(GPIOD,GPIO_Pin_13);
//        GPIO_ResetBits(GPIOD,GPIO_Pin_12);
    }
      
//	if (( hashArr[0].data[0] >> Out6 ) & 1 ) ptr_trg_vo2->set=1;                        //открыть управление правой заслонкой через триггер 2-ой 
//    else {ptr_trg_vo2->reset=ptr_trg_vo2->set=0;}
//	if(Sep.Valve[SECOND].io.open){//if(Valve2OpenByTRG()){
//        GPIO_SetBits(GPIOD,GPIO_Pin_11);                                                        //ключ открыть заслонку
//        //ptr_trg_vo2->set=ptr_trg_vo2->reset=0;                                                    //сбрасываем триггер управления заслонкой
//    } else GPIO_ResetBits(GPIOD,GPIO_Pin_11);                                                   //снять U с ключа - открыть заслонку    
    
	if (( hashArr[0].data[0] >> Out7 ) & 1 ) GPIO_SetBits(GPIOD,GPIO_Pin_10);
	else GPIO_ResetBits(GPIOD,GPIO_Pin_10 );   
	if (( hashArr[0].data[0] >> Out8 ) & 1 ) GPIO_SetBits(GPIOD,GPIO_Pin_9 );
	else GPIO_ResetBits(GPIOD,GPIO_Pin_9 ); 
    if (( hashArr[0].data[1] >> Out9 ) & 1 ) GPIO_SetBits(GPIOD,GPIO_Pin_8);
	else GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	if (( hashArr[0].data[1] >> Out10 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_15);
	else GPIO_ResetBits(GPIOE,GPIO_Pin_15 );
	if (( hashArr[0].data[1] >> Out11 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_14);
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_14 );
	if (( hashArr[0].data[1] >> Out12 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_13);
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_13 );
	if (( hashArr[0].data[1] >> Out13 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_12);
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_12);
	if (( hashArr[0].data[1] >> Out14 ) & 1 ) GPIO_SetBits(GPIOE,GPIO_Pin_11);
	else	GPIO_ResetBits(GPIOE,GPIO_Pin_11 );
	if (( hashArr[0].data[2] >> Out21 ) & 1 ) GPIO_SetBits(GPIOF,GPIO_Pin_6);
	else	GPIO_ResetBits(GPIOF,GPIO_Pin_6);    
    
}
