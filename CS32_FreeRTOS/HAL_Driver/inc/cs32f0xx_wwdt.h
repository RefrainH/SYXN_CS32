/**
  * @file    cs32f0xx_wwdt.h
  * @brief   This file contains all the functions prototypes for the WWDT 
  *          firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_WWDT_H__
#define __CS32F0XX_WWDT_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup WWDT
  * @{
  */ 


/** 
  * @brief WWDT_Prescaler
  */
#define WWDT_PRESCALER_1    ((uint32_t)0x00000000)  //!<  the WWDT Prescaler WWDT counter clock = (PCLK1/4096)/1
#define WWDT_PRESCALER_2    ((uint32_t)0x00000080)  //!<  the WWDT Prescaler WWDT counter clock = (PCLK1/4096)/2
#define WWDT_PRESCALER_4    ((uint32_t)0x00000100)  //!<  the WWDT Prescaler WWDT counter clock = (PCLK1/4096)/4
#define WWDT_PRESCALER_8    ((uint32_t)0x00000180)  //!<  the WWDT Prescaler WWDT counter clock = (PCLK1/4096)/8
#define WWDT_PRESCALER_CHECK(prescaler) (((prescaler) == WWDT_PRESCALER_1) || \
                                         ((prescaler) == WWDT_PRESCALER_2) || \
                                         ((prescaler) == WWDT_PRESCALER_4) || \
                                         ((prescaler) == WWDT_PRESCALER_8))


/**
  *@fn void wwdt_def_init(void)
  * @brief  Deinitializes the WWDT peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void wwdt_def_init(void);


/**
  *@fn void wwdt_prescaler_set(uint32_t wwdt_prescaler)
  * @brief  Sets the WWDT Prescaler.
  * @param  wwdt_prescaler: specifies the WWDT Prescaler.
  *          This parameter can be one of the following values:
  *            @arg WWDT_PRESCALER_1(WWDT counter clock = (PCLK1/4096)/1)
  *            @arg WWDT_PRESCALER_2(WWDT counter clock = (PCLK1/4096)/2)
  *            @arg WWDT_PRESCALER_4(WWDT counter clock = (PCLK1/4096)/4)
  *            @arg WWDT_PRESCALER_8(WWDT counter clock = (PCLK1/4096)/8)
  * @return None
  */
void wwdt_prescaler_set(uint32_t wwdt_prescaler);


/**
  *@fn void wwdt_window_value_set(uint8_t window_value)
  * @brief  Sets the WWDT window value.
  * @param   window_value: specifies the window value to be compared to the downcounter.
  *          This parameter value must be lower than 0x80.
  * @return None
  */
void wwdt_window_value_set(uint8_t window_value);


/**
  *@fn void wwdt_interrupt_enable(void)
  * @brief  Enables the WWDT Wakeup interrupt.
  * @note   Once enabled this interrupt cannot be disabled except by a system reset. 
  * @param  None
  * @return None
  */
void wwdt_interrupt_enable(void);


/**
  *@fn void wwdt_counter_set(uint8_t counter)
  * @brief  Sets the WWDT counter value.
  * @param   counter: specifies the watchdog counter value.
  *          This parameter must be a number between 0x40 and 0x7F (to prevent 
  *          generating an immediate reset).
  * @return None
  */
void wwdt_counter_set(uint8_t counter);


/**
  *@fn void wwdt_counter_set(uint8_t counter)
  * @brief  Sets the WWDT counter value.
  * @param   counter: specifies the watchdog counter value.
  *          This parameter must be a number between 0x40 and 0x7F (to prevent 
  *          generating an immediate reset).
  * @return None
  */
void wwdt_enable(uint8_t counter);


/**
  *@fn bit_status_t wwdt_status_flag_get(void)
  * @brief  Checks whether the Early Wakeup interrupt flag is set or not.
  * @param  None
  * @return The new state of the Early Wakeup interrupt flag (SET or RESET).
  */
bit_status_t wwdt_status_flag_get(void);


/**
  *@fn void wwdt_flag_clear(void)
  * @brief  Clears Early Wakeup interrupt flag.
  * @param  None
  * @return None
  */
void wwdt_flag_clear(void);


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

