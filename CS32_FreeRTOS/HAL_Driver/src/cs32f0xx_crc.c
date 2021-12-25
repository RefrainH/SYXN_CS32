/**
  * @file    cs32f0xx_crc.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of CRC computation unit peripheral:
  *          Configuration of the CRC computation unit;
  *          CRC computation of one/many 32-bit data;
  *          CRC Free Data register (FREDATA) access.
  *
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
      1. Enable CRC AHB clock using rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_CRC, ENABLE).
      2. If required, select the reverse operation on input data using crc_input_data_reverse().
      3. If required, enable the reverse operation on output data using crc_output_data_reverse_enable_ctrl(Enable).             
      4. use crc_crc32_calc() function to compute the CRC of a 32-bit data or use crc_data_buffer_calc()
         to compute the CRC if a 32-bit data buffer        
     @endverbatim
  *
  */
  

#include "cs32f0xx_crc.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup CRC
  * @{
  */


/**
  * @fn void crc_def_init(void) 
  * @brief  Deinitializes CRC peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void crc_def_init(void)
{
    /* Set DATA register to reset value */
    CRC->DATA = 0xFFFFFFFF;  
    CRC->POL = 0x04C11DB7;
  
    /* Reset FREDATA register */
    CRC->FREDATA = 0x00;
  
    /* Set INITCFG register to reset value */
    CRC->INITCFG = 0xFFFFFFFF;
  
    /* Reset the CRC calculation unit */
    CRC->CTR = CRC_CTR_RST;
}

/**
  * @fn void crc_data_reset(void)
  * @brief  Resets the CRC calculation unit and sets INITCFG register content in DATA register.
  * @param  None
  * @return None
  */
void crc_data_reset(void)
{
    /* Reset CRC generator */
    CRC->CTR |= CRC_CTR_RST;
}

/**
  * @fn void crc_polynomail_select(uint32_t value)
  * @param  value: Specifies the polynomial size, it can be:
  *          @arg CRC_POL_SIZE_7(7-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_8(8-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_16(16-bit polynomial for CRC calculation)
  *          @arg CRC_POL_SIZE_32(32-bit polynomial for CRC calculation)
  * @return None
  */
void crc_polynomail_select(uint32_t value)
{
    uint32_t temp = 0;

    ASSERT(CRC_POL_SIZE_CHECK(value));

    temp = CRC->CTR;

    /* Reset POL_SIZE bits */
    temp &= (uint32_t)~((uint32_t)CRC_CTR_POLSIZE);
    /* Set the polynomial size */
    temp |= (uint32_t)value;
    /* Write to CRC CTR register */
    CRC->CTR = (uint32_t)temp;
}

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
void crc_input_data_reverse(uint32_t value)
{
    uint32_t temp = 0;

    ASSERT(CRC_REVERSE_INPUT_DATA_CHECK(value));

    temp = CRC->CTR;

    /* Reset DINREVMOD bits */
    temp &= (uint32_t)~((uint32_t)CRC_CTR_DINREVMOD);
    /* Set the reverse operation */
    temp |= (uint32_t)value;

    CRC->CTR = (uint32_t)temp;
}

/**
  * @fn void crc_output_data_reverse_enable_ctrl(enable_state_t enable)
  * @brief  Enables or disable the reverse operation on output data.
  *         The reverse operation on output data is performed on 32-bit.
  * @param  enable: new state of the reverse operation on output data.
  *         This parameter can be: ENABLE or DISABLE.
  * @return None
  */
void crc_output_data_reverse_enable_ctrl(enable_state_t enable)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable));

    if (enable != DISABLE)
    {
        /* Enable reverse operation on output data */
        CRC->CTR |= CRC_CTR_DOREVEN;
    }
    else
    {
        /* Disable reverse operation on output data */
        CRC->CTR &= (uint32_t)~((uint32_t)CRC_CTR_DOREVEN);
    }
}

/**
  * @fn void crc_init_value_set(uint32_t value)
  * @brief  Initializes the INITCFG register.
  * @note   After resetting CRC calculation unit, set CRC_DATA register
  * @param  value: Programmable initial CRC value
  * @return None
  */
void crc_init_value_set(uint32_t value)
{
  CRC->INITCFG = value;
}

/**
  * @fn uint32_t crc_crc32_calc(uint32_t value)
  * @brief  Computes the 32-bit CRC of a given data word(32-bit).
  * @param  value: data word(32-bit) to compute its CRC
  * @return 32-bit CRC
  */
uint32_t crc_crc32_calc(uint32_t value)
{
    CRC->DATA = value;
  
    return (CRC->DATA);
}

/**
  * @fn uint32_t crc_crc16_calc(uint16_t value)
  * @brief  Computes the 16-bit CRC of a given 16-bit data. 
  * @param  value: data half-word(16-bit) to compute its CRC
  * @return 16-bit CRC
  */
uint32_t crc_crc16_calc(uint16_t value)
{
    *(uint16_t*)(CRC_BASE) = (uint16_t) value;
  
    return (CRC->DATA);
}

/**
  * @fn uint32_t crc_crc8_calc(uint8_t value)
  * @brief  Computes the 8-bit CRC of a given 8-bit data. 
  * @param  value: 8-bit data to compute its CRC
  * @return 8-bit CRC
  */
uint32_t crc_crc8_calc(uint8_t value)
{
    *(uint8_t*)(CRC_BASE) = (uint8_t) value;

    return (CRC->DATA);
}

/**
  * @fn uint32_t crc_data_buffer_calc(uint32_t buf[], uint32_t length)
  * @brief  Computes the 32-bit CRC of a given buffer of data word(32-bit).
  * @param  buf: pointer to the buffer containing the data to be computed
  * @param  length: length of the buffer to be computed
  * @return 32-bit CRC
  */
uint32_t crc_data_buffer_calc(uint32_t buf[], uint32_t length)
{
    uint32_t index = 0;
  
    for(index = 0; index < length; index++)
    {
        CRC->DATA = buf[index];
    }
    return (CRC->DATA);
}

/**
  * @fn uint32_t crc_value_get(void)
  * @brief  Returns the current CRC value.
  * @param  None
  * @return 32-bit CRC
  */
uint32_t crc_value_get(void)
{
    return (CRC->DATA);
}

/**
  * @fn void crc_fredata_register_set(uint8_t value)
  * @brief Set an 8-bit data in Free Data register (FREDATA).
  * @param  value: set 8-bit value in the FREDATA register 
  * @return None
  */
void crc_fredata_register_set(uint8_t value)
{
    CRC->FREDATA = value;
}

/**
  * @fn uint8_t crc_fredata_register_get(void)
  * @brief  Get the 8-bit data stored in Free Data register (FREDATA).
  * @param  None
  * @return 8-bit value of the FREDATA register 
  */
uint8_t crc_fredata_register_get(void)
{
    return (CRC->FREDATA);
}

/**
  * @}
  */

/**
  * @}
  */

