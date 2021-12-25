/**
  * @file    cs32f0xx_pmu.h
  * @brief   This file contains all the functions prototypes for the PMU firmware 
  *          library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */
  
#ifndef __CS32F0XX_PMU_H__
#define __CS32F0XX_PMU_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup PMU
  * @{
  */ 


/** 
  * @brief PMU LVDSEL level definition 
  */ 
#define PMU_LVDSEL_LEVEL0                  PMU_CTR_LVDSEL_LEVEL0 //!< PMU LVD (low voltage detector) level 0
#define PMU_LVDSEL_LEVEL1                  PMU_CTR_LVDSEL_LEVEL1 //!< PMU LVD (low voltage detector) level 1
#define PMU_LVDSEL_LEVEL2                  PMU_CTR_LVDSEL_LEVEL2 //!< PMU LVD (low voltage detector) level 2
#define PMU_LVDSEL_LEVEL3                  PMU_CTR_LVDSEL_LEVEL3 //!< PMU LVD (low voltage detector) level 3
#define PMU_LVDSEL_LEVEL4                  PMU_CTR_LVDSEL_LEVEL4 //!< PMU LVD (low voltage detector) level 4
#define PMU_LVDSEL_LEVEL5                  PMU_CTR_LVDSEL_LEVEL5 //!< PMU LVD (low voltage detector) level 5
#define PMU_LVDSEL_LEVEL6                  PMU_CTR_LVDSEL_LEVEL6 //!< PMU LVD (low voltage detector) level 6
#define PMU_LVDSEL_LEVEL7                  PMU_CTR_LVDSEL_LEVEL7 //!< PMU LVD (low voltage detector) level 7 

#define PMU_LVDSEL_LEVEL_CHECK(value) (((value) == PMU_LVDSEL_LEVEL0) || ((value) == PMU_LVDSEL_LEVEL1)|| \
                                           ((value) == PMU_LVDSEL_LEVEL2) || ((value) == PMU_LVDSEL_LEVEL3)|| \
                                           ((value) == PMU_LVDSEL_LEVEL4) || ((value) == PMU_LVDSEL_LEVEL5)|| \
                                           ((value) == PMU_LVDSEL_LEVEL6) || ((value) == PMU_LVDSEL_LEVEL7))

/** 
  * @brief PMU WakeUp Pins definition
  */
#define PMU_WAKEUP_PIN1                 PMU_CS_WUPEN1 //!< PMU wakeup pin 1
#define PMU_WAKEUP_PIN2                 PMU_CS_WUPEN2 //!< PMU wakeup pin 2
#define PMU_WAKEUP_PIN3                 PMU_CS_WUPEN3 //!< PMU wakeup pin 3 
#define PMU_WAKEUP_PIN4                 PMU_CS_WUPEN4 //!< PMU wakeup pin 4 
#define PMU_WAKEUP_PIN5                 PMU_CS_WUPEN5 //!< PMU wakeup pin 5 
#define PMU_WAKEUP_PIN6                 PMU_CS_WUPEN6 //!< PMU wakeup pin 6 
#define PMU_WAKEUP_PIN7                 PMU_CS_WUPEN7 //!< PMU wakeup pin 7 
#define PMU_WAKEUP_PIN8                 PMU_CS_WUPEN8 //!< PMU wakeup pin 8 
#define PMU_WAKEUP_PIN_CHECK(pin) (((pin) == PMU_WAKEUP_PIN1) || ((pin) == PMU_WAKEUP_PIN2) || \
                                      ((pin) == PMU_WAKEUP_PIN3) || ((pin) == PMU_WAKEUP_PIN4) || \
                                      ((pin) == PMU_WAKEUP_PIN5) || ((pin) == PMU_WAKEUP_PIN6) || \
                                      ((pin) == PMU_WAKEUP_PIN7) || ((pin) == PMU_WAKEUP_PIN8))

/** 
  * @brief PMU LDO low power Control definition
  */
#define PMU_LDO_ON              ((uint32_t)0x00000000)
#define PMU_LDO_LOW_POWER       PMU_CTR_LDOLM
#define PMU_LDO_CHECK(flag)    (((flag) == PMU_LDO_ON) || ((flag) == PMU_LDO_LOW_POWER))
                                     
/** 
  * @brief PMU SLEEP mode entry 
  */
#define PMU_SLEEP_ENTRY_WFI              ((uint8_t)0x01) //!< Enter SLEEP mode with WFI instruction
#define PMU_SLEEP_ENTRY_WFE              ((uint8_t)0x02) //!< Enter SLEEP mode with WFE instruction
#define PMU_SLEEP_ENTRY_CHECK(value) (((value) == PMU_SLEEP_ENTRY_WFI) || ((value) == PMU_SLEEP_ENTRY_WFE))

/** 
  * @brief PMU STOP mode entry
  */
#define PMU_DSM_ENTRY_WFI               ((uint8_t)0x01) //!< Enter STOP mode with WFI instruction
#define PMU_DSM_ENTRY_WFE               ((uint8_t)0x02) //!< Enter STOP mode with WFE instruction
#define PMU_DSM_ENTRY_SLEEPONEXIT       ((uint8_t)0x03) //!< Enter STOP mode with SLEEPONEXIT instruction
#define PMU_STOP_ENTRY_CHECK(value) (((value) == PMU_DSM_ENTRY_WFI) || \
                                     ((value) == PMU_DSM_ENTRY_WFE) || \
                                     ((value) == PMU_DSM_ENTRY_SLEEPONEXIT))

/** 
  * @brief PMU flag definition
  */
#define PMU_FLAG_WUPF                     PMU_CS_WUPF //!< PMU wakeup flag 
#define PMU_FLAG_PWDF                     PMU_CS_PWDF //!< PMU power down flag 
#define PMU_FLAG_LVDO                     PMU_CS_LVDO //!< PMU LVDO flag 

#define PMU_GET_FLAG_CHECK(flag) (((flag) == PMU_FLAG_WUPF) || ((flag) == PMU_FLAG_PWDF) || \
                                  ((flag) == PMU_FLAG_LVDO))

#define PMU_CLEAR_FLAG_CHECK(flag) (((flag) == PMU_FLAG_WUPF) || ((flag) == PMU_FLAG_PWDF))




/**
  * @fn void pmu_def_init(void)
  * @brief  Deinitializes the PMU peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void pmu_def_init(void);

/**
  * @fn void pmu_vbat_write_enable_ctrl(enable_state_t flag)
  * @brief  Enables or disables access to VBAT domain.
  * @param  flag: ENABLE or DISABLE.
  * @return None
  */
void pmu_vbat_write_enable_ctrl(enable_state_t flag);

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
void pmu_low_vol_detector_config(uint32_t level);

/**
  * @fn void pmu_low_vol_detector_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables LVD( low vlotage detector).
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void pmu_low_vol_detector_ctrl(enable_state_t enable_flag);

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
void pmu_wakeup_pin_enable_ctrl(uint32_t pin_num, enable_state_t enable_flag);

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
void pmu_sleep_mode_set(uint8_t value);

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
void pmu_deep_sleep_mode_enter(uint32_t value, uint8_t flag);

/**
  * @fn void pmu_power_down_start(void)
  * @brief  Enters power down mode. The Wakeup flag (WUF) need to be cleared
  *         at application level before to call this function.    
  * @param  None
  * @return None
  */
void pmu_power_down_start(void);

/**
  * @fn bit_status_t pmu_flag_status_get(uint32_t flag)
  * @brief  Checks whether the specified PMU flag is set or not.
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *            @arg PMU_FLAG_WUPF(Wake Up flag)
  *            @arg PMU_FLAG_PWDF(power down flag)
  *            @arg PMU_FLAG_LVDO(low vlotage detector Output)
  * @return SET or RESET.
  */
bit_status_t pmu_flag_status_get(uint32_t flag);

/**
  * @fn void pmu_flag_clear(uint32_t flag)
  * @brief  Clears the PMU's pending flags.
  * @param  flag: specifies the flag to clear, it can be one of the following values:
  *            @arg PMU_FLAG_WUPF(Wake Up flag)
  *            @arg PMU_FLAG_PWDF(power down flag)
  * @return None
  */
void pmu_flag_clear(uint32_t flag);

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

