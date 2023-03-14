#ifndef ENGINE_H
#define ENGINE_H

enum ENGINE_STATUS_LIST{
    ERRHHSTART=1,           //0x01
    ERRHHSTOP,              //0x02
    ERR1800START,           //0x03
    ERR1800STOP,            //0x04
    ENGHH_STARTING,         //0x05
    ENGHH_STOPPING,         //0x06
    ENG1800_STARTING,       //0x07
    ENG1800_STOPPING,       //0x08
    ENGHH_ISSTART,          //0x09
    ENGHH_ISSTOP,           //0x0A
    ENG1800_ISSTART,        //0x0B
    ENG1800_ISSTOP,         //0x0C
    ENG_ISSTOP              //0x0D
};

void Engine_start(void);
void Engine_hh_stop(void);
void Engine_1800_start(void);
void Engine_1800_stop(void);
void EngineController(void);

/*******************************************************************************
* Function Name  : uiControlSpeed300
* Description    : контроль Обороты двигателя 300 =1 в течении 3 мин.
* Input          : Обороты двигателя 300 
* Output         : нет
* Return         : 1 - Обороты двигателя 300 в течении 3-х минут; 0 - менее 3-х минут
*******************************************************************************/
unsigned char uiControlSpeed300(void);

#endif
