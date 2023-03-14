#ifndef DISEL_SEP_H
#define DISEL_SEP_H

extern unsigned char globalState;
extern unsigned char end_stopping;

void AutoStart(unsigned char en);
void AutoStop(unsigned char en);
/*
функция отслеживает состояния оборудования,
входящего в состав дизельного двигателя
*/
void DiselStatusController(void);
/**/
void ProtectSwitchToManual(void);

#endif  //DISEL_SEP_H
