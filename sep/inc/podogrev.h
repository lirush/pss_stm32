#ifndef PODOGREV_H
#define PODOGREV_H

#define LVLSIGNAL_FROM      (4)                 //4-20 mA
#define LVLSIGNAL_TO        (20)
#define TEMP_COOLLIQ_TO     (125)               //диапазон датчика температуры -60 - 125
#define TEMP_COOLLIQ_FROM   (60)               //диапазон датчика температуры -60 - 125
#define ADC_MAX             (1024)
#define SET_TEMP_COOLIQ     (50)
#define TEMP_COOLIQ_ISHOT   ((SET_TEMP_COOLIQ *ADC_MAX)/TEMP_COOLLIQ_TO)
#define VAL_OF_DIV					(1024/185)
#define TEMP_COEFF_B				(0.32139)
#define TEMP_COEFF_C				(-113.5)

extern unsigned char podogrevState;

void PodogrevON(void);
void PodogrevOFF(void);

void PodogrevController(void);
signed char AlgPodogrev(void);
unsigned char LiquidIsHot(void);
signed int TempLiquidCalc(signed char temp);

#endif
