/**
  * @file    cs32f0xx_misc.c
  * @brief   This file provides all the miscellaneous firmware functions. 
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
      It provide functions allowing to configure the NVIC interrupts (IRQ).
      The Cortex-M0 exceptions are managed by CMSIS functions.
      1. Enable and Configure the priority of the selected IRQ Channels,
         The priority can be 0.1.2.3.                
    @endverbatim
  *
  */

#include "cs32f0xx_misc.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup MISC 
  * @{
  */



/**
  * @fn void nvic_init(nvic_config_t* ptr_config)
  * @brief  Initializes the NVIC peripheral according to the specified
  *         parameters in the ptr_config.
  * @param  ptr_config: pointer to a nvic_config_t structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @return None
  */
void nvic_init(nvic_config_t* ptr_config)
{
    uint32_t priority = 0x00;
  
    ASSERT(FUNCTIONAL_STATE_CHECK(ptr_config->nvic_enable_flag));
    ASSERT(NVIC_PRIORITY_CHECK(ptr_config->nvic_channel_priority));  
    
    if (ptr_config->nvic_enable_flag != DISABLE)
    {
        /* Compute the Corresponding IRQ Priority*/    
        priority = NVIC->IP[ptr_config->nvic_IRQ_channel >> 0x02];
        priority &= (uint32_t)(~(((uint32_t)0xFF) << ((ptr_config->nvic_IRQ_channel & 0x03) * 8)));
        priority |= (uint32_t)((((uint32_t)ptr_config->nvic_channel_priority << 6) & 0xFF) << ((ptr_config->nvic_IRQ_channel & 0x03) * 8));    
    
        NVIC->IP[ptr_config->nvic_IRQ_channel >> 0x02] = priority;
    
        /* Enable the Selected IRQ Channels */
        NVIC->ISER[0] = (uint32_t)0x01 << (ptr_config->nvic_IRQ_channel & (uint8_t)0x1F);
    }
    else
    {
        /* Disable the Selected IRQ Channels */
        NVIC->ICER[0] = (uint32_t)0x01 << (ptr_config->nvic_IRQ_channel & (uint8_t)0x1F);
    }
}

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
void nvic_system_low_power_set(uint8_t mode, enable_state_t enable_flag)
{
    ASSERT(NVIC_LP_CHECK(mode));  
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));  
  
    if (enable_flag != DISABLE)
    {
        SCB->SCR |= mode;
    }
    else
    {
        SCB->SCR &= (uint32_t)(~(uint32_t)mode);
    }
}

/**
  * @fn void systick_clock_config(uint32_t select)
  * @brief  Configures the SysTick clock source.
  * @param  select: specifies the SysTick clock source, It can be 
  *            @arg SYSTICK_CLKSEL_HCLK_DIV8
  *            @arg SYSTICK_CLKSEL_HCLK
  * @return None
  */
void systick_clock_config(uint32_t select)
{
    ASSERT(SYSTICK_CLKSEL_CHECK(select));
  
    if (select == SYSTICK_CLKSEL_HCLK)
    {
        SysTick->CTRL |= SYSTICK_CLKSEL_HCLK;
    }
    else
    {
        SysTick->CTRL &= SYSTICK_CLKSEL_HCLK_DIV8;
    }
}

/**
  * @}
  */

/**
  * @}
  */

