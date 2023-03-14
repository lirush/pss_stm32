#include "schusep.h"
#include "sdata.h"
#include "can.h"
#include "disel.h"
#include "timer.h"
#include "podogrev.h"

void UpdateDataIn(void)
{
    Sep_typeDef *pSep = &Sep;
    Disel_TypeDef *pDis = &Sep.Disel;
    Dev_ofEngineType *pEng = &pDis->Engine; 
    Schusep_TypeDef *pSchusep = &pSep->SchuSep;
//    Dev_ofValveType *pV1 = pSep->Valve+FIRST;
//    Dev_ofValveType *pV2 = pSep->Valve+SECOND;
    Dev_ofStarterType *pStarter = &pSep->Disel.Starter;
    Dev_ofStarterType *pSvechi = &pSep->Disel.Svechi;
    Dev_ofStarterType *pVent = &pDis->Ventilator;
    DSensors_TypeDef *pSensors = &pDis->Engine.DSensors;
		AnSensors_TypeDef *pAnSensors = &pDis->Engine.AnSensors;
    Dev_ofPodogrevType *pHot = &pDis->Podogrev;
		Pki_TypeDef *pPki = pSep->Gens.Pki+0;
    
//    pV1->io.isopen = !GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0);                              //З1 открыта
//    pV1->io.isclose = !GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1);                             //З1 закрыта
//    
//    pV2->io.isopen = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14);                             //З2 открыта
//    pV2->io.isclose = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);                            //З2 закрыта
    
    pSep->io.in0 = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13);                               //авария СЭП
    pSep->io.isopen = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6);                             //работа СЭП
    
    pSchusep->SB11_valves_open = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6);                  //SB11 открыть заслонки
    pSchusep->Ind_podogrev = pHot->io.isopen = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);    //индикация подогрев
    pSchusep->SB1_off_sep = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1);                       //откл сэп
    pSchusep->SB2_on_extpower = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0);                   //включить вип
    pSchusep->SB3_on_BG_dis = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15);                    //включить БГ диз.
    pSchusep->SB4_on_BG_dis_sep = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14);                //включить БГ диз. СЭП
    pSchusep->SA1_auto_dis_sep = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11);                 //авт.диз.сэп
    pSchusep->SB6_auto_start_sep = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_10);               //авт.старт СЭП
    pSchusep->ppo = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_9);                               //ППО
    pSchusep->vu1 = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7);                               //работа ВУ1
    pSchusep->SA5_stop = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);                          //цепь стоп
    pSchusep->SA5_hh = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2);                            //цепь ХХ
    pSchusep->SA5_1800 = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1);                          //цепь 1800
    pSchusep->SA8_stop_podogrev = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12);                //выкл. подогрев
    pSchusep->Ind_svechi = pSvechi->io.in0 = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7);      //индикация свечи
		pSchusep->START_eng_PU = (hashArr[PMV].data[3] & 0x01)?1:0;																	//запустить двигатель СЭП от ПУ
		pSchusep->Alarm_STOP_eng_PU = (hashArr[PMV].data[3] & 0x02)?1:0;												//аварийный останов и блокировка СЭП от ПУ
    
    pSensors->oil_press_dis = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);                    //давление масла диз.
    pSensors->oil_press_rdk = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10);                    //давление масла РДК
    pSensors->B105_peregrev_ohlag = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);               //перегрев ОЖ
    pSensors->B106_oil_level_poddon = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);             //уровень масла в поддоне
    pSensors->B109_filter_zasoren = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);               //фильтр засорен
    pSensors->B107_oil_level_ohlag = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);              //уровень ОЖ в блоке радиатора
    pSensors->rdk_sep = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_8);
		pAnSensors->oil_press = (hashArr[BRI2PN2AN2].data[0] | (hashArr[BRI2PN2AN2].data[1]<<8));					//датчик давления масла двигателя СЭП
    
    pVent->io.isopen = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0);                            //работа вент
    
    pStarter->io.isopen = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5);                         //состояние стартера
    pStarter->io.in0 = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4);                            //состояние ключа XP14
    
    pSvechi->io.isopen = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4);                          //состояние свечи
    
    //pHot->io.isopen = !GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2);                           //состояние ключа XP15, см. схему на ЩУСЭП
    
    pDis->io.isopen = !GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5);                             //работа диз. СЭП
    
    pEng->Ebu.eng_speed = (hashArr[EBU].data[3] | (hashArr[EBU].data[4]<<8));
    pEng->Ebu.io1.in0 = !GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_6);                           //авария ЭБУ
    
    pHot->temp = (hashArr[BRI2PN2AN2].data[2] | (hashArr[BRI2PN2AN2].data[3]<<8));
		
		for(int dev=2;dev<=7;dev++)
		{
			pPki->phase_a = hashArr[dev].data[0];
			pPki->phase_b = hashArr[dev].data[1];
			pPki->phase_c = hashArr[dev].data[2];
			pPki->freq = hashArr[dev].data[3] | hashArr[dev].data[4]<<8;
			if(dev<7)pPki++;			
		}

}

void UpdateDataOut(void)
{
    Sep_typeDef *pSep = &Sep;
    Disel_TypeDef *pDis = &Sep.Disel;
    Dev_ofValveType *pV1=pSep->Valve+FIRST;
//		uint32_t pki_status = pSep->Gens.status;
//		Schusep_TypeDef *pSchusep = &pSep->SchuSep;
    
    for(int i=0;i<8;i++){
        pr_data.SepTx_Message[0].Data[i] = 0x00;
        pr_data.SepTx_Message[1].Data[i] = 0x00;
    }
    
    pr_data.SepTx_Message[0].Data[0] = stateSep;
    
    pr_data.SepTx_Message[0].Data[1] = pV1->status;
    
    pr_data.SepTx_Message[0].Data[2] = pDis->Starter.status;
    
    pr_data.SepTx_Message[0].Data[3] = pDis->Svechi.status;
    
    pr_data.SepTx_Message[0].Data[4] = pDis->Ventilator.status;
    
    pr_data.SepTx_Message[0].Data[5] = pDis->Podogrev.status;
    
    pr_data.SepTx_Message[0].Data[6] = pDis->Engine.status;
    
    pr_data.SepTx_Message[0].Data[7] = globalState;
		
		pr_data.SepTx_Message[1].Data[0] = (GPIO_ReadOutputDataBit(GPIOG,GPIO_Pin_2)<<0)			//Вкл./Выкл. БУ двигателя СЭП //(pSep->status.on_off_bu_eng_sep<<0) 
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_15)<<1)		//Перевод двигателя СЭП на рабочие обороты  (pSep->status.eng_start_work<<1) 
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_14)<<2)		//Заслон. двигателя СЭП открыть (pSep->status.cmd_valves_open<<2)
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_13)<<3)		//Заслон. двигателя СЭП закрыть //(pSep->status.cmd_stop_eng_sep<<7);
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_12)<<4)		//Заслон. прав. борта двигателя СЭП открыть 
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_11)<<5)		//Заслон. прав. борта двигателя СЭП закрыть 
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_10)<<6)		//Разрешение на запуск дизеля СЭП
																			| (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_9)<<7);		//Отключить двигатель СЭП 
																			
		pr_data.SepTx_Message[1].Data[1] = (GPIO_ReadOutputDataBit(GPIOD,GPIO_Pin_8)<<0)			//Включить свечи двигателя СЭП //(pSep->status.eng_start_auto<<1);
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_15)<<1)		//Пуск двигателя СЭП в авт. реж. 
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_14)<<2)		//Включить подогрев. двигателя СЭП 
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_13)<<3)		//Отключить подогрев. двигателя СЭП 
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_12)<<4)		//Включить вент. редукт. двигателя СЭП  
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_11)<<5)		//Обороты двигателя СЭП более 300 
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_10)<<6)		//Включить ВИП № 2 
																			| (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_9)<<7);		//Включить ВИП № 1
																			
		pr_data.SepTx_Message[1].Data[2] = (GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_2)<<0)			//Включить БГ-ДИЗ. Г-2 (pSep->status.stop_eng_5min<<4);
																			| (GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_3)<<1)			//Включить БГ-ДИЗ.  Г-1 
																			| (GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_4)<<2)			//Включить БГ-ДИЗ. СЭП Г-2 
																			| (GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_5)<<3)			//Включить БГ-ДИЗ. СЭП Г-1 
																			| (GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_6)<<4);		//Останова 5 мин. 
																			
		pr_data.SepTx_Message[1].Data[3] = (pSep->status.auto_start_eng_impossible<<0) 
																			| (pSep->status.eng_start_not_to_load<<1) 
																			| (pSep->status.eng_didnt_start_manual<<2) 
																			| (pSep->status.eng_didnt_start_oil<<3) 
																			| (pSep->status.eng_wait_load<<4)
																			| (pSep->status.alarm_shutdown_load<<5);
		
		pr_data.SepTx_Message[1].Data[4] = Flag_BlockSepFromPU | (pSep->status.valve_left_didnt_open<<0) | (pSep->status.valve_right_didnt_open<<1);

		pr_data.SepTx_Message[1].Data[5] = podogrevState;
}
