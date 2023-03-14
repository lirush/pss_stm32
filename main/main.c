#include <stm32f2xx.h>
#include "main.h"
#include "init_peripherals.h"
#include "sdata.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "stm32f4xx_flash.h"

//---------------------------------------

//-----------------------------------

#define SEND			2
#define	READ			5

//__IO uint16_t      ADCValues[size_masiv_vish*4]; //отправляется экстерном в init_peripherals

void TIM2_Init(void);

TIM_TypeDef tim2;
TIM_TimeBaseInitTypeDef tim2_init;

volatile int timeslots = 0;
volatile int tim2_flag=0;

void delay(uint32_t time_delay)
{	
    uint32_t i,j;
    for(i = 0; i < time_delay; i++) {j++;}
}

void InitCANMessage(void)
{
    pr_data.Tx_Message.IDE = CAN_Id_Extended;                          //данные от платы ПУИК в хост (ПМВ)
    pr_data.Tx_Message.DLC = 6;
    pr_data.Tx_Message.ExtId = telemetry_data;
    
		pr_data.SepTx_Message[0].IDE = CAN_Id_Extended;										//состояние устройст алгоритма автоматического старта/останова двигателя
    pr_data.SepTx_Message[0].DLC = 8;
    pr_data.SepTx_Message[0].ExtId = sep_data_1;
    
    pr_data.SepTx_Message[1].IDE = CAN_Id_Extended;										//состояние устройст алгоритма автоматического старата/останова двигателя
		pr_data.SepTx_Message[1].DLC = 8;																	//предпологалось записывать в этот пакет состояние ключей
    pr_data.SepTx_Message[1].ExtId = state_keys;
}

/*******************************************************************************
* Function Name  : xWriteFlashAddressPki
* Description    : записывает во флэш адреса ПКИ (младший байт протокола J1939)
									 данные из CAN шины
* Input          : 
* Output         : 
* Return         : status - возвращает состояние записи во флеш
*******************************************************************************/
FLASH_Status xWriteFlashAddressPki(void)
{
	FLASH_Status status=0;
	uint8_t data=0;

	FLASH_Unlock();
	FLASH_EraseSector(FLASH_Sector_11,VoltageRange_1);	
	for(int i=0; i<6;i++)
	{
//		if(hashArr[PMV_PKI].data[i+1] & (1<<0))
		{
			data = ((hashArr[PMV_PKI].data[i+1]));
			status = FLASH_ProgramByte(MY_FLASH_PAGE_SECTOR+i,data);
		}
	}
	FLASH_Lock();
	return status;
}

/*******************************************************************************
* Function Name  : vReadFlashAddressPki
* Description    : читает из флэш адреса ПКИ (младший байт протокола J1939)
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
uint64_t vReadFlashAddressPki(void)
{
	uint64_t data=0;
	
	((unsigned int *)&data)[0] = *(__IO uint32_t *)MY_FLASH_PAGE_SECTOR;
	((unsigned int *)&data)[1] = *(__IO uint32_t *)(MY_FLASH_PAGE_SECTOR+4) & (0x0000FFFF);

	return data;
}

/*******************************************************************************
* Function Name  : vUpdateAddressesPki
* Description    : обновляет адреса ПКИ
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void vUpdateAddressesPki(void)
{
	uint64_t data=vReadFlashAddressPki();
	
	hashArr[PKI1n+0].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(0*8));				//ПКИ1 ДИЗ СЭП
	hashArr[PKI1n+5].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(1*8));				//ПКИ2 ДИЗ СЭП
	
	hashArr[PKI1n+4].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(2*8));				//ПКИ2 ВИП
	hashArr[PKI1n+3].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(3*8));				//ПКИ1 ВИП
	
	hashArr[PKI1n+1].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(4*8));				//ПКИ1 ВИП ДИЗ
	hashArr[PKI1n+2].key = (ID_PKI1 &0xffffff10) | (uint8_t)(data>>(5*8));				//ПКИ1 ВИП ДИЗ
}

/*******************************************************************************
* Function Name  : xResponseToRequestReprogPki
* Description    : отправляет хосту (ПМВ) ответ на возможность 
									 перепрограммирования адресов ПКИ устанавливает разрешение на 
									 перепрограммирование. Разрешает перепрограммировать, если 
									 обороты двигателя равны 0.
* Input          : data 
* Output         : response, status
* Return         : нет
*******************************************************************************/
void vRequestReprogPki(uint8_t data)
{
	Ebu_TypeDef *pEbu =&Sep.Disel.Engine.Ebu;
	
	static enum{
		init,
		res,
		prog,
		st_prog,
		prog_is_ready,
		send_status
	}step;
	
	static CanTxMsg TmpMsg;
	TmpMsg.IDE = CAN_Id_Extended;
	TmpMsg.ExtId = ID_PMV_PKI_RES;
	TmpMsg.DLC = 1;
	
	static uint8_t cnt_trans=0;	
	
	static FLASH_Status status;
	
	if(pEbu->eng_speed==0)
	{
		switch((uint8_t)step)
		{
			case init:
				cnt_trans=0;
				if(data==REQUEST_REPROG_PKI)									//запрос на перепрограмиирование
					step=res;
			break;
			case res:
				if(cnt_trans++<5)
				{
						TmpMsg.Data[0] = RESPONSE_REPROG_PKI; 
						CAN_Transmit(CAN1,&TmpMsg);
				} else step=prog;
			break;
			case prog:
				cnt_trans=0;
				if(data==REQUEST_CMD_REPROG_PKI)
					step=st_prog;
			break;
			case st_prog:
				status =xWriteFlashAddressPki();
				step=prog_is_ready;
			break;
			case prog_is_ready:
				vUpdateAddressesPki();
				TmpMsg.Data[0] = status+2;
				step=send_status;
			break;
			case send_status:
				if(cnt_trans++<5)
				{ 
					CAN_Transmit(CAN1,&TmpMsg);	
				}else step=init;		
			break;
			default:
				step=init;
			break;
		}
	}
}

/*******************************************************************************
* Function Name  : vRequestAddressesPki
* Description    : отправляет хосту (ПМВ) ответ на запрос адресов ПКИ.
* Input          : data 
* Output         : нет
* Return         : нет
*******************************************************************************/
void vRequestAddressesPki(uint8_t data)
{
		static enum{
			init,
			pack_res,
			send_res
		}step;
		static CanTxMsg TmpMsg;
		TmpMsg.IDE = CAN_Id_Extended;
		TmpMsg.ExtId = ID_PMV_PKI_RES;
		TmpMsg.DLC = 8;		
		static uint8_t cnt_trans=0;
		
		switch((uint8_t)step)
		{
			case init:
				cnt_trans=0;
				if(data==REQUEST_ADDRESSES_PKI)
				{
					step=pack_res;
					hashArr[PMV_PKI].data[0]=0x00;
				}
			break;
			case pack_res:
				TmpMsg.Data[0] = RESPONSE_ADDRESSES_PKI;
				uint64_t tmp =vReadFlashAddressPki() & 0x0000FFFFFFFFFFFFUL;
				memcpy(TmpMsg.Data+1,&tmp,7);
				step=send_res;
			break;
			case send_res:
				if(cnt_trans++<5)
					CAN_Transmit(CAN1,&TmpMsg);
				else step=init;
			break;
			default:
				step=init;
			break;			
		}
}

void vTask1(void)
{
    Read_from_sensor();
	
		uint8_t tmp = hashArr[PMV_PKI].data[0];
			
		vRequestReprogPki(tmp);	
	
		vRequestAddressesPki(tmp);
}

void vTask2(void)
{
	Schusep_TypeDef *pSchusep = &Sep.SchuSep;
	Dev_ofPodogrevType *pHot = &Sep.Disel.Podogrev;	
	static uint8_t trig=0;
    UpdateDataIn();
    if(pSchusep->SA1_auto_dis_sep)
        {Sep.ControlDisel();trig=0;}                                      //если идет автоматическое управление дизелем СЭП, то заслонками от внешних устройств управлять нельзя
    else
    {        
        uint8_t V1Close = Valve1CloseByTRG();
        uint8_t V1Open = Valve1OpenByTRG();
        uint8_t V2Open = Valve2OpenByTRG();
				if(!trig)
				ProtectSwitchToManual();
				pHot->io.in0=0;
				trig=1;
    }
    DiselStatusController();
    UpdateDataOut();
}

void vTask3(void)
{
    CAN_Transmit(CAN1,&pr_data.Tx_Message);
    CAN_Transmit(CAN1,&pr_data.SepTx_Message[0]);
    CAN_Transmit(CAN1,&pr_data.SepTx_Message[1]);
}

void vTask4(void)
{
    Write_to_key();
    if(!Sep.SchuSep.SA1_auto_dis_sep)                                           //если тумблер авт.диз.сэп !=1 -> управляем
        {Write_to_keys_by_if();}                                                //ключами которые управляют работой СЭП
}

void vTask5(void)
{
	char tmp[300];
	uint16_t len=0;
//	static uint8_t cnt=0;
	for(int i=0;i<=7;i++)
	{
		sprintf(tmp+len,"\rbyte %d \t%02X",i,pr_data.SepTx_Message[0].Data[i]);
		len = strlen(tmp);
	}
	UART1_send((uint8_t *)&tmp,strlen(tmp));
}

int main()
{
		RCC_Configuration();
		TIM2_Init();
		GPIO_Init_TE(); 	
		NVIC_Config();
		CAN1_Config(); //прием по J1939
		CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //прерывание по приему на шину FIFO0
		CAN_ITConfig(CAN1, CAN_IT_FMP1, ENABLE); //прерывание по приему на шину FIFO1
		NVIC_EnableIRQ(CAN1_RX0_IRQn); //разрешаем прерывания CAN1
		NVIC_EnableIRQ(CAN1_RX1_IRQn); //разрешаем прерывания CAN1
		USART1_Init_TE();
		TIM6_init();
		InitCANMessage();
		TIM7_init();
		InitSep();
	
		vReadFlashAddressPki();					//преднастройка адресов ПКИ

		xCreateTask(vTask1);
		xCreateTask(vTask2);
		xCreateTask(vTask3);
		xCreateTask(vTask4);
		xCreateTask(vTask5);
			
		vTaskScheduler();  
}

void  TIM2_Init(void)
{
	
	/* Подаём такты на TIM2 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Настраиваем предделитель так, чтобы таймер считал миллисекунды.
     На бо́льших частотах следите, чтобы предделитель не превысил
     максимальное значение uint16_t - 0xFFFF (65535) */
  TIM_TimeBaseInitTypeDef timer_base;
  TIM_TimeBaseStructInit(&timer_base);
  timer_base.TIM_Prescaler = 120;                           //Fclk=120МГц частота 50Гц T=1/50=0.02 sec -> 20мс
	timer_base.TIM_Period = 10000;
  TIM_TimeBaseInit(TIM2, &timer_base);


  /* Разрешаем таймеру генерировать прерывание по захвату */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE);
  /* Включаем таймер */
  NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
    {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        tim2_flag=1;
	}
}

