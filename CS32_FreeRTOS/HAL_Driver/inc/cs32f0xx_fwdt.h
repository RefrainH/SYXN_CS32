/**
  * @file    cs32f0xx_fwdt.h
  * @brief   This file contains all the functions prototypes for the FWDT 
  *          firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_FWDT_H__
#define __CS32F0XX_FWDT_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup FWDT
  * @{
  */


/**  
  *@def FWDT_WRITE_ACCESS_ENABLE     ((uint16_t)0x5555)
  *@brief FWDT write access enable
  *Details.
  */
#define FWDT_WRITE_ACCESS_ENABLE     ((uint16_t)0x5555)

/**  
  *@def FWDT_WRITE_ACCESS_DISABLE     ((uint16_t)0x0000)
  *@brief FWDT write access disable
  *Details.
  */
#define FWDT_WRITE_ACCESS_DISABLE    ((uint16_t)0x0000)

/**  
  *@brief FWDT prescaler selection
  */
#define FWDT_PRESCALER_4            ((uint8_t)0x00) //!<FWDT prescaler divider 4
#define FWDT_PRESCALER_8            ((uint8_t)0x01) //!<FWDT prescaler divider 8
#define FWDT_PRESCALER_16           ((uint8_t)0x02) //!<FWDT prescaler divider 16
#define FWDT_PRESCALER_32           ((uint8_t)0x03) //!<FWDT prescaler divider 32
#define FWDT_PRESCALER_64           ((uint8_t)0x04) //!<FWDT prescaler divider 64
#define FWDT_PRESCALER_128          ((uint8_t)0x05) //!<FWDT prescaler divider 128
#define FWDT_PRESCALER_256          ((uint8_t)0x06) //!<FWDT prescaler divider 256


/** 
  *@ brief FWDT STS register flag 
  */
#define FWDT_FLAG_DRF               FWDT_STS_DRF
#define FWDT_FLAG_UVRF              FWDT_STS_UVRF
#define FWDT_FLAG_WRF               FWDT_STS_WRF
#define FWDT_FLAG_CHECK(flag) (((flag) == FWDT_FLAG_DRF)  || \
                               ((flag) == FWDT_FLAG_UVRF) || \
                               ((flag) == FWDT_FLAG_WRF))


/**
  * @fn void fwdt_write_access_enable_ctrl(uint16_t write_ctrl);
  * @brief  Enables or disables write access to FWDT_PDIV and FWDT_UVAL registers.
  * @param  write_ctrl: This parameter can be Enables or disables.
  * @return None
  */
void fwdt_write_access_enable_ctrl(uint16_t write_ctrl);


/**
  * @fn void fwdt_prescaler_set(uint8_t prescaler_value)
  * @brief  Sets FWDT Prescaler value.
  * @param  prescaler_value: specifies the FWDT Prescaler value.
  *          This parameter can be one of the following values:
  *            @arg FWDT_PRESCALER_4(FWDT prescaler set to 4)
  *            @arg FWDT_PRESCALER_8(FWDT prescaler set to 8)
  *            @arg FWDT_PRESCALER_16(FWDT prescaler set to 16)
  *            @arg FWDT_PRESCALER_32(FWDT prescaler set to 32)
  *            @arg FWDT_PRESCALER_64(FWDT prescaler set to 64)
  *            @arg FWDT_PRESCALER_128(FWDT prescaler set to 128)
  *            @arg FWDT_PRESCALER_256(FWDT prescaler set to 256)
  * @return None
  */
void fwdt_prescaler_set(uint8_t prescaler_value);


/**
  * @fn void fwdt_reload_set(uint16_t reload_value)
  * @brief  Sets FWDT Reload value.
  * @param  reload_value: specifies the FWDT Reload value.
  *          This parameter must be a number between 0 and 0x0FFF.
  * @return None
  */
void fwdt_reload_set(uint16_t reload_value);


/**
  * @fn void fwdt_reload_counter(void)
  * @brief  Reloads FWDT counter with value defined in the reload register
  * @param  None
  * @return None
  */
void fwdt_reload_counter(void);


/**
  * @fn void fwdt_window_value_set(uint16_t window_value) 
  * @brief  Sets the FWDT window value.
  * @param  window_value: specifies the window value to be compared to the downcounter.
  * @return None
  */
void fwdt_window_value_set(uint16_t window_value);


/**
  * @fn void fwdt_enable(void)
  * @brief  Enables FWDT (write access to FWDT_PDIV and FWDT_UVAL registers disabled).
  * @param  None
  * @return None
  */
void fwdt_enable(void);

/**
  * @fn bit_status_t fwdt_flag_status_get(uint16_t fwdt_flag)
  * @brief  Checks whether the specified FWDT flag is set or not.
  * @param  fwdt_flag: specifies the flag to check.It can be one of the following values:           
  *            @arg FWDT_FLAG_DRF(Prescaler Value Update on going)
  *            @arg FWDT_FLAG_UVRF(Reload Value Update on going)
  *            @arg FWDT_FLAG_WRF(Counter Window Value Update on going)
  * @return  bit status (SET or RESET).
  */
bit_status_t fwdt_flag_status_get(uint16_t fwdt_flag);



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

