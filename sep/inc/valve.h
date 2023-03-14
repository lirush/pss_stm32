/*!
\file 
\details

Данный файл содержит в себе определение типов и функций
управления заслонками
*/
#ifndef VALVE_H
#define VALVE_H

typedef void(*pValveRstOpen)(void);
typedef void(*pValveRstClose)(void);
typedef unsigned char(*pValveStatus)(void);

unsigned char Valve1OpenByTRG(void);
unsigned char Valve1CloseByTRG(void);
unsigned char Valve2OpenByTRG(void);
unsigned char Valve2CloseByTRG(void);

void ValvesOpen(void);
void Valve2Open(void);
void ValvesClose(void);
void Valve2Close(void);
signed char AlgValve(void);

unsigned char VsKeyOpenState(void);
unsigned char VsKeyCloseState(void);
unsigned char V2KeyOpenState(void);
unsigned char V2KeyCloseState(void);

void ValveResetKeys(void);

/*!
Инициализирует указатели на функции: RstOpen, RstClose, Status
*/
void InitLevelValve(void);
/*!
Контролирует сброс портов управления заслонками.
Контролирует статус заслонок.
*/
void ValveController(void); 

#endif
