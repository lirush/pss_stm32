#ifndef FMS_H
#define FMS_H

typedef enum{
    STATE1,     //0
    STATE2,     //1
    STATE3,     //2
    STATE4,     //3
    STATE5,     //4
    STATE6,     //5
    STATE7,     //6
    STATE8,     //7
    STATE9,     //8
    STATE10,    //9
    STATE11,    //A
    STATE12,    //B
    STATE13,    //C
    STATE14,    //D
    STATE15,    //E
    STATE16,    //F
    STATE17,    //10
    STATE18,    //11
		STATE19,    //12
		STATE20     //13	
}State;

typedef enum{
    STEP1,
    STEP2
}Event;

typedef struct{
    State step;
    Event event;
    signed char (*EventFunc)();
}Transition;

/*
Функция перехода КА
входные параметры:
    int elementsCount - количество элементов таблицы переходов
    Transition *transitionTable - указатель на таблицу переходов
    State currentState - текущее состояния КА
    Event eventToFire - событие для КА
*/
signed char FMSFunc(int elementsCount, Transition *transitionTable, State currentState, Event eventToFire);

#endif          //FMS_H

