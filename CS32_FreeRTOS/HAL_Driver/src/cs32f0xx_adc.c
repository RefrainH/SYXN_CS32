/**
  * @file    cs32f0xx_adc.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC) peripheral:
  *           + Initialization and Configuration
  *           + Power saving
  *           + Analog Watchdog configuration
  *           + Temperature Sensor, Vrefint (Internal Reference Voltage) and 
  *             Vbat (Voltage battery) management 
  *           + ADC Channels Configuration
  *           + ADC Channels DMA Configuration
  *           + Interrupts and flags management
  *
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  *  @verbatim
  ----------------------- this driver instruction for use--------------------------- 
    1.ADC clock configuration:
      a. Enable the ADC interface clock using 
        rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_ADC, ENABLE); 
      b. ADC pins configuration:
       (1) Enable the clock for the ADC GPIOs using the following function:
            rcu_ahb_periph_clock_enable_ctrl(RCC_AHBPeriph_GPIOx, ENABLE);   
       (2) Configure these ADC pins in analog mode using gpio_init();  
      c. Configure the ADC conversion resolution, data alignment, external
        trigger and edge, scan direction and Enable/Disable the continuous mode
        using the adc_init() function.
      d. Activate the ADC peripheral using adc_enable_ctrl() function.

    2.ADC channels group configuration:
      a. To configure the ADC channels features, use adc_init() and 
        adc_channel_config() functions.
      b. To activate the continuous mode, use the adc_continuous_mode_enable_ctrl()
        function.
      c. To activate the Discontinuous mode, use the adc_discontinuous_mode_enable_ctrl() functions. 
      d. To activate the overrun mode, use the adc_overrun_mode_enable_ctrl() functions.
      e. To activate the calibration mode, use the adc_calibration_value_get() functions.
      f. To read the ADC converted values, use the adc_conversion_value_get()
        function.

    3.DMA for ADC channels features configuration:
      a. To enable the DMA mode for ADC channels group, use the adc_dma_enable_ctrl() function.
      b. To configure the DMA transfer request, use adc_dma_mode_set() function.
  *  @endverbatim
  *
  */

#include "cs32f0xx_adc.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup ADC
  * @{
  */



/** 
  * @brief ADC CFG mask definition
  */ 
#define ADC_CFG_CLEAR_MASK           ((uint32_t)0xFFFFD203)

/** 
  * @brief ADC Calibration time out definition  
  */ 
#define ADC_CALIBRATION_TIMEOUT       ((uint32_t)0x0000F000)


/**
  * @fn void adc_def_init(adc_reg_t* ptr_adc)
  * @brief  Deinitializes ADC1 peripheral registers to their default reset values.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_def_init(adc_reg_t* ptr_adc)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));

    if (ptr_adc == ADC1)
    {
        /* Enable ADC1 reset state */
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_ADC, ENABLE);

        /* Release ADC1 from reset state */
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_ADC, DISABLE);
#if defined (CS32F030) || defined (CS32F031)		
        ptr_adc->STAT = 0x00000000;
        ptr_adc->INTEN = 0x00000000;
        ptr_adc->CTR = 0x00000000;
        ptr_adc->CFG = 0x00000000;
        ptr_adc->CLK = 0x00000000;
        ptr_adc->SMPLR = 0x00000000;
        ptr_adc->WDTH = 0x0FFF0000;
        ptr_adc->CHANSEL = 0x00000000;
        ptr_adc->OUTDAT = 0x00000000;
        ptr_adc->INTEN = 0x00000000;
#endif
    }
}

/**
  * @fn void adc_init(adc_reg_t* ptr_adc, adc_config_t* ptr_config)
  * @brief  Initializes the ADCx peripheral according to the specified parameters.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  ptr_config: pointer to an adc_config_t structure that contains 
  *         the configuration information for the specified ADC peripheral.
  * @return None
  */
void adc_init(adc_reg_t* ptr_adc, adc_config_t* ptr_config)
{
    uint32_t reg = 0;

    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_CONV_RES_CHECK(ptr_config->adc_resolution));
    ASSERT(FUNCTIONAL_STATE_CHECK(ptr_config->conversion_mode));
    ASSERT(ADC_TRIG_MODE_SEL_CHECK(ptr_config->trigger_mode));
    ASSERT(ADC_HW_TRIG_SEL_CHECK(ptr_config->hardware_trigger));
    ASSERT(ADC_DATA_ALIGN_CHECK(ptr_config->data_align));
    ASSERT(ADC_CONV_SEQ_DIR_CHECK(ptr_config->scan_direction)); 

    reg = ptr_adc->CFG;

    /* Clear CGDIR, DATRES[1:0], DATALG, HTRGSEL[2:0], TRGMODE[1:0] and GCONT bits */
    reg &= ADC_CFG_CLEAR_MASK;
    /* Config ADCx CFG register */
    reg  |= (uint32_t)(ptr_config->adc_resolution | ((uint32_t)(ptr_config->conversion_mode) << 13) |
             ptr_config->trigger_mode | ptr_config->hardware_trigger |
             ptr_config->data_align | ptr_config->scan_direction);

    ptr_adc->CFG = reg;
}

/**
  * @fn void adc_config_struct_init(adc_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its default value.
  * @param  ptr_config: pointer to an adc_config_t structure which will be initialized.
  * @return None
  */
void adc_config_struct_init(adc_config_t* ptr_config)
{
    /* Initialize the adc_resolution parameter */
    ptr_config->adc_resolution = ADC_CONV_RES_12BITS;
    ptr_config->conversion_mode = DISABLE;
    ptr_config->trigger_mode = ADC_TRIG_MODE_SEL_NONE;
    ptr_config->hardware_trigger = ADC_HW_TRIG_SEL_T1_TRIG0;
    ptr_config->data_align = ADC_DATA_ALIGN_RIGHT;
    ptr_config->scan_direction = ADC_CONV_SEQ_DIR_UPWARD;
}

/**
  * @fn void adc_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void adc_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{

    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the ADC  */
        ptr_adc->CTR |= (uint32_t)(ADC_CTR_ADCON);
    }
    else
    {
        /*  Disable the ADC  */
        ptr_adc->CTR |= (uint32_t)ADC_CTR_ADCOFF;
    }
}

/**
  * @fn void adc_clock_select(adc_reg_t* ptr_adc, uint32_t value)
  * @brief Select the ADC to either be clocked by the asynchronous clock(which is independent,
  *        the dedicated 14MHz clock) or the synchronous clock derived from the APB clock of the
  *        ADC bus interface divided by 2 or 4.It can be called only when ADC is disabled.
  * @param ptr_adc: pointer to ADCx where x can be 1.
  * @param value: It can be: 
  *          @arg ADC_CLOCK_SEL_ASYN(ADC clocked by the dedicated 14MHz clock)
  *          @arg ADC_CLOCK_SEL_SYN_DIV2(ADC clocked by PCLK/2)
  *          @arg ADC_CLOCK_SEL_SYN_DIV4(ADC clocked by PCLK/4) 
  * @return None
  */
void adc_clock_select(adc_reg_t* ptr_adc, uint32_t value)
{  
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_CLOCK_SEL_CHECK(value));

    /* Configure ADC clock source  */
    /* Configure ADC clock source  */
    ptr_adc->CLK = (uint32_t)value;
}

#ifdef CS32F036
/**
  * @fn void adc_auto_standby_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the ADC Auto standby. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_auto_standby_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
  if (enable_flag != DISABLE)
  {
    /* Enable the ADC Auto standby */
    ptr_adc->CFG |= ADC_CFG_ATSTDBY;
  }
  else
  {
    /* Disable the ADC Auto standby */
    ptr_adc->CFG &= (uint32_t)~ADC_CFG_ATSTDBY;
  }
}

/**
  * @fn void adc_conversion_pause_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables conversion pause mode. When the CPU clock is not fast
  *         enough to manage the data rate, a Hardware delay can be introduced between
  *         ADC conversions to reduce this data rate. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_conversion_pause_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
  if (enable_flag != DISABLE)
  {
    /* Enable the ADC conversion pause */
    ptr_adc->CFG |= ADC_CFG_PAUSE;
  }
  else
  {
    /* Disable the ADC conversion pause */
    ptr_adc->CFG &= (uint32_t)~ADC_CFG_PAUSE;
  }
}

/**
  * @fn void adc_watchdog_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the analog watchdog 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_watchdog_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
  if (enable_flag != DISABLE)
  {
    /* Enable the ADC Analog Watchdog */
    ptr_adc->CFG |= ADC_CFG_WDGEN;
  }
  else
  {
    /* Disable the ADC Analog Watchdog */
    ptr_adc->CFG &= (uint32_t)~ADC_CFG_WDGEN;
  }
}

/**
  * @fn void adc_watchdog_thresholds_set(adc_reg_t* ptr_adc, uint16_t high, uint16_t low) 
  * @brief  Configures the high and low thresholds of the analog watchdog. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  high: the ADC analog watchdog High threshold value( 12bit value).
  * @param  low: the ADC analog watchdog Low threshold value( 12bit value).
  * @return None
  */
void adc_watchdog_thresholds_set(adc_reg_t* ptr_adc, uint16_t high, uint16_t low)                                       
{
  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(ADC_THRESHOLD_CHECK(high));
  ASSERT(ADC_THRESHOLD_CHECK(low));

  /* Set the ADC high and low threshold */
  ptr_adc->WDTH = low | ((uint32_t)high << 16);

}

/**
  * @fn void adc_watchdog_channel_select(adc_reg_t* ptr_adc, uint32_t value)
  * @brief  Selection the analog watchdog monitor channel .
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  value: the ADC channel to configure for the analog watchdog.
  *         It can be a values between ADC_AN_WDG_SEL_CH0 ~ ADC_AN_WDG_SEL_CH18  
  *          (ADC Channel1 ~ ADC Channel18).
  * @note   The channel selected on the WDGCHAN must be also set into the CHANSEL register.     
  * @return None
  */
void adc_watchdog_channel_select(adc_reg_t* ptr_adc, uint32_t value)
{
  uint32_t reg = 0;

  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(ADC_AN_WDG_SEL_CHECK(value));

  reg = ptr_adc->CFG;

  /* Clear the Analog watchdog channel select bits */
  reg &= ~ADC_CFG_WDGCHAN;
  /* Set the Analog watchdog channel value */
  reg |= value;
  /* Write to CFG */
  ptr_adc->CFG = reg;
}

/**
  * @fn void adc_watchdog_channel_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the ADC Analog channel monitor mode.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_watchdog_channel_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
  ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

  if (enable_flag != DISABLE)
  {
    /* Enable the ADC Analog Watchdog channel monitor mode */
    ptr_adc->CFG |= ADC_CFG_WDGCHMODE;
  }
  else
  {
    /* Disable the ADC Analog Watchdog channel monitor mode */
    ptr_adc->CFG &= (uint32_t)~ADC_CFG_WDGCHMODE;
  }
}
#endif

/**
  * @fn void adc_temp_sensor_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the temperature sensor channel.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_temp_sensor_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the temperature sensor */
        ADC->INNCHEN |= (uint32_t)ADC_INNCHEN_TSEN;
    }
    else
    {
        /* Disable the temperature sensor */
        ADC->INNCHEN &= (uint32_t)(~ADC_INNCHEN_TSEN);
    }
}

/**
  * @fn void adc_vrefint_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Vrefint channel.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_vrefint_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the Vrefint channel*/
        ADC->INNCHEN |= (uint32_t)ADC_INNCHEN_VREFINTEN;
    }
    else
    {
    /* Disable the Vrefint channel*/
        ADC->INNCHEN &= (uint32_t)(~ADC_INNCHEN_VREFINTEN);
    }
}

/**
  * @fn void adc_vbat_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Vbat channel. 
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_vbat_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the Vbat channel*/
        ADC->INNCHEN |= (uint32_t)ADC_INNCHEN_VBATMEN;
    }
    else
    {
        /* Disable the Vbat channel*/
        ADC->INNCHEN &= (uint32_t)(~ADC_INNCHEN_VBATMEN);
    }
}

/**
  * @fn void adc_channel_config(adc_reg_t* ptr_adc, uint32_t channel, uint32_t time)
  * @brief  Selection ADC conversion channel and its sampling time.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  channel: the ADC channel to configure , it can be any combination of the following values:
  *          @arg ADC_CONV_CHANNEL_0(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_1(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_2(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_3(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_4(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_5(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_6(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_7(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_8(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_9(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_10(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_11(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_12(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_13(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_14(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_15(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_16(ADC Channel0 selected)
  *          @arg ADC_CONV_CHANNEL_17(ADC Channel0 selected)
  * @param  time: The sample time value to be set. It can be one of the following values:
  *          @arg ADC_SAMPLE_TIMES_1_5(Sample time equal to 1.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_7_5(Sample time equal to 7.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_13_5(Sample time equal to 13.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_28_5(Sample time equal to 28.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_41_5(Sample time equal to 41.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_55_5(Sample time equal to 55.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_71_5(Sample time equal to 71.5 cycles)
  *          @arg ADC_SAMPLE_TIMES_239_5(Sample time equal to 239.5 cycles)
  * @return None
  */
void adc_channel_config(adc_reg_t* ptr_adc, uint32_t channel, uint32_t time)
{
    uint32_t reg = 0;

    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_CONV_CHANNEL_CHECK(channel));
    ASSERT(ADC_SAMPLE_TIMES_CHECK(time));

    /* Configure the ADC Channel */
    ptr_adc->CHANSEL |= (uint32_t)channel;
    
    /* Clear the Sampling time Selection bits */
    reg &= ~ADC_SMPLR_SMPLT;
  
    /* Set the Sampling time register */
    reg |= (uint32_t)time;
    ptr_adc->SMPLR = reg ;
}

/**
  * @fn void adc_continuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the Continuous mode for the selected ADCx channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_continuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the Continuous mode*/
        ptr_adc->CFG |= (uint32_t)ADC_CFG_GCONT;
    }
    else
    {
        /* Disable the Continuous mode */
        ptr_adc->CFG &= (uint32_t)(~ADC_CFG_GCONT);
    }
}

/**
  * @fn void adc_discontinuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the discontinuous mode for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_discontinuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the Discontinuous mode */
        ptr_adc->CFG |= (uint32_t)ADC_CFG_DISCONT;
    }
    else
    {
        /* Disable the Discontinuous mode */
        ptr_adc->CFG &= (uint32_t)(~ADC_CFG_DISCONT);
    }
}

/**
  * @fn void adc_overrun_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the Overrun mode for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_overrun_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the Overrun mode */
        ptr_adc->CFG |= (uint32_t)ADC_CFG_DOVRWRT;
    }
    else
    {
        /* Disable the Overrun mode */
        ptr_adc->CFG &= (uint32_t)(~ADC_CFG_DOVRWRT);
    }
}

/**
  * @fn uint32_t adc_calibration_value_get(adc_reg_t* ptr_adc)
  * @brief  Active the Calibration operation for the selected ADC, the Calibration can be 
  *         initiated only when ADC is still in the reset configuration (ADCON must be equal to 0).    
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return ADC Calibration factor 
  */
uint32_t adc_calibration_value_get(adc_reg_t* ptr_adc)
{
    uint32_t reg = 0;
    uint32_t count = 0;
    uint32_t status = 0;

    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  
    /* Set the ADC calibartion */
    ptr_adc->CTR |= (uint32_t)(ADC_CTR_CALB);
  
    /* Wait until no ADC calibration is completed */
    do
    {
        status = ptr_adc->CTR & ADC_CTR_CALB;
        count++;  
    } while ((count != ADC_CALIBRATION_TIMEOUT) && (status != 0x00));
    
    if ((uint32_t)(ptr_adc->CTR & ADC_CTR_CALB) == RESET)
    {
        /*Get the calibration data */
        reg = ptr_adc->OUTDAT;
    }
    else
    {
        /* Error factor */
        reg = 0x00000000;
    }
    return reg;
}

/**
  * @fn void adc_conversion_stop(adc_reg_t* ptr_adc)
  * @brief  Stop the on going conversions for the selected ADC. When ADSTOP is set, any on going
  *         conversion is aborted, and the ADC data register is not updated with current conversion.    
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_conversion_stop(adc_reg_t* ptr_adc)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ptr_adc->CTR |= (uint32_t)ADC_CTR_ADSTOP;
}

/**
  * @fn void adc_conversion_start(adc_reg_t* ptr_adc)
  * @brief  Start Conversion for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_conversion_start(adc_reg_t* ptr_adc)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
  
    ptr_adc->CTR |= (uint32_t)ADC_CTR_ADSTOP; 
    ptr_adc->CTR |= (uint32_t)ADC_CTR_ADSTRT;
}

/**
  * @fn uint16_t adc_conversion_value_get(adc_reg_t* ptr_adc)
  * @brief  Returns the last ADCx conversion result data for ADC channel.  
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return The Data conversion value.
  */
uint16_t adc_conversion_value_get(adc_reg_t* ptr_adc)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));

    return (uint16_t) ptr_adc->OUTDAT;
}

/**
  * @fn void adc_dma_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the specified ADC DMA request.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_dma_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the selected ADC DMA  */
        ptr_adc->CFG |= (uint32_t)ADC_CFG_DMAEN;
    }
    else
    {
        /* Disable the selected ADC DMA  */
        ptr_adc->CFG &= (uint32_t)(~ADC_CFG_DMAEN);
    }
}

/**
  * @fn void adc_dma_mode_set(adc_reg_t* ptr_adc, uint32_t mode)
  * @brief  Enables or disables the ADC DMA request after last transfer (Single-ADC mode)
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  mode: the ADC channel to configure, it can be
  *          @arg ADC_DMA_MODE_SINGLE(DMA single Mode)
  *          @arg ADC_DMA_MODE_CIRCULAR (DMA Circular Mode)
  *  @return None
  */
void adc_dma_mode_set(adc_reg_t* ptr_adc, uint32_t mode)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));

    ptr_adc->CFG &= (uint32_t)~ADC_CFG_DMAMODE;
    ptr_adc->CFG |= (uint32_t)mode;
}

/**
  * @fn void adc_interrupt_config(adc_reg_t* ptr_adc, uint32_t value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified ADC interrupts.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  value: specifies the ADC interrupt sources to be enabled or disabled.
  *          It can be one of the following values:
  *          @arg ADC_INTR_EOI(End of ADC initialization interrupt)
  *          @arg ADC_INTR_EOSPIE(End of sampling phase interrupt)
  *          @arg ADC_INTR_EOC(End of channel conversion interrupt)
  *          @arg ADC_INTR_EOG(End of conversion group)
  *          @arg ADC_INTR_DOVRIE(ADC data overrun interrupt)
  *          @arg ADC_INTR_WDEVT(analog watchdog event interrupt)
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_interrupt_config(adc_reg_t* ptr_adc, uint32_t value, enable_state_t enable_flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    ASSERT(ADC_CONFIG_INTR_CHECK(value)); 

    if (enable_flag != DISABLE)
    {
        /* Enable the selected ADC interrupts */
        ptr_adc->INTEN |= value;
    }
    else
    {
        /* Disable the selected ADC interrupts */
        ptr_adc->INTEN &= (~(uint32_t)value);
    }
}

/**
  * @fn bit_status_t adc_flag_status_get(adc_reg_t* ptr_adc, uint32_t flag)
  * @brief  Checks whether the specified ADC flag is set or not.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *          @arg ADC_FLAG_WDEVT(analog watchdog event flag)
  *          @arg ADC_FLAG_DOVR(ADC data overrun flag)
  *          @arg ADC_FLAG_EOCG(End of conversion group flag)
  *          @arg ADC_FLAG_EOCH(End of channel conversion flag)
  *          @arg ADC_FLAG_EOSP(End of sampling phase flag)
  *          @arg ADC_FLAG_EOI(End of ADC initialization)
  *          @arg ADC_FLAG_ADCON(ADC power on)
  *          @arg ADC_FLAG_ADCOFF(ADC power off)
  *          @arg ADC_FLAG_ADSTRT(ADC start conversion)
  *          @arg ADC_FLAG_ADSTOP(ADC stop conversion)
  *          @arg ADC_FLAG_CALB(ADC calibration control)
  * @return SET or RESET.
  */
bit_status_t adc_flag_status_get(adc_reg_t* ptr_adc, uint32_t flag)
{
    bit_status_t status = RESET;
    uint32_t reg = 0;
    
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_GET_FLAG_CHECK(flag));

    if ((uint32_t)(flag & 0x01000000))
    {
        reg = ptr_adc->CTR & 0xFEFFFFFF;
    }
    else
    {
        reg = ptr_adc->STAT;
    }
  
    /* Check ADC  status   */
    if ((reg & flag) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
  
    return  status;
}

/**
  * @fn void adc_flag_clear(adc_reg_t* ptr_adc, uint32_t flag)
  * @brief  Clears the ADCx's flags.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  flag: specifies the flag to clear, it can be any combination of the following values:
  *          @arg ADC_FLAG_WDEVT(analog watchdog event flag)
  *          @arg ADC_FLAG_DOVR(ADC data overrun flag)
  *          @arg ADC_FLAG_EOCG(End of conversion group flag)
  *          @arg ADC_FLAG_EOCH(End of channel conversion flag)
  *          @arg ADC_FLAG_EOSP(End of sampling phase flag)
  *          @arg ADC_FLAG_EOI(End of ADC initialization)
  * @return None
  */
void adc_flag_clear(adc_reg_t* ptr_adc, uint32_t flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_CLEAR_FLAG_CHECK(flag));

    ptr_adc->STAT = (uint32_t)flag;
}

/**
  * @fn bit_status_t adc_intetrrupt_status_get(adc_reg_t* ptr_adc, uint32_t flag)
  * @brief  Checks whether the specified ADC interrupt has occurred or not.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  flag: specifies the ADC interrupt source to check.It can be one of the following values:
  *          @arg ADC_INTR_EOI(End of ADC initialization interrupt)
  *          @arg ADC_INTR_EOSPIE(End of sampling phase interrupt)
  *          @arg ADC_INTR_EOC(End of channel conversion interrupt)
  *          @arg ADC_INTR_EOG(End of conversion group)
  *          @arg ADC_INTR_DOVRIE(ADC data overrun interrupt)
  *          @arg ADC_INTR_WDEVT(analog watchdog event interrupt)    
  * @return SET or RESET.
  */
bit_status_t adc_intetrrupt_status_get(adc_reg_t* ptr_adc, uint32_t flag)
{
    bit_status_t status = RESET;
    uint32_t temp = 0;

    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_GET_INTR_CHECK(flag));

    /* Get the ADC interrupts enable bit status */
    temp = (uint32_t)(ptr_adc->INTEN & flag); 

    /* Check the status of the specified ADC interrupt */
    if (((uint32_t)(ptr_adc->STAT & flag) != (uint32_t)RESET) && (temp != (uint32_t)RESET))
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
  
    return  status;
}

/**
  * @fn void adc_intetrrupt_flag_clear(adc_reg_t* ptr_adc, uint32_t flag)
  * @brief  Clears the ADCx's interrupt bits.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  flag: specifies the ADC interrupt bit to clear, it can be one of the following values:
  *          @arg ADC_INTR_EOI(End of ADC initialization interrupt);
  *          @arg ADC_INTR_EOSPIE(End of sampling phase interrupt);
  *          @arg ADC_INTR_EOC(End of channel conversion interrupt);
  *          @arg ADC_INTR_EOG(End of conversion group);
  *          @arg ADC_INTR_DOVRIE(ADC data overrun interrupt);
  *          @arg ADC_INTR_WDEVT(analog watchdog event interrupt); 
  * @return None
  */
void adc_intetrrupt_flag_clear(adc_reg_t* ptr_adc, uint32_t flag)
{
    ASSERT(ADC_ALL_LIST_CHECK(ptr_adc));
    ASSERT(ADC_CLEAR_INTR_CHECK(flag));

    ptr_adc->STAT = (uint32_t)flag; 
}


/**
  * @}
  */ 

/**
  * @}
  */ 

