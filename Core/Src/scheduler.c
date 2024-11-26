/*
 * scheduler.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */
#include "scheduler.h"

List list;
List list_run;

int current_time = 0;
void SCH_Init(void){
	list.head = NULL;
	list.tail = NULL;
    list.numTask = 0;

    list_run.head = NULL;
    list_run.tail = NULL;
    list_run.numTask = 0;
}
void SCH_Add_Task(void (*function)(), uint32_t Delay, uint32_t Period){
	if(SCH_Is_Task_Exist(function) == 1)
		return;
	sTask * newTask = (sTask *) malloc (sizeof(sTask));
	if (newTask == NULL)
		return;

	newTask->pTask = function;
	newTask->Delay = Delay/10; // scale for TIM2 10ms run
	newTask->Period = Period/10;
	newTask->next_run = current_time + Delay;
	newTask->next = NULL;
	newTask->prev = NULL;

	if(list.head == NULL){
		list.head = newTask;
		list.tail = newTask;
	}
	else{
		newTask->prev = list.tail;
		list.tail->next = newTask;
		list.tail = newTask;
	}
	list.numTask++;
}

void SCH_Update(void){ // TIM2 run with 100hz / 10ms
	current_time += 10;
	sTask * temp = list.head;
	while(temp != NULL && temp->next_run < current_time){
		if(temp->Delay > 0){
			temp->Delay--;
		}
		else{
			add_ListRun(temp->pTask);
			temp->Delay = temp->Period;
			if(temp->Period == 0){
				sTask * del = temp;
				temp = temp->next;
				SCH_Delete_Task(del->pTask);
				continue;
			}
		}
		temp = temp->next;
	}
}
void SCH_Dispatch_Task(void){
	sTask * temp = list_run.head;
	while(temp != NULL){
		temp->pTask();
		sTask* del = temp;
		temp = temp->next;
		delete_ListRun(del->pTask);
	}
}
void add_ListRun(void (*function)()){
	sTask * newTask = (sTask *) malloc ( sizeof(sTask));
	newTask->pTask = function;
	newTask->next = NULL;
	newTask->prev = NULL;
	if(list_run.numTask == 0){
		list_run.head = newTask;
		list_run.tail = newTask;

	}
	else{
		newTask->prev = list_run.tail;
		list_run.tail->next = newTask;
		list_run.tail = newTask;
	}
	list_run.numTask++;
}
void delete_ListRun(void (*function)()){
	if(list_run.numTask == 0)
		return ;
	if(list_run.numTask == 1){
		sTask * del = list_run.head;
		list_run.head = NULL;
		list_run.tail = NULL;
		free(del);
		list_run.numTask--;
		return;
	}
	sTask * temp = list_run.head;
	while(temp != 0){
		if(temp->pTask == function){
			if(temp->prev == NULL){ // delete head
				temp->next->prev = NULL;
				list_run.head = temp->next;
			}
			else if (temp->next == NULL){ //delete tail
				temp->prev->next = NULL;
				list_run.tail = temp->prev;
			}
			else{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			list_run.numTask--;
			free(temp);
			return;
		}
		temp = temp->next;
	}
}

uint8_t SCH_Delete_Task(void (*function)()){
	if(list.numTask == 0)
		return 0;
	sTask * temp = list.head;
	while(temp != 0){
		if(temp->pTask == function){
			if(temp->prev == NULL){ // delete head
				temp->next->prev = NULL;
				list.head = temp->next;
			}
			else if (temp->next == NULL){ //delete tail
				temp->prev->next = NULL;
				list.tail = temp->prev;
			}
			else{
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
			}
			list.numTask--;
			free(temp);
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}
uint8_t SCH_Is_Task_Exist(void (*function)()){
	if(list.numTask == 0) return 0;
	sTask * temp = list.head;
	while(temp != NULL){
		if(temp->pTask == function)
			return 1;
		temp = temp->next;
	}
	return 0;
}

