/**
  * @file    cs32f0xx_syscfg.c  
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the SYSCFG peripheral:
  *             Remapping the memory mapped at 0x00000000;  
  *             Remapping the DMA channels;
  *             Enabling I2C fast mode plus driving capability for I2C pins;   
  *             Configuring the EXTI lines connection to the GPIO port;
  *             Configuring the ERRLOCK features (SYSCFG error and timer break lock).
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "cs32f0xx_syscfg.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup SYSCFG 
  * @{
  */ 


/**
  * @fn void syscfg_def_init(void)
  * @brief  Deinitializes the SYSCFG registers to their default reset values.
  * @note   MEM_REMAP and ERRLOCK are not affected by APB reset.
  * @param  None
  * @return None
  */
void syscfg_def_init(void)
{
    /* Set SYSCFG_CFGR1 register to reset value without affecting MEM_RMAP bit */
    SYSCFG->RMAPCFG &= SYSCFG_RMAPCFG_MEM_RMAP;
    SYSCFG->EXTISRC[0] = 0;
    SYSCFG->EXTISRC[1] = 0;
    SYSCFG->EXTISRC[2] = 0;
    SYSCFG->EXTISRC[3] = 0;
    /*  clear SRAM parity error flag */
    SYSCFG->ERRLOCK |= (uint32_t) SYSCFG_ERRLOCK_SRAM_PRTY_ERR;
}

/**
  * @fn void syscfg_memory_remap_set(uint32_t value)
  * @brief  Set the memory mapping at address 0x00000000.
  * @param  value: selects the memory remapping, it can be:
  *          @arg SYSCFG_MEM_REMAP_FLASH(Main Flash memory mapped at 0x00000000)
  *          @arg SYSCFG_MEM_REMAP_SYS_MEMORY(System Flash memory mapped at 0x00000000)
  *          @arg SYSCFG_MEM_REMAP_SRAM(Embedded SRAM mapped at 0x00000000)
  * @return None
  */
void syscfg_memory_remap_set(uint32_t value)
{
    uint32_t temp = 0;
    
    ASSERT(SYSCFG_MEM_REMAP_CHECK(value));

    temp = SYSCFG->RMAPCFG;

    /* Clear MEM_RMAP bits */
    temp &= (uint32_t) (~SYSCFG_RMAPCFG_MEM_RMAP);
    temp |= (uint32_t) value;
    SYSCFG->RMAPCFG = temp;
}

/**
  * @fn void syscfg_dma_channel_remap_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configure the DMA channels remapping.
  * @param  value: selects the DMA channels remap, It can be
  *          @arg SYSCFG_DMA_REMAP_TIM17(Remap TIM17 DMA from channel 1 to channel 2)
  *          @arg SYSCFG_DMA_REMAP_TIM16(Remap TIM16 DMA from channel 3 to channel 4)
  *          @arg SYSCFG_DMA_REMAP_USART1RX(Remap USART1 Rx DMA from channel 3 to channel 5)
  *          @arg SYSCFG_DMA_REMAP_USART1TX(Remap USART1 Tx DMA from channel 2 to channel 4)
  *          @arg SYSCFG_DMA_REMAP_ADC1(Remap ADC1 DMA requests from channel 1 to channel 2)
  * @param  enable_flag: ENABLE or DISABLE the DMA channel remapping. 
  * @return None
  */
void syscfg_dma_channel_remap_config(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(SYSCFG_DMA_REMAP_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Remap the DMA channel */
        SYSCFG->RMAPCFG |= (uint32_t)value;
    }
    else
    {
        /* use the default DMA channel mapping */
        SYSCFG->RMAPCFG &= (uint32_t)(~value);
    }
}

/**
  * @fn void syscfg_exti_line_config(uint8_t port, uint8_t pin)
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  port: selects the GPIO port to be used as source for EXTI lines                    
  * @param  pin: specifies the EXTI line to be configured.
  * @return None
  */
void syscfg_exti_line_config(uint8_t port, uint8_t pin)
{
    uint32_t temp = 0x00;
    
    ASSERT(SYSCFG_EXTI_PORT_CHECK(port));
    ASSERT(SYSCFG_EXTI_PIN_CHECK(pin));
  
    temp = ((uint32_t)0x0F) << (0x04 * (pin & (uint8_t)0x03));
    SYSCFG->EXTISRC[pin >> 0x02] &= ~temp;
    SYSCFG->EXTISRC[pin >> 0x02] |= (((uint32_t)port) << (0x04 * (pin & (uint8_t)0x03)));
}

/**
  * @fn void syscfg_time_break_config(uint32_t value)
  * @brief  Connect the selected parameter to the TIM1 break.
  * @note   The selected configuration is locked and can be unlocked by system reset
  * @param  value: selects the configuration to be connected to TIM1 break. It can be 
  *          @arg SYSCFG_TIM_BREAK_LVD (Connects the LVD event to TIM1 break enable)
  *          @arg SYSCFG_TIM_BREAK_SRAM_PARITY(Connects the SRAM_PARITY error signal to TIM1 break enable)
  *          @arg SYSCFG_TIM_BREAK_LOCKUP(Connects Lockup output of CortexM0 to TIM1 break enable)
  * @return None
  */
void syscfg_time_break_config(uint32_t value)
{
    ASSERT(SYSCFG_TIM_BREAK_CHECK(value));

    SYSCFG->ERRLOCK |= (uint32_t) value;
}

/**
  * @fn bit_status_t syscfg_flag_status_get(uint32_t flag)
  * @brief   Checks whether the specified SYSCFG flag is set or not.
  * @param   flag: specifies the SYSCFG flag to check. It can be
  *          @arg SYSCFG_FLAG_PE(SRAM parity error flag)
  * @return  state SET or RESET.
  */
bit_status_t syscfg_flag_status_get(uint32_t flag)
{
    UNUSED(flag);
    bit_status_t status = RESET;

    ASSERT(SYSCFG_FLAG_CHECK(flag));

    if ((SYSCFG->ERRLOCK & SYSCFG_ERRLOCK_SRAM_PRTY_ERR) != (uint32_t)RESET)
    {
        /* The flag is set*/
        status = SET;
    }
    else
    {
        /* The flag is reset*/
        status = RESET;
    }
  
    return  status;
}   

/**
  * @fn void syscfg_flag_clear(uint32_t flag)
  * @brief  Clear the selected SYSCFG flag.
  * @param  flag: selects the flag to be cleared, it can be 
  *          @arg SYSCFG_FLAG_PE(SRAM parity error flag)
  * @return None
  */
void syscfg_flag_clear(uint32_t flag)
{
    ASSERT(SYSCFG_FLAG_CHECK(flag));

    /* Clear the selected SYSCFG flag */
    SYSCFG->ERRLOCK |= (uint32_t) flag;
}


/**
  * @}
  */ 

/**
  * @}
  */ 

