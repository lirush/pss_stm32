#ifndef TIMER_H
#define TIMER_H
#endif

struct Tim_ms{
	unsigned int first					:1;
	unsigned int second					:1;
	unsigned int third					:1;
	unsigned int four						:1;
	unsigned int fifth					:1;
	unsigned int sixth					:1;
	unsigned int seventh				:1;
	unsigned int eigth					:1;
  unsigned int ninth      		:1;
  unsigned int tenth      		:1;
  unsigned int eleventh   		:1;
  unsigned int twelfth    		:1;
  unsigned int thirteenth 		:1;
  unsigned int fourteenth 		:1;
  unsigned int fifteenth  		:1;
  unsigned int sixteenth  		:1;
	unsigned int seventeenth  	:1;
	unsigned int eighteenth  		:1;
	unsigned int nineteenth  		:1;
	unsigned int twentieth  		:1;
	unsigned int twentyfirst 		:1;
	unsigned int twentysecond  	:1;
	unsigned int twentythird  	:1;
	unsigned int twentyfour	  	:1;
	unsigned int								:8;
};

union Tim20ms_un{
	struct Tim_ms Tim20msFlags;
	unsigned int build;
};

union Tim10ms_un{
	struct Tim_ms Tim10msFlags;
	unsigned int build;
};

union Tim1min_un{
	struct Tim_ms Tim1minFlags;
	unsigned int build;
};

typedef struct{
	unsigned char FlagSetTimer;
	unsigned int InsideCntTimer;
	unsigned char busy;											//флаг занятости таймера
}Tim20msUser_TypeDef;

typedef struct{
	unsigned char FlagSetTimer;
	unsigned int InsideCntTimer;
}Tim10msUser_TypeDef;

typedef struct{
	unsigned char FlagSetTimer;
	unsigned int InsideCntTimer;
}Tim1minUser_TypeDef;

extern unsigned char tim6_clk_1ms;
extern unsigned int tim6_clk_1s;
extern Tim20msUser_TypeDef Tim20msUser[24];
extern Tim10msUser_TypeDef Tim10msUser[16];
extern Tim1minUser_TypeDef Tim1minUser[16];

void TIM6_init(void);
void TIM7_init(void);
void Tim20ms(unsigned char EN,unsigned char num, unsigned int setting);
void Tim10ms(unsigned char EN,unsigned char num, unsigned int setting);
void Tim1min(unsigned char EN, unsigned char num, unsigned char setting);


