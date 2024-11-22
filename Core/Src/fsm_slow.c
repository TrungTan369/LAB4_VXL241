/*
 * fsm_slow.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */

#include "fsm_slow.h"

void fsm_slow_run(){
	HAL_GPIO_TogglePin(Y0_GPIO_Port, Y0_Pin);
	HAL_GPIO_TogglePin(Y1_GPIO_Port, Y1_Pin);
	if(isButtonPress(0)==1){
		SCH_Add_Task(fsm_auto_run, 100, 10);
		SCH_Add_Task(Scan7SEG, 0, 250);
		SCH_Delete_Task_By_Function(fsm_slow_run);
		status = auto_init;
	}
}
