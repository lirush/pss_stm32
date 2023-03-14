/*!
\file
\details

Данный файлл содержит описание типов и функций
управления стартером
*/

#ifndef STARTER_H
#define STARTER_H

#define QUANTITY_TRY            (3)

void StarterON(void);
void StarterOFF(void);
unsigned char StarterKeyONState(void);
/*
функция блокирует/разблокирует стартер
*/
void xStarterLocker(void);
/*!
Контролирует состояние стартера.
*/
void StarterController(void);
/*
функция запускаяет алгоритм запуска двигателя 
дизеля по стартеру
для того чтобы функция заработала ее нужно 
завести с помощью флага таймера - Tim20msUser[FOURTEENTH].FlagSetTimer
*/
signed char AlgStarter(void);

#endif
