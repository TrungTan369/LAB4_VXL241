/*
 * fsm_manual.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */
#include "fsm_manual.h"

void fsm_manual(){
	switch (status) {
		case manual_red_green:
			HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, SET);
			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, SET);
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, RESET);
			HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, RESET);
			if(isButtonPress(1) == 1){
				status = manual_green_red;
			}
			if(isButtonPress(0) == 1){
				status = auto_red_green;
				count0 = (time_red_green + time_red_yellow)/1000;
				count1 = time_red_green / 1000;
				setTimer(0, time_red_green);
				return;
			}
			break;
		case manual_green_red:
			HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, SET);
			HAL_GPIO_WritePin(G0_GPIO_Port, G0_Pin, SET);
			HAL_GPIO_WritePin(R0_GPIO_Port, R0_Pin, RESET);
			HAL_GPIO_WritePin(G1_GPIO_Port, G1_Pin, RESET);
			if(isButtonPress(1) == 1){
				status = manual_red_green;
			}
			if(isButtonPress(0) == 1){
				status = auto_green_red;
				count0 = time_red_green/1000;
				count1 =   (time_red_green + time_red_yellow)/1000;
				setTimer(0, time_red_green);
				return;
			}
			break;
		default:
			return;
	}

	// -------SWITCHING SETTING MODE ------------
	if(isButtonPress(2) == 1){
		setTimer(0, 100);
		single_LED_off();
		status = set_green;
		time_red_green = 0;
		time_red_yellow = 0;
	}
}
