/**
  * @file    cs32f0xx_exti.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the EXTI peripheral:Initialization and Configuration; 
  *          Interrupts and flags management.
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
    1. In order to use an I/O pin as an external interrupt source, follow steps below:         
      a.Configure the I/O in input mode using gpio_init().
      b. Select the input source pin for the EXTI line using syscfg_exti_line_config().        
      c. Select the mode(interrupt, event) and configure the trigger selection 
        (Rising, falling or both) using exti_init(). For the internal interrupt,
        the trigger selection is not needed( the active edge is always the rising one).
      d. Configure NVIC IRQ channel mapped to the EXTI line using nvic_init().
      e. Optionally, you can generate a software     interrupt using the function exti_sw_interrupt_trigger().
      f. SYSCFG APB clock must be enabled to get write access to SYSCFG_EXTICRx
      registers using rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_SYSCFG, ENABLE);
    @endverbatim
  *
  */

#include "cs32f0xx_exti.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup EXTI
  * @{
  */

/**
  * @brief No interrupt selected definition
  */
#define EXTI_LINE_NONE     ((uint32_t)0x00000)     


/**
  * @fn void exti_def_init(void)
  * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void exti_def_init(void)
{
    EXTI->INTEN = 0x0F940000;
    EXTI->EVTEN = 0x00000000;
  
    EXTI->RTEN = 0x00000000;
    EXTI->FTEN = 0x00000000;
    EXTI->PDF = 0x006BFFFF;
}

/**
  * @fn void exti_init(exti_config_t* ptr_config)
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the ptr_config.
  * @param  ptr_config: pointer to a exti_config_t structure that contains the 
  *         configuration for the EXTI peripheral.
  * @return None
  */
void exti_init(exti_config_t* ptr_config)
{
    uint32_t temp = 0;

    ASSERT(EXTI_MODE_CHECK(ptr_config->exti_mode));
    ASSERT(EXTI_TRIGGER_EDGE_CHECK(ptr_config->exti_trigger));
    ASSERT(EXTI_LINE_CHECK(ptr_config->exti_line));
    ASSERT(FUNCTIONAL_STATE_CHECK(ptr_config->exti_line_cmd));

    temp = (uint32_t)EXTI_BASE;

    if (ptr_config->exti_line_cmd != DISABLE)
    {
        /* Clear EXTI line configuration */
        EXTI->INTEN &= ~ptr_config->exti_line;
        EXTI->EVTEN &= ~ptr_config->exti_line;

        temp += ptr_config->exti_mode;

        *(__IO uint32_t *) temp |= ptr_config->exti_line;

        /* Clear Rising Falling edge configuration */
        EXTI->RTEN &= ~ptr_config->exti_line;
        EXTI->FTEN &= ~ptr_config->exti_line;

        /* Select the trigger for the selected interrupts */
        if (ptr_config->exti_trigger == EXTI_TRIGGER_RISING_FALLING)
        {
            /* Rising Falling edge */
            EXTI->RTEN |= ptr_config->exti_line;
            EXTI->FTEN |= ptr_config->exti_line;
        }
        else
        {
            temp = (uint32_t)EXTI_BASE;
            temp += ptr_config->exti_trigger;

            *(__IO uint32_t *) temp |= ptr_config->exti_line;
        }
    }
    else
    {
        temp += ptr_config->exti_mode;

        /* Disable the selected external lines */
        *(__IO uint32_t *) temp &= ~ptr_config->exti_line;
    }
}

/**
  * @fn void exti_struct_init(exti_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its reset value.
  * @param  ptr_config: pointer to a exti_config_t structure which will be initialized
  * @return None
  */
void exti_struct_init(exti_config_t* ptr_config)
{
    ptr_config->exti_line = EXTI_LINE_NONE;
    ptr_config->exti_mode = EXTI_MODE_INTR;
    ptr_config->exti_trigger = EXTI_TRIGGER_FALLING;
    ptr_config->exti_line_cmd = DISABLE;
}

/**
  * @fn void exti_sw_interrupt_trigger(uint32_t value)
  * @brief  Generates a Software interrupt on selected EXTI line.
  * @param  value: specifies the EXTI line on which the software interrupt will be generated.
  *         It can be any combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_sw_interrupt_trigger(uint32_t value)
{
    ASSERT(EXTI_LINE_CHECK(value));

    EXTI->SWTIEN |= value;
}

/**
  * @fn bit_status_t exti_flag_status_get(uint32_t value)
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  value: specifies the EXTI line flag to check, it can be:
  *          EXTI_Linex where x can be (0~27).
  * @return SET or RESET
  */
bit_status_t exti_flag_status_get(uint32_t value)
{
    bit_status_t status = RESET;
   
    ASSERT(EXTI_LINE_GET_CHECK(value));

    if ((EXTI->PDF & value) != (uint32_t)RESET)
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
  * @fn void exti_flag_clear(uint32_t value)
  * @brief  Clears the EXTI's line flags.
  * @param  value: specifies the EXTI lines flags to clear, it can be any
  *         combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_flag_clear(uint32_t value)
{
    ASSERT(EXTI_LINE_CHECK(value));

    EXTI->PDF = value;
}

/**
  * @fn bit_status_t exti_interrupt_status_get(uint32_t value)
  * @brief  Checks whether the specified EXTI line is set or not.
  * @param  value: specifies the EXTI line to check, it can be 
  *         EXTI_Linex where x can be (0~27).
  * @return SET or RESET.
  */
bit_status_t exti_interrupt_status_get(uint32_t value)
{
    bit_status_t status = RESET;

    ASSERT(EXTI_LINE_GET_CHECK(value));

    if ((EXTI->PDF & value) != (uint32_t)RESET)
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
  * @fn void exti_interrupt_status_clear(uint32_t value)
  * @brief  Clears the EXTI's line  bits.
  * @param  value: specifies the EXTI lines to clear, it can be any
  *         combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_interrupt_status_clear(uint32_t value)
{
    ASSERT(EXTI_LINE_CHECK(value));

    EXTI->PDF = value;
}

/**
  * @}
  */

/**
  * @}
  */

