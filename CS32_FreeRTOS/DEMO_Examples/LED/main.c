/**
  * @file    LED/main.c 
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
  
/** @addtogroup LED
  * @{
  */

void delay_simple(void);


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
    while (1)
    {
        delay_simple();
        cs_start_led2_on();
        delay_simple();
        cs_start_led2_off();
    }
}

/**
  * @fn void delay(void)
  * @brief  Simple delay      
  * @param  None
  * @return None
  */
void delay_simple(void)
{
    int i,j;
    for(i=0;i<1000;i++)
    {
        for(j=0;j<1000;j++);
    }
}

/**
  * @}
  */

/**
  * @}
  */

