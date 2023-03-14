/*!
\file
\details

Данный файл содержит в себе определение типов и функций
алгоритма СЭП
*/
#ifndef SCHUSEP_H
#define SCHUSEP_H

#include "stm32f2xx.h"
#include "config.h"

#define _300MS  (300/TSYS)
#define _400MS  (400/TSYS)
#define _1SEC   (1000/TSYS)
#define _1500MS (1500/TSYS)
#define _2SEC   (2000/TSYS)
#define _3SEC   (3000/TSYS)
#define _4SEC   (4000/TSYS)
#define _5SEC   (5000/TSYS)
#define _9SEC   (9000/TSYS)
#define _10SEC  (10000/TSYS)
#define _30SEC  (30000/TSYS)
#define _40SEC  (40000/TSYS)
#define _3MIN   (4)
#define _4MIN   (5)
#define _5MIN   (6)
#define _30MIN  (31)

#define OIL_PRESS_02_MPa	(175)

typedef enum {IORESET = 0, IOSET = 1} IOFlagStatus;
enum {EN=1,DIS};
enum {OPEN,CLOSE};
typedef enum {DEFAULT=0,START,STOP,STARTING,STOPPING}ModeSep;

extern uint8_t stateSep;
extern uint8_t status_pki_need_clear;

enum IndNumber{
    FIRST,
    SECOND,
    THIRD,
    FOURTH,
    FIFTH,
    SIXTH,
    SEVENTH,
    EIGHT,
    NINTH,
    TENTH,
    ELEVENTH,
    TWELFTH,
    THIRTEENTH,
    FOURTEENTH,
    FIFTEENTH,
    SIXTEENTH,
		SEVENTEENTH,
		EIGHTEENTH,
		NINETEENTH,
		TWENTIETH,
		TWENTYFIRST,
		TWENTYSECOND,
		TWENTYTHIRD,
		TWENTYFOUR
	};


typedef enum{
  OPENING=0x01,
	CLOSING=0x02,	
	ISOPEN=0x05,
	ISOPENVALVE=0x04,
	ISCLOSE=0x0A,
	ISCLOSEVALVE=0x08,
	ERRORCONNECT=0x0C,
	ERROROPEN1=0x0D,
	ERRORCLOSE1=0x12,
	ERROROPEN2=0x19,
	ERRORCLOSE2=0x0E,	
	ERROROPEN3=0x1D,
	ERRORCLOSE3=0x16
}VALVE_STATE;

typedef struct{
    GPIO_TypeDef *Port;
    uint16_t Pin;
}Port_TypeDef;

typedef struct{
    unsigned char SB7_alarm_sep         :1;
    unsigned char SB11_valves_open      :1;
    unsigned char Ind_podogrev          :1;
    unsigned char SB1_off_sep           :1;
    unsigned char SB2_on_extpower       :1;
    unsigned char SB3_on_BG_dis         :1;
    unsigned char SB4_on_BG_dis_sep     :1;
    unsigned char SA8_podogrev          :1;
    unsigned char SB9_valves_close      :1;
    unsigned char SA1_auto_dis_sep      :1;
    unsigned char SB6_auto_start_sep    :1;
    unsigned char ppo                   :1;
    unsigned char vu1                   :1;
    unsigned char SA5_stop              :1;
    unsigned char SA5_hh                :1;
    unsigned char SA5_1800              :1;
    unsigned char SA8_stop_podogrev     :1;
    unsigned char Ind_svechi            :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char                       :1;
    unsigned char Alarm_STOP_eng_PU     :1;	 //Аварийный останов и блокировка СЭП от ПУ 
    unsigned char START_eng_PU          :1;  //Запустить двигатель СЭП от ПУ
    unsigned char SEP_not_norm          :1;
}Schusep_TypeDef;

struct io_bit{
    unsigned char isopen        :1; //!состояние - открыта
    unsigned char isclose       :1; //!состояние - закрыта
    unsigned char in0           :1;
    unsigned char in1           :1;
    unsigned char open          :1; //!открыть
    unsigned char close         :1; //!закрыть
    unsigned char out0          :1;
    unsigned char out1          :1;
};

typedef struct {
    struct io_bit io;
    void (*Open)(void);
    void (*Close)(void);
    unsigned char (*KeyOpenState)(void);
    unsigned char (*KeyCloseState)(void);
    unsigned char status;
}Dev_ofValveType;

typedef struct {
    struct io_bit io;
    void (*On)(void);
    void (*Off)(void);
    unsigned char (*KeyState)(void);
    unsigned char cnt_start;
    signed char (*Algorithm)(void);
    unsigned char status;
}Dev_ofStarterType;

typedef struct {
    struct io_bit io;
    void (*On)(void);
    void (*Off)(void);
    unsigned char (*KeyState)(void);
    signed char (*Algorithm)(void);
    unsigned int temp;
    unsigned char status;
}Dev_ofPodogrevType;

typedef struct{
    unsigned char oil_press_dis          :1; //давление масла дизеля
    unsigned char oil_press_rdk          :1; //давление масла РДК
    unsigned char B105_peregrev_ohlag    :1; //перегрев охладающей жидкости
    unsigned char B106_oil_level_poddon  :1; //уровень масла в поддоне
    unsigned char B109_filter_zasoren    :1; //фильтр засорен
    unsigned char B107_oil_level_ohlag   :1; //уровень ОЖ в блоке радиатора
    unsigned char rdk_sep                :1; //РДК сэп
    unsigned char                        :1;
}DSensors_TypeDef;

typedef struct{
	unsigned int oil_press;									//аналоговый датчик
}AnSensors_TypeDef;

typedef struct{
    struct io_bit io1;
    struct io_bit io2;
    unsigned int eng_speed;
}Ebu_TypeDef;

typedef struct{
    struct io_bit io1;
    struct io_bit io2;
    DSensors_TypeDef DSensors;
		AnSensors_TypeDef AnSensors;
    Ebu_TypeDef Ebu;
    void (*start_hh)(void);
    void (*stop_hh)(void);
    void (*start_1800)(void);
    void (*stop_1800)(void);
    unsigned char (*IsHH)(void);
    unsigned char status;
}Dev_ofEngineType;

typedef struct{
    struct io_bit io;
    Dev_ofStarterType Starter;
    Dev_ofStarterType Svechi;
    Dev_ofStarterType Ventilator;
    Dev_ofPodogrevType Podogrev;
    Dev_ofEngineType Engine;
    void (*AutoStart)(unsigned char);
    void (*AutoStop)(unsigned char);
    unsigned char (*State)(void);
}Disel_TypeDef;

struct SepStatus{
	unsigned char auto_start_eng_impossible	:1;
	unsigned char eng_start_not_to_load			:1;
	unsigned char eng_didnt_start_manual		:1;
	unsigned char eng_didnt_start_oil				:1;
	unsigned char eng_wait_load							:1;
	unsigned char alarm_shutdown_load				:1;
	unsigned char heater_didnt_start_manual	:1;
	unsigned char valve_left_didnt_open			:1;
	unsigned char valve_right_didnt_open		:1;
	unsigned char cmd_valves_open						:1;
	unsigned char pusk_eng_sep_auto					:1;
	unsigned char on_off_bu_eng_sep					:1;
	unsigned char eng_start_auto						:1;
	unsigned char eng_start_work						:1;
	unsigned char cmd_stop_eng_sep					:1;
	unsigned char stop_eng_5min							:1;
};

typedef struct{
	unsigned char phase_a;
	unsigned char phase_b;
	unsigned char phase_c;
	unsigned short int freq;
}Pki_TypeDef;

typedef struct{
	Pki_TypeDef Pki[6];
	unsigned int status;
	unsigned int cmd;												//команды (например для отключения СЭП)
}Gens_TypeDef;

typedef struct{
    struct io_bit io;
    Schusep_TypeDef SchuSep;
    Dev_ofValveType Valve[2];
    Disel_TypeDef Disel;
    void (*ControlDisel)(void); 
		Gens_TypeDef Gens;
		struct SepStatus status;
}Sep_typeDef;
extern Sep_typeDef Sep;

typedef struct{
    unsigned char ItNeedStop;
    unsigned char EndStopping;
}StopCtrl_TypeDef;
extern StopCtrl_TypeDef StopCtrl;

void init_sep(void);
void StartStopDiselSep(void);
void HowDoSepFeel(void);

extern volatile uint8_t Flag_BlockSepFromPU;

#endif
