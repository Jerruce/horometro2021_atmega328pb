/*
 * MT_piezoelectric_sensor.c
 *
 * Created: 21/05/2021 12:07:36
 *  Author: mtorres
 */ 


/* File inclusion */
#include "piezoelectric_sensor.h"


/* Variable definition */

static uint32_t calib_time_step_counter = 0;
static uint32_t calib_time_seconds = 0;

static uint32_t working_time_step_counter = 0;
static uint32_t working_time_hours = 0;
static uint8_t working_time_minutes = 0;
static uint8_t working_time_seconds = 0;

static uint32_t alarm1_setpoint_hours = 0; 
static uint32_t alarm1_time_step_counter = 0;
static uint32_t alarm1_time_hours = 0;
static uint8_t alarm1_time_minutes = 0;
static uint8_t alarm1_time_seconds = 0;

static uint32_t alarm2_setpoint_hours = 0; 
static uint32_t alarm2_time_step_counter = 0;
static uint32_t alarm2_time_hours = 0;
static uint8_t alarm2_time_minutes = 0;
static uint8_t alarm2_time_seconds = 0;

static uint32_t alarm3_setpoint_hours = 0; 
static uint32_t alarm3_time_step_counter = 0;
static uint32_t alarm3_time_hours = 0;
static uint8_t alarm3_time_minutes = 0;
static uint8_t alarm3_time_seconds = 0;

static uint8_t screen_refresh_counter_hours = 0;
static uint8_t screen_refresh_period_index = 0;
static uint8_t screen_refresh_period_hours[DISPLAY_PERIOD_N_OPTIONS] =  {
																				WORKING_COUNT_DISPLAY_PERIOD_01_IN_HOURS,
																				WORKING_COUNT_DISPLAY_PERIOD_02_IN_HOURS,
																				WORKING_COUNT_DISPLAY_PERIOD_03_IN_HOURS
																			};


/* Function definition */

void Piezoelectric_Sensor_Initialize(void){
	
	/* Configure acceleration sensor pin as INPUT and enable the internal pull-up resistor if required */
	DDR_ACCEL_SENSOR &= ~(1 << ACCEL_SENSOR);
	//PORT_ACCEL_SENSOR |= (1 << ACCEL_SENSOR);
	
	/* Configure external interrupt INT0 to activate with every RISING EDGE */
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC01);
		//
	/* Clear the INT0 flag */
	EIFR |= (1 << INTF0);	
}


uint8_t Piezoelectric_Sensor_Read(void){
	
	uint8_t val;
	
	val = EIFR & (1 << INTF0);
	EIFR |= (1 << INTF0);
	
	return val; 	
}


void Calib_Time_Count_Update(void){
	
	calib_time_step_counter++;
	if(calib_time_step_counter >= HOROMETER_SECOND_N_STEPS){
		calib_time_step_counter = 0;
		calib_time_seconds++;
	}

}


void Calib_Time_Reset(void){
	calib_time_step_counter = 0;
	calib_time_seconds = 0;
}

uint32_t Calib_Time_Get(void){
	return calib_time_seconds;
}


void Working_Time_Count_Update(void){
	
	working_time_step_counter++;
	if(working_time_step_counter >= HOROMETER_SECOND_N_STEPS){
		
		working_time_step_counter = 0;
		working_time_seconds++;
		if(working_time_seconds >= 60){
			working_time_seconds = 0;
			working_time_minutes++;
			if(working_time_minutes >= 60){
				working_time_minutes = 0;
				working_time_hours++;
				
				/* Refresh main screen periodically (every 12 hours of work, for example) */
				screen_refresh_counter_hours++;
				if(screen_refresh_counter_hours  >= screen_refresh_period_hours[screen_refresh_period_index]){
					screen_refresh_counter_hours  = 0;
					system_flags &= ~((uint32_t)1 << TOGGLE_SCREEN_INDEX_FLAG);
					system_flags |= ((uint32_t)1 << SHOW_MAIN_OR_ALARM_SCREEN_FLAG);
				}
				
			}
		}
	}
}


void Working_Time_Set(uint32_t hh, uint8_t mm, uint8_t ss){
	working_time_step_counter = 0;
	working_time_hours = hh;
	working_time_minutes = mm;
	working_time_seconds = ss;	
}


void Working_Time_Get(uint32_t *hh, uint8_t *mm, uint8_t *ss){
	*hh = working_time_hours;
	*mm = working_time_minutes;
	*ss = working_time_seconds;
}


void Working_Time_Reset(void){
	
	working_time_step_counter = 0;
	working_time_hours = 0;
	working_time_minutes = 0;
	working_time_seconds = 0;
	
	screen_refresh_counter_hours = 0;	
}


void Alarm1_Time_Count_Update(void){
			
	alarm1_time_step_counter++;
	if(alarm1_time_step_counter >= HOROMETER_SECOND_N_STEPS){
		alarm1_time_step_counter = 0;
		alarm1_time_seconds++;
		if(alarm1_time_seconds >= 60){
			alarm1_time_seconds = 0;
			alarm1_time_minutes++;
			if(alarm1_time_minutes >= 60){
				alarm1_time_minutes = 0;
				alarm1_time_hours++;
				
				if(alarm1_time_hours >= alarm1_setpoint_hours){
					cli();
					system_flags |= ((uint32_t)1 << ALARM_01_REACHED_FLAG);
					sei();
				}
			}
		}
	}
		
}


void Alarm1_Time_Set(uint32_t hh, uint8_t mm, uint8_t ss){
	alarm1_time_step_counter = 0;
	alarm1_time_hours = hh;
	alarm1_time_minutes = mm;
	alarm1_time_seconds = ss;
}


void Alarm1_Time_Get(uint32_t *hh, uint8_t *mm, uint8_t *ss){
	*hh = alarm1_time_hours;
	*mm = alarm1_time_minutes;
	*ss = alarm1_time_seconds;
}


void Alarm1_Time_Reset(void){
	
	alarm1_time_step_counter = 0;
	alarm1_time_hours = 0;
	alarm1_time_minutes = 0;
	alarm1_time_seconds = 0;
}


void Alarm1_Setpoint_Set(uint32_t sp){
	alarm1_setpoint_hours = sp;
}


uint32_t Alarm1_Setpoint_Get(void){
	return alarm1_setpoint_hours;
}


void Alarm2_Time_Count_Update(void){
		
	alarm2_time_step_counter++;
	if(alarm2_time_step_counter >= HOROMETER_SECOND_N_STEPS){
		alarm2_time_step_counter = 0;
		alarm2_time_seconds++;
		if(alarm2_time_seconds >= 60){
			alarm2_time_seconds = 0;
			alarm2_time_minutes++;
			if(alarm2_time_minutes >= 60){
				alarm2_time_minutes = 0;
				alarm2_time_hours++;

				if(alarm2_time_hours >= alarm2_setpoint_hours){
					cli();
					system_flags |= ((uint32_t)1 << ALARM_02_REACHED_FLAG);
					sei();
				}
			}
		}
	}
}


void Alarm2_Time_Set(uint32_t hh, uint8_t mm, uint8_t ss){
	alarm2_time_step_counter = 0;
	alarm2_time_hours = hh;
	alarm2_time_minutes = mm;
	alarm2_time_seconds = ss;
}


void Alarm2_Time_Get(uint32_t *hh, uint8_t *mm, uint8_t *ss){
	*hh = alarm2_time_hours;
	*mm = alarm2_time_minutes;
	*ss = alarm2_time_seconds;
}


void Alarm2_Time_Reset(void){
	
	alarm2_time_step_counter = 0;
	alarm2_time_hours = 0;
	alarm2_time_minutes = 0;
	alarm2_time_seconds = 0;
	
}


void Alarm2_Setpoint_Set(uint32_t sp){
	alarm2_setpoint_hours = sp;
}


uint32_t Alarm2_Setpoint_Get(void){
	return alarm2_setpoint_hours;
}


void Alarm3_Time_Count_Update(void){
		
	alarm3_time_step_counter++;
	if(alarm3_time_step_counter >= HOROMETER_SECOND_N_STEPS){
		alarm3_time_step_counter = 0;
		alarm3_time_seconds++;
		if(alarm3_time_seconds >= 60){
			alarm3_time_seconds = 0;
			alarm3_time_minutes++;
			if(alarm3_time_minutes >= 60){
				alarm3_time_minutes = 0;
				alarm3_time_hours++;

				if(alarm3_time_hours >= alarm3_setpoint_hours){
					cli();
					system_flags |= ((uint32_t)1 << ALARM_03_REACHED_FLAG);
					sei();
				}
			}
		}
	}
}


void Alarm3_Time_Set(uint32_t hh, uint8_t mm, uint8_t ss){
	alarm3_time_step_counter = 0;
	alarm3_time_hours = hh;
	alarm3_time_minutes = mm;
	alarm3_time_seconds = ss;
}


void Alarm3_Time_Get(uint32_t *hh, uint8_t *mm, uint8_t *ss){
	*hh = alarm3_time_hours;
	*mm = alarm3_time_minutes;
	*ss = alarm3_time_seconds;
}


void Alarm3_Time_Reset(void){
	
	alarm3_time_step_counter = 0;
	alarm3_time_hours = 0;
	alarm3_time_minutes = 0;
	alarm3_time_seconds = 0;
}


void Alarm3_Setpoint_Set(uint32_t sp){
	alarm3_setpoint_hours = sp;
}


uint32_t Alarm3_Setpoint_Get(void){
	return alarm3_setpoint_hours;
}


void Screen_Refresh_Period_Set(uint8_t period_index){
	
	if(period_index > (DISPLAY_PERIOD_N_OPTIONS - 1)){
		screen_refresh_period_index = DISPLAY_PERIOD_N_OPTIONS - 1;
	}else{
		screen_refresh_period_index = period_index;	
	}
}


uint8_t Screen_Refresh_Period_Get(void){
	return screen_refresh_period_index;
}
