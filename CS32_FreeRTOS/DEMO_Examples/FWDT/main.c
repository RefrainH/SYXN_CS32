/**
  * @file    FWDT/main.c 
  * @brief   Main program body
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */
  
#include "main.h"

/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup FWDT
  * @{
  */

volatile uint16_t capture_count = 0;

/**
  * @fn int main(void)
  * @brief  Main program.   MCU clock setting is  configured  through SystemInit() 
  *          in startup file (startup_cs32f0xx.s) before to enter to application main.       
  * @param  None
  * @return None
  */
int main(void)
{  
    cs_start_led2_init();
    cs_start_key_init();
    cs_start_key_nvic_init();
    cs_start_systick_config();
    cs_start_usart_config();
	
    printf("-----------------FWDT_Reset example-----------------\n\r");
    if (rcu_flag_status_get(RCU_FLAG_FWDTRST) != RESET) //Check if the system has resumed from FWDT reset
    {
        printf("FWDT reset\n\r");
    }
	
#ifdef LRC_TIM_MEASURE
    cs_start_tim14_lrc_config();   // TIM Configuration
    while(capture_count != 2); // Wait until the TIM14 get 2 LRC edges
    tim_interrupt_config(TIM14, TIM_INTR_CH1, DISABLE); // Disable TIM14 CC1 Interrupt Request
#endif
	
    cs_start_fwdt_config();
		
    while(1)
    {
        cs_start_led2_toggle();
        delay(220);
        fwdt_reload_counter();
    }
}


/**
  * @}
  */

/**
  * @}
  */

