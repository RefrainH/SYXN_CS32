/**
  * @file    TIME_BASE/main.c 
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
  
/** @addtogroup TIM_BASE
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
    cs_start_base_time_config();
	
    cs_start_gpio_init(GPIOA,0);
    cs_start_gpio_init(GPIOA,1);
    cs_start_gpio_init(GPIOA,2);
    cs_start_gpio_init(GPIOA,3);
	
    while(1)
    {
		
    }
	
}


/**
  * @}
  */

/**
  * @}
  */

