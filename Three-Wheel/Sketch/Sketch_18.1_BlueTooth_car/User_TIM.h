#ifndef _USER_TIM_H_
#define _USER_TIM_H_

#include "Bluetooth.h"
#include "Encoder.h"
#include <RPi_Pico_TimerInterrupt.h>
#include "IR.h" 

#define TIMER0_INTERVAL_MS 1000

bool timer_1ms_control(struct repeating_timer *t);
void tim_init();

#endif