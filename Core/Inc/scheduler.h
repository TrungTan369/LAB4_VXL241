/*
 * scheduler.h
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "global.h"
#define SCH_MAX_TASKS 10

typedef struct{
	void (*pTask)(void);
	uint32_t Delay;
	uint32_t Period;
	uint8_t RunMe;
	uint32_t TaskID;
}sTask;

void SCH_Init(void);
void SCH_Add_Task (void (*function) (), uint32_t, uint32_t);
void SCH_Update(void);
void SCH_Dispatch_Task(void);
uint8_t SCH_Delete_Task(uint8_t );

#endif /* INC_SCHEDULER_H_ */
