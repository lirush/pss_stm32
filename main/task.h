#ifndef TASK_H
#define TASK_H

void xCreateTask(void (*func)(void));
void vTaskScheduler(void);

void vTask1(void);
void vTask2(void);
void vTask3(void);
void vTask4(void);

#endif
