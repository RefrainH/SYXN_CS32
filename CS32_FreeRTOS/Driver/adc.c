#include "adc.h"

/**
  * @fn  void cs_start_adc_config(void)
  * @brief  ADC Configuration
  * @param  None
  * @return None
  */
void cs_start_adc_config(void)
{
    adc_config_t   adc_config_struct;
    gpio_config_t  gpio_config_struct;

    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTA, ENABLE); //GPIOA  clock enable.    
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_ADC, ENABLE); //ADC1  clock enable.

    //Configure ADC CH0 GPIO as analog input.
    gpio_config_struct.gpio_pin = GPIO_PIN_0 ;
    gpio_config_struct.gpio_mode = GPIO_MODE_AN;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL ;
    gpio_init(GPIOA, &gpio_config_struct);

    adc_def_init(ADC1);   
    adc_config_struct_init(&adc_config_struct);  

    // Configure the ADC1 in continuous mode 
    adc_config_struct.adc_resolution = ADC_CONV_RES_12BITS;
    adc_config_struct.conversion_mode = ENABLE; 
    adc_config_struct.trigger_mode = ADC_TRIG_MODE_SEL_NONE;
    adc_config_struct.data_align = ADC_DATA_ALIGN_RIGHT;
    adc_config_struct.scan_direction = ADC_CONV_SEQ_DIR_UPWARD;
    adc_init(ADC1, &adc_config_struct); 

    // Set the ADC1 CH0 with 239.5 Cycles
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_0 , ADC_SAMPLE_TIMES_239_5); 
    adc_calibration_value_get(ADC1); // ADC Calibration.  
    adc_enable_ctrl(ADC1, ENABLE);  // Enable the ADC.    

    while(!adc_flag_status_get(ADC1, ADC_FLAG_EOI)); // Wait the EOI flag.    
    adc_conversion_start(ADC1);  //ADC1 regular Software Start Conv. 
}

    
