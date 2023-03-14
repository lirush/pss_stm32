/*!
\file
\details

Данный файл содержит объявления оборотов двигателя,
получаемых от ЭБУ, входящего в состав системы
дизельного двигателя, и функции уонтроля оборотов.
*/
#ifndef EBU_H
#define EBU_H

#define ENGINESPEED100		(800)
#define ENGINESPEED300		(2800L)
#define ENGINESPEED400		(2800L)
#define ENGINESPEED600		(4800L)
#define ENGINESPEED800		(6400L)
#define ENGINESPEED900		(7200L)
#define ENGINESPEED1000		(8000L)
#define ENGINESPEED1700		(13600L)
#define ENGINESPEED1746		(13968L)
#define ENGINESPEED1750		(14000L)
#define ENGINESPEED1790		(14320L)
#define ENGINESPEED1800		(14400L)
#define ENGINESPEED1810		(14480L)
#define ENGINESPEED1850		(14800L)
#define ENGINESPEED1854		(14832L)
#define ENGINESPEED1900		(15200L)

/*!
Обороты двигателя выключены
*/
unsigned char SpeedIsNot(void);
/*!
Обороты двигателя есть
*/
unsigned char SpeedIs(void);
/*!
Обороты двигателя >=400
*/
unsigned char SpeedIs400(void);
/*!
Обороты холостого хода
*/
unsigned char SpeedIsHH(void);
/*!
Обороты рабочего хода
*/
unsigned char SpeedIs1800(void);

#endif  //EBU_H
