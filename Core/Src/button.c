/*
 * button.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */

#include "button.h"


int buttonFlag[3] = {0,0,0};

void getKeyinput(){
	//-----PROCESS BUTTON 1------------
	if((timer_flag[3] == 1) && (HAL_GPIO_ReadPin(BUTTON_1_GPIO_Port, BUTTON_1_Pin) == 0)){
		buttonFlag[0] = 1;
		setTimer(3, 300);
	}
	// ----PROCESS BUTTON 2-----------
	if((timer_flag[3] == 1) && (HAL_GPIO_ReadPin(BUTTON_2_GPIO_Port, BUTTON_2_Pin) == 0)){
		buttonFlag[1] = 1;
		setTimer(3, 300);
	}
	// ----- PROCESS BUTTON 3 -----------
	if((timer_flag[3] == 1) && (HAL_GPIO_ReadPin(BUTTON_3_GPIO_Port, BUTTON_3_Pin) == 0)){
		setTimer(3, 300);
		buttonFlag[2] = 1;
	}

}
int isButtonPress(int i){
	if(buttonFlag[i] == 1){
		buttonFlag[i] = 0;
		return 1;
	}
	return 0;
}
