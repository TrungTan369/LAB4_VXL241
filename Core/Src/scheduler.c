/*
 * scheduler.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */
#include "scheduler.h"

sTask SCH_Task[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void){
	current_index_task = 0;
}

void SCH_Add_Task(void (*function)(), uint32_t Delay, uint32_t Period){
	if(current_index_task == SCH_MAX_TASKS){
		return;
	}
	SCH_Task[current_index_task].pTask = function;
	SCH_Task[current_index_task].Delay = Delay/10;
	SCH_Task[current_index_task].Period = Period/10;
	SCH_Task[current_index_task].RunMe = 0;
	SCH_Task[current_index_task].TaskID = current_index_task;
	current_index_task++;
}
void SCH_Update(void){
	for(uint8_t i = 0; i < current_index_task; i++){
		if(SCH_Task[i].Delay > 0){
			SCH_Task[i].Delay--;
		}
		else{
			SCH_Task[i].Delay = SCH_Task[i].Period;
			SCH_Task[i].RunMe++;
		}
	}
}
void SCH_Dispatch_Task(void){
	for(uint8_t i = 0; i < current_index_task; i++){
		if(SCH_Task[i].RunMe > 0){
			SCH_Task[i].RunMe--;
			(*SCH_Task[i].pTask)();
			if(SCH_Task[i].Period == 0){
				SCH_Delete_Task(i);
			}
		}
	}
}
uint8_t SCH_Delete_Task(uint8_t ID){
	if(SCH_Task[ID].pTask == 0){
		return 0;
	}
	for(uint8_t i = ID; i < current_index_task - 1; i++){
		SCH_Task[i].pTask = SCH_Task[i + 1].pTask;
		SCH_Task[i].Period = SCH_Task[i + 1].Period;
		SCH_Task[i].Delay = SCH_Task[i + 1].Delay;
		SCH_Task[i].TaskID = SCH_Task[i + 1].TaskID;
	}
	current_index_task--;
	return 1;
}
