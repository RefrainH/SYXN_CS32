/**
  * @file   cs32f0xx_debug.h 
  * @brief  This file contains all the functions prototypes for the DEBUG firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_DEBUG_H__
#define __CS32F0XX_DEBUG_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"



/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup DEBUG
  * @{
  */ 


/**
  * @brief DEBUG
  */

/** 
  * @ brief Debug support mode for low power configuration register 
  */
#define DEBUG_MODE_DSM              DBG_LPWCFG_DSM_DBGEN  //!< Stop mode debug support 
#define DEBUG_MODE_PWD              DBG_LPWCFG_PWD_DBGEN   //!< Standby mode debug support

#define DEBUG_MODE_CHECK(value) ((((value) & 0xFFFFFFF9) == 0x00) && ((value) != 0x00))

/** 
  * @ brief Debug support mode for APB1 peripherals configuration register 
  */
#define DEBUG_TIM2_PAUSE             DBG_APB1CFG_TIM2_DBG_PAUSE //!< TIM2 counter paused when system is in debug mode 
#define DEBUG_TIM3_PAUSE             DBG_APB1CFG_TIM3_DBG_PAUSE //!< TIM3 counter paused when system is in debug mode
#define DEBUG_TIM6_PAUSE             DBG_APB1CFG_TIM6_DBG_PAUSE //!< TIM6 counter paused when system is in debug mode
#define DEBUG_TIM7_PAUSE             DBG_APB1CFG_TIM7_DBG_PAUSE //!< TIM7 counter paused when system is in debug mode  
#define DEBUG_TIM14_PAUSE            DBG_APB1CFG_TIM14_DBG_PAUSE//!< TIM14 counter paused when system is in debug mode
#define DEBUG_RTC_PAUSE              DBG_APB1CFG_RTC_DBG_PAUSE  //!< RTC paused when system is in debug mode
#define DEBUG_WWDT_PAUSE             DBG_APB1CFG_WWDT_DBG_PAUSE //!< Window watchdog paused when system is in debug mode
#define DEBUG_FWDT_PAUSE             DBG_APB1CFG_FWDT_DBG_PAUSE //!< Free watchdog paused when system is in debug mode
#define DEBUG_I2C1_SMBUS_PAUSE       DBG_APB1CFG_I2C1_SMBUS_DBG_PAUSE //!< I2C1 SMBUS paused when system is in debug mode
#define DEBUG_CAN1_PAUSE             DBG_APB1CFG_CAN_DBG_PAUSE  //!< CAN1 paused when system is in debug mode

#define DEBUG_APB1PERIP_PAUSE_CHECK(periph) ((((periph) & 0xFDDFE2CC) == 0x00) && ((periph) != 0x00))

/** 
  * @ brief Debug support mode for APB2 peripherals configuration register 
  */
#define DEBUG_TIM1_PAUSE             DBG_APB2CFG_TIM1_DBG_PAUSE  //!< TIM1 counter paused when system is in debug mode
#define DEBUG_TIM15_PAUSE            DBG_APB2CFG_TIM15_DBG_PAUSE //!< TIM15 counter paused when system is in debug mode
#define DEBUG_TIM16_PAUSE            DBG_APB2CFG_TIM16_DBG_PAUSE //!< TIM16 counter paused when system is in debug mode
#define DEBUG_TIM17_PAUSE            DBG_APB2CFG_TIM17_DBG_PAUSE //!< TIM17 counter paused when system is in debug mode
#define DEBUG_APB2PERIP_PAUSE_CHECK(periph) ((((periph) & 0xFFF8F7FF) == 0x00) && ((periph) != 0x00))


/**
  * @fn uint32_t debug_revision_id_get(void)
  * @brief  Returns the device revision identifier(REVID).
  * @param  None
  * @return Device revision identifier
  */
uint32_t debug_revision_id_get(void);

/**
  * @fn uint32_t debug_dev_id_get(void)
  * @brief  Returns the device identifier(DEVID).
  * @param  None
  * @return Device identifier
  */
uint32_t debug_dev_id_get(void);

/**
  * @fn void debug_low_power_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configures low power mode behavior when the MCU is in Debug mode.
  * @param  value: specifies the low power mode, it can be:
  *          @arg DEBUG_MODE_DSM(stop mode debug support)
  *          @arg DEBUG_MODE_PWD(Standby mode debug support)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void debug_low_power_config(uint32_t value, enable_state_t enable_flag);

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
void debug_apb1_peripheral_config(uint32_t value, enable_state_t enable_flag);

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
void debug_apb2_peripheral_config(uint32_t value, enable_state_t enable_flag);

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

