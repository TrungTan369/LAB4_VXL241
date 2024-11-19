/*
 * fsm_slow.c
 *
 *  Created on: Nov 19, 2024
 *      Author: HOME
 */

#include "fsm_slow.h"

void fsm_slow_run(){
	if(status == SLOW){
		if(timer_flag[0]==1){
			HAL_GPIO_TogglePin(Y0_GPIO_Port, Y0_Pin);
			HAL_GPIO_TogglePin(Y1_GPIO_Port, Y1_Pin);
			setTimer(0, 500);
		}
		if(isButtonPress(0)==1){
			single_LED_off();
			status = auto_red_green;
			count0 = (time_red_green + time_red_yellow)/1000;
			count1 = time_red_green / 1000;
			setTimer(0, time_red_green);
		}
	}
	else
		return;
}
