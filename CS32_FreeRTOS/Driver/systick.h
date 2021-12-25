#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "cs32f0xx.h"
#include "cs32f0xx_conf.h"

/**
* @file systick.h
* @brief Systick initialization and delay
*/

void cs_start_systick_config(void);
void delay(volatile uint32_t value);
void timing_delay_decrement(void);

#endif 


