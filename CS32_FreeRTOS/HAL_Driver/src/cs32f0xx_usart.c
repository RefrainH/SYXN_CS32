/**
  * @file    cs32f0xx_usart.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Universal synchronous asynchronous receiver
  *          transmitter (USART), This module Initialization and Configuration 
  *          includes: STOP Mode; AutoBaudRate; Data transfers;LIN mode;
  *            Multi-Processor Communication;Half-duplex mode;Smartcard mode;
  *            IrDA mode;RS485 mode;DMA transfers management;
  *            Interrupts and flags management.
  *           
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  * @verbatim  
  *
  ----------------------- this driver instruction for use--------------------------- 
    1. Enable peripheral clock using rcu_apb2_periph_clock_enable_ctrl(RCC_APB2Periph_USART1, ENABLE)
        function for USART1 or using rcu_apb1_periph_clock_enable_ctrl(RCC_APB1Periph_USARTx, ENABLE)
        function for USART2 and USART3.
    2.According to the USART mode, enable the GPIO clocks using 
        rcu_ahb_periph_clock_enable_ctrl() function. (The I/O can be TX, RX, CTS, 
        or and SCLK). 
    3. Peripheral's  multi-function: 
        a. Connect the pin to the desired peripherals' MFSEL (multi-function selection)
              using gpio_mf_config() function.
        b. Configure the desired pin in  multi-function by:
             gpio_configStruct->gpio_mode = GPIO_MODE_MF.
        c. Select the type, pull-up/pull-down and output speed via 
             gpio_pull, gpio_out_type and gpio_speed members.
        d. Call gpio_init() function.        
    4. Program the Baud Rate, Word Length , Stop Bit, Parity, Hardware 
        flow control and Mode(Receiver/Transmitter) using the spi_init()
        function.  
   5. For synchronous mode, enable the clock and program the polarity,
        phase and last bit using the usart_clock_config() function.  
   6.Enable the NVIC and the corresponding interrupt using the function 
        usart_interrupt_config() if you need to use interrupt mode.   
    7.When using the DMA mode: 
        a. Configure the DMA using dma_init() function.
        b. Active the needed channel Request using usart_dma_enable_ctrl() function.   
    8. Enable the USART using the usart_enable_ctrl() function.   
    9.Enable the DMA using the dma_enable_ctrl() function, when using DMA mode.        
    @endverbatim
  *
  */

#include "cs32f0xx_usart.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup USART 
  * @brief USART driver modules
  * @{
  */


//!< USART CTR1  clear Mask ((~(uint32_t)0xFFFFE6F3)) 
#define USART_CTR1_CLEAR_MASK     ((uint32_t)(USART_CTR1_DL | USART_CTR1_PEN | \
                                   USART_CTR1_ODDS | USART_CTR1_TEN | USART_CTR1_REN))

                                              
//!< USART CTR3  clear Mask ((~(uint32_t)0xFFFFFCFF)) */
#define USART_CTR3_CLEAR_MASK            ((uint32_t)(USART_CTR3_RTSEN | USART_CTR3_CTSEN))

//!< USART Interrupts mask */
#define USART_INT_MASK                   ((uint32_t)0x000000FF)


 
/**
  * @fn void usart_def_init(usart_reg_t* ptr_usart)
  * @brief  Initializes registers  of the ptr_usart to default values.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @return None
  */
void usart_def_init(usart_reg_t* ptr_usart)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));

    if (ptr_usart == USART1)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART1, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART1, DISABLE);
    }
    else if (ptr_usart == USART2)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART2, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART2, DISABLE);
    }
    else if (ptr_usart == USART3)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART3, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART3, DISABLE);
    }
    else if (ptr_usart == USART4)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART4, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART4, DISABLE);
    }
    else if (ptr_usart == USART5)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART5, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_USART5, DISABLE);
    }
    else if (ptr_usart == USART6)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART6, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART6, DISABLE);
    }
    else if (ptr_usart == USART7)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART7, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART7, DISABLE);
    }
    else if  (ptr_usart == USART8)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART8, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_USART8, DISABLE);
    }
}

/**
  * @fn void usart_init(usart_reg_t* ptr_usart, usart_config_t* ptr_usart_config)
  * @brief  Initializes the ptr_usart peripheral according to the specified parameters
  *         in the ptr_usart_config .
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  ptr_usart_config: pointer to a usart_config_t structure containing
  *         the configuration for the specified USART peripheral.
  * @return None
  */
void usart_init(usart_reg_t* ptr_usart, usart_config_t* ptr_usart_config)
{
    uint32_t divider = 0;
    uint32_t freq = 0;
    uint32_t reg = 0;
    rcu_clock_t rcu_clock;
  
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_BAUD_RATE_CHECK(ptr_usart_config->usart_rate));  
    ASSERT(USART_DATA_WIDTH_CHECK(ptr_usart_config->data_width));
    ASSERT(USART_STOP_BIT_CHECK(ptr_usart_config->stop_bits));
    ASSERT(USART_PARITY_CHECK(ptr_usart_config->usart_parity));
    ASSERT(USART_MODE_CHECK(ptr_usart_config->usart_mode));
    ASSERT(USART_FLOW_CONTROL_CHECK(ptr_usart_config->flow_control));
  
    /* Disable USART */
    ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_UEN);  
    reg = ptr_usart->CTR2;
        
    reg &= (uint32_t)~((uint32_t)USART_CTR2_STOPLEN); /* Clear STOPLEN[13:12] bits */  
    reg |= (uint32_t)ptr_usart_config->stop_bits;  /* Set STOPLEN[13:12]  */   
    ptr_usart->CTR2 = reg;
    
    reg = ptr_usart->CTR1;  
    reg &= (uint32_t)~((uint32_t)USART_CTR1_CLEAR_MASK); /* Clear DH, PEN, ODDS, TEN, REN */
  
    /* Configure the USART data width, Parity and mode  */
    reg |= (uint32_t)ptr_usart_config->data_width | ptr_usart_config->usart_parity |
                    ptr_usart_config->usart_mode;
    ptr_usart->CTR1 = reg;
  
    reg = ptr_usart->CTR3; 
    reg &= (uint32_t)~((uint32_t)USART_CTR3_CLEAR_MASK);  /* Clear CTR3 CTSEN and RTSEN bits */ 
    reg |= ptr_usart_config->flow_control;    
    ptr_usart->CTR3 = reg;
  
    /* Configure the USART Baud Rate*/
    rcu_clk_freq_get(&rcu_clock);  
    if (ptr_usart == USART1)
    {
        freq = rcu_clock.usart1_clk_freq;
    }
    else if (ptr_usart == USART2)
    {
        freq = rcu_clock.usart2_clk_freq;
    }
    else if (ptr_usart == USART3)
    {
        freq = rcu_clock.usart3_clk_freq;
    }
    else
    {
        freq = rcu_clock.pclk_freq;
    }
  
    if ((ptr_usart->CTR1 & USART_CTR1_OVRS) != 0)
    {
        /* (divider * 10) computing in case Oversampling mode is 8 Samples */
        divider = (uint32_t)((2 * freq) / (ptr_usart_config->usart_rate));
        reg  = (uint32_t)((2 * freq) % (ptr_usart_config->usart_rate));
    }
    else 
    {
        /* (divider * 10) computing in case Oversampling mode is 16 Samples */
        divider = (uint32_t)((freq) / (ptr_usart_config->usart_rate));
        reg  = (uint32_t)((freq) % (ptr_usart_config->usart_rate));
    }
  
    /* Round the divider : if fractional part i greater than 0.5 increment divider */
    if (reg >=  (ptr_usart_config->usart_rate) / 2)
    {
        divider++;
    } 
  
    /* Implement the divider in case Oversampling mode is 8 Samples */
    if ((ptr_usart->CTR1 & USART_CTR1_OVRS) != 0)
    {
        /* Get the LSB of divider and shift it to the right by 1 bit, update the divider value */
        reg = (divider & (uint16_t)0x000F) >> 1;
        divider = (divider & (uint16_t)0xFFF0) | reg;
    }
  
    /* Write divider to baud rate register */
    ptr_usart->BRT = (uint16_t)divider;
}

/**
  * @fn void usart_config_struct_init(usart_config_t* ptr_usart_config)
  * @brief  Fills each ptr_usart_config member with its default value.
  * @param  ptr_usart_config: pointer to a usart_config_t structure to be initialized.
  * @return None
  */
void usart_config_struct_init(usart_config_t* ptr_usart_config)
{
    /* ptr_usart_config members initialized with default value*/
    ptr_usart_config->usart_rate = 9600;
    ptr_usart_config->data_width = USART_DATA_WIDTH_8;
    ptr_usart_config->stop_bits = USART_STOP_BIT_1;
    ptr_usart_config->usart_parity = USART_PARITY_NO ;
    ptr_usart_config->usart_mode = USART_MODE_RX | USART_MODE_TX;
    ptr_usart_config->flow_control = USART_FLOW_CONTROL_NONE;  
}

/**
  * @fn void usart_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void usart_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the specified USART */
        ptr_usart->CTR1 |= USART_CTR1_UEN;
    }
    else
    {
        /* Disable the specified USART */
        ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_UEN);
    }
}

/**
  * @fn void usart_direction_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's transmitter or receiver.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  direction: specifies the USART direction,it can be any of the following values:
  *          @arg USART_MODE_TX(USART Transmitter)
  *          @arg USART_MODE_RX(USART Receiver)
  * @param  enable_flag: ENABLE or DISABLE the USART transfer direction.
  * @return None
  */
void usart_direction_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_MODE_CHECK(direction));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag)); 

    if (enable_flag != DISABLE)
    {
        /* Enable the USART's transfer interface */
        ptr_usart->CTR1 |= direction;
    }
    else
    {
        /* Disable the USART's transfer interface */       
        ptr_usart->CTR1 &= (uint32_t)~direction;
    }
}

/**
  * @fn void usart_oversampling_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's oversampling mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the USART oversampling mode.
  * @return None
  */
void usart_oversampling_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the Oversampling mode */
        ptr_usart->CTR1 |= USART_CTR1_OVRS;
    }
    else
    {
        /* Disable the Oversampling mode */
        ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_OVRS);
    }
}  

/**
  * @fn void usart_sample_method_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's one bit sampling method.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the USART one bit sampling method.
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_sample_method_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        /* Enable one bit sample method by setting the SPMS bit*/
        ptr_usart->CTR3 |= USART_CTR3_SPMS;
    }
    else
    {
        /* Disable one bit sample method by clearing the SPMS bit */
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_SPMS);
    }
}

/**
  * @fn void usart_msb_first_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's most significant bit first 
  *         transmitted/received following the start bit.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the USART most significant bit first
  *         transmitted/received following the start bit.
  * @return None
  */
void usart_msb_first_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable MSB transmitted/received */
        ptr_usart->CTR2 |= USART_CTR2_MSBF;
    }
    else
    {
        /* Disable MSB transmitted/received */      
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_MSBF);
    }
}

/**
  * @fn void usart_data_reverse_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the binary data inversion.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE new defined levels for the USART data.
  * @return None
  */
void usart_data_reverse_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the binary data inversion feature */      
        ptr_usart->CTR2 |= USART_CTR2_DINV;
    }
    else
    {
        /* Disable the binary data inversion feature */       
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_DINV);
    }
}

/**
  * @fn void usart_pin_reverse_enable_ctrl(usart_reg_t* ptr_usart, uint32_t usart_invpin, enable_state_t enable_flag)
  * @brief  Enables or disables the Pin(s) active level inversion.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  usart_invpin: specifies the USART pin(s) to invert.
  *          This parameter can be any combination of the following values:
  *           @arg USART_INVERSION_PIN_TX(USART Tx pin active level inversion)
  *           @arg USART_INVERSION_PIN_RX(USART Rx pin active level inversion)
  * @param  enable_flag: ENABLE or DISABLE new active level status for the USART pin(s).
  * @note   It has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_pin_reverse_enable_ctrl(usart_reg_t* ptr_usart, uint32_t usart_invpin, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_INVERSION_PIN_CHECK(usart_invpin));  
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag)); 

    if (enable_flag != DISABLE)
    {
        /* Enable the active level inversion for selected pins  */
        ptr_usart->CTR2 |= usart_invpin;
    }
    else
    {
        /* Disable the active level inversion for selected requests  */
        ptr_usart->CTR2 &= (uint32_t)~usart_invpin;
    }
}

/**
  * @fn void usart_pin_swap_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the swap Tx/Rx pins.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the USARTx TX/RX pins pinout.
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_pin_swap_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the TX/RX pin swap feature by setting the TXRXSWAP bit */
        ptr_usart->CTR2 |= USART_CTR2_TXRXSWAP;
    }
    else
    {
        /* Disable the TX/RX pin swap feature by clearing the TXRXSWAP bit */
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_TXRXSWAP);
    }
}

/**
  * @fn void usart_recv_overtime_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the receiver Time Out feature.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the ptr_usart receiver Time Out.
  * @return None
  */
void usart_recv_overtime_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the receiver time out  by setting the ROTEN bit */    
        ptr_usart->CTR2 |= USART_CTR2_ROTEN;
    }
    else
    {
        /* Disable the receiver time out feature by clearing the RTOEN bit */
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_ROTEN);
    }
}

/**
  * @fn void usart_recv_overtime_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the receiver Time Out value.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  value: specifies the Receiver Time Out value.
  * @return None
  */
void usart_recv_overtime_set(usart_reg_t* ptr_usart, uint32_t value)
{    
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_TIMEOUT_CHECK(value));
  
    /* Clear the receiver time_out value */
    ptr_usart->ROT &= (uint32_t)~((uint32_t)USART_ROT_RXOVR);
    /* Set the receiver time_out value */
    ptr_usart->ROT|= value;
}

/**
  * @fn void usart_prescaler_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the system clock prescaler.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  value: specifies the prescaler clock.
  * @note   This function has to be called before calling usart_enable_ctrl() function.    
  * @return None
  */
void usart_prescaler_set(usart_reg_t* ptr_usart, uint8_t value)
{ 
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
  
    /* Clear the USART prescaler */
    ptr_usart->GTPDIV &= USART_GTPDIV_GUDT;
    /* Set the USART prescaler */
    ptr_usart->GTPDIV |= value;
}


/**
  * @fn void usart_stop_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART peripheral in STOP Mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the ptr_usart peripheral state in stop mode.
  * @return None
  */
void usart_stop_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the selected USART wakeup mcu from stop mode */
        ptr_usart->CTR1 |= USART_CTR1_WKUPDSM;
    }
    else
    {
        /* Disable the selected USART wakeup mcu from stop mode */
        ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_WKUPDSM);
    }
}

/**
  * @fn void usart_stop_mode_wakeup_config(usart_reg_t* ptr_usart, uint32_t usart_wakeupsource)
  * @brief  Selects the USART WakeUp method form stop mode.
  * @note   This function is not available for CS32F030 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  usart_wakeupsource: specifies the selected USART wakeup method.
  * @note   This function has to be called before calling usart_enable_ctrl() function.   
  * @return None
  */
void usart_stop_mode_wakeup_config(usart_reg_t* ptr_usart, uint32_t usart_wakeupsource)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_WAKEUP_METHOD_CHECK(usart_wakeupsource));

    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_WKUPMTHD);
    ptr_usart->CTR3 |= usart_wakeupsource;
}

/**
  * @fn void usart_auto_baud_rate_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the Auto Baud Rate.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the USARTx auto baud rate.
  * @return None
  */
void usart_auto_baud_rate_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the auto baud rate feature */       
        ptr_usart->CTR2 |= USART_CTR2_ABREN;
    }
    else
    {
        /* Disable the auto baud rate feature */       
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_ABREN);
    }
}

/**
  * @fn void usart_auto_baud_rate_config(usart_reg_t* ptr_usart, uint32_t rate_value)
  * @brief  Selects the USART auto baud rate method.
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  rate_value: specifies the selected USART auto baud rate method.
  *          This parameter can be one of the following values:
  *            @arg USART_AUTO_RATE_MEASURE_START_BIT(Start Bit duration measurement)
  *            @arg USART_AUTO_RATE_MEASURE_FALLING_EDGE(Falling edge to falling edge measurement)
  * @note   This function has to be called before calling usart_enable_ctrl() function.  
  * @return None
  */
void usart_auto_baud_rate_config(usart_reg_t* ptr_usart, uint32_t rate_value)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_AUTO_RATE_MEASURE_CHECK(rate_value));

    ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_ABRSEL);
    ptr_usart->CTR2 |= rate_value;
}

/**
  * @fn void usart_data_send(usart_reg_t* ptr_usart, uint16_t value)
  * @brief  Transmits single data through the ptr_usart peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  value: the data to transmit.
  * @return None
  */
void usart_data_send(usart_reg_t* ptr_usart, uint16_t value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DATA_CHECK(value)); 
    
    /* USART transmit data */
    ptr_usart->TXBUF = (value & (uint16_t)0x01FF);
}

/**
  * @fn uint16_t usart_data_recv(usart_reg_t* ptr_usart)
  * @brief  Returns the most recent received data by the ptr_usart peripheral.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @return The received data.
  */
uint16_t usart_data_recv(usart_reg_t* ptr_usart)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
  
    /* USART receive data */
    return (uint16_t)(ptr_usart->RXBUF & (uint16_t)0x01FF);
}

/**
  * @fn void usart_address_set(usart_reg_t* ptr_usart, uint8_t addr)
  * @brief  Sets the address of the USART node.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  addr: Indicates the address of the USART node.
  * @return None
  */
void usart_address_set(usart_reg_t* ptr_usart, uint8_t addr)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
  
    /* Clear the USART address */
    ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_UADR);
    /* Set the USART address */
    ptr_usart->CTR2 |=((uint32_t)addr << (uint32_t)0x18);
}

/**
  * @fn void usart_recv_mask_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's reception mask mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void usart_recv_mask_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag)); 
  
    if (enable_flag != DISABLE)
    {
        /* Enable reception mask mode */
        ptr_usart->CTR1 |= USART_CTR1_RXMSKEN;
    }
    else
    {
        /* Disable reception mask mode*/
        ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_RXMSKEN);
    }
}

/**
  * @fn void usart_recv_wakeup_config(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Selects the USART WakeUp method from mute mode.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  flag: specifies the USART wakeup method.
  *          This parameter can be one of the following values:
  *            @arg USART_RX_WAKEUP_DISABLE(WakeUp by an idle line detection)
  *            @arg USART_RX_WAKEUP_ENABLE(WakeUp by an address mark)
  * @return None
  */
void usart_recv_wakeup_config(usart_reg_t* ptr_usart, uint32_t flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_RX_WAKEUP_CHECK(flag));

    ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_RXWKUPS);
    ptr_usart->CTR1 |= flag;
}

/**
  * @fn void usart_addr_detect_set(usart_reg_t* ptr_usart, uint32_t addr)
  * @brief  Configure the the USART Address detection length.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  addr: specifies the USART address length detection.
  *          This parameter can be one of the following values:
  *            @arg USART_ADDRESS_4BIT(4-bit address length detection) 
  *            @arg USART_ADDRESS_7BIT(7-bit address length detection) 
  * @return None
  */
void usart_addr_detect_set(usart_reg_t* ptr_usart, uint32_t addr)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_ADDRESS_BIT_CHECK(addr));

    ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_ADRM7);
    ptr_usart->CTR2 |= addr;
}

/**
  * @fn void usart_11bit_break_detect_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the USART LIN Break detection length.
  * @note   This function is not available for CS32F030 devices. 
  * @note   This function is not available for CS32F036 devices.   
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  value: specifies the LIN break detection length.
  *          This parameter can be one of the following values:
  *            @arg USART_LIN_BREAK_10BIT(10-bit break detection)
  *            @arg USART_LIN_BREAK_11BIT(11-bit break detection)
  * @return None
  */
void usart_11bit_break_detect_set(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_LIN_BREAK_BIT_CHECK(value));

    ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_LINBRK11);
    ptr_usart->CTR2 |= value;  
}

/**
  * @fn void usart_lin_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's LIN mode.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1  to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the USART LIN mode.
  * @return None
  */
void usart_lin_mode_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the LIN mode  */
        ptr_usart->CTR2 |= USART_CTR2_LINEN;
    }
    else
    {
        /* Disable the LIN mode */
        ptr_usart->CTR2 &= (uint32_t)~((uint32_t)USART_CTR2_LINEN);
    }
}   

/**
  * @fn void usart_half_duplex_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's Half Duplex communication.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  enable_flag: ENABLE or DISABLE the USART Communication.
  * @return None
  */
void usart_half_duplex_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the Half-Duplex mode  */
        ptr_usart->CTR3 |= USART_CTR3_HDEN;
    }
    else
    {
        /* Disable the Half-Duplex mode */
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_HDEN);
    }
}

/**
  * @fn void usart_guard_time_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the specified USART guard time.
  * @note   This function is not available for CS32F030 devices.
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.  
  * @param  value: specifies the guard time.
  * @return None
  */
void usart_guard_time_set(usart_reg_t* ptr_usart, uint8_t value)
{    
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));

    /* Clear the USART Guard time */
    ptr_usart->GTPDIV &= USART_GTPDIV_PDIV;
    /* Set the USART guard time */
    ptr_usart->GTPDIV |= (uint16_t)((uint16_t)value << 0x08);
}

/**
  * @fn void usart_smartcard_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's SmartCard mode.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the Smart Card mode.
  * @return None
  */
void usart_smartcard_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        if ((ptr_usart->GTPDIV & USART_GTPDIV_PDIV) != 0x00)
        {
            /* Enable the SmartCard mode */
            ptr_usart->CTR3 |= USART_CTR3_SCEN;
        }
    }
    else
    {
        /* Disable the SmartCard mode */
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_SCEN);
    }
}

/**
  * @fn void usart_smartcard_nack_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables NACK transmission.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  enable_flag: ENABLE or DISABLE the NACK transmission.
  * @return None
  */
void usart_smartcard_nack_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart)); 
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the NACK transmission   */
        ptr_usart->CTR3 |= USART_CTR3_SCNACK;
    }
    else
    {
        /* Disable the NACK transmission*/
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_SCNACK);
    }
}


/**
  * @fn void usart_auto_retry_times_set(usart_reg_t* ptr_usart, uint8_t value)
  * @brief  Sets the Smart Card number of retries in transmit and receive.
  * @note   This function is not available for CS32F030 devices.
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  value: specifies the Smart Card auto retry times.
  * @return None
  */
void usart_auto_retry_times_set(usart_reg_t* ptr_usart, uint8_t value)
{    
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_RETRY_NUM_CHECK(value));
  
    /* Clear the USART auto retry count */
    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_SCANUM);
    /* Set the USART auto retry count*/
    ptr_usart->CTR3 |= (uint32_t)((uint32_t)value << 0x11);
}

/**
  * @fn void usart_block_value_set(usart_reg_t* ptr_usart, uint8_t block_number)
  * @brief  Sets the Smar Card Block number.
  * @note   This function is not available for CS32F030 devices. 
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.
  * @param  block_number: specifies the Smart Card block number.
  * @return None
  */
void usart_block_value_set(usart_reg_t* ptr_usart, uint8_t block_number)
{    
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));

    /* Clear the Smartcard block number */
    ptr_usart->ROT &= (uint32_t)~((uint32_t)USART_ROT_NUMBLK);
    /* Set the Smartcard block number */
    ptr_usart->ROT |= (uint32_t)((uint32_t)block_number << 0x18);
}

/**
  * @fn void usart_irda_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Configures the USART's IrDA interface.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral. 
  * @param  value: specifies the IrDA mode,it can be one of the following values:
  *          @arg USART_IRDA_MODE_SELECT_LOW 
  *          @arg USART_IRDA_MODE_SELECT_NORMAL.
  * @return None
  */
void usart_irda_config(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(USART_IRDA_MODE_SELECT_CHECK(value));

    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_IRMS);
    ptr_usart->CTR3 |= value;
}

/**
  * @fn void usart_irda_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's IrDA interface.
  * @note   This function is not available for CS32F030 devices.  
  * @note   This function is not available for CS32F036 devices.  
  * @param  ptr_usart: pointer to USARTx where x can be 1 to select USART peripheral.  
  * @param  enable_flag: ENABLE or DISABLE the IrDA mode.
  * @return None
  */
void usart_irda_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_123_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the IrDA mode */
        ptr_usart->CTR3 |= USART_CTR3_IRDAEN;
    }
    else
    {
        /* Disable the IrDA mode */
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_IRDAEN);
    }
}

/**
  * @fn void usart_driver_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
  * @brief Enables or disables the USART's DE functionality.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices. 
  * @param  enable_flag: ENABLE or DISABLE the driver enable mode.
  * @return None
  */
void usart_driver_enable_ctrl(usart_reg_t* ptr_usart, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the DE functionality */
        ptr_usart->CTR3 |= USART_CTR3_DEN;
    }
    else
    {
    /* Disable the DE functionality */
        ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_DEN);
    }
}

/**
  * @fn void usart_driver_enable_polarity_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Configures the USART's DE polarity
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies the DE polarity, it can be one of the following values:
  *          @arg USART_DE_POLARITY_LOW 
  *          @arg USART_DE_POLARITY_HIGH
  * @return None
  */
void usart_driver_enable_polarity_config(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DE_POLARITY_CHECK(value));

    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_DEPS);
    ptr_usart->CTR3 |= value;
}

/**
  * @fn void usart_driver_enable_preactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the specified RS485 DE Pre-active time 
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  value: specifies DE Pre-active time between the activation of the DE
  *         signal and the the beginning of the start bit
  * @return None
  */
void usart_driver_enable_preactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DE_START_STABLE_TIME_CHECK(value)); 

    /* Clear the DE Pre-active time */
    ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_PRETDE);
    /* Set the new value for the DE Pre-active time */
    ptr_usart->CTR1 |=((uint32_t)value << (uint32_t)0x15);
}

/**
  * @fn void usart_driver_enable_posactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Sets the specified RS485 DE Pos-active time
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  value: specifies the time between the middle of the last stop bit 
  *         in a transmitted message and the Pos_activation of the DE signal .
  * @return None
  */
void usart_driver_enable_posactive_time_set(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DE_START_STABLE_TIME_CHECK(value)); 
    
    /* Clear the Pos-active time */
    ptr_usart->CTR1 &= (uint32_t)~((uint32_t)USART_CTR1_POSTDE);
    /* Set the new value for the DE Pos-active time */
    ptr_usart->CTR1 |=((uint32_t)value << (uint32_t)0x10);
}

/**
  * @fn void usart_dma_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
  * @brief  Enables or disables the USART's DMA interface.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2 to select USART peripheral.
  * @param  direction: It can be any combination of the following values:
  *          @arg USART_DMA_TX(USART DMA transmit)
  *          @arg USART_DMA_RX(USART DMA receive)
  * @param  enable_flag: ENABLE or DISABLE the DMA Request sources.
  * @return None
  */
void usart_dma_enable_ctrl(usart_reg_t* ptr_usart, uint32_t direction, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DMA_CHECK(direction));  
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag)); 

    if (enable_flag != DISABLE)
    {
        /* Enable the DMA transfer */
        ptr_usart->CTR3 |= direction;
    }
    else
    {
        /* Disable the DMA transfer */
        ptr_usart->CTR3 &= (uint32_t)~direction;
    }
}

/**
  * @fn void usart_dma_recv_error_config(usart_reg_t* ptr_usart, uint32_t value)
  * @brief  Enables or disables the USART's DMA interface when reception error occurs.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2 to select USART peripheral.  
  * @param  value: specifies the DMA status in case of reception error.
  *          It can be any combination of the following values:
  *           @arg USART_DMA_RX_ERROR_ENABLE 
  *           @arg USART_DMA_RX_ERROR_DISABLE
  * @return None
  */
void usart_dma_recv_error_config(usart_reg_t* ptr_usart, uint32_t value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_DMA_RX_ERROR_CHECK(value)); 
  
    /* Clear the DMA Reception error detection bit */
    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_DRMRE);
    /* Set the new value for the DMA Reception error detection bit */
    ptr_usart->CTR3 |= value;
}


/**
  * @fn void usart_interrupt_config(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified USART interrupts.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  set_value: specifies the USART interrupt sources to be enabled or disabled.
  *          It can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)
  *           @arg USART_INT_ROT(Receive overtime interrupt)
  *           @arg USART_INT_CTS(CTS interrupt)
  *           @arg USART_INT_TC(Transmission complete interrupt)
  *           @arg USART_INT_TXE(USART transmit empty)
  *           @arg USART_INT_RXNE(USART receiver buffer not empty)
  *           @arg USART_INT_ERR(USART error interrupt )
  *           @arg USART_INT_PERR(Parity Error interrupt)
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, not for CS32F030, CS32F036 devices)
  *           @arg USART_INT_ENDBLK(End of block interrupt, not for CS32F030, CS32F036 devices)
  *           @arg USART_INT_WKUP(Wakeup interrupt, not  for CS32F030 devices)
  * @param  enable_flag: ENABLE or DISABLE the specified USARTx interrupts.
  * @return None
  */
void usart_interrupt_config(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
{
    uint32_t usart_reg = 0;
    uint32_t itpos = 0;
    uint32_t itmask = 0;
    uint32_t usartx_base = 0;
  
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_INT_CONFIG_CHECK(set_value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    usartx_base = (uint32_t)ptr_usart;
  
    /* Get the USART register index */
    usart_reg = (((uint16_t)set_value) >> 0x08);
  
    /* Get the interrupt position */
    itpos = set_value & USART_INT_MASK;
    itmask = (((uint32_t)0x01) << itpos);
  
    if (usart_reg == 0x02) /*  The interrupt occurs in CTR2 register */
    {
        usartx_base += 0x04;
    }
    else if (usart_reg == 0x03) /* The interrupt occurs in CTR3 register */
    {
        usartx_base += 0x08;
    }
    else /* The interrupt occurs in CTR1 register */
    {
    }
    if (enable_flag != DISABLE)
    {
        *(__IO uint32_t*)usartx_base  |= itmask;
    }
    else
    {
        *(__IO uint32_t*)usartx_base &= ~itmask;
    }
}

/**
  * @fn void usart_trigger_enable_ctrl(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables the specified USART's trigger.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param  set_value: specifies the USART trigger.
  *          It can be any combination of the following values:
  *           @arg USART_TRIGGER_OPERATE_TXESET(Transmit data trigger);
  *           @arg USART_TRIGGER_OPERATE_RXNECLR(Receive data clear trigger); 
  *           @arg USART_TRIGGER_OPERATE_RXMSKT(Reception mask mode trigger); 
  *           @arg USART_TRIGGER_OPERATE_BRKFST(Break frame send trigger);
  *           @arg USART_TRIGGER_OPERATE_ABRT(Auto baud rate trigger).
  * @param  enable_flag: ENABLE or DISABLE the DMA interface when reception error occurs.
  * @return None
  */
void usart_trigger_enable_ctrl(usart_reg_t* ptr_usart, uint32_t set_value, enable_state_t enable_flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_TRIGGER_OPERATE_CHECK(set_value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag)); 
    
    if (enable_flag != DISABLE)
    {
        /* Enable the USART trigger by setting SWTR */  
        ptr_usart->SWTR |= set_value;
    }
    else
    {
        /* Disable the USART trigger by clearing SWTR */
        ptr_usart->SWTR &= (uint32_t)~set_value;
    }
}

/**
  * @fn void usart_recveive_overflow_config(usart_reg_t* ptr_usart, uint32_t set_value)
  * @brief   Enables or disables the USART's overflow detection.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.  
  * @param   set_value: specifies the OVR detection status in case of NORXOF error.
  *          It can be any combination of the following values:
  *           @arg USART_RX_OVERFLOW_DETECT_ENABLE(NORXOF error detection enabled); 
  *           @arg USART_RX_OVERFLOW_DETECT_DISABLE(NORXOF error detection disabled).
  * @return None
  */
void usart_recveive_overflow_config(usart_reg_t* ptr_usart, uint32_t set_value)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_RX_OVERFLOW_DETECT_CHECK(set_value));
  
    /* Clear the NORXOF detection bit */
    ptr_usart->CTR3 &= (uint32_t)~((uint32_t)USART_CTR3_NORXOF);
    /* Set the new value for the NORXOF detection bit */
    ptr_usart->CTR3 |= set_value;
}

/**
  * @fn bit_status_t usart_flag_status_get(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Checks whether the specified USART flag is set or not.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *          @arg USART_FLAG_RENACTF(Receive Enable acknowledge flag);
  *          @arg USART_FLAG_TENACTF(Transmit Enable acknowledge flag);
  *          @arg USART_FLAG_BRKSF(Send Break flag)  ;
  *          @arg USART_FLAG_CMF(Character match flag)  ;
  *          @arg USART_FLAG_BSYF(Busy flag);
  *          @arg USART_FLAG_ABRTF(Auto baud rate flag)  ;
  *          @arg USART_FLAG_ABRTERRF(Auto baud rate error flag);
  *          @arg USART_FLAG_ROTF(Receive overtime flag)  ;
  *          @arg USART_FLAG_CTSF(CTS flag);
  *          @arg USART_FLAG_CTSIF(CTS interrupt flag);
  *          @arg USART_FLAG_TXE (Transmit data register empty flag);
  *          @arg USART_FLAG_TCF(Transmission Complete flag);
  *          @arg USART_FLAG_RXNE(Receive data register not empty flag);
  *          @arg USART_FLAG_IDLE(Idle frame detection flag);
  *          @arg USART_FLAG_OVRERRF(Overflow Error flag);
  *          @arg USART_FLAG_NF(Noise flag);
  *          @arg USART_FLAG_FERRF(Frame Error flag);
  *          @arg USART_FLAG_PERRF(Parity Error flag);
  *          @arg USART_FLAG_LINBKF(LIN Break detection flag, only for USART1);
  *          @arg USART_FLAG_ENDBLKF(End of block flag);
  *          @arg USART_FLAG_RXMSKF(Reception mask mode flag);
  *          @arg USART_FLAG_WKUPF(Wake up flag, only for USART1).
  * @return The new state of flag (SET or RESET).
  */
bit_status_t usart_flag_status_get(usart_reg_t* ptr_usart, uint32_t flag)
{
    bit_status_t status = RESET;

    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_FLAG_CHECK(flag));
    
    if ((ptr_usart->STS & flag) != (uint16_t)RESET)
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
  * @fn void usart_flag_clear(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Clears the USARTx's pending flags.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  flag: specifies the flag to clear,it can be any combination of the following values:
  *          @arg USART_FLAG_CMF(Character match flag);
  *          @arg USART_FLAG_ROTF(Receive overtime flag);
  *          @arg USART_FLAG_CTSIF(CTS interrupt flag);
  *          @arg USART_FLAG_TCF(Transmission Complete flag);
  *          @arg USART_FLAG_IDLE(Idle frame detection flag);
  *          @arg USART_FLAG_OVRERRF(Overflow Error flag);
  *          @arg USART_FLAG_NF(Noise flag);
  *          @arg USART_FLAG_FERRF(Frame Error flag);
  *          @arg USART_FLAG_PERRF(Parity Error flag);
  *          @arg USART_FLAG_LINBKF(LIN Break detection flag, only for USART1);  
  *          @arg USART_FLAG_ENDBLKF(End of block flag);
  *          @arg USART_FLAG_WKUPF(Wake up flag, only for USART1).
  * @return None
  */
void usart_flag_clear(usart_reg_t* ptr_usart, uint32_t flag)
{
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_CLEAR_FLAG_CHECK(flag));
        
    ptr_usart->IFCLR = flag;
}

/**
  * @fn  bit_status_t usart_interrupt_status_get(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Checks whether the specified USART interrupt has occurred or not.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param  flag: specifies the USART interrupt source to check, it can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)  ;
  *           @arg USART_INT_ROT(Receive overtime interrupt)  ;
  *           @arg USART_INT_CTS(CTS interrupt);
  *           @arg USART_INT_TXE(Transmit data register empty interrupt);
  *           @arg USART_INT_TC(Transmission complete interrupt);
  *           @arg USART_INT_RXNE(Receive data register not empty interrupt);
  *           @arg USART_INT_OVRERR(Overflow Error interrupt)  ;
  *           @arg USART_INT_NE(Noise Error interrupt);
  *           @arg USART_INT_FERR(Frame Error interrupt);
  *           @arg USART_INT_PERR(Parity Error interrupt);
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, only for USART1);
  *           @arg USART_INT_ENDBLK(End of block interrupt);
  *           @arg USART_INT_WKUP(Wakeup interrupt, only for USART1).
  * @return The state of flag (SET or RESET).
  */
bit_status_t usart_interrupt_status_get(usart_reg_t* ptr_usart, uint32_t flag)
{
    uint32_t temp = 0;
    uint32_t mask = 0;
    uint32_t reg = 0;
    bit_status_t status = RESET;
    
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_GET_INT_CHECK(flag)); 
    
    /* Get the USART register index */
    reg = (((uint16_t)flag) >> 0x08);
    /* Get the interrupt position */
    mask = flag & USART_INT_MASK;
    mask = (uint32_t)0x01 << mask;
  
    if (reg == 0x01) /* The interrupt occurs in CTR1 register */
    {
        mask &= ptr_usart->CTR1;
    }
    else if (reg == 0x02) /* The interrupt occurs in CTR2 register */
    {
        mask &= ptr_usart->CTR2;
    }
    else /* The interrupt occurs in CTR3 register */
    {
            mask &= ptr_usart->CTR3;
    }
  
    temp = flag >> 0x10;
    temp = (uint32_t)0x01 << temp;
    temp &= ptr_usart->STS;
    if ((mask != (uint16_t)RESET)&&(temp != (uint16_t)RESET))
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
  * @fn void usart_interrupt_bit_clear(usart_reg_t* ptr_usart, uint32_t flag)
  * @brief  Clears the ptr_usart's interrupt pending bits.
  * @param  ptr_usart: pointer to USARTx where x can be 1, 2, 6, 7 or 8 to select USART peripheral.
  * @note   USART6 is only applicable for CS32F031 devices.
  * @note   USART7 is only applicable for CS32F031 devices.
  * @note   USART8 is only applicable for CS32F031 devices.   
  * @param   flag: specifies the interrupt pending bit to clear, it can be one of the following values:
  *           @arg USART_INT_CM(Character match interrupt)  ;
  *           @arg USART_INT_TXE(USART transmit empty)  ;
  *           @arg USART_INT_RXNE(USART receiver buffer not empty)  ;
  *           @arg USART_INT_ROT(Receive overtime interrupt)  ;
  *           @arg USART_INT_CTS(CTS interrupt);
  *           @arg USART_INT_TC(Transmission complete interrupt);
  *           @arg USART_INT_OVRERR(Overflow Error interrupt)  ;
  *           @arg USART_INT_NE(Noise Error interrupt);
  *           @arg USART_INT_FERR(Frame Error interrupt);
  *           @arg USART_INT_PERR(Parity Error interrupt);
  *           @arg USART_INT_LINBK(LIN Break detection interrupt, only for USART1);
  *           @arg USART_INT_ENDBLK(End of block interrupt);
  *           @arg USART_INT_WKUP(Wakeup interrupt, only for USART1).
  * @return None
  */
void usart_interrupt_bit_clear(usart_reg_t* ptr_usart, uint32_t flag)
{
    uint32_t temp = 0;
    uint32_t mask = 0;
  
    ASSERT(USART_SELECT_CHECK(ptr_usart));
    ASSERT(USART_CLEAR_INT_CHECK(flag)); 
  
    temp = flag >> 0x10;
    mask = ((uint32_t)0x01 << (uint32_t)temp);
    ptr_usart->IFCLR = (uint32_t)mask;
}

/**
  * @}
  */

/**
  * @}
  */

