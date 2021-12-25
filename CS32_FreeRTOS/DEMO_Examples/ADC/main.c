/**
  * @file    ADC/main.c 
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
  
/** @addtogroup ADC
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
    uint32_t sample_value;
    uint32_t real_value;
    cs_start_usart_config();
    cs_start_adc_config();
    printf("--------ADC DEMO--------\n\r");

    while(1)
    {
        while(adc_flag_status_get(ADC1, ADC_FLAG_EOCH) == RESET); // Check EOC flag	
        sample_value = adc_conversion_value_get(ADC1);  
        real_value = (sample_value *3300)/0xFFF;     
		
        printf("ADC_CH0 = %d mV.\n\r",real_value);
    }	
}

/**
  * @}
  */

/**
  * @}
  */

