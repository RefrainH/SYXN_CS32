/**
  * @file    cs32f0xx_pmu.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of Power management unit (PMU) peripheral:
  *           VBAT Domain Access; LDV configuration
  *           WakeUp pins configuration; Low Power modes configuration.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "cs32f0xx_pmu.h"
#include "cs32f0xx_rcu.h"
/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup PMU
  * @{
  */ 



/* CTR register bit mask */
#define PMU_CTR_BIT0_BIT1_MASK            ((uint32_t)0xFFFFFFFC)
#define PMU_CTR_LVDSEL_MASK               ((uint32_t)0xFFFFFF1F)
  

/**
  * @fn void pmu_def_init(void)
  * @brief  Deinitializes the PMU peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void pmu_def_init(void)
{
    rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_PMU, ENABLE);
    rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_PMU, DISABLE);
}

/**
  * @fn void pmu_vbat_write_enable_ctrl(enable_state_t flag)
  * @brief  Enables or disables access to VBAT domain.
  * @param  flag: ENABLE or DISABLE.
  * @return None
  */
void pmu_vbat_write_enable_ctrl(enable_state_t flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(flag));
  
    if (flag != DISABLE)
    {
        /*Enable access to VBAT domain */
        PMU->CTR |= PMU_CTR_VBTWEN;
    }
    else
    {
        /* Disable access to VBAT domain */
        PMU->CTR &= (uint32_t)~((uint32_t)PMU_CTR_VBTWEN);
    } 
}

/**
  * @fn void pmu_low_vol_detector_config(uint32_t level)
  * @brief  Selection LVD (low voltage detector) threshold.
  * @param  level: specifies the LVD detection level, it can be one of
  *         the following values:
  *            @arg PMU_LVDSEL_LEVEL0
  *            @arg PMU_LVDSEL_LEVEL1
  *            @arg PMU_LVDSEL_LEVEL2
  *            @arg PMU_LVDSEL_LEVEL3
  *            @arg PMU_LVDSEL_LEVEL4
  *            @arg PMU_LVDSEL_LEVEL5
  *            @arg PMU_LVDSEL_LEVEL6
  *            @arg PMU_LVDSEL_LEVEL7
  * @return None
  */
void pmu_low_vol_detector_config(uint32_t level)
{
    uint32_t reg = 0;
  
    ASSERT(PMU_LVDSEL_LEVEL_CHECK(level));
    reg = PMU->CTR; 
  
    reg &= PMU_CTR_LVDSEL_MASK;  /* Clear LVDSEL[7:5] bits */   
    reg |= level;      /* Set LVDSEL[7:5] bits */ 
    PMU->CTR = reg;
}

/**
  * @fn void pmu_low_vol_detector_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables LVD( low vlotage detector).
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void pmu_low_vol_detector_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the LVD */
        PMU->CTR |= PMU_CTR_LVDEN;
    }
    else
    {
        /* Disable the LVD */
        PMU->CTR &= (uint32_t)~((uint32_t)PMU_CTR_LVDEN);
    } 
}

/**
  * @fn void pmu_wakeup_pin_enable_ctrl(uint32_t pin_num, enable_state_t enable_flag)
  * @brief  Enables or disables the WakeUp Pin functionality.
  * @param  pin_num: specifies the WakeUpPin, it can be one of the following values:
  *            @arg PMU_WAKEUP_PIN1
  *            @arg PMU_WAKEUP_PIN2
  *            @arg PMU_WAKEUP_PIN3
  *            @arg PMU_WAKEUP_PIN4
  *            @arg PMU_WAKEUP_PIN5
  *            @arg PMU_WAKEUP_PIN6
  *            @arg PMU_WAKEUP_PIN7
  *            @arg PMU_WAKEUP_PIN8       
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void pmu_wakeup_pin_enable_ctrl(uint32_t pin_num, enable_state_t enable_flag)
{
    ASSERT(PMU_WAKEUP_PIN_CHECK(pin_num));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the WKUP pin */
        PMU->CS |= pin_num;
    }
    else
    {
        /* Disable the EWUPx pin */
        PMU->CS &= ~pin_num;
    }
}

/**
  * @fn void pmu_sleep_mode_set(uint8_t value)
  * @brief  Enters Sleep mode.
  * @note   In Sleep mode, all I/O pins keep the same state as in Run mode.
  * @param  value: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg PMU_SLEEP_ENTRY_WFI (enter SLEEP mode with WFI instruction)
  *            @arg PMU_SLEEP_ENTRY_WFE (enter SLEEP mode with WFE instruction)
  * @return None
  */
void pmu_sleep_mode_set(uint8_t value)
{
    ASSERT(PMU_SLEEP_ENTRY_CHECK(value));

    /* Clear SLEEPDEEP bit */
    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
  
    if(value == PMU_SLEEP_ENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE(); 
        __WFE();
    }
}

/**
  * @fn void pmu_deep_sleep_mode_enter(uint32_t value, uint8_t flag)
  * @brief  Enters STOP mode.
  * @param  value: specifies the regulator state in STOP mode. It can be:
  *          PMU_LDO_ON or PMU_LDO_LOW_POWER().
  * @param  flag: specifies if STOP mode in entered with WFI or WFE instruction.
  *          It can be one of the following values:
  *            @arg PMU_DSM_ENTRY_WFI(enter STOP mode with WFI instruction)
  *            @arg PMU_DSM_ENTRY_WFE(enter STOP mode with WFE instruction)
  *            @arg PMU_DSM_ENTRY_SLEEPONEXIT(enter STOP mode with SLEEPONEXIT instruction)
  * @return None
  */
void pmu_deep_sleep_mode_enter(uint32_t value, uint8_t flag)
{
    uint32_t reg = 0;
  
    ASSERT(PMU_LDO_CHECK(value));
    ASSERT(PMU_STOP_ENTRY_CHECK(flag));
  
    reg = PMU->CTR;
  
    /* Clear DSMODE and LDOLM bits */
    reg &= PMU_CTR_BIT0_BIT1_MASK;
    
    /* Set LDOLM bit  */
    reg |= value;  
    PMU->CTR = reg;
    
    /* Set SLEEPDEEP bit of Cortex-M0 System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    
    if(flag == PMU_DSM_ENTRY_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
        /* Reset SLEEPDEEP bit of Cortex System Control Register */
        SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk); 
    }
    else if (flag == PMU_DSM_ENTRY_WFE)
    {
        /* Request Wait For Event */
        __WFE();
        /* Reset SLEEPDEEP bit of Cortex System Control Register */
        SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);   
    }
    else
    {
        /* Set SLEEP on exit bit of Cortex-M0 System Control Register */
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }
}


/**
  * @fn void pmu_power_down_start(void)
  * @brief  Enters power down mode. The Wakeup flag (WUF) need to be cleared
  *         at application level before to call this function.    
  * @param  None
  * @return None
  */
void pmu_power_down_start(void)
{
    int32_t i;
    for (i = 0; i < 1450; i++)
    {
        __nop();
    }
    PMU->CTR |= PMU_CTR_DSMODE;

    /* Set SLEEPDEEP bit of Cortex-M0 System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /* Request Wait For Interrupt */
    __WFI();
}

/**
  * @fn bit_status_t pmu_flag_status_get(uint32_t flag)
  * @brief  Checks whether the specified PMU flag is set or not.
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *            @arg PMU_FLAG_WUPF(Wake Up flag)
  *            @arg PMU_FLAG_PWDF(power down flag)
  *            @arg PMU_FLAG_LVDO(low vlotage detector Output)
  * @return SET or RESET.
  */
bit_status_t pmu_flag_status_get(uint32_t flag)
{
    bit_status_t status = RESET;
  
    ASSERT(PMU_GET_FLAG_CHECK(flag));

    if ((PMU->CS & flag) != (uint32_t)RESET)
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
  * @fn void pmu_flag_clear(uint32_t flag)
  * @brief  Clears the PMU's pending flags.
  * @param  flag: specifies the flag to clear, it can be one of the following values:
  *            @arg PMU_FLAG_WUPF(Wake Up flag)
  *            @arg PMU_FLAG_PWDF(power down flag)
  * @return None
  */
void pmu_flag_clear(uint32_t flag)
{
    ASSERT(PMU_CLEAR_FLAG_CHECK(flag));

    PMU->CTR |=  flag << 2;
}


/**
  * @}
  */

/**
  * @}
  */

