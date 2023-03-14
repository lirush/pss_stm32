#include "task.h"
#include "config.h"

extern volatile int tim2_flag;
extern volatile int timeslots;

void xRunTask(void (*func)(void))
{
    func();
}

void (*p_addr[QUANTITY_TASKS])(void);
unsigned char ucQTasks=0;

void xCreateTask(void (*func)(void))
{
    p_addr[ucQTasks]= func;
    ucQTasks++;
}

void vTaskScheduler(void)
{
    while(1){
        if(tim2_flag){ tim2_flag=0;
					xRunTask(p_addr[timeslots]);
					if(timeslots++>=QUANTITY_TASKS-1)timeslots=0;
        }
    }
}
