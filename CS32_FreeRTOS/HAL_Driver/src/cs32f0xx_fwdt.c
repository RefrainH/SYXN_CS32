/**
  * @file    cs32f0xx_fwdt.c  
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Free Watchdog Timer(FWDT) peripheral:           
  *           + Prescaler and Counter configuration
  *           + FWDT activation
  *           + Flag management
  *
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  * @verbatim  
  *
  ----------------------- this driver instruction for use--------------------------- 
    The driver using to FWDT peripheral with either window option enabled or disabled.         
    1. Window option is enabled:    
        a.Start the FWDT using fwdt_enable() function, when the FWDT is used
             in software mode (no need to enable the LRC, it will be enabled by hardware).                     
        b.FWDT_PDIV and FWDT_UVAL registers can be accessed by
             fwdt_write_access_enable_ctrl() function.
        c.Configure the FWDT prescaler using fwdt_prescaler_set() function.
        d.Configure the FWDT counter value using fwdt_reload_set() function.
             This value will be loaded in the FWDT counter each time the counter
             is reloaded, then the FWDT will start counting down from this value.
        e.Wait for the FWDT registers to be updated using fwdt_flag_status_get() function.
        f. Configure the FWDT refresh window using fwdt_window_value_set() function.

    2. Window option is disabled:    
        a. Enable write access to FWDT_PDIV and FWDT_UVAL registers using
             fwdt_write_access_enable_ctrl() function.
        b. Configure the FWDT prescaler using fwdt_prescaler_set() function.
        c. Configure the FWDT counter value using fwdt_reload_set() function.
             This value will be loaded in the FWDT counter each time the counter
             is reloaded, then the FWDT will start counting down from this value.
        d. Wait for the FWDT registers to be updated using fwdt_flag_status_get() function.
        e. reload the FWDT counter at regular intervals during normal operation 
             to prevent an MCU reset, using fwdt_reload_counter() function.
        f. Start the FWDT using fwdt_enable() function, when the FWDT is used
             in software mode (no need to enable the LRC, it will be enabled
             by hardware).
              
    @endverbatim
  *
  */


#include "cs32f0xx_fwdt.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup FWDT
  * @brief FWDT driver modules
  * @{
  */ 



/* 
  *@brief FWDT CCODE register bit mask 
  */
#define CCODE_CCODE_RELOAD    ((uint16_t)0xAAAA)
#define CCODE_CCODE_ENABLE    ((uint16_t)0xCCCC)


/** 
  *@ brief reload value check 
  */
#define FWDT_RELOAD_CHECK(reload) ((reload) <= 0xFFF)
#define FWDT_WINDOW_VALUE_CHECK(value) ((value) <= 0xFFF)


/* 
  *@brief FWDT write access check 
  */
#define FWDT_WRITE_ACCESS_CHECK(access) (((access) == FWDT_WRITE_ACCESS_ENABLE) || \
                                         ((access) == FWDT_WRITE_ACCESS_DISABLE))

/* 
  *@brief FWDT prescaler check 
  */
#define FWDT_PRESCALER_CHECK(prescaler) (((prescaler) == FWDT_PRESCALER_4)   || \
                                         ((prescaler) == FWDT_PRESCALER_8)   || \
                                         ((prescaler) == FWDT_PRESCALER_16)  || \
                                         ((prescaler) == FWDT_PRESCALER_32)  || \
                                         ((prescaler) == FWDT_PRESCALER_64)  || \
                                         ((prescaler) == FWDT_PRESCALER_128) || \
                                         ((prescaler) == FWDT_PRESCALER_256))



/**
  * @fn void fwdt_write_access_enable_ctrl(uint16_t write_ctrl)
  * @brief  Enables or disables write access to FWDT_PDIV and FWDT_UVAL registers.
  * @param  write_ctrl: This parameter can be Enables or disables.
  * @return None
  */
void fwdt_write_access_enable_ctrl(uint16_t write_ctrl)
{
    ASSERT(FWDT_WRITE_ACCESS_CHECK(write_ctrl));
  
    FWDT->CCODE = write_ctrl;
}

/**
  * @fn void fwdt_prescaler_set(uint8_t prescaler_value)
  * @brief  Sets FWDT Prescaler value.
  * @param  prescaler_value: specifies the FWDT Prescaler value.
  *          This parameter can be one of the following values:
  *            @arg FWDT_PRESCALER_4(FWDT prescaler set to 4)
  *            @arg FWDT_PRESCALER_8(FWDT prescaler set to 8)
  *            @arg FWDT_PRESCALER_16(FWDT prescaler set to 16)
  *            @arg FWDT_PRESCALER_32(FWDT prescaler set to 32)
  *            @arg FWDT_PRESCALER_64(FWDT prescaler set to 64)
  *            @arg FWDT_PRESCALER_128(FWDT prescaler set to 128)
  *            @arg FWDT_PRESCALER_256(FWDT prescaler set to 256)
  * @return None
  */
void fwdt_prescaler_set(uint8_t prescaler_value)
{
    ASSERT(FWDT_PRESCALER_CHECK(prescaler_value));
  
    FWDT->PDIV = prescaler_value;
}

/**
  * @fn void fwdt_reload_set(uint16_t reload_value)
  * @brief  Sets FWDT Reload value.
  * @param  reload_value: specifies the FWDT Reload value.
  *          This parameter must be a number between 0 and 0x0FFF.
  * @return None
  */
void fwdt_reload_set(uint16_t reload_value)
{
    ASSERT(FWDT_RELOAD_CHECK(reload_value));
  
    FWDT->UVAL = reload_value;
}

/**
  * @fn void fwdt_reload_counter(void)
  * @brief  Reloads FWDT counter with value defined in the reload register
  * @param  None
  * @return None
  */
void fwdt_reload_counter(void)
{
    FWDT->CCODE = CCODE_CCODE_RELOAD;
}

/**
  * @fn void fwdt_window_value_set(uint16_t window_value) 
  * @brief  Sets the FWDT window value.
  * @param  window_value: specifies the window value to be compared to the downcounter.
  * @return None
  */
void fwdt_window_value_set(uint16_t window_value)
{
    ASSERT(FWDT_WINDOW_VALUE_CHECK(window_value));
  
    FWDT->WINVAL = window_value;
}

/**
  * @fn void fwdt_enable(void)
  * @brief  Enables FWDT (write access to FWDT_PDIV and FWDT_UVAL registers disabled).
  * @param  None
  * @return None
  */
void fwdt_enable(void)
{
    FWDT->CCODE = CCODE_CCODE_ENABLE;
}

/**
  * @fn bit_status_t fwdt_flag_status_get(uint16_t fwdt_flag)
  * @brief  Checks whether the specified FWDT flag is set or not.
  * @param  fwdt_flag: specifies the flag to check.It can be one of the following values:           
  *            @arg FWDT_FLAG_DRF(Prescaler Value Update on going)
  *            @arg FWDT_FLAG_UVRF(Reload Value Update on going)
  *            @arg FWDT_FLAG_WRF(Counter Window Value Update on going)
  * @return  bit status (SET or RESET).
  */
bit_status_t fwdt_flag_status_get(uint16_t fwdt_flag)
{
    bit_status_t bit_status = RESET;
  
    ASSERT(FWDT_FLAG_CHECK(fwdt_flag));
  
    if ((FWDT->STS & fwdt_flag) != (uint32_t)RESET)
    {
        bit_status = SET;
    }
    else
    {
        bit_status = RESET;
    }
    
    return bit_status;
}

/**
  * @}
  */


/**
  * @}
  */

