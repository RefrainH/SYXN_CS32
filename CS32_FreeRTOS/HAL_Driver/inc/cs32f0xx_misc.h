/**
  * @file    cs32f0xx_misc.h
  * @brief   This file contains all the functions prototypes for the miscellaneous
  *          firmware library functions.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_MISC_H__
#define __CS32F0XX_MISC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup MISC
  * @{
  */


/** 
  * @struct nvic_config_t
  * @brief  NVIC config Structure definition  
  */
typedef struct
{
    uint8_t nvic_IRQ_channel;          //!< Specifies the IRQ channel to be enabled or disabled
    uint8_t nvic_channel_priority;     //!< Specifies the priority level(0~3) for the IRQ channel 
    enable_state_t nvic_enable_flag;   //!< Enabled or disabled the IRQ channel                                                                                        
} nvic_config_t;

/** 
  * @brief MISC system LP(low power) definition
  */
#define NVIC_LP_SEVONPEND            ((uint8_t)0x10) //!< Specifies low power sev on pend 
#define NVIC_LP_SLEEPDEEP            ((uint8_t)0x04) //!< Low power deep_sleep request 
#define NVIC_LP_SLEEPONEXIT          ((uint8_t)0x02) //!< Low power sleep on exit 
#define NVIC_LP_CHECK(flag) (((flag) == NVIC_LP_SEVONPEND) || ((flag) == NVIC_LP_SLEEPDEEP) || \
                               ((flag) == NVIC_LP_SLEEPONEXIT))                        

/**  
  * @brief MISC preemption priority group definition
  */
#define NVIC_PRIORITY_CHECK(value)  ((value) < 0x04)

/**
  * @brief MISC SysTick clock select definition
  */
#define SYSTICK_CLKSEL_HCLK_DIV8    ((uint32_t)0xFFFFFFFB)
#define SYSTICK_CLKSEL_HCLK         ((uint32_t)0x00000004)
#define SYSTICK_CLKSEL_CHECK(value) (((value) == SYSTICK_CLKSEL_HCLK) || ((value) == SYSTICK_CLKSEL_HCLK_DIV8))
                                       




/**
  * @fn void nvic_init(nvic_config_t* ptr_config)
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the ptr_config.
  * @param  ptr_config: pointer to a nvic_config_t structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @return None
  */
void nvic_init(nvic_config_t* ptr_config);

/**
  * @fn void nvic_system_low_power_set(uint8_t mode, enable_state_t enable_flag)
  * @brief  Selects the condition for the system to enter LPM (low power mode).
  * @param  mode: Specifies the new mode for the system to enter low power mode. It can be: 
  *            @arg NVIC_LP_SEVONPEND (Low Power SEV on Pend)
  *            @arg NVIC_LP_SLEEPDEEP(Low Power DEEPSLEEP request)
  *            @arg NVIC_LP_SLEEPONEXIT( Low Power Sleep on Exit)
  * @param  enable_flag: ENABLE or DISABLE LP condition. 
  * @return None
  */
void nvic_system_low_power_set(uint8_t mode, enable_state_t enable_flag);

/**
  * @fn void systick_clock_config(uint32_t select)
  * @brief  Configures the SysTick clock source.
  * @param  select: specifies the SysTick clock source, It can be 
  *            @arg SYSTICK_CLKSEL_HCLK_DIV8
  *            @arg SYSTICK_CLKSEL_HCLK
  * @return None
  */
void systick_clock_config(uint32_t select);

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

