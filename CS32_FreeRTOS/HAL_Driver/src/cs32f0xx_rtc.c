/**
  * @file    cs32f0xx_rtc.c  
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Real-Time Clock (RTC) peripheral and 
  *          is not applicable for CS32F036 devices:
  *           + Initialization
  *           + Calendar (Time and Date) configuration
  *           + Alarms configuration
  *           + Daylight Saving configuration
  *           + Output pin Configuration
  *           + Digital Calibration configuration  
  *           + Output Type Config configuration
  *           + Shift control synchronisation  
  *           + Interrupts and flags management
  *
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2020 ChipSea</center></h2>
  *
  * @verbatim  
  *
  ----------------------- this driver instruction for use--------------------------- 
     1. Backup Domain Operating Condition 
       a.The real-time clock (RTC) and the RTC backup registers can be powered
         from the VBAT voltage when the main VDD supply is powered off.
         To retain the content of the RTC backup registers and supply the RTC 
         when VDD is turned off, VBAT pin can be connected to an optional
         standby voltage supplied by a battery or by another source.
  
       b.To allow the RTC to operate even when the main digital supply (VDD) 
         is turned off, the VBAT pin powers the The RTC,The LXT oscillator,
         and PC13 to PC15 I/Os I/Os (when available)blocks.

      c. If the backup domain is supplied by VDD (analog switch connected 
         to VDD), PC14 and PC15 can be used as either GPIO or LXT pins,
         PC13 can be used as a GPIO or as the RTC_MF1 pin.
                                                                                           
     2. Time and Date configuration 
        a. Configuring the RTC Calendar (Time and Date),can use the rtc_time_set()
            and rtc_date_set() functions.
        b. Reading the RTC Calendar, can use the rtc_time_get() and rtc_date_get()functions.            
        c. Reading the RTC subsecond,can use the rtc_subsecond_get() function.
        d. Use the rtc_daylight_saving_config() function to add or sub one
            hour to the RTC Calendar.

     3. Alarm configuration
        a. To configure the RTC Alarm use the rtc_alarm_set() function.
        b. Enable the selected RTC Alarm using the rtc_alarm_enable_ctrl() function  
        c. To read the RTC Alarm, use the rtc_alarm_get() function.
        d. To read the RTC alarm SubSecond, use the rtc_alarm_subsecond_get() function.

    4. RTC Wakeup configuration 
        a. Configure the RTC Wakeup Clock source use the rtc_wakeup_clock_config() function.            
        b. Configure the RTC WakeUp Counter using the rtc_wakeup_value_set() function .              
        c. Enable the RTC WakeUp using the rtc_wakeup_enable_ctrl() function.  
        d.To read the RTC WakeUp Counter register, use the rtc_wakeup_value_get() function.
              
    5. Outputs configuration the RTC has 2 different outputs
        a. To manage the RTC Alarm, use the rtc_output_config().                      
        b.  The output is 512Hz signal or 1Hz use the rtc_cali_output_enable_ctrl().           
  
    6. Original Digital Calibration configuration 
        a. Configure the RTC Original Digital Calibration Value and the corresponding
         calibration cycle period (32s,16s and 8s) using the rtc_swallow_config().
  
   7.Recording configuration      
        a. Configure the trigger and enables the RTC  
            using the rtc_time_recording_enable_ctrl() function.
        b. To read the RTC recording Time and Date register, use the 
            rtc_time_recording_get() function.
        c. To read the RTC recording SubSecond register, use the 
            rtc_time_recording_subsecond_get() function.
  
   8. Rtc inbreak configuration 
        a. Configure the inbreak filter count using rtc_inbreak_filter_config().            
        b. Configure the RTC recording trigger Edge or Level according to the recording 
            filter value.
        c. Configure the inbreak sampling frequency using rtc_inbreak_sample_freq_config().
        d. Configure the inbreak precharge or discharge duration using rtc_inbreak_precharge_duration().            
        e. Enable the inbreak Pull-UP using rtc_inbreak_pull_up_enable_ctrl() .
        f. Enable the RTC inbreak using the rtc_inbreak_enable_ctrl() .
        g. Enable the Time recording on detection event .       
  
   9. RTC always on registers configuration 
        a.To write to the RTC always on registers, use the rtc_ao_register_write().
        b. To read the RTC always on registers, use the rtc_ao_register_read().

   10. RTC and low power modes
    a. The MCU can be woken up from a low power mode by an RTC multi-function. 
    b.The RTC  multi-functions are the RTC alarm , RTC inbreak 
         event detection and RTC time recording event detection.
         These RTC  multi-function can wake up the system from the Stop 
         and Standby lowpower modes.
         The system can also wake up from low power modes without depending 
         on an external interrupt (Auto-wakeup mode), by using the RTC alarm events.
    c.The RTC provides a programmable time base for waking up from the 
         Stop or Standby mode at regular intervals.
         Wakeup from STOP and Standby modes is possible only when the RTC 
         clock source is LXT or LRC.
   @endverbatim
  *
  */

#include "cs32f0xx_rtc.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup RTC 
  * @brief RTC driver modules
  * @{
  */

/** 
  * @brief  RTC register bit mask  definition  
  */ 
#define RTC_TIME_RESERVED_MASK    ((uint32_t)0x007F7F7F)  //!< RTC time register RESERVED mask
#define RTC_DATE_RESERVED_MASK    ((uint32_t)0x00FFFF3F)  //!< RTC date register RESERVED mask
#define RTC_INIT_MASK             ((uint32_t)0xFFFFFFFF)  //!< RTC date register RESERVED mask 
#define RTC_RSF_MASK              ((uint32_t)0xFFFFFF5F)  //!< RTC status register synchronization flag mask
#define RTC_FLAGS_MASK            ((uint32_t)(RTC_FLAG_TSOVF | RTC_FLAG_TSF | RTC_FLAG_ALRF | \
                                              RTC_FLAG_RSF | RTC_FLAG_INITS |RTC_FLAG_INITF | \
                                              RTC_FLAG_INBREAK1F | RTC_FLAG_INBREAK2F | RTC_FLAG_CALBSY | \
                                              RTC_FLAG_SHF_BUSY))
/** 
  * @brief RTC timeout definition	
  */ 
#define RTC_INIT_MODE_TIMEOUT       ((uint32_t) 0x00004000)  //!< RTC INIT state timeout
#define RTC_SYNCHRO_TIMEOUT         ((uint32_t) 0x00008000)  //!< synchro timeout 
#define RTC_CALI_TIMEOUT            ((uint32_t) 0x00001000)  //!< Calibration timeout
#define RTC_SHPF_TIMEOUT            ((uint32_t) 0x00001000)  //!< Shitf timeout


static uint8_t rtc_byte_convert_bcd(uint8_t Value);
static uint8_t rtc_bcd_convert_byte(uint8_t Value);

/**
  * @fn error_status_t rtc_def_init(void)
  * @brief  Deinitializes the RTC registers to their default reset values.                     
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_def_init(void)
{
    error_status_t status = ERROR;
  
    /*Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Set Initialization mode */
    if (rtc_init_mode_enter() == ERROR)
    {
        status = ERROR;
    }  
    else
    {
        /* Reset TIME, DATE and CTR registers */
        RTC->TIME      = (uint32_t)0x00000000;
        RTC->WUTR      = (uint32_t)0x0000FFFF;
        RTC->DATE      = (uint32_t)0x00002101;
        RTC->CTR      &= (uint32_t)0x00000000;
        RTC->PDIV      = (uint32_t)0x007F00FF;
        RTC->ACFG1     = (uint32_t)0x00000000;
        RTC->ADJ       = (uint32_t)0x00000000;
        RTC->RTC_1SCAL = (uint32_t)0x00000000;
        RTC->ACFG2     = (uint32_t)0x00000000;

        /* Reset STS register and exit initialization mode */
        RTC->STS = (uint32_t)0x00000000;
    
        /* Reset  multi-function configuration register */
        RTC->MF = 0x00000000;
      
        /* Wait till the RTC RSF flag is set */
        if (rtc_synchro_wait() == ERROR)
        {
            status = ERROR;
        }
        else
        {
            status = SUCCESS;
        }

    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;  

    return status;
}

/**
  * @fn error_status_t rtc_init(rtc_init_t* ptr_init_struct)
  * @brief  Initializes the RTC registers according to the specified parameters in ptr_init_struct.
  *         RTC Prescaler register can be written in initialization step only.
  * @param  ptr_init_struct: pointer to a rtc_init_t structure that contains 
  *         the configuration information for the RTC peripheral.
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_init(rtc_init_t* ptr_init_struct)
{
    error_status_t status = ERROR;

    ASSERT(RTC_HOUR_FORMAT_CHECK(ptr_init_struct->hour_format));
    ASSERT(RTC_PDIV1_CHECK(ptr_init_struct->rtc_divider1));
    ASSERT(RTC_PDIV2_CHECK(ptr_init_struct->rtc_divider2));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Set Initialization mode */
    if (rtc_init_mode_enter() == ERROR)
    {
        status = ERROR;
    }
    else
    {
        /* Clear RTC_CTR_FMT12 Bit */
        RTC->CTR &= ((uint32_t)~(RTC_CTR_FMT12));
        /* Set RTC_CTR register */
        RTC->CTR |= ((uint32_t)(ptr_init_struct->hour_format));
        
        /* Configure the RTC PDIV */
        RTC->PDIV = (uint32_t)(ptr_init_struct->rtc_divider2);
        RTC->PDIV |= (uint32_t)(ptr_init_struct->rtc_divider1 << 16);
    
        /* Exit Initialization mode */
        rtc_init_mode_exit();
        status = SUCCESS;
    }
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;

    return status;
}

/**
  * @fn void rtc_struct_init(rtc_init_t* ptr_init_struct)
  * @brief  Fills each rtc init_struct member with its default value.
  * @param  ptr_init_struct: pointer to a ptr_init_struct structure which will be initialized.         
  * @return None
  */
void rtc_struct_init(rtc_init_t* ptr_init_struct)
{
    /* Initialize the hour_format member */
    ptr_init_struct->hour_format = RTC_HOUR_FORMAT_24;
    
    /* Init the first pre-divider factor member */
    ptr_init_struct->rtc_divider1 = (uint32_t)0x7F;

    /* Init the second pre-divider factor member */
    ptr_init_struct->rtc_divider2 = (uint32_t)0xFF; 
}   

/**
  * @fn void rtc_ccode_register_set(enable_state_t enable_flag)
  * @brief  Enables or disables the RTC registers write protection, CCODE (RTC control code register).            
  *         It can be Writ a wrong key reactivates the write protection.
  *         The protection mechanism is not affected by system reset.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_ccode_register_set(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the write protection for RTC registers */
        RTC->CCODE = 0xFF;
    }
    else
    {
        /* Disable the write protection for RTC registers */
        RTC->CCODE = 0xCA;
        RTC->CCODE = 0x53;
    }
}

/**
  * @fn error_status_t rtc_init_mode_enter(void)
  * @brief  Enters the RTC Initialization mode.
  *         When the RTC Initialization mode is write protected, can be use the 
  *         rtc_ccode_register_set(DISABLE) before calling this function.
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_init_mode_enter(void)
{
    __IO uint32_t init_counter = 0x00;
    uint32_t status_value = 0x00;
    error_status_t status = ERROR;

    /* Check if the Initialization mode is set */
    if ((RTC->STS & RTC_STS_INITF) == (uint32_t)RESET)
    {
        /* Set the Initialization mode */
        RTC->STS = (uint32_t)RTC_INIT_MASK;
        
        /* Wait till RTC is in INIT state and if Time out is reached exit */
        do
        {
            status_value = RTC->STS & RTC_STS_INITF;
            init_counter++;  
        } while ((init_counter != RTC_INIT_MODE_TIMEOUT) && (status_value == 0x00));
        
        if ((RTC->STS & RTC_STS_INITF) != RESET)
        {
            status = SUCCESS;
        }
        else
        {
            status = ERROR;
        }
    }
    else
    {
        status = SUCCESS;
    }

    return (status);
}

/**
  * @fn void rtc_init_mode_exit(void)
  * @brief  Exits the RTC Initialization mode.      
  * @param  None
  * @return None
  */
void rtc_init_mode_exit(void)
{
    /* Exit Initialization mode */
    RTC->STS &= (uint32_t)~RTC_STS_INITE;
}

/**
  * @fn error_status_t rtc_synchro_wait(void)
  * @brief  Waits until the RTC Time and Date registers (RTC_TIME and RTC_DATE) are 
  *         synchronized with RTC APB clock.
  * @note   When the RTC Initialization mode is write protected, can use the 
  *         rtc_ccode_register_set(DISABLE) before calling this function.  
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_synchro_wait(void)
{
    __IO uint32_t sync_counter = 0;
    uint32_t temp_status = 0x00;
    error_status_t status = ERROR;

    if ((RTC->CTR & RTC_CTR_DAR) != RESET)
    {
        /* direct access mode */
        status = SUCCESS;
    }
    else
    {
        /* Disable the write protection for RTC registers */
        RTC->CCODE = 0xCA;
        RTC->CCODE = 0x53;

        /* Clear RSF flag */
        RTC->STS &= (uint32_t)RTC_RSF_MASK;

        /* Wait the registers to be synchronised */
        do
        {
            temp_status = RTC->STS & RTC_STS_RSF;
            sync_counter++;  
        } while((sync_counter != RTC_SYNCHRO_TIMEOUT) && (temp_status == 0x00));

        if ((RTC->STS & RTC_STS_RSF) != RESET)
        {
            status = SUCCESS;
        }
        else
        {
            status = ERROR;
        }

        /* Enable the write protection for RTC registers */
        RTC->CCODE = 0xFF;
    }

    return (status);
}

/**
  * @fn error_status_t rtc_outside_clock_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables RTC OPCLKEN (outside precise clock enable) detection.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_outside_clock_enable_ctrl(enable_state_t enable_flag)
{
    error_status_t status = ERROR;

    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Set Initialization mode */
    if (rtc_init_mode_enter() == ERROR)
    {
        status = ERROR;
    }
    else
    {
        if (enable_flag != DISABLE)
        {
            /* Enable the RTC outside precise clock detection */
            RTC->CTR |= RTC_CTR_OPCLKEN;
        }
        else
        {
            /* Disable the RTC outside precise clock detection */
            RTC->CTR &= ~RTC_CTR_OPCLKEN;
        }
        /* Exit Initialization mode */
        rtc_init_mode_exit();

        status = SUCCESS;
    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;

    return status;
}

/**
  * @fn void rtc_direct_access_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables direct access register.
  * @note   When direct access is enabled the calendar value are taken 
  *         directly from the Calendar counter.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
*/
void rtc_direct_access_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
  
    if (enable_flag != DISABLE)
    {
        /* Set direct access (DAR) bit */
        RTC->CTR |= (uint8_t)RTC_CTR_DAR;
    }
    else
    {
        /* Reset direct access (DAR) bit */
        RTC->CTR &= (uint8_t)~RTC_CTR_DAR;
    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn error_status_t rtc_time_set(uint32_t format_flag, rtc_time_t* ptr_time_struct)
  * @brief  Set the RTC current time.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).        
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that contains 
  *         the time configuration information for the RTC.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_time_set(uint32_t format_flag, rtc_time_t* ptr_time_struct)
{
    uint32_t reg = 0;
    error_status_t status = ERROR;

    ASSERT(RTC_FORMAT_CHECK(format_flag));
  
    if (format_flag == RTC_FORMAT_BIN)
    {
        if ((RTC->CTR & RTC_CTR_FMT12) != (uint32_t)RESET)
        {
            ASSERT(RTC_HOUR12_CHECK(ptr_time_struct->rtc_hours));
            ASSERT(RTC_AM_PM_HOUR12_CHECK(ptr_time_struct->am_pm_sel));
        }
        else
        {
            ptr_time_struct->am_pm_sel = 0x00;
            ASSERT(RTC_HOUR24_CHECK(ptr_time_struct->rtc_hours));
        }
        ASSERT(RTC_MINUTES_CHECK(ptr_time_struct->rtc_minutes));
        ASSERT(RTC_SECONDS_CHECK(ptr_time_struct->rtc_seconds));
    }
    else
    {
        if ((RTC->CTR & RTC_CTR_FMT12) != (uint32_t)RESET)
        {
            reg = rtc_bcd_convert_byte(ptr_time_struct->rtc_hours);
            ASSERT(RTC_HOUR12_CHECK(reg));
            ASSERT(RTC_AM_PM_HOUR12_CHECK(ptr_time_struct->am_pm_sel)); 
        } 
        else
        {
            ptr_time_struct->am_pm_sel = 0x00;
            ASSERT(RTC_HOUR24_CHECK(rtc_bcd_convert_byte(ptr_time_struct->rtc_hours)));
        }
        ASSERT(RTC_MINUTES_CHECK(rtc_bcd_convert_byte(ptr_time_struct->rtc_minutes)));
        ASSERT(RTC_SECONDS_CHECK(rtc_bcd_convert_byte(ptr_time_struct->rtc_seconds)));
    }
  
    /* Check the input parameters format */
    if (format_flag != RTC_FORMAT_BIN)
    {
        reg = (((uint32_t)(ptr_time_struct->rtc_hours) << 16) | \
              ((uint32_t)(ptr_time_struct->rtc_minutes) << 8) | \
              ((uint32_t)ptr_time_struct->rtc_seconds) | \
              ((uint32_t)(ptr_time_struct->am_pm_sel) << 16)); 
    }
    else
    {
        reg = (uint32_t)(((uint32_t)rtc_byte_convert_bcd(ptr_time_struct->rtc_hours) << 16) | \
                    ((uint32_t)rtc_byte_convert_bcd(ptr_time_struct->rtc_minutes) << 8) | \
                    ((uint32_t)rtc_byte_convert_bcd(ptr_time_struct->rtc_seconds)) | \
                    (((uint32_t)ptr_time_struct->am_pm_sel) << 16));
    } 

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Set Initialization mode */
    if (rtc_init_mode_enter() == ERROR)
    {
        status = ERROR;
    } 
    else
    {
        /* Set the RTC_TIME register */
        RTC->TIME = (uint32_t)(reg & RTC_TIME_RESERVED_MASK);

        /* Exit Initialization mode */
        rtc_init_mode_exit(); 

        /* If RTC_CTR_DAR bit = 0, wait for synchro else this check is not needed */
        if ((RTC->CTR & RTC_CTR_DAR) == RESET)
        {
            if (rtc_synchro_wait() == ERROR)
            {
                status = ERROR;
            }
            else
            {
                status = SUCCESS;
            }
        }       
        else
        {
            status = SUCCESS;
        }
  
    }
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
    
    return status;
}

/**
  * @fn void rtc_time_struct_init(rtc_time_t* ptr_time_struct)
  * @brief  rtc structure member with its default value (00h:00min:00sec).  *         
  * @param  ptr_time_struct: pointer to a rtc_time_t structure which will be 
  *         initialized.
  * @return None
  */
void rtc_time_struct_init(rtc_time_t* ptr_time_struct)
{
    /* Time = 00h:00min:00sec */
    ptr_time_struct->am_pm_sel = RTC_AM_HOUR12;
    ptr_time_struct->rtc_hours = 0;
    ptr_time_struct->rtc_minutes = 0;
    ptr_time_struct->rtc_seconds = 0; 
}

/**
  * @fn void rtc_time_get(uint32_t format_flag, rtc_time_t* ptr_time_struct)
  * @brief  Get the RTC current Time.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that will 
  *         contain the returned current time configuration.
  * @return None
  */
void rtc_time_get(uint32_t format_flag, rtc_time_t* ptr_time_struct)
{
    uint32_t reg = 0;

    /* Check the parameters */
    ASSERT(RTC_FORMAT_CHECK(format_flag));

    /* Get the RTC_TIME register */
    reg = (uint32_t)(RTC->TIME & RTC_TIME_RESERVED_MASK); 
  
    /* Fill the structure fields with the read parameters */
    ptr_time_struct->rtc_hours = (uint8_t)((reg & (RTC_TIME_HORTEN | RTC_TIME_HORUNT)) >> 16);
    ptr_time_struct->rtc_minutes = (uint8_t)((reg & (RTC_TIME_MINTEN | RTC_TIME_MINUNT)) >>8);
    ptr_time_struct->rtc_seconds = (uint8_t)(reg & (RTC_TIME_SECTEN | RTC_TIME_SECUNT));
    ptr_time_struct->am_pm_sel = (uint8_t)((reg & (RTC_TIME_PM)) >> 16);  
    
    /* Check the input parameters format */
    if (format_flag == RTC_FORMAT_BIN)
    {
        /* Convert the structure parameters to Binary format */
        ptr_time_struct->rtc_hours = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_hours);
        ptr_time_struct->rtc_minutes = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_minutes);
        ptr_time_struct->rtc_seconds = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_seconds);
    }
}

/**
  * @fn uint32_t rtc_subsecond_get(void)
  * @brief  Gets the RTC current Calendar Subseconds value.
  * @param  None
  * @return RTC current Calendar Subseconds value.
  */
uint32_t rtc_subsecond_get(void)
{
    uint32_t reg = 0;
  
    /* Get subseconds values from the correspondent registers*/
    reg = (uint32_t)(RTC->SSEC);
  
    /* Read DATE register to unfroze calendar registers */
    (void) (RTC->DATE);
  
    return (reg);
}

/**
  * @fn error_status_t rtc_date_set(uint32_t format_flag, rtc_date_t* ptr_date_struct)
  * @brief  Set the RTC current date.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that contains 
  *         the date configuration information for the RTC.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_date_set(uint32_t format_flag, rtc_date_t* ptr_date_struct)
{
    uint32_t reg = 0;
    error_status_t status = ERROR;
  
    ASSERT(RTC_FORMAT_CHECK(format_flag));

    if ((format_flag == RTC_FORMAT_BIN) && ((ptr_date_struct->rtc_month & 0x10) == 0x10))
    {
        ptr_date_struct->rtc_month = (ptr_date_struct->rtc_month & (uint32_t)~(0x10)) + 0x0A;
    }  
    if (format_flag == RTC_FORMAT_BIN)
    {
        ASSERT(RTC_YEAR_CHECK(ptr_date_struct->rtc_year));
        ASSERT(MONTH_CHECK(ptr_date_struct->rtc_month));
        ASSERT(DATE_CHECK(ptr_date_struct->rtc_date));
    }
    else
    {
        ASSERT(RTC_YEAR_CHECK(rtc_bcd_convert_byte(ptr_date_struct->rtc_year)));
        reg = rtc_bcd_convert_byte(ptr_date_struct->rtc_month);
        ASSERT(MONTH_CHECK(reg));
        reg = rtc_bcd_convert_byte(ptr_date_struct->rtc_date);
        ASSERT(DATE_CHECK(reg));
    }
    ASSERT(WEEKDAY_CHECK(ptr_date_struct->rtc_weekday));

    /* Check the input parameters format */
    if (format_flag != RTC_FORMAT_BIN)
    {
        reg = ((((uint32_t)ptr_date_struct->rtc_year) << 16) | \
              (((uint32_t)ptr_date_struct->rtc_month) << 8) | \
              ((uint32_t)ptr_date_struct->rtc_date) | \
              (((uint32_t)ptr_date_struct->rtc_weekday) << 13)); 
    }  
    else
    {
        reg = (((uint32_t)rtc_byte_convert_bcd(ptr_date_struct->rtc_year) << 16) | \
              ((uint32_t)rtc_byte_convert_bcd(ptr_date_struct->rtc_month) << 8) | \
              ((uint32_t)rtc_byte_convert_bcd(ptr_date_struct->rtc_date)) | \
              ((uint32_t)ptr_date_struct->rtc_weekday << 13));
    }

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Set Initialization mode */
    if (rtc_init_mode_enter() == ERROR)
    {
        status = ERROR;
    } 
    else
    {
        /* Set the RTC_DATE register */
        RTC->DATE = (uint32_t)(reg & RTC_DATE_RESERVED_MASK);
    
        /* Exit Initialization mode */
        rtc_init_mode_exit(); 
    
        /* If  RTC_CTR_DAR bit = 0, wait for synchro else this check is not needed */
        if ((RTC->CTR & RTC_CTR_DAR) == RESET)
        {
            if (rtc_synchro_wait() == ERROR)
            {
                status = ERROR;
            }
            else
            {
                status = SUCCESS;
            }
        }
        else
        {
            status = SUCCESS;
        }
    }
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
  
    return status;
}

/**
  * @fn void rtc_date_struct_init(rtc_date_t* ptr_date_struct)
  * @brief  rtc date struct member default value (Monday, January 01 xx00).          
  * @param  ptr_date_struct: pointer to a rtc_date_t structure which will be 
  *         initialized.
  * @return None
  */
void rtc_date_struct_init(rtc_date_t* ptr_date_struct)
{
    /* Monday, January 01 xx00 */
    ptr_date_struct->rtc_weekday = WEEKDAY_MONDAY;
    ptr_date_struct->rtc_date = 1;
    ptr_date_struct->rtc_month = MONTH_JANUARY;
    ptr_date_struct->rtc_year = 0;
}

/**
  * @fn void rtc_date_get(uint32_t format_flag, rtc_date_t* ptr_date_struct)
  * @brief  Get the RTC current date.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that will 
  *         contain the returned current date configuration.
  * @return None
  */
void rtc_date_get(uint32_t format_flag, rtc_date_t* ptr_date_struct)
{
    uint32_t reg = 0;

    ASSERT(RTC_FORMAT_CHECK(format_flag));
  
    /* Get the RTC_TIME register */
    reg = (uint32_t)(RTC->DATE & RTC_DATE_RESERVED_MASK); 

    /* Fill the structure fields with the read parameters */
    ptr_date_struct->rtc_year = (uint8_t)((reg & (RTC_DATE_YTEN | RTC_DATE_YUNT)) >> 16);
    ptr_date_struct->rtc_month = (uint8_t)((reg & (RTC_DATE_MTEN | RTC_DATE_MUNT)) >> 8);
    ptr_date_struct->rtc_date = (uint8_t)(reg & (RTC_DATE_DTEN | RTC_DATE_DUNT));
    ptr_date_struct->rtc_weekday = (uint8_t)((reg & (RTC_DATE_WUNT)) >> 13);  

    /* Check the input parameters format */
    if (format_flag == RTC_FORMAT_BIN)
    {
        /* Convert the structure parameters to Binary format */
        ptr_date_struct->rtc_year = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_year);
        ptr_date_struct->rtc_month = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_month);
        ptr_date_struct->rtc_date = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_date);
        ptr_date_struct->rtc_weekday = (uint8_t)(ptr_date_struct->rtc_weekday);   
    }
}

/**
  * @fn void rtc_alarm_set(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
  * @brief  Set the specified RTC Alarm.The Alarm register can only be written 
  *         when the corresponding Alarm is disabled (Use the rtc_alarm_enable_ctrl(DISABLE)).       
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  alarm_flag: Alarm enable(RTC_ALARM_ENABLE).
  * @param  ptr_alarm_struct: pointer to a rtc_alarm_t structure that 
  *         contains the alarm configuration parameters.
  * @return None
  */
void rtc_alarm_set(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
{
    UNUSED(alarm_flag);
    uint32_t reg = 0;
  
    ASSERT(RTC_FORMAT_CHECK(format_flag));
    ASSERT(RTC_ALARM_ENABLE_CHECK(alarm_flag));
    ASSERT(RTC_ALARM_MASK_CHECK(ptr_alarm_struct->alarm_mask));
    ASSERT(RTC_DATE_WEEKDAY_SEL_CHECK(ptr_alarm_struct->date_weekday_sel));

    if (format_flag == RTC_FORMAT_BIN)
    {
        if ((RTC->CTR & RTC_CTR_FMT12) != (uint32_t)RESET)
        {
            ASSERT(RTC_HOUR12_CHECK(ptr_alarm_struct->alarm_time.rtc_hours));
            ASSERT(RTC_AM_PM_HOUR12_CHECK(ptr_alarm_struct->alarm_time.am_pm_sel));
        } 
        else
        {
            ptr_alarm_struct->alarm_time.am_pm_sel = 0x00;
            ASSERT(RTC_HOUR24_CHECK(ptr_alarm_struct->alarm_time.rtc_hours));
        }
        ASSERT(RTC_MINUTES_CHECK(ptr_alarm_struct->alarm_time.rtc_minutes));
        ASSERT(RTC_SECONDS_CHECK(ptr_alarm_struct->alarm_time.rtc_seconds));
    
        if(ptr_alarm_struct->date_weekday_sel == RTC_DATE_WEEKDAY_SEL_DATE)
        {
            ASSERT(DATE_CHECK(ptr_alarm_struct->date_weekday));
        }
        else
        {
            ASSERT(WEEKDAY_CHECK(ptr_alarm_struct->date_weekday));
        }
    }
    else
    {
        if ((RTC->CTR & RTC_CTR_FMT12) != (uint32_t)RESET)
        {
            reg = rtc_bcd_convert_byte(ptr_alarm_struct->alarm_time.rtc_hours);
            ASSERT(RTC_HOUR12_CHECK(reg));
            ASSERT(RTC_AM_PM_HOUR12_CHECK(ptr_alarm_struct->alarm_time.am_pm_sel));
        } 
        else
        {
            ptr_alarm_struct->alarm_time.am_pm_sel = 0x00;
            ASSERT(RTC_HOUR24_CHECK(rtc_bcd_convert_byte(ptr_alarm_struct->alarm_time.rtc_hours)));
        }
        
        ASSERT(RTC_MINUTES_CHECK(rtc_bcd_convert_byte(ptr_alarm_struct->alarm_time.rtc_minutes)));
        ASSERT(RTC_SECONDS_CHECK(rtc_bcd_convert_byte(ptr_alarm_struct->alarm_time.rtc_seconds)));
    
        if(ptr_alarm_struct->date_weekday_sel == RTC_DATE_WEEKDAY_SEL_DATE)
        {
            reg = rtc_bcd_convert_byte(ptr_alarm_struct->date_weekday);
            ASSERT(DATE_CHECK(reg));    
        }
        else
        {
            reg = rtc_bcd_convert_byte(ptr_alarm_struct->date_weekday);
            ASSERT(WEEKDAY_CHECK(reg));      
        }    
    }
    
    /* Check the input parameters format */
    if (format_flag != RTC_FORMAT_BIN)
    {
        reg = (((uint32_t)(ptr_alarm_struct->alarm_time.rtc_hours) << 16) | \
              ((uint32_t)(ptr_alarm_struct->alarm_time.rtc_minutes) << 8) | \
              ((uint32_t)ptr_alarm_struct->alarm_time.rtc_seconds) | \
              ((uint32_t)(ptr_alarm_struct->alarm_time.am_pm_sel) << 16) | \
              ((uint32_t)(ptr_alarm_struct->date_weekday) << 24) | \
              ((uint32_t)ptr_alarm_struct->date_weekday_sel) | \
              ((uint32_t)ptr_alarm_struct->alarm_mask)); 
    }  
    else
    {
        reg = (((uint32_t)rtc_byte_convert_bcd(ptr_alarm_struct->alarm_time.rtc_hours) << 16) | \
              ((uint32_t)rtc_byte_convert_bcd(ptr_alarm_struct->alarm_time.rtc_minutes) << 8) | \
              ((uint32_t)rtc_byte_convert_bcd(ptr_alarm_struct->alarm_time.rtc_seconds)) | \
              ((uint32_t)(ptr_alarm_struct->alarm_time.am_pm_sel) << 16) | \
              ((uint32_t)rtc_byte_convert_bcd(ptr_alarm_struct->date_weekday) << 24) | \
              ((uint32_t)ptr_alarm_struct->date_weekday_sel) | \
              ((uint32_t)ptr_alarm_struct->alarm_mask)); 
    }

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Configure the Alarm register */
    RTC->ACFG1 = (uint32_t)reg;

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn void rtc_alarm_struct_init(rtc_alarm_t* ptr_alarm_struct)
  * @brief  Init alarm struct member default value (time  = 00h:00mn:00sec // Date 
  *         = 1st day of the month // Mask = all fields are not masked).
  * @param  ptr_alarm_struct: pointer to a @ref rtc_alarm_t structure which
  *         will be initialized.
  * @return None
  */
void rtc_alarm_struct_init(rtc_alarm_t* ptr_alarm_struct)
{
    /* Alarm Time Settings : Time = 00h:00mn:00sec */
    ptr_alarm_struct->alarm_time.am_pm_sel = RTC_AM_HOUR12;
    ptr_alarm_struct->alarm_time.rtc_hours = 0;
    ptr_alarm_struct->alarm_time.rtc_minutes = 0;
    ptr_alarm_struct->alarm_time.rtc_seconds = 0;

    /* Alarm Date Settings : Date = 1st day of the month */
    ptr_alarm_struct->date_weekday_sel = RTC_DATE_WEEKDAY_SEL_DATE;
    ptr_alarm_struct->date_weekday = 1;

    /* Alarm Masks Settings : Mask =  all fields are not masked */
    ptr_alarm_struct->alarm_mask = RTC_ALARM_MASK_NONE;
}

/**
  * @fn void rtc_alarm_get(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
  * @brief  Get the RTC Alarm value and masks.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  alarm_flag: Alarm enable (RTC_ALARM_ENABLE).
  * @param  ptr_alarm_struct: pointer to a rtc_alarm_t structure that will 
  *         contains the output alarm configuration values.
  * @return None
  */
void rtc_alarm_get(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
{
    UNUSED(alarm_flag);
    uint32_t reg = 0;

    ASSERT(RTC_FORMAT_CHECK(format_flag));
    ASSERT(RTC_ALARM_ENABLE_CHECK(alarm_flag)); 

    /* Get the RTC_ACFG1 register */
    reg = (uint32_t)(RTC->ACFG1);

    /* Fill the structure with the read parameters */
    ptr_alarm_struct->alarm_time.rtc_hours = (uint8_t)((reg & (RTC_ACFG1_HORTEN | \
                                                     RTC_ACFG1_HORUNT)) >> 16);
    ptr_alarm_struct->alarm_time.rtc_minutes = (uint8_t)((reg & (RTC_ACFG1_MINTEN | \
                                                     RTC_ACFG1_MINUNT)) >> 8);
    ptr_alarm_struct->alarm_time.rtc_seconds = (uint8_t)(reg & (RTC_ACFG1_SECTEN | \
                                                     RTC_ACFG1_SECUNT));
    ptr_alarm_struct->alarm_time.am_pm_sel = (uint8_t)((reg & RTC_ACFG1_PM) >> 16);
    ptr_alarm_struct->date_weekday = (uint8_t)((reg & (RTC_ACFG1_DTEN | RTC_ACFG1_DUNT)) >> 24);
    ptr_alarm_struct->date_weekday_sel = (uint8_t)(reg & RTC_ACFG1_WDSEL);
    ptr_alarm_struct->alarm_mask = (uint8_t)(reg & RTC_ALARM_MASK_ALL);

    if (format_flag == RTC_FORMAT_BIN)
    {
        ptr_alarm_struct->alarm_time.rtc_hours = rtc_bcd_convert_byte(ptr_alarm_struct-> \
                                                        alarm_time.rtc_hours);
        ptr_alarm_struct->alarm_time.rtc_minutes = rtc_bcd_convert_byte(ptr_alarm_struct-> \
                                                        alarm_time.rtc_minutes);
        ptr_alarm_struct->alarm_time.rtc_seconds = rtc_bcd_convert_byte(ptr_alarm_struct-> \
                                                        alarm_time.rtc_seconds);
        ptr_alarm_struct->date_weekday = rtc_bcd_convert_byte(ptr_alarm_struct->date_weekday);
    }  
}

/**
  * @fn error_status_t rtc_alarm_enable_ctrl(uint32_t alarm_flag, enable_state_t enable_flag)
  * @brief  Enables or disables the specified RTC Alarm.
  * @param  alarm_flag: Alarm enable(RTC_ALARM_ENABLE).
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_alarm_enable_ctrl(uint32_t alarm_flag, enable_state_t enable_flag)
{
    __IO uint32_t alarm_counter = 0x00;
    uint32_t alarm_status = 0x00;
    error_status_t status = ERROR;
    
    ASSERT(RTC_ALARM_ENABLE_CHECK(alarm_flag));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
    
    /* Configure the Alarm state */
    if (enable_flag != DISABLE)
    {
        RTC->CTR |= (uint32_t)alarm_flag;

        status = SUCCESS;    
    }
    else
    { 
        /* Disable the Alarm in RTC_CTR register */
        RTC->CTR &= (uint32_t)~alarm_flag;
   
        /* Wait till RTC ALR flag is set and if time out is reached exit */
        do
        {
            alarm_status = RTC->STS & (alarm_flag >> 8);
            alarm_counter++;  
        } while ((alarm_counter != RTC_INIT_MODE_TIMEOUT) && (alarm_status == 0x00));
    
        if ((RTC->STS & (alarm_flag >> 8)) == RESET)
        {
            status = ERROR;
        } 
        else
        {
            status = SUCCESS;
        }        
    } 
    
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
  
    return status;
}

/**
  * @fn void rtc_alarm_subsecond_config(uint32_t alarm_flag, uint32_t subsecond_value, uint8_t subsecond_mask)
  * @brief  Configure the RTC Alarm Subseconds value and mask.
  * @note   This function is performed only when the Alarm is disabled. 
  * @param  alarm_flag: Alarm enable (RTC_ALARM_ENABLE).
  * @param  subsecond_value: It can be a value from 0 to 0x00007FFF.
  * @param  subsecond_mask:  specifies the Subseconds Mask bits (RTC_ACFG2 SSEC[14:0]).
  * @return None
  */
void rtc_alarm_subsecond_config(uint32_t alarm_flag, uint32_t subsecond_value, uint8_t subsecond_mask)
{
    UNUSED(alarm_flag);
    uint32_t reg = 0;

    ASSERT(RTC_ALARM_ENABLE_CHECK(alarm_flag));
    ASSERT(RTC_SUB_SECOND_VALUE_CHECK(subsecond_value));
    ASSERT(RTC_SUB_SECOND_MASK_CHECK(subsecond_mask));
  
    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
  
    /* Configure the Alarm and its subSecond registers */
    reg = (uint32_t) (((uint32_t)(subsecond_value)) | ((uint32_t)(subsecond_mask) << 24));
  
    /* Configure the Alarm's SubSecond register */
    RTC->ACFG2 = reg;

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;

}

/**
  * @fn uint32_t rtc_alarm_subsecond_get(void)
  * @brief  Gets the RTC Alarm Subseconds value.
  * @param  None
  * @return RTC Alarm Subseconds value.
  */
uint32_t rtc_alarm_subsecond_get(void)
{
    uint32_t reg = 0;
  
    /* Get the RTC_ACFG1 register */
    reg = (uint32_t)((RTC->ACFG2) & RTC_ACFG2_SSEC);

    return (reg);
}

/**
  * @fn void rtc_wakeup_clock_config(uint32_t wakeup_clk)
  * @brief  Configures the RTC Wakeup clock source only.   
  *         The WakeUp Clock source can only be changed when the RTC WakeUp
  *         is disabled (Use the rtc_wakeup_enable_ctrl(DISABLE)).
  * @param  wakeup_clk: It can be one of the following values:           
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV16
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV8
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV4 
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV2
  *          @arg RTC_WAKEUP_CLOCK_SPRE_16BITS
  *          @arg RTC_WAKEUP_CLOCK_SPRE_17BITS.
  * @return None
  */
void rtc_wakeup_clock_config(uint32_t wakeup_clk)
{
    ASSERT(RTC_WAKEUP_CLOCK_CHECK(wakeup_clk));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Clear the Wakeup Timer clock source bits */
    RTC->CTR &= (uint32_t)~RTC_CR_WUCKSEL;

    /* Configure the clock source */
    RTC->CTR|= (uint32_t)wakeup_clk;
  
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn void rtc_wakeup_value_set(uint32_t counter) 
  * @brief  Configures the RTC Wakeup counter .
  * @note   The RTC WakeUp counter can only be written when the RTC WakeUp
  *         is disabled (Use the rtc_wakeup_enable_ctrl(DISABLE)).
  * @param  counter: It can be a value from 0x0000 to 0xFFFF.  *           
  * @return None
  */
void rtc_wakeup_value_set(uint32_t counter)
{
    ASSERT(RTC_WAKEUP_COUNTER_CHECK(counter));
    
    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
    
    /* Configure the Wakeup Timer counter */
    RTC->WUTR = (uint32_t)counter;
  
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn uint32_t rtc_wakeup_value_get(void)
  * @brief  Get the RTC WakeUp timer counter value.
  * @param  None
  * @return The RTC WakeUp Counter value.
  */
uint32_t rtc_wakeup_value_get(void)
{
    /* Get the counter value */
    return ((uint32_t)(RTC->WUTR & RTC_WUTR_WUT));
}

/**
  * @fn error_status_t rtc_wakeup_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables the RTC WakeUp timer.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
error_status_t rtc_wakeup_enable_ctrl(enable_state_t enable_flag)
{
    __IO uint32_t wakeup_counter = 0x00;
    uint32_t wakeup_status = 0x00;
    error_status_t status = ERROR;
  
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    if (enable_flag != DISABLE)
    {
        /* Enable the Wakeup Timer */
        RTC->CTR |= (uint32_t)RTC_CR_WUTE;
        status = SUCCESS;    
    }
    else
    {
        /* Disable the Wakeup Timer */
        RTC->CTR &= (uint32_t)~RTC_CR_WUTE;
        /* Wait till RTC WUTWF flag is set and if Time out is reached exit */
        do
        {
            wakeup_status = RTC->STS & RTC_ISR_WUTWF;
            wakeup_counter++;  
        } while((wakeup_counter != RTC_INIT_MODE_TIMEOUT) && (wakeup_status == 0x00));
    
        if ((RTC->STS & RTC_ISR_WUTWF) == RESET)
        {
            status = ERROR;
        }
        else
        {
            status = SUCCESS;
        }    
    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
  
    return status;
}

/**
  * @fn void rtc_daylight_saving_config(uint32_t save_time_value, uint32_t flag)
  * @brief  Adds or substract one hour from the current time.
  * @param  save_time_value: It can be one of the following values:
  *          @arg RTC_DAYLIGHT_SAVING_DEC1H(substract one hour)
  *          @arg RTC_DAYLIGHT_SAVING_INC1H(add one hour)
  * @param  flag: Specifies the value to be written in the SAVEF bit Reset or Set.
  * @return None
  */
void rtc_daylight_saving_config(uint32_t save_time_value, uint32_t flag)
{
    ASSERT(RTC_DAYLIGHT_SAVING_CHECK(save_time_value));
    ASSERT(RTC_DAYLIGHT_SAVING_FLAG_CHECK(flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Clear the bits to be configured */
    RTC->CTR &= (uint32_t)~(RTC_CTR_SAVEF);

    /* Configure the RTC_CTR register */
    RTC->CTR |= (uint32_t)(save_time_value | flag);

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn uint32_t rtc_daylight_saving_flag_get(void)
  * @brief  Returns the RTC Day Light Saving flag.
  * @param  None
  * @return RTC_CTR Day Light Saving stored operation.
  */
uint32_t rtc_daylight_saving_flag_get(void)
{
    return (RTC->CTR & RTC_CTR_SAVEF);
}

/**
  * @fn void rtc_output_config(uint32_t config_value, uint32_t config_polarity)
  * @brief  Configures the RTC output source.
  * @param  config_value: It can be one of the following values:
  *          @arg RTC_OUTPUT_SOURCE_DISABLE(rtc output clock suorce disable)
  *          @arg RTC_OUTPUT_SOURCE_ALARM(select alarm output clock suorce)
  *          @arg RTC_OUTPUT_SOURCE_WAKEUP(select wakeup output clock suorce)
  * @param  config_polarity: It can be one of the following:
  *          @arg RTC_OUTPUT_POLARITY_High(output pin is high)                                          
  *          @arg RTC_OUTPUT_POLARITY_LOW(output pin is low)                        
  * @return None
  */
void rtc_output_config(uint32_t config_value, uint32_t config_polarity)
{
    ASSERT(RTC_OUTPUT_SOURCE_CHECK(config_value));
    ASSERT(RTC_OUTPUT_POLARITY_CHECK(config_polarity));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Clear the bits to be configured */
    RTC->CTR &= (uint32_t)~(RTC_CTR_OUTSRC | RTC_CTR_APOL);

    /* Configure the output selection and polarity */
    RTC->CTR |= (uint32_t)(config_value | config_polarity);

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn void rtc_cali_output_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables calibration Output.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_cali_output_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
  
    if (enable_flag != DISABLE)
    {
        /* Enable the RTC clock output */
        RTC->CTR |= (uint32_t)RTC_CTR_CALOE;
    }
    else
    { 
        /* Disable the RTC clock output */
        RTC->CTR &= (uint32_t)~RTC_CTR_CALOE;
    }
  
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF; 
}

/**
  * @fn void rtc_cali_output_config(uint32_t config_value)
  * @brief  Configure the Calibration Pinout (RTC_CTR_CALSRC) Selection (1Hz or 512Hz).
  * @param  config_value: It can be one of the following values:
  *          @arg RTC_CALI_OUTPUT_512HZ(regular waveform at 512Hz) 
  *          @arg RTC_CALI_OUTPUT_1HZ(regular waveform at 1Hz)
  * @return None
*/
void rtc_cali_output_config(uint32_t config_value)
{
    ASSERT(RTC_CALI_OUTPUT_CHECK(config_value));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
  
    /* clear flags before config */
    RTC->CTR &= (uint32_t)~(RTC_CTR_CALSRC);

    /* Configure the RTC_CTR register */
    RTC->CTR |= (uint32_t)config_value;

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn error_status_t rtc_swallow_config(uint32_t period_value,uint32_t insert_value, uint32_t swallow_value) 
  * @brief  Configures swallow rtc clock Settings.
  * @param  period_value: It can be can be one of the following values:
  *          @arg RTC_CALI_PERIOD_32SECOND(calibration periode is 32s)
  *          @arg RTC_CALI_PERIOD_16SECOND(calibration periode is 16s)
  *          @arg RTC_CALI_PERIOD_8SECOND(calibartion periode is 8s)
  * @param  insert_value: It can be one of the following values:
  *          @arg RTC_CALI_INSERT_CLOCK_SET(Add one RTCCLK puls every 2**11 pulses)
  *          @arg RTC_CALI_INSERT_CLOCK_RESET(No RTCCLK pulses are added)
  * @param  swallow_value: Select the value of SWACLK[8:0] bits,value from 0 to 0x000001FF.
  * @return :SUCCESS or ERROR.
*/
error_status_t rtc_swallow_config(uint32_t period_value,uint32_t insert_value, uint32_t swallow_value)
{
    error_status_t status = ERROR;
    uint32_t counter = 0;

    ASSERT(RTC_CALI_PERIOD_CHECK(period_value));
    ASSERT(RTC_CALI_INSERT_CLOCK_CHECK(insert_value));
    ASSERT(RTC_SWALLOW_CLOCK_VALUE_CHECK(swallow_value));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
  
    /* check if a calibration is pending */
    if ((RTC->STS & RTC_STS_CALBSY) != RESET)
    {
        /* wait until the Calibration is completed */
        while (((RTC->STS & RTC_STS_CALBSY) != RESET) && (counter != RTC_CALI_TIMEOUT))
        {
            counter++;
        }
    }

    /* check if the calibration pending is completed or if there is no calibration operation at all */
    if ((RTC->STS & RTC_STS_CALBSY) == RESET)
    {
        /* Configure the calibration settings */
        RTC->RTC_1SCAL = (uint32_t)((uint32_t)period_value | (uint32_t)insert_value | (uint32_t)swallow_value);

        status = SUCCESS;
    }
    else
    {
        status = ERROR;
    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
  
    return (error_status_t)(status);
}

/**
  * @fn void rtc_time_recording_enable_ctrl(uint32_t edge_flag, enable_state_t enable_flag)
  * @brief  Enables or Disables the RTC recording functionality. 
  * @param  edge_flag: It can be one of the following:
  *          @arg RTC_RECORD_RISING_EDGE(the Time stamp event occurs on the rising edge)                                          
  *          @arg RTC_RECORD_FALLING_EDGE(the Time stamp event occurs on the falling edge)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */     
void rtc_time_recording_enable_ctrl(uint32_t edge_flag, enable_state_t enable_flag)
{
    uint32_t reg = 0;

    ASSERT(RTC_RECORD__EDGE_CHECK(edge_flag));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Get the RTC_CTR register and clear the bits to be configured */
    reg = (uint32_t)(RTC->CTR & (uint32_t)~(RTC_CTR_TRESEL | RTC_CTR_TRE));

    /* Get the new configuration */
    if (enable_flag != DISABLE)
    {
        reg |= (uint32_t)(edge_flag | RTC_CTR_TRE);
    }
    else
    {
        reg |= (uint32_t)(edge_flag);
    }

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    /* Configure the recording TRESEL and Enable bits */
    RTC->CTR = (uint32_t)reg;

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
}

/**
  * @fn void rtc_time_recording_get(uint32_t format_flag, rtc_time_t* ptr_time_struct, rtc_date_t* ptr_date_struct)
  * @brief  Get the RTC time recording value and masks.
  * @param  format_flag: It can be one of the following values:
  *          @arg RTC_FORMAT_BIN(Binary data format) 
  *          @arg RTC_FORMAT_BCD(BCD data format)
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that will 
  *         contains the recording time values. 
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that will 
  *         contains the recording date values.     
  * @return None
  */
void rtc_time_recording_get(uint32_t format_flag, rtc_time_t* ptr_time_struct, rtc_date_t* ptr_date_struct)
{
    uint32_t temp_time = 0;
    uint32_t temp_date = 0;

    ASSERT(RTC_FORMAT_CHECK(format_flag));

    /* Get the recording time and date registers values */
    temp_time = (uint32_t)(RTC->TR2 & RTC_TIME_RESERVED_MASK);
    temp_date = (uint32_t)(RTC->TR1 & RTC_DATE_RESERVED_MASK);

    /* Fill the Time structure fields with the read parameters */
    ptr_time_struct->rtc_hours = (uint8_t)((temp_time & (RTC_TIME_HORTEN | RTC_TIME_HORUNT)) >> 16);
    ptr_time_struct->rtc_minutes = (uint8_t)((temp_time & (RTC_TIME_MINTEN | RTC_TIME_MINUNT)) >> 8);
    ptr_time_struct->rtc_seconds = (uint8_t)(temp_time & (RTC_TIME_SECTEN | RTC_TIME_SECUNT));
    ptr_time_struct->am_pm_sel = (uint8_t)((temp_time & (RTC_TIME_PM)) >> 16);  

    /* Fill the Date structure fields with the read parameters */
    ptr_date_struct->rtc_year = 0;
    ptr_date_struct->rtc_month = (uint8_t)((temp_date & (RTC_DATE_MTEN | RTC_DATE_MUNT)) >> 8);
    ptr_date_struct->rtc_date = (uint8_t)(temp_date & (RTC_DATE_DTEN | RTC_DATE_DUNT));
    ptr_date_struct->rtc_weekday = (uint8_t)((temp_date & (RTC_DATE_WUNT)) >> 13);

    /* Check the input parameters format */
    if (format_flag == RTC_FORMAT_BIN)
    {
        /* Convert the Time structure parameters to Binary format */
        ptr_time_struct->rtc_hours = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_hours);
        ptr_time_struct->rtc_minutes = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_minutes);
        ptr_time_struct->rtc_seconds = (uint8_t)rtc_bcd_convert_byte(ptr_time_struct->rtc_seconds);

        /* Convert the Date structure parameters to Binary format */
        ptr_date_struct->rtc_month = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_month);
        ptr_date_struct->rtc_date = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_date);
        ptr_date_struct->rtc_weekday = (uint8_t)rtc_bcd_convert_byte(ptr_date_struct->rtc_weekday);
    }
}

/**
  * @fn uint32_t rtc_time_recording_subsecond_get(void)
  * @brief  Get the RTC recording  Subseconds(SSEC) value.
  * @param  None
  * @return RTC current  Subseconds value.
  */
uint32_t rtc_time_recording_subsecond_get(void)
{
    /* Get recording subseconds values from the correspondent registers */
    return (uint32_t)(RTC->TR3);
}

/**
  * @fn void rtc_inbreak_enable_config(uint32_t set_value, uint32_t trigger_flag)
  * @brief  Configures the select inbreak input enable pin and triggle condition.
  * @param  set_value: Selected inbreak pin. It can be any combination of the following values:
  *          @arg RTC_INBREAK1_ENABLE(Select inbreak 1)
  *          @arg RTC_INBREAK2_ENABLE(Select inbreak 2)
  * @param  trigger_flag: Specifies the trigger on the inbreak pin condition 
  *          It can be one of the following values:           
  *           @arg TC_INBREAK1_RISING_EDGE(Rising Edge of the inbreak pin)
  *           @arg RTC_INBREAK1_FALLING_EDGE(Falling Edge of the inbreak pin)
  *           @arg RTC_INBREAK1_LOW_LEVEL(Low Level of the inbreak pin)
  *           @arg RTC_INBREAK1_HIGH_LEVEL(High Level of the inbreak pin)  
  * @return None
  */
void rtc_inbreak_enable_config(uint32_t set_value, uint32_t trigger_flag)
{
    ASSERT(RTC_INBREAK_ENABLE_CHECK(set_value)); 
    ASSERT(RTC_INBREAK1_LEVEL_CHECK(trigger_flag));
 
    if (trigger_flag == RTC_INBREAK1_RISING_EDGE)
    {  
        /* Configure the RTC_MF register */
        RTC->MF &= (uint32_t)((uint32_t)~(set_value << 1));
    }
    else
    { 
        /* Configure the RTC_MF register */
        RTC->MF |= (uint32_t)(set_value << 1);  
    }  
}

/**
  * @fn void rtc_inbreak_enable_ctrl(uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or Disables the inbreak detection.
  * @param  set_value: Selected inbreak pin. It can be any combination of the following values:
  *          @arg RTC_INBREAK1_ENABLE(Select inbreak 1)
  *          @arg RTC_INBREAK2_ENABLE(Select inbreak 2)
  * @param  enable_flag: It can be: ENABLE or DISABLE.                  
  * @return None
  */
void rtc_inbreak_enable_ctrl(uint32_t set_value, enable_state_t enable_flag)
{
    ASSERT(RTC_INBREAK_ENABLE_CHECK(set_value));  
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the selected inbreak pin */
        RTC->MF |= (uint32_t)set_value;
    }
    else
    {
        /* Disable the selected inbreak pin */
        RTC->MF &= (uint32_t)~set_value;    
    }  
}

/**
  * @fn void rtc_inbreak_filter_config(uint32_t set_value)
  * @brief  Configures the inbreak Filter.
  * @param  set_value: Specifies the inbreak filter,it can be one of the following values:
  *          @arg RTC_INBREAK_FILTER_DISABLE(inbreak filter is disabled)
  *          @arg RTC_INBREAK_FILTER_2SAMPLE(inbreak is activated after 2 consecutive samples at the active level) 
  *          @arg RTC_INBREAK_FILTER_4SAMPLE(inbreak is activated after 4 consecutive samples at the active level) 
  *          @arg RTC_INBREAK_FILTER_8SAMPLE(inbreak is activated after 8 consecutive samples at the active level)   
  * @return None
  */
void rtc_inbreak_filter_config(uint32_t set_value)
{
    ASSERT(RTC_INBREAK_FILTER_CHECK(set_value));
   
    /* Clear INBRKFLT[1:0] bits in the RTC_MF register */
    RTC->MF &= (uint32_t)~(RTC_MF_INBRKFLT);

    /* Configure the RTC_MF register */
    RTC->MF |= (uint32_t)set_value;
}

/**
  * @fn void rtc_inbreak_sample_freq_config(uint32_t freq_vale)
  * @brief  Configures the inbreak sampling frequency.
  * @param  freq_vale: Specifies the inbreak sampling frequency,it can be one of the following values:
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV32768(each of the inbreak inputs are sampled with freq RTCCLK / 32768)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV16384(each of the inbreak inputs are sampled with freq RTCCLK / 16384)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV8192(each of the inbreak inputs are sampled with freq RTCCLK / 8192)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV4096(each of the inbreak inputs are sampled with freq RTCCLK / 4096)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV2048(each of the inbreak inputs are sampled with freq RTCCLK / 2048)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV1024(each of the inbreak inputs are sampled with freq RTCCLK / 1024)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV512(each of the inbreak inputs are sampled with freq RTCCLK / 512)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV256(each of the inbreak inputs are sampled with freq RTCCLK / 256)                                                       
  * @return None
  */
void rtc_inbreak_sample_freq_config(uint32_t freq_vale)
{
    ASSERT(RTC_INBREAK_SAMPLE_FREQ_CHECK(freq_vale));
 
    /* Clear INBRKFREQ[2:0] bits in the RTC_MF register */
    RTC->MF &= (uint32_t)~(RTC_MF_INBRKFREQ);

    /* Configure the RTC_MF register */
    RTC->MF |= (uint32_t)freq_vale;
}

/**
  * @fn void rtc_inbreak_precharge_duration(uint32_t set_value)
  * @brief  Configures the inbreak Pins input Precharge Duration.
  * @param  set_value: it can be one of the following values:
  *          @arg RTC_INBREAK_PRECHARGE_1RTCCLK(inbreak pins are precharged before sampling during 1 RTCCLK cycle)
  *          @arg RTC_INBREAK_PRECHARGE_2RTCCLK(inbreak pins are precharged before sampling during 2 RTCCLK cycle) 
  *          @arg RTC_INBREAK_PRECHARGE_4RTCCLK(inbreak pins are precharged before sampling during 4 RTCCLK cycle) 
  *          @arg RTC_INBREAK_PRECHARGE_8RTCCLK(inbreak pins are precharged before sampling during 8 RTCCLK cycle)  
  * @return None
  */
void rtc_inbreak_precharge_duration(uint32_t set_value)
{
    ASSERT(RTC_INBREAK_PRECHARGE_CHECK(set_value));
   
    /* Clear INBRKPRCH[1:0] bits in the RTC_MF register */
    RTC->MF &= (uint32_t)~(RTC_MF_INBRKPRCH);

    /* Configure the RTC_MF register */
    RTC->MF |= (uint32_t)set_value;
}

/**
  * @fn void rtc_recording_inbreak_detect_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables recording on inbreak detection event. 
  * @param  enable_flag: it can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_recording_inbreak_detect_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
   
    if (enable_flag != DISABLE)
    {
        /* Save recording on inbreak detection event */
        RTC->MF |= (uint32_t)RTC_MF_INBRKTR;
    }
    else
    {
        /* inbreak detection does not cause a recording to be saved */
        RTC->MF &= (uint32_t)~RTC_MF_INBRKTR;    
    }
}

/**
  * @fn void rtc_inbreak_pull_up_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables the Precharge of Tamper pin.
  * @param  enable_flag: new state of tamper pull up.
  *         This parameter can be: ENABLE or DISABLE.                   
  * @return None
  */
void rtc_inbreak_pull_up_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable precharge of the selected inbreak pin */
        RTC->MF &= (uint32_t)~RTC_MF_INBRKPUDIS; 
    } 
    else
    {
        /* Disable precharge of the selected inbreak pin */
        RTC->MF |= (uint32_t)RTC_MF_INBRKPUDIS;    
    } 
}

/**
  * @fn void rtc_ao_register_write(uint32_t ao_reg, uint32_t set_value)
  * @brief Writes a data in the specified AO (always on) registers.
  * @param  ao_reg: AO (always on) registers number, it can be: A00~A04.
  * @param  set_value: Data to be written in the specified AO (always on) registers.                    
  * @return None
  */
void rtc_ao_register_write(uint32_t ao_reg, uint32_t set_value)
{
    __IO uint32_t temp = 0;
  
    ASSERT(RTC_AO_CHECK(ao_reg));

    temp = RTC_BASE + 0x50;
    temp += (ao_reg * 4);

    /* Write the specified register */
    *(__IO uint32_t *)temp = (uint32_t)set_value;
}

/**
  * @fn uint32_t rtc_ao_register_read(uint32_t ao_reg)
  * @brief  Reads data from the specified AO (always on) registers.
  * @param  ao_reg: AO (always on) registers number, it can be: A00~A04.                                          
  * @return the value of the specified AO (always on) registers.
  */
uint32_t rtc_ao_register_read(uint32_t ao_reg)
{
    __IO uint32_t temp = 0;
  
    ASSERT(RTC_AO_CHECK(ao_reg));

    temp = RTC_BASE + 0x50;
    temp += (ao_reg * 4);
  
    /* Read the specified register */
    return (*(__IO uint32_t *)temp);
}

/**
  * @fn void rtc_output_type_config(uint32_t output_type)
  * @brief  Configures the RTC Output Pin mode. 
  * @param  output_type: specifies the RTC Output (PC13) pin mode.
  *          It can be one of the following values:
  *           @arg RTC_OUTPUT_OPEN_DRAIN(RTC output (PC13) is configured in open drain mode)                                     
  *           @arg RTC_OUTPUT_PUSH_PULL(RTC Output (PC13) is configured in Push Pull mode)                                    
  * @return None
  */
void rtc_output_type_config(uint32_t output_type)
{
    ASSERT(RTC_OUTPUT_TYPE_CHECK(output_type));
  
    RTC->MF &= (uint32_t)~(RTC_MF_PC13DATA);
    RTC->MF |= (uint32_t)(output_type);  
}

/**
  * @fn error_status_t rtc_adj_register_config(uint32_t second_flag, uint32_t subsecond_value)
  * @brief Configures the adjustment(ADJ) register.
  * @param  second_flag: Select to add or not 1 second to the time Calendar.
  *          It can be one of the following values :
  *           @arg RTC_ADJ_INC1S_SET(add one second to the clock calendar)
  *           @arg RTC_ADJ_INC1S_RESET(no effect)
  * @param  subsecond_value: It can be one any value from 0 to 0x7FFF.
  * @return An error_status_t: SUCCESS or ERROR.
*/
error_status_t rtc_adj_register_config(uint32_t second_flag, uint32_t subsecond_value)
{
    error_status_t status = ERROR;
    uint32_t count = 0;

    ASSERT(RTC_ADJ_INC1S_CHECK(second_flag));
    ASSERT(RTC_ADJ_SSEC_CHECK(subsecond_value));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;
    
    /* Check if a Shift is pending */
    if ((RTC->STS & RTC_STS_SHF_BUSY) != RESET)
    {
        /* Wait until the shift is completed */
        while (((RTC->STS & RTC_STS_SHF_BUSY) != RESET) && (count != RTC_SHPF_TIMEOUT))
        {
            count++;
        }
    }

    /* Check if the Shift pending is completed or if there is no Shift operation at all */
    if ((RTC->STS & RTC_STS_SHF_BUSY) == RESET)
    {
        /* check if the reference clock detection is disabled */
        if((RTC->CTR & RTC_CTR_OPCLKEN) == RESET)
        {
            /* Configure the adjustment register */
            RTC->ADJ = (uint32_t)(uint32_t)(subsecond_value) | (uint32_t)(second_flag);
    
            if(rtc_synchro_wait() == ERROR)
            {
                status = ERROR;
            }
            else
            {
                status = SUCCESS;
            }
        }
        else
        {
            status = ERROR;
        }
    }
    else
    {
        status = ERROR;
    }

    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF;
  
    return (error_status_t)(status);
}

/**
  * @fn void rtc_interrupt_config(uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified RTC interrupts.
  * @param  set_value: specifies the RTC interrupt sources to be enabled or disabled. 
  * @param  enable_flag: new state of the specified RTC interrupts, it can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_interrupt_config(uint32_t set_value, enable_state_t enable_flag)
{
    ASSERT(RTC_CONFIG_INTERRUPT_CHECK(set_value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    /* Disable the write protection for RTC registers */
    RTC->CCODE = 0xCA;
    RTC->CCODE = 0x53;

    if (enable_flag != DISABLE)
    {
        /* Configure the Interrupts in the RTC_CTR register */
        RTC->CTR |= (uint32_t)(set_value & ~RTC_MF_INBRKIE);
        /* Configure the Tamper Interrupt in the RTC_MF */
        RTC->MF |= (uint32_t)(set_value & RTC_MF_INBRKIE);
    }
    else
    {
        /* Configure the Interrupts in the RTC_CTR register */
        RTC->CTR &= (uint32_t)~(set_value & (uint32_t)~RTC_MF_INBRKIE);
        /* Configure the Tamper Interrupt in the RTC_MF */
        RTC->MF &= (uint32_t)~(set_value & RTC_MF_INBRKIE);
    }
    /* Enable the write protection for RTC registers */
    RTC->CCODE = 0xFF; 
}

/**
  * @fn bit_status_t rtc_flag_status_get(uint32_t set_flag) 
  * @brief  Checks whether the specified RTC flag is set or not.
  * @param  set_flag: specifies the RTC_STS register status bit.
  * @return The new state of  set_flag (SET or RESET).
  */
bit_status_t rtc_flag_status_get(uint32_t set_flag)
{
    bit_status_t status = RESET;
    uint32_t reg = 0;
  
    ASSERT(RTC_FLAG_GET_CHECK(set_flag));
  
    /* Get all the flags */
    reg = (uint32_t)(RTC->STS & RTC_FLAGS_MASK);
  
    /* Return the status of the flag */
    if ((reg & set_flag) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    return status;
}

/**
  * @fn void rtc_flag_clear(uint32_t set_flag)
  * @brief  Clear the Flags in the RTC_STS register.
  * @param  set_flag: specifies the clear bit in the RTC_STS register.
  * @return None
  */
void rtc_flag_clear(uint32_t set_flag)
{
    ASSERT(RTC_FLAG_CLEAR_CHECK(set_flag));
	
    /* Clear the Flags in the RTC_STS register */
    RTC->STS = (uint32_t)((uint32_t)(~((set_flag | RTC_STS_INITE)& 0x0001FFFF) | (uint32_t)(RTC->STS & RTC_STS_INITE)));    
}

/**
  * @fn bit_status_t rtc_interrupt_status_get(uint32_t status_bit)
  * @brief  Get the specified RTC interrupt status in RTC_STS register.
  * @param  status_bit: specifies the RTC interrupt source in RTC_STS register.
  * @return The new state of status_bit (SET or RESET).
  */
bit_status_t rtc_interrupt_status_get(uint32_t status_bit)
{
    bit_status_t temp_status = RESET;
    uint32_t reg = 0;
    uint32_t enable_status = 0;
 
    ASSERT(RTC_GET_INTERRUPT_CHECK(status_bit));
  
    /* Get the inbreak Interrupt enable bit and pending bit */
    reg = (uint32_t)(RTC->MF & (RTC_MF_INBRKIE));
 
    /* Get the Interrupt enable Status */
    enable_status = (uint32_t)((RTC->CTR & status_bit) | (reg & ((status_bit >> (status_bit >> 18)) >> 15)));
  
    /* Get the Interrupt pending bit */
    reg = (uint32_t)((RTC->STS & (uint32_t)(status_bit >> 4)));
  
    /* Get the status of the Interrupt */
    if ((enable_status != (uint32_t)RESET) && ((reg & 0x0000FFFF) != (uint32_t)RESET))
    {
        temp_status = SET;
    }
    else
    {
        temp_status = RESET;
    }
    return temp_status;
}

/**
  * @fn void rtc_interrupt_flag_clear(uint32_t status_bit)
  * @brief  Clears the RTC's interrupt pending bits.
  * @param  status_bit: specifies the RTC interrupt pending bit to clear.
  * @return None
  */
void rtc_interrupt_flag_clear(uint32_t status_bit)
{
    uint32_t reg = 0;

    ASSERT(RTC_CLEAR_INTERRUPT_CHECK(status_bit));

    /* Get the RTC_STS Interrupt pending bits mask */
    reg = (uint32_t)(status_bit >> 4);

    /* Clear the interrupt pending bits in the RTC_STS register */
    RTC->STS = (uint32_t)((uint32_t)(~((reg | RTC_STS_INITE)& 0x0000FFFF) | (uint32_t)(RTC->STS & RTC_STS_INITE))); 
}

/**
  * @fn static uint8_t rtc_byte_convert_bcd(uint8_t value)
  * @brief  Converts a 2 digit decimal to BCD format.
  * @param  Value: Byte to be converted.
  * @return Converted byte
  */
static uint8_t rtc_byte_convert_bcd(uint8_t value)
{
    uint8_t bcd_high = 0;
  
    while (value >= 10)
    {
        bcd_high++;
        value -= 10;
    }
  
    return  ((uint8_t)(bcd_high << 4) | value);
}

/**
  * @fn static uint8_t rtc_bcd_convert_byte(uint8_t Value)
  * @brief  Convert from 2 digit BCD to Binary.
  * @param  Value: BCD value to be converted.
  * @return Converted byte.
  */
static uint8_t rtc_bcd_convert_byte(uint8_t value)
{
    uint8_t temp = 0;
  
    temp = ((uint8_t)(value & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
    return (temp + (value & (uint8_t)0x0F));
}



/**
  * @}
  */ 

/**
  * @}
  */ 

