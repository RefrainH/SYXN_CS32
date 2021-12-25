/**
  * @file    cs32f0xx_crc.h
  * @brief   This file contains all the functions prototypes for the CRC firmware library.   
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_CRC_H
#define __CS32F0XX_CRC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup CRC
  * @{
  */

/**  
  * @brief CRC Reverse Input Data definition
  */
#define CRC_REVERSE_INPUT_DATA_NO             ((uint32_t)0x00000000)     //!< No reverse operation of Input Data 
#define CRC_REVERSE_INPUT_DATA_8BITS          CRC_CTR_DINREVMOD_0        //!< Reverse operation of Input Data on 8 bits 
#define CRC_REVERSE_INPUT_DATA_16BITS         CRC_CTR_DINREVMOD_1        //!< Reverse operation of Input Data on 16 bits
#define CRC_REVERSE_INPUT_DATA_32BITS         CRC_CTR_DINREVMOD          //!< Reverse operation of Input Data on 32 bits

#define CRC_REVERSE_INPUT_DATA_CHECK(value) (((value) == CRC_REVERSE_INPUT_DATA_NO)     || \
                                             ((value) == CRC_REVERSE_INPUT_DATA_8BITS)  || \
                                             ((value) == CRC_REVERSE_INPUT_DATA_16BITS) || \
                                             ((value) == CRC_REVERSE_INPUT_DATA_32BITS))

/** 
  * @brief CRC_Polynomial Size definition
  */
#define CRC_POL_SIZE_7                       CRC_CTR_POLSIZE        //!< 7-bit polynomial for CRC calculation 
#define CRC_POL_SIZE_8                       CRC_CTR_POLSIZE_1      //!< 8-bit polynomial for CRC calculation 
#define CRC_POL_SIZE_16                      CRC_CTR_POLSIZE_0      //!< 16-bit polynomial for CRC calculation 
#define CRC_POL_SIZE_32                      ((uint32_t)0x00000000) //!< 32-bit polynomial for CRC calculation 

#define CRC_POL_SIZE_CHECK(len) (((len) == CRC_POL_SIZE_7)  || \
                                 ((len) == CRC_POL_SIZE_8)  || \
                                 ((len) == CRC_POL_SIZE_16) || \
                                 ((len) == CRC_POL_SIZE_32))
                              


/**
  * @fn void crc_def_init(void) 
  * @brief  Deinitializes CRC peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void crc_def_init(void);

/**
  * @fn void crc_data_reset(void)
  * @brief  Resets the CRC calculation unit and sets INITCFG register content in DATA register.
  * @param  None
  * @return None
  */
void crc_data_reset(void);

/**
  * @fn void crc_polynomail_select(uint32_t value)
  * @param  value: Specifies the polynomial size, it can be:
  *          @arg CRC_POL_SIZE_7(7-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_8(8-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_16(16-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_32(32-bit polynomial for CRC calculation)
  * @return None
  */
void crc_polynomail_select(uint32_t value);

/**
  * @fn void crc_input_data_reverse(uint32_t value)
  * @brief  Selects the reverse operation to be performed on input data.
  * @param  value: Specifies the reverse operation on input data.
  *          It can be: 
  *          @arg CRC_REVERSE_INPUT_DATA_NO(No reverse operation is performed)
  *          @arg CRC_REVERSE_INPUT_DATA_8BITS(reverse operation performed on 8 bits)
  *          @arg CRC_REVERSE_INPUT_DATA_16BITS(reverse operation performed on 16 bits)
  *          @arg CRC_REVERSE_INPUT_DATA_32BITS(reverse operation performed on 32 bits)
  * @return None
  */
void crc_input_data_reverse(uint32_t value);

/**
  * @fn void crc_output_data_reverse_enable_ctrl(enable_state_t enable)
  * @brief  Enables or disable the reverse operation on output data.
  *         The reverse operation on output data is performed on 32-bit.
  * @param  enable: new state of the reverse operation on output data.
  *         This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void crc_output_data_reverse_enable_ctrl(enable_state_t enable);

/**
  * @fn void crc_init_value_set(uint32_t value)
  * @brief  Initializes the INITCFG register.
  * @note   After resetting CRC calculation unit, set CRC_DATA register
  * @param  value: Programmable initial CRC value
  * @return None
  */
void crc_init_value_set(uint32_t value); 

/**
  * @fn uint32_t crc_crc32_calc(uint32_t value)
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param  value: data word(32-bit) to compute its CRC
  * @return 32-bit CRC
  */
uint32_t crc_crc32_calc(uint32_t value);

/**
  * @fn uint32_t crc_crc16_calc(uint16_t value)
  * @brief  Computes the 16-bit CRC of a given 16-bit data . 
  * @param  value: data half-word(16-bit) to compute its CRC
  * @return 16-bit CRC
  */
uint32_t crc_crc16_calc(uint16_t value);

/**
  * @fn uint32_t crc_crc8_calc(uint8_t value)
  * @brief  Computes the 8-bit CRC of a given 8-bit data. 
  * @param  value: 8-bit data to compute its CRC
  * @return 8-bit CRC
  */
uint32_t crc_crc8_calc(uint8_t value);

/**
  * @fn uint32_t crc_data_buffer_calc(uint32_t buf[], uint32_t length)
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  buf: pointer to the buffer containing the data to be computed
  * @param  length: length of the buffer to be computed
  * @return 32-bit CRC
  */
uint32_t crc_data_buffer_calc(uint32_t buf[], uint32_t length);

/**
  * @fn uint32_t crc_value_get(void)
  * @brief  Returns the current CRC value.
  * @param  None
  * @return 32-bit CRC
  */
uint32_t crc_value_get(void);

/**
  * @fn void crc_fredata_register_set(uint8_t value)
  * @brief Set an 8-bit data in Free Data register (FREDATA).
  * @param  value: set 8-bit value in the FREDATA register 
  * @return None
  */
void crc_fredata_register_set(uint8_t value);

/**
  * @fn uint8_t crc_fredata_register_get(void)
  * @brief  Get the 8-bit data stored in Free Data register (FREDATA).
  * @param  None
  * @return 8-bit value of the FREDATA register 
  */
uint8_t crc_fredata_register_get(void);

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

