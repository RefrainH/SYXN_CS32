/**
  * @file    cs32f0xx_adc.h
  * @brief   This file contains all the functions prototypes for the ADC firmware 
  *          library
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_ADC_H__
#define __CS32F0XX_ADC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup ADC
  * @{
  */



/** 
  * @struct adc_config_t
  * @brief ADC config struct definition  
  */ 
typedef struct
{
    uint32_t adc_resolution;        //!< Selects the resolution of the conversion                                         
    enable_state_t conversion_mode; //!< Specifies whether the conversion is performed in Continuous or Single mode   
    uint8_t reserved0;
    uint16_t reserved1;    
    uint32_t trigger_mode;          //!< Selects the external trigger Edge and enables the trigger of a regular group
    uint32_t hardware_trigger;      //!< Defines the hardware trigger used to start the analog to digital conversion of regular channels
    uint32_t data_align;            //!< Specifies whether the ADC data alignment is left or right
    uint32_t scan_direction;        //!< Specifies in which direction the channels will be conversion in the sequence                                                  
}adc_config_t;



/** 
  * @brief Check ADC list definition  
  */ 
#define ADC_ALL_LIST_CHECK(bus)                  ((bus) == ADC1)

/**
  * @brief ADC clock source selection definition
  */ 
#define ADC_CLOCK_SEL_ASYN             ((uint32_t)0x00000000)   //!< ADC Asynchronous clock mode 
#define ADC_CLOCK_SEL_SYN_DIV2          ADC_CLK_CKSRC_0         //!< Synchronous clock mode divided by 2 
#define ADC_CLOCK_SEL_SYN_DIV4          ADC_CLK_CKSRC_1         //!< Synchronous clock mode divided by 4 
#define ADC_CLOCK_SEL_CHECK(value) (((value) == ADC_CLOCK_SEL_ASYN)  || \
                                    ((value) == ADC_CLOCK_SEL_SYN_DIV2) || \
                                    ((value) == ADC_CLOCK_SEL_SYN_DIV4))
/** 
  * @brief ADC Conversion data reselution definition
  */ 
#define ADC_CONV_RES_12BITS      ((uint32_t)0x00000000)   //!< Conversion data resolution is 12 bits
#define ADC_CONV_RES_10BITS       ADC_CFG_DATRES_0        //!< Conversion data resolution is 10 bits
#define ADC_CONV_RES_8BITS        ADC_CFG_DATRES_1        //!< Conversion data resolution is 8 bits
#define ADC_CONV_RES_6BITS        ADC_CFG_DATRES          //!< Conversion data resolution is 6 bits

#define ADC_CONV_RES_CHECK(value) (((value) == ADC_CONV_RES_12BITS)||((value) == ADC_CONV_RES_10BITS) || \
                                  ((value) == ADC_CONV_RES_8BITS) || ((value) == ADC_CONV_RES_6BITS))
                                  
                                       
/**  
  * @brief ADC Converstion Trigger mode selection definition
  */ 
#define ADC_TRIG_MODE_SEL_NONE                 ((uint32_t)0x00000000) //!< Disable ADC conversion trigger mode 
#define ADC_TRIG_MODE_SEL_RISING               ADC_CFG_TRGMODE_0      //!< ADC conversion trigger signal is rising edge 
#define ADC_TRIG_MODE_SEL_FALLING              ADC_CFG_TRGMODE_1      //!< ADC conversion trigger signal is falling edge 
#define ADC_TRIG_MODE_SEL_RISING_FALLING       ADC_CFG_TRGMODE        //!< ADC conversion trigger signal is rising_falling 

#define ADC_TRIG_MODE_SEL_CHECK(edge) (((edge) == ADC_TRIG_MODE_SEL_NONE) || \
                                       ((edge) == ADC_TRIG_MODE_SEL_RISING) || \
																			 ((edge) == ADC_TRIG_MODE_SEL_FALLING) || \
																			 ((edge) == ADC_TRIG_MODE_SEL_RISING_FALLING))
                                                                                                              
/** 
  * @brief ADC Hardware trigger source  selection definition
  */ 
#define ADC_HW_TRIG_SEL_T1_TRIG0               ((uint32_t)0x00000000) 
#define ADC_HW_TRIG_SEL_T1_CH4CC               ADC_CFG_HTRGSEL_0      
#define ADC_HW_TRIG_SEL_T2_TRIG0               ADC_CFG_HTRGSEL_1      
#define ADC_HW_TRIG_SEL_T3_TRIG0               ((uint32_t)(ADC_CFG_HTRGSEL_0 | ADC_CFG_HTRGSEL_1)) 
#define ADC_HW_TRIG_SEL_T15_TRIG0              ADC_CFG_HTRGSEL_2    

#define ADC_HW_TRIG_SEL_CHECK(value) (((value) == ADC_HW_TRIG_SEL_T1_TRIG0) || \
                                      ((value) == ADC_HW_TRIG_SEL_T1_CH4CC) || \
                                      ((value) == ADC_HW_TRIG_SEL_T2_TRIG0) || \
                                      ((value) == ADC_HW_TRIG_SEL_T3_TRIG0) || \
                                      ((value) == ADC_HW_TRIG_SEL_T15_TRIG0)) 

/** 
  * @brief ADC Conversion data format alignment definition.
  */ 
#define ADC_DATA_ALIGN_RIGHT             ((uint32_t)0x00000000) //!< ADC data format is right aligen 
#define ADC_DATA_ALIGN_LEFT               ADC_CFG_DATALG        //!< ADC data format is left aligen 

#define ADC_DATA_ALIGN_CHECK(value) (((value) == ADC_DATA_ALIGN_RIGHT) || \
                                     ((value) == ADC_DATA_ALIGN_LEFT))
                                  


/** 
  * @brief ADC Converation group sequence direction definition
  */   
#define ADC_CONV_SEQ_DIR_UPWARD                ((uint32_t)0x00000000) //!< ADC conversion sequence upward direction 
#define ADC_CONV_SEQ_DIR_BACKWARD              ADC_CFG_CGDIR          //!< ADC conversion sequence backward direction 

#define ADC_CONV_SEQ_DIR_CHECK(value) (((value) == ADC_CONV_SEQ_DIR_UPWARD) || \
                                       ((value) == ADC_CONV_SEQ_DIR_BACKWARD)) 
                                          
/** 
  * @brief ADC DMA mode definition
  */   
#define ADC_DMA_MODE_SINGLE                        ((uint32_t)0x00000000) //!< DMA single mode 
#define ADC_DMA_MODE_CIRCULAR                       ADC_CFG_DMAMODE       //!< DMA circle mode

#define ADC_DMA_MODE_CHECK(flag) (((flag) == ADC_DMA_MODE_SINGLE) || \
                                  ((flag) == ADC_DMA_MODE_CIRCULAR))
                                  
/** 
  * @brief ADC analog watchdog monitor channel selection definition
  */  
#define ADC_AN_WDG_SEL_CH0                 ((uint32_t)0x00000000) //!< Analog watchdog monitor channel 0 selected
#define ADC_AN_WDG_SEL_CH1                 ((uint32_t)0x04000000) //!< Analog watchdog monitor channel 1 selected
#define ADC_AN_WDG_SEL_CH2                 ((uint32_t)0x08000000) //!< Analog watchdog monitor channel 2 selected
#define ADC_AN_WDG_SEL_CH3                 ((uint32_t)0x0C000000) //!< Analog watchdog monitor channel 3 selected
#define ADC_AN_WDG_SEL_CH4                 ((uint32_t)0x10000000) //!< Analog watchdog monitor channel 4 selected
#define ADC_AN_WDG_SEL_CH5                 ((uint32_t)0x14000000) //!< Analog watchdog monitor channel 5 selected
#define ADC_AN_WDG_SEL_CH6                 ((uint32_t)0x18000000) //!< Analog watchdog monitor channel 6 selected
#define ADC_AN_WDG_SEL_CH7                 ((uint32_t)0x1C000000) //!< Analog watchdog monitor channel 7 selected
#define ADC_AN_WDG_SEL_CH8                 ((uint32_t)0x20000000) //!< Analog watchdog monitor channel 8 selected
#define ADC_AN_WDG_SEL_CH9                 ((uint32_t)0x24000000) //!< Analog watchdog monitor channel 9 selected
#define ADC_AN_WDG_SEL_CH10                ((uint32_t)0x28000000) //!< Analog watchdog monitor channel 10 selected, not available for CS32F031 devices 
#define ADC_AN_WDG_SEL_CH11                ((uint32_t)0x2C000000) //!< Analog watchdog monitor channel 11 selected, not available for CS32F031 devices
#define ADC_AN_WDG_SEL_CH12                ((uint32_t)0x30000000) //!< Analog watchdog monitor channel 12 selected, not available for CS32F031 devices 
#define ADC_AN_WDG_SEL_CH13                ((uint32_t)0x34000000) //!< Analog watchdog monitor channel 13 selected, not available for CS32F031 devices 
#define ADC_AN_WDG_SEL_CH14                ((uint32_t)0x38000000) //!< Analog watchdog monitor channel 14 selected, not available for CS32F031 devices 
#define ADC_AN_WDG_SEL_CH15                ((uint32_t)0x3C000000) //!< Analog watchdog monitor channel 15 selected, not available for CS32F031 devices
#define ADC_AN_WDG_SEL_CH16                ((uint32_t)0x40000000) //!< Analog watchdog monitor channel 16 selected
#define ADC_AN_WDG_SEL_CH17                ((uint32_t)0x44000000) //!< Analog watchdog monitor channel 17 selected
#define ADC_AN_WDG_SEL_CH18                ((uint32_t)0x48000000) //!< Analog watchdog monitor channel 18 selected


#define ADC_AN_WDG_SEL_CHECK(value) (((value) == ADC_AN_WDG_SEL_CH0) ||  ((value) == ADC_AN_WDG_SEL_CH1)  || \
                                    ((value) == ADC_AN_WDG_SEL_CH2)  ||  ((value) == ADC_AN_WDG_SEL_CH3)  || \
                                    ((value) == ADC_AN_WDG_SEL_CH4)  ||  ((value) == ADC_AN_WDG_SEL_CH5)  || \
                                    ((value) == ADC_AN_WDG_SEL_CH6)  ||  ((value) == ADC_AN_WDG_SEL_CH7)  || \
                                    ((value) == ADC_AN_WDG_SEL_CH8)  ||  ((value) == ADC_AN_WDG_SEL_CH9)  || \
                                    ((value) == ADC_AN_WDG_SEL_CH10) ||  ((value) == ADC_AN_WDG_SEL_CH11) || \
                                    ((value) == ADC_AN_WDG_SEL_CH12) ||  ((value) == ADC_AN_WDG_SEL_CH13) || \
                                    ((value) == ADC_AN_WDG_SEL_CH14) ||  ((value) == ADC_AN_WDG_SEL_CH15) || \
                                    ((value) == ADC_AN_WDG_SEL_CH16) ||  ((value) == ADC_AN_WDG_SEL_CH17) || \
                                    ((value) == ADC_AN_WDG_SEL_CH18))
                                        
/**   
  * @brief ADC sampling times cycles definition
  */ 
#define ADC_SAMPLE_TIMES_1_5                     ((uint32_t)0x00000000) //!< Sampling time is 1.5 cycles 
#define ADC_SAMPLE_TIMES_7_5                     ((uint32_t)0x00000001) //!< Sampling time is 7.5 cycles 
#define ADC_SAMPLE_TIMES_13_5                    ((uint32_t)0x00000002) //!< Sampling time is 13.5 cycles 
#define ADC_SAMPLE_TIMES_28_5                    ((uint32_t)0x00000003) //!< Sampling time is 28.5 cycles 
#define ADC_SAMPLE_TIMES_41_5                    ((uint32_t)0x00000004) //!< Sampling time is 41.5 cycles 
#define ADC_SAMPLE_TIMES_55_5                    ((uint32_t)0x00000005) //!< Sampling time is 55.5 cycles 
#define ADC_SAMPLE_TIMES_71_5                    ((uint32_t)0x00000006) //!< Sampling time is 71.5 cycles 
#define ADC_SAMPLE_TIMES_239_5                   ((uint32_t)0x00000007) //!< Sampling time is 239.5 cycles 

#define ADC_SAMPLE_TIMES_CHECK(value) (((value) == ADC_SAMPLE_TIMES_1_5)  || ((value) == ADC_SAMPLE_TIMES_7_5)  || \
                                       ((value) == ADC_SAMPLE_TIMES_13_5) || ((value) == ADC_SAMPLE_TIMES_28_5) || \
                                       ((value) == ADC_SAMPLE_TIMES_41_5) || ((value) == ADC_SAMPLE_TIMES_55_5) || \
                                       ((value) == ADC_SAMPLE_TIMES_71_5) || ((value) == ADC_SAMPLE_TIMES_239_5))
/** 
  * @brief Check ADC thresholds definition
  */   
#define ADC_THRESHOLD_CHECK(value) ((value) <= 0xFFF)

/** 
  * @brief ADC conversion channel selection definition
  */  
#define ADC_CONV_CHANNEL_0                              ADC_CHANSEL_CHANSEL0 //!< ADC Channel 0 selected
#define ADC_CONV_CHANNEL_1                              ADC_CHANSEL_CHANSEL1 //!< ADC Channel 1 selected
#define ADC_CONV_CHANNEL_2                              ADC_CHANSEL_CHANSEL2 //!< ADC Channel 2 selected
#define ADC_CONV_CHANNEL_3                              ADC_CHANSEL_CHANSEL3 //!< ADC Channel 3 selected
#define ADC_CONV_CHANNEL_4                              ADC_CHANSEL_CHANSEL4 //!< ADC Channel 4 selected
#define ADC_CONV_CHANNEL_5                              ADC_CHANSEL_CHANSEL5 //!< ADC Channel 5 selected
#define ADC_CONV_CHANNEL_6                              ADC_CHANSEL_CHANSEL6 //!< ADC Channel 6 selected
#define ADC_CONV_CHANNEL_7                              ADC_CHANSEL_CHANSEL7 //!< ADC Channel 7 selected
#define ADC_CONV_CHANNEL_8                              ADC_CHANSEL_CHANSEL8 //!< ADC Channel 8 selected
#define ADC_CONV_CHANNEL_9                              ADC_CHANSEL_CHANSEL9 //!< ADC Channel 9 selected
#define ADC_CONV_CHANNEL_10                             ADC_CHANSEL_CHANSEL10 //!< ADC Channel 10 selected, not available for CS32F031 devices 
#define ADC_CONV_CHANNEL_11                             ADC_CHANSEL_CHANSEL11 //!< ADC Channel 11 selected, not available for CS32F031 devices 
#define ADC_CONV_CHANNEL_12                             ADC_CHANSEL_CHANSEL12 //!< ADC Channel 12 selected, not available for CS32F031 devices  
#define ADC_CONV_CHANNEL_13                             ADC_CHANSEL_CHANSEL13 //!< ADC Channel 13 selected, not available for CS32F031 devices 
#define ADC_CONV_CHANNEL_14                             ADC_CHANSEL_CHANSEL14 //!< ADC Channel 14 selected, not available for CS32F031 devices 
#define ADC_CONV_CHANNEL_15                             ADC_CHANSEL_CHANSEL15 //!< ADC Channel 15 selected, not available for CS32F031 devices 
#define ADC_CONV_CHANNEL_16                             ADC_CHANSEL_CHANSEL16 //!< ADC Channel 16 selected
#define ADC_CONV_CHANNEL_17                             ADC_CHANSEL_CHANSEL17 //!< ADC Channel 17 selected

#define ADC_CONV_CHANNEL_TEMP        ((uint32_t)ADC_CONV_CHANNEL_16) //!< ADC temperature sensor channel 
#define ADC_CONV_CHANNEL_VREF        ((uint32_t)ADC_CONV_CHANNEL_17) //!< ADC Vrefint channel 

#define ADC_CONV_CHANNEL_CHECK(value) (((value) != (uint32_t)RESET) && (((value) & 0xFFF80000) == (uint32_t)RESET))
 
/** 
  * @brief ADC interrupts definition
  */   
#define ADC_INTR_EOI                ADC_INTEN_EOIIE    //!< End of ADC initialization interrupt enable 
#define ADC_INTR_EOSPIE             ADC_INTEN_EOSPIE  //!< End of sampling phase interrupt enable 
#define ADC_INTR_EOC                ADC_INTEN_EOCIE    //!< End of channel conversion interrupt enable 
#define ADC_INTR_EOG                ADC_INTEN_EOGIE   //!< End of conversion group interrupt enable 
#define ADC_INTR_DOVRIE             ADC_INTEN_DOVRIE  //!< ADC data overrun interrupt enable 
#define ADC_INTR_WDEVT              ADC_INTEN_WDEVTIE //!< Analog watchdog event interrupt enable 
 
#define ADC_CONFIG_INTR_CHECK(value) (((value) != (uint32_t)RESET) && (((value) & 0xFFFFFF60) == (uint32_t)RESET))

#define ADC_GET_INTR_CHECK(value) (((value) == ADC_INTR_EOI)    || ((value) == ADC_INTR_EOSPIE) || \
                                   ((value) == ADC_INTR_EOC)    || ((value) == ADC_INTR_EOG)    || \
                                   ((value) == ADC_INTR_DOVRIE) || ((value) == ADC_INTR_WDEVT))

#define ADC_CLEAR_INTR_CHECK(value) (((value) != (uint32_t)RESET) && (((value) & 0xFFFFFF60) == (uint32_t)RESET))

/** 
  * @brief ADC flags status definition
  */ 
#define ADC_FLAG_EOI           ADC_STAT_EOI  //!< End of ADC initialization flag
#define ADC_FLAG_EOSP          ADC_STAT_EOSP //!< End of sampling phase flag 
#define ADC_FLAG_EOCH          ADC_STAT_EOCH //!< End of channel conversion flag 
#define ADC_FLAG_EOCG          ADC_STAT_EOCG //!< End of conversion group flag 
#define ADC_FLAG_DOVR          ADC_STAT_DOVR //!< ADC data overrun flag 
#define ADC_FLAG_WDEVT         ADC_STAT_WDEVT//!< Analog watchdog event flag 

#define ADC_FLAG_ADCON         ((uint32_t)0x01000001) //!< ADC power on command flag 
#define ADC_FLAG_ADCOFF        ((uint32_t)0x01000002) //!< ADC power off command flag 
#define ADC_FLAG_ADSTRT        ((uint32_t)0x01000004) //!< ADC start conversion command flag 
#define ADC_FLAG_ADSTOP        ((uint32_t)0x01000010) //!< ADC stop conversion command flag 
#define ADC_FLAG_CALB          ((uint32_t)0x81000000) //!< ADC calibration control flag 

#define ADC_CLEAR_FLAG_CHECK(flag) (((flag) != (uint32_t)RESET) && (((flag) & 0xFFFFFF60) == (uint32_t)RESET))

#define ADC_GET_FLAG_CHECK(flag) (((flag) == ADC_FLAG_EOI)    || ((flag) == ADC_FLAG_EOSP)   || \
                                  ((flag) == ADC_FLAG_EOCH)   || ((flag) == ADC_FLAG_EOCG)   || \
                                  ((flag) == ADC_FLAG_WDEVT)  || ((flag) == ADC_FLAG_DOVR)   || \
                                  ((flag) == ADC_FLAG_ADCON)  || ((flag) == ADC_FLAG_ADCOFF) || \
                                  ((flag) == ADC_FLAG_ADSTRT) || ((flag) == ADC_FLAG_ADSTOP) || \
                                  ((flag) == ADC_FLAG_CALB))
/**
  * @}
  */ 


/**
  * @fn void adc_def_init(adc_reg_t* ptr_adc)
  * @brief  Deinitializes ADC1 peripheral registers to their default reset values.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_def_init(adc_reg_t* ptr_adc);

/* Initialization and Configuration functions *********************************/ 

/**
  * @fn void adc_init(adc_reg_t* ptr_adc, adc_config_t* ptr_config)
  * @brief  Initializes the ADCx peripheral according to the specified parameters.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  ptr_config: pointer to an adc_config_t structure that contains 
  *         the configuration information for the specified ADC peripheral.
  * @return None
  */
void adc_init(adc_reg_t* ptr_adc, adc_config_t* ptr_config);

/**
  * @fn void adc_config_struct_init(adc_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its default value.
  * @param  ptr_config: pointer to an adc_config_t structure which will be initialized.
  * @return None
  */
void adc_config_struct_init(adc_config_t* ptr_config);

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
void adc_clock_select(adc_reg_t* ptr_adc, uint32_t value);

/**
  * @fn void adc_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the specified ADC peripheral.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void adc_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

#ifdef CS32F036
/* Power saving functions *****************************************************/

/**
  * @fn void adc_auto_standby_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the ADC Auto standby. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_auto_standby_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn void adc_conversion_pause_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables conversion pause mode. When the CPU clock is not fast
  *         enough to manage the data rate, a Hardware delay can be introduced between
  *         ADC conversions to reduce this data rate. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_conversion_pause_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/* Analog Watchdog configuration functions ************************************/

/**
  * @fn void adc_watchdog_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the analog watchdog 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_watchdog_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn void adc_watchdog_thresholds_set(adc_reg_t* ptr_adc, uint16_t high, uint16_t low) 
  * @brief  Configures the high and low thresholds of the analog watchdog. 
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  high: the ADC analog watchdog High threshold value( 12bit value).
  * @param  low: the ADC analog watchdog Low threshold value( 12bit value).
  * @return None
  */
void adc_watchdog_thresholds_set(adc_reg_t* ptr_adc, uint16_t high, uint16_t low);

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
void adc_watchdog_channel_select(adc_reg_t* ptr_adc, uint32_t value);

/**
  * @fn void adc_watchdog_channel_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the ADC Analog channel monitor mode.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_watchdog_channel_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);
#endif

/* Temperature Sensor , Vrefint and Vbat management function ******************/

/**
  * @fn void adc_temp_sensor_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the temperature sensor channel.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_temp_sensor_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void adc_vrefint_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Vrefint channel.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_vrefint_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void adc_vbat_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Vbat channel. 
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_vbat_enable_ctrl(enable_state_t enable_flag);

/* Channels Configuration functions *******************************************/

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
void adc_channel_config(adc_reg_t* ptr_adc, uint32_t channel, uint32_t time);

/**
  * @fn void adc_continuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the Continuous mode for the selected ADCx channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_continuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn void adc_discontinuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the discontinuous mode for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_discontinuous_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn void adc_overrun_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enable the Overrun mode for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_overrun_mode_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn uint32_t adc_calibration_value_get(adc_reg_t* ptr_adc)
  * @brief  Active the Calibration operation for the selected ADC, the Calibration can be 
  *         initiated only when ADC is still in the reset configuration (ADCON must be equal to 0).    
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return ADC Calibration factor 
  */
uint32_t adc_calibration_value_get(adc_reg_t* ptr_adc);

/**
  * @fn void adc_conversion_stop(adc_reg_t* ptr_adc)
  * @brief  Stop the on going conversions for the selected ADC. When ADSTOP is set, any on going
  *         conversion is aborted, and the ADC data register is not updated with current conversion.    
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_conversion_stop(adc_reg_t* ptr_adc);

/**
  * @fn void adc_conversion_start(adc_reg_t* ptr_adc)
  * @brief  Start Conversion for the selected ADC channels.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc_conversion_start(adc_reg_t* ptr_adc);

/**
  * @fn uint16_t adc_conversion_value_get(adc_reg_t* ptr_adc)
  * @brief  Returns the last ADCx conversion result data for ADC channel.  
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return The Data conversion value.
  */
uint16_t adc_conversion_value_get(adc_reg_t* ptr_adc);

/* Regular Channels DMA Configuration functions *******************************/

/**
  * @fn void adc_dma_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag)
  * @brief  Enables or disables the specified ADC DMA request.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void adc_dma_enable_ctrl(adc_reg_t* ptr_adc, enable_state_t enable_flag);

/**
  * @fn void adc_dma_mode_set(adc_reg_t* ptr_adc, uint32_t mode)
  * @brief  Enables or disables the ADC DMA request after last transfer (Single-ADC mode)
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @param  mode: the ADC channel to configure, it can be
  *          @arg ADC_DMA_MODE_SINGLE(DMA single Mode)
  *          @arg ADC_DMA_MODE_CIRCULAR (DMA Circular Mode)
  *  @return None
  */
void adc_dma_mode_set(adc_reg_t* ptr_adc, uint32_t mode);

/* Interrupts and flags management functions **********************************/

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
void adc_interrupt_config(adc_reg_t* ptr_adc, uint32_t value, enable_state_t enable_flag);

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
bit_status_t adc_flag_status_get(adc_reg_t* ptr_adc, uint32_t flag);

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
void adc_flag_clear(adc_reg_t* ptr_adc, uint32_t flag);

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
bit_status_t adc_intetrrupt_status_get(adc_reg_t* ptr_adc, uint32_t flag);

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
void adc_intetrrupt_flag_clear(adc_reg_t* ptr_adc, uint32_t flag);

#ifdef __cplusplus
}
#endif

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

