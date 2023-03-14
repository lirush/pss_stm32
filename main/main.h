#ifndef MAIN_H
#define MAIN_H

#include "stm32f2xx.h"
#include "stm32f2xx_conf.h "

#include "declar.h"
//#include "schusep.h"
//#include "valve.h"
//#include "timer.h"
#include "can.h"
#include "sensors.h"
#include "keys.h"
#include "task.h"
#include "uart.h"


#define DELTA_NULL 100
#define MY_FLASH_PAGE_SECTOR 0x080E0000

#define REQUEST_REPROG_PKI			(0x01)
#define RESPONSE_REPROG_PKI			(0x02)
#define REQUEST_CMD_REPROG_PKI	(0x03)
#define REQUEST_ADDRESSES_PKI		(0x0B)
#define RESPONSE_ADDRESSES_PKI	(0x0C)


struct spi_encoder
{
	uint8_t 	counter_SPI;
	uint16_t 	sum; 					//сумма из десяти показаний  
	uint8_t 	on_off; 			//включен или нет SPIx  1 - on, 0 - off	
};
	
struct adc_channel1_2
{
	uint8_t 	counter_adc;
	volatile	uint16_t *pr_adc_values; //указатель на массив значений
	//uint8_t  mod_dut_akt; //0 - конфигурация ДУТ и АКБ, 1 - актуатор	
};

struct status_information
{
	uint16_t counter_stat_inf;
	uint8_t device;  //подключенне устройства
	uint8_t error;   //зафиксированные ошибки
};


#endif

