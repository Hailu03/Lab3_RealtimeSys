#include "software_timer.h"

int timer_counter[10];
int timer_flag[10];

//Set the counter for software timer
void setTimer(int index, int value){
	timer_counter[index] = value;
	timer_flag[index]  = 0;
}


//Check if the timer is expired
int isTimerExpired(int index){
	if(timer_flag[index] == 1){
		timer_flag[index] = 0;
		return 1;
	}
	return 0;
}

//Function to be invoked in Timer_ISR
void timerRun(){
	for(int i = 0; i < 10; i++){
		if(timer_counter[i] > 0){
			timer_counter[i] --;
			if(timer_counter[i] == 0) timer_flag[i] = 1;
		}
	}
}

