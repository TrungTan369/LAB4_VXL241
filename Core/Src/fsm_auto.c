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
			status = auto_red_green;
			count0 = (time_red_green + time_red_yellow)/1000;
			count1 = time_red_green/1000;
			updateClockBuffer(count0, count1);
			setTimer(0, time_red_green);
			setTimer(1, 1000); // count 1s
			setTimer(2, 100);  // scan led
			break;
		case auto_red_green:
			HAL_GPIO_WritePin(Y0_GPIO_Port, Y0_Pin, RESET); // yellow 0 off
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET); // red 1 on
			HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, SET); // red0 on
			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET); // green1 on
			if(timer_flag[0] == 1){
				status = auto_red_yellow;
				setTimer(0, time_red_yellow);
				count0 = time_red_yellow/1000;;
				count1 = time_red_yellow/1000;
				updateClockBuffer(count0, count1);
			}
			//-----SWITCHING MANNUAL MODE -----------
			if(isButtonPress(1) == 1){
				status = manual_red_green;
				Diable_Led();
				return;
			}
			break;
		case auto_red_yellow:
			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET); // green1 off
			HAL_GPIO_WritePin(Y1_GPIO_Port, Y1_Pin, SET); // yellow1 on

			if(timer_flag[0] == 1){
				status = auto_green_red;
				setTimer(0, time_red_green);
				count0 = (time_red_green)/1000;
				count1 = (time_red_green + time_red_yellow)/1000;
				updateClockBuffer(count0, count1);
			}
			break;
		case auto_green_red:
			HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, RESET); // red 0 off
			HAL_GPIO_WritePin(Y1_GPIO_Port, Y1_Pin, RESET); // yellow 1 off
			HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, SET); // green 0 on
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET); // red 1 on

			if(timer_flag[0] == 1){
				status = auto_yellow_red;
				setTimer(0, time_red_yellow);
				count0 = time_red_yellow/1000;
				count1 = time_red_yellow/1000;
				updateClockBuffer(count0, count1);
			}
			//-----SWITCHING MANNUAL MODE -----------
			if(isButtonPress(1) == 1){
				status = manual_green_red;
				Diable_Led();
				return;
			}
			break;
		case auto_yellow_red:
			HAL_GPIO_WritePin(Y0_GPIO_Port, Y0_Pin, SET); // yellow 0 on
			HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, RESET); // green 0 off
			if(timer_flag[0] == 1){
				status = auto_red_green;
				setTimer(0, time_red_green);
				count0 = (time_red_green + time_red_yellow)/1000;
				count1 = time_red_green / 1000;
				updateClockBuffer(count0, count1);
			}

			break;
		default: // ----- MANUAL MODE & SETTING MODE & SLOW MODE---------
			return;
	}

	if(timer_flag[2] == 1){
		setTimer(2, 100);
		Scan7SEG();
	}
	if(timer_flag[1] == 1){
		setTimer(1, 1000);
		count0 --; count1 --;
		updateClockBuffer(count0, count1);
	}
	// -------SWITCHING SLOW MODE ---------------
	if(isButtonPress(0)==1){
		single_LED_off();// ------ALL LED OFF----------
		Diable_Led(); //--- Disable led 7 segment ------
		status = SLOW;
		setTimer(0, 500);
		return;
	}
	// -------SWITCHING SETTING MODE ------------
	if(isButtonPress(2) == 1){
		time_red_yellow = 0;
		time_red_green = 0;
		single_LED_off();
		status = set_green;
		setTimer(0, 100);
	}
}