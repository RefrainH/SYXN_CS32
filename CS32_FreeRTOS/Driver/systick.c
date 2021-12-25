#include "systick.h"
#include "cs32f0xx.h"
/**
* @file systick.c
* @brief Systick initialization and delay
*/

extern uint32_t SystemCoreClock;
static volatile uint32_t timing_delay = 0;

/**
  * @fn void cs_start_systick_config(void)
  * @brief  Configuration systick. 
  * @param  None
  * @return None
  */

void cs_start_systick_config(void)
{
    SysTick_Config(SystemCoreClock / 1000);//Setup SysTick Timer for 1 msec interrupts 
}

/**
  * @fn void delay(volatile uint32_t value)
  * @brief  Inserts a delay time.
  * @param  value: specifies the delay time length, in 10 ms.
  * @return None
  */
void delay(volatile uint32_t value)
{
    timing_delay = value;

    while(timing_delay != 0);
}

/**
  * @fn void TimingDelay_Decrement(void)
  * @brief  Decrements the timing_delay variable.
  * @param  None
  * @return None
  */
void timing_delay_decrement(void)
{
    if (timing_delay != 0x00)
    {
        timing_delay--;
    }
}

