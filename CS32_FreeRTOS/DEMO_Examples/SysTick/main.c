/**
  * @file    SysTick/main.c 
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
  
/** @addtogroup SysTick 
  * @{
  */


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
    cs_start_led2_on();
    cs_start_systick_config();
    while (1)
    {
        cs_start_led2_toggle();
        delay(50);
    }
}


/**
  * @}
  */

/**
  * @}
  */

