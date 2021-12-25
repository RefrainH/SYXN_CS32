/**
  * @file    cs32f0xx_wwdt.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Window Watchdog Timer(WWDT) peripheral:
  *           + Prescaler, Refresh window and Counter configuration
  *           + WWDT activation
  *           + Interrupts and flags management

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
        1. Enable WWDT clock using rcu_apb1_periph_clock_enable_ctrl(RCU_APB1_PERI_WWDT, ENABLE) 
            function.
              
        2. Configure the WWDT prescaler using wwdt_prescaler_set() function.
                             
        3. Configure the WWDT refresh window using wwdt_window_value_set() function.
              
        4. Set the WWDT counter value and start it using wwdt_enable() function.
            When the WWDT is enabled the counter value should be configured to 
            a value greater than 0x40 to prevent generating an immediate reset.
              
        5. Optionally you can enable the Early wakeup interrupt which is 
            generated when the counter reach 0x40.
            Once enabled this interrupt cannot be disabled except by a system reset.
                   
        6. Then the application program must refresh the WWDT counter at regular
            intervals during normal operation to prevent an MCU reset, using
            wwdt_counter_set() function. This operation must occur only when
            the counter value is lower than the refresh window value, 
            programmed using wwdt_window_value_set().
  
     @endverbatim
  *
  */  

#include "cs32f0xx_wwdt.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup WWDT
  * @brief WWDT driver modules
  * @{
  */
  


/**
  *@brief WWDT CTR2 register bit mask 
  */
#define WWDT_CTR2_PDIV_MASK    	 ((uint32_t)0xFFFFFE7F)
#define WWDT_CTR2_WVAL_MASK      ((uint32_t)0xFFFFFF80)
#define WWDT_BIT_MASK            ((uint8_t)0x7F)


/** 
  * @brief WWDT window value check
  */                                      
#define WWDT_WINDOW_VALUE_CHECK(value) ((value) <= 0x7F)

/** 
  * @brief WWDT counter value check
  */
#define WWDT_COUNTER_CHECK(counter)    (((counter) >= 0x40) && ((counter) <= 0x7F))


/**
  *@fn void wwdt_def_init(void)
  * @brief  Deinitializes the WWDT peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void wwdt_def_init(void)
{
    rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_WWDT, ENABLE);
    rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_WWDT, DISABLE);
}

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
void wwdt_prescaler_set(uint32_t wwdt_prescaler)
{
    uint32_t reg = 0;
  
    ASSERT(WWDT_PRESCALER_CHECK(wwdt_prescaler));
  
    /* Clear PDIV[1:0] bits ,Set PDIV[1:0] bits according to wwdt_prescaler value*/
    reg = (WWDT->CTR2 & WWDT_CTR2_PDIV_MASK) | wwdt_prescaler;
  
    /* Store the new value */
    WWDT->CTR2 = reg;
}

/**
  *@fn void wwdt_window_value_set(uint8_t window_value)
  * @brief   Sets the WWDT window value.
  * @param   window_value: specifies the window value to be compared to the downcounter.
  *          This parameter value must be lower than 0x80.
  * @return None
  */
void wwdt_window_value_set(uint8_t window_value)
{
    __IO uint32_t reg = 0;

    ASSERT(WWDT_WINDOW_VALUE_CHECK(window_value));
    
    /* Clear WVAL[6:0] bits */
    reg = WWDT->CTR2 & WWDT_CTR2_WVAL_MASK;
    
    /* Set WVAL[6:0] bits according to WindowValue value */
    reg |= window_value & (uint32_t) WWDT_BIT_MASK;
    
    /* Store the new value */
    WWDT->CTR2 = reg;
}

/**
  *@fn void wwdt_interrupt_enable(void)
  * @brief  Enables the WWDT Wakeup interrupt.
  * @note   Once enabled this interrupt cannot be disabled except by a system reset. 
  * @param  None
  * @return None
  */
void wwdt_interrupt_enable(void)
{
    WWDT->CTR2 |= WWDT_CTR2_RMDIE;
}

/**
  *@fn void wwdt_counter_set(uint8_t counter)
  * @brief  Sets the WWDT counter value.
  * @param   counter: specifies the watchdog counter value.
  *          This parameter must be a number between 0x40 and 0x7F (to prevent 
  *          generating an immediate reset).
  * @return None
  */
void wwdt_counter_set(uint8_t counter)
{
    ASSERT(WWDT_COUNTER_CHECK(counter));
  
    /* Write to CVAL[6:0] bits to configure the counter value, no need to do
        a read-modify-write; writing a 0 to WWDTEN bit does nothing */
    WWDT->CTR1 = counter & WWDT_BIT_MASK;
}

/**
  *@fn void wwdt_enable(uint8_t counter) 
  * @brief  Enables WWDT and load the counter value.                  
  * @param  counter: specifies the watchdog counter value.
  *          This parameter must be a number between 0x40 and 0x7F (to prevent 
  *          generating an immediate reset).
  * @return None
  */
void wwdt_enable(uint8_t counter)
{
    ASSERT(WWDT_COUNTER_CHECK(counter));
  
    WWDT->CTR1 = WWDT_CTR1_WWDTEN | counter;
}

/**
  *@fn bit_status_t wwdt_status_flag_get(void)
  * @brief  Checks whether the Early Wakeup interrupt flag is set or not.
  * @param  None
  * @return The new state of the Early Wakeup interrupt flag (SET or RESET).
  */
bit_status_t wwdt_status_flag_get(void)
{
    bit_status_t bit_status = RESET;
    
    if ((WWDT->STS) != (uint32_t)RESET)
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
  *@fn void wwdt_flag_clear(void)
  * @brief  Clears Early Wakeup interrupt flag.
  * @param  None
  * @return None
  */
void wwdt_flag_clear(void)
{
    WWDT->STS = (uint32_t)RESET;
}


/**
  * @}
  */


/**
  * @}
  */

