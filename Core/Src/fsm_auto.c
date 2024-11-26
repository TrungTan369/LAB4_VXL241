/*
 * fsm_auto.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */

#include "fsm_auto.h"


void fsm_auto_run(){
	switch (status) {   // LINE 1
		case auto_init:
			// ------- ALL LED OFF -------------
			single_LED_off();
			count0 = (time_red_green + time_red_yellow)/1000;
			count1 = time_red_green/1000;
			updateClockBuffer(count0, count1);
			status = auto_red_green;
			HAL_GPIO_WritePin(Y0_GPIO_Port, Y0_Pin, RESET); // yellow 0 off
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET); // red 1 on
			HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, SET); // red0 on
			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET); // green1 on
			SCH_Add_Task(fsm_switch_state_auto_red_yellow, time_red_green, 0);
			break;
		case auto_red_green:
			//-----SWITCHING MANNUAL MODE -----------
			if(isButtonPress(1) == 1){
				SCH_Delete_Task(fsm_switch_state_auto_red_yellow); // DELETE NEXT STATE OF AUTO MODE
				SCH_Delete_Task(fsm_auto_run);
				SCH_Delete_Task(Scan7SEG);
				SCH_Add_Task(fsm_manual, 100, 10);
				status = manual_red_green;
				Diable_Led();
				single_LED_off();
				HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, SET);
				HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
				HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
				HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, RESET);
				return;
			}
			break;
		case auto_red_yellow:
			break;
		case auto_green_red:
			//-----SWITCHING MANNUAL MODE -----------
			if(isButtonPress(1) == 1){
				SCH_Delete_Task(fsm_switch_state_auto_yellow_red); // DELETE NEXT STATE OF AUTO MODE
				SCH_Delete_Task(fsm_auto_run);
				SCH_Delete_Task(Scan7SEG);
				SCH_Add_Task(fsm_manual, 10, 10);
				status = manual_green_red;
				Diable_Led();
				single_LED_off();
				HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
				HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, SET);
				HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, RESET);
				HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
				return;
			}
			break;
		case auto_yellow_red:
			break;
		default: // ----- MANUAL MODE & SETTING MODE & SLOW MODE---------
			return;
	}
	// -------SWITCHING SLOW MODE ---------------
	if(isButtonPress(0) == 1){
		SCH_Delete_Task(list.tail->pTask); // DELETE TASK SWITCH STATE
		SCH_Delete_Task(fsm_auto_run);
		SCH_Delete_Task(Scan7SEG);
		SCH_Add_Task(fsm_slow_run, 0, 500);
		single_LED_off();// ------ALL LED OFF----------
		Diable_Led(); //--- Disable led 7 segment ------
		return;
	}
	// -------SWITCHING SETTING MODE ------------
	if(isButtonPress(2) == 1){
		time_red_yellow = 0;
		time_red_green = 0;
		updateClockBuffer(0, 1);
		status = set_green;
		SCH_Delete_Task(list.tail->pTask); // DELETE TASK SWITCH STATE
		SCH_Delete_Task(fsm_auto_run);
		SCH_Delete_Task(count_1_second);
		SCH_Add_Task(fsm_setting, 10, 10);
		single_LED_off();
		HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, 1);
		HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, 1);
	}
}
void fsm_switch_state_auto_init(){
	status = auto_init;
}
void fsm_switch_state_auto_red_green(){
	HAL_GPIO_WritePin(Y0_GPIO_Port, Y0_Pin, RESET); // yellow 0 off
	HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET); // red 1 on
	HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, SET); // red0 on
	HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET); // green1 on
	status = auto_red_green;
	count0 = (time_red_green + time_red_yellow)/1000;
	count1 = time_red_green/1000;
	SCH_Add_Task(fsm_switch_state_auto_red_yellow, time_red_green, 0); // NEXT STATE
}
void fsm_switch_state_auto_red_yellow(){
	HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET); // green1 off
	HAL_GPIO_WritePin(Y1_GPIO_Port, Y1_Pin, SET); // yellow1 on
	status = auto_red_yellow;
	count0 = count1 = time_red_yellow/1000;;
	SCH_Add_Task(fsm_switch_state_auto_green_red, time_red_yellow, 0); // NEXT STATE
}
void fsm_switch_state_auto_green_red(){
	HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, RESET); // red 0 off
	HAL_GPIO_WritePin(Y1_GPIO_Port, Y1_Pin, RESET); // yellow 1 off
	HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, SET); // green 0 on
	HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET); // red 1 on
	status = auto_green_red;
	count0 = (time_red_green)/1000;
	count1 = (time_red_green + time_red_yellow)/1000;
	SCH_Add_Task(fsm_switch_state_auto_yellow_red, time_red_green, 0);// NEXT STATE
}
void fsm_switch_state_auto_yellow_red(){
	HAL_GPIO_WritePin(Y0_GPIO_Port, Y0_Pin, SET); // yellow 0 on
	HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, RESET); // green 0 off
	status = auto_yellow_red;
	count0 = count1 = time_red_yellow/1000;
	SCH_Add_Task(fsm_switch_state_auto_red_green, time_red_yellow, 0);
}
void count_1_second(){
	if(count0 > 0)
		count0 --;
	if(count1 > 0)
		count1 --;
	updateClockBuffer(count0, count1);
}
