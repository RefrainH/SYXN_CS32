/**
  * @file    cs32f0xx_debug.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Debug MCU peripheral:
  *           Device and Revision ID management;
  *           Peripherals Configuration.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "cs32f0xx_debug.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup DEBUG
  * @{
  */


#define IDCODE_DEVID_MASK    ((uint32_t)0x00000FFF)

/**
  * @fn uint32_t debug_revision_id_get(void)
  * @brief  Returns the device revision identifier(REVID).
  * @param  None
  * @return Device revision identifier
  */
uint32_t debug_revision_id_get(void)
{
    /* Return device revision ID */
    return(DBGMCU->ID >> 16);
}

/**
  * @fn uint32_t debug_dev_id_get(void)
  * @brief  Returns the device identifier(DEVID).
  * @param  None
  * @return Device identifier
  */
uint32_t debug_dev_id_get(void)
{
    /* Return device ID */
    return(DBGMCU->ID & IDCODE_DEVID_MASK);
}

/**
  * @fn void debug_low_power_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configures low power mode behavior when the MCU is in Debug mode.
  * @param  value: specifies the low power mode, it can be:
  *          @arg DEBUG_MODE_DSM(stop mode debug support)
  *          @arg DEBUG_MODE_PWD(Standby mode debug support)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void debug_low_power_config(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(DEBUG_MODE_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable low power mode */
        DBGMCU->LPWCFG |= value;
    }
    else
    {
        /* Disable low power mode */
        DBGMCU->LPWCFG &= ~value;
        }
}

/**
  * @fn void debug_apb1_peripheral_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configures APB1 peripheral behavior when the system is in Debug mode.
  * @param  value: specifies the APB1 peripheral, it can be:
  *          @arg DEBUG_TIM2_PAUSE(TIM2 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM3_PAUSE(TIM3 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM6_PAUSE(TIM6 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM7_PAUSE(TIM7 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM14_PAUSE(TIM14 counter paused when system is in debug mode)
  *          @arg DEBUG_RTC_PAUSE(RTC counter paused when system is in debug mode)
  *          @arg DEBUG_WWDT_PAUSE( Debug WWDT stopped when system is in debug mode)
  *          @arg DEBUG_FWDT_PAUSE(Debug FWDT stopped when system is in debug mode)
  *          @arg DEBUG_I2C1_SMBUS_PAUSE(I2C1 SMBUS timeout mode stopped when system is in debug mode)                                     
  *          @arg DEBUG_CAN1_PAUSE(Debug CAN stopped when system is in debug mode)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void debug_apb1_peripheral_config(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(DEBUG_APB1PERIP_PAUSE_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        DBGMCU->APB1CFG |= value;
    }
    else    
    {
        DBGMCU->APB1CFG &= ~value;
    }
}

/**
  * @fn void debug_apb2_peripheral_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configures APB2 peripheral behavior when the MCU is in Debug mode.
  * @param  value: specifies the APB2 peripheral, it can be: 
  *          @arg DEBUG_TIM1_PAUSE(TIM2 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM15_PAUSE(TIM15 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM16_PAUSE(TIM16 counter paused when system is in debug mode)
  *          @arg DEBUG_TIM17_PAUSE(TIM17 counter paused when system is in debug mode)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void debug_apb2_peripheral_config(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(DEBUG_APB2PERIP_PAUSE_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        DBGMCU->APB2CFG |= value;
    }
    else
    {
        DBGMCU->APB2CFG &= ~value;
    }
}


/**
  * @}
  */

/**
  * @}
  */

