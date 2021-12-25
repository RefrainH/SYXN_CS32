/**
  * @file    cs32f0xx_spi.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Serial peripheral interface (SPI):
  *           a. Initialization and Configuration;
  *           b. Data transfers functions;
  *           c. Hardware CRC Calculation;
  *           d. DMA transfers management;
  *           e. Interrupts and flags management;
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
        1. Enable peripheral clock using rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_SPI1, ENABLE)
            function for SPI1 or using rcu_apb1_periph_clock_enable_ctrl(RCU_APB1_PERI_SPI2, ENABLE)
            function for SPI2.
  
        2. Enable SCK, MOSI, MISO and NSS GPIO clocks using 
            rcu_ahb_periph_clock_enable_ctrl() function. 
  
        3. Peripherals  multi-function: 
            a. Connect the pin to the desired peripherals' multi-function(MF) 
                 using gpio_pin_af_config() function.
            b. Configure the desired pin in  multi-function by:
                 gpio_configStruct->GPIO_Mode = GPIO_MODE_MF.
            c. Select the type, pull-up/pull-down and output speed via 
                 GPIO_PuPd, GPIO_OType and GPIO_Speed members.
            d. Call gpio_init() function.
  
        4. Program the Polarity, Phase, First Data, Baud Rate Prescaler, Slave 
            Management, Peripheral Mode and CRC Polynomial values using the spi_init()
            function.In I2S mode, program the Mode, Standard, Data Format, MCLK 
            Output, Audio frequency and Polarity using i2s_init() function.
  
        5. Configure the FIFO threshold using spi_fifo_recv_threshold_set() to select 
            at which threshold the RXNE event is generated.
            
        6. Enable the NVIC and the corresponding interrupt using the function 
            SPI_ITConfig() if you need to use interrupt mode. 
  
        7. When using the DMA mode 
            a. Configure the DMA using dma_init() function.
            b. Active the needed channel Request using spi_i2s_dma_enable_ctrl() function.
   
        8. Enable the SPI using the spi_enable_ctrl() function or enable the I2S using
            i2s_enable_ctrl().
   
        9. Enable the DMA using the dma_enable_ctrl() function when using DMA mode. 
  
        10. Optionally, you can enable/configure the following parameters without
            re-initialization (i.e there is no need to call again spi_init() function):
            a. When bidirectional mode (SPI_DIR_1LINES_RX or SPI_DIR_1LINES_TX)
                 is programmed as Data direction parameter using the spi_init() 
                 function it can be possible to switch between SPI_TRANSMIT_DIR_TX 
                 or SPI_TRANSMIT_DIR_RX using the spi_transmit_direct_set() function.
            b. When SPI_SSM_SW is selected as Slave Select Management parameter 
                 using the spi_init() function it can be possible to manage the 
                 NSS internal signal using the spi_software_nss_config() function.
            c. Reconfigure the data size using the spi_data_width_set() function.
            d. Enable or disable the SS output using the spi_nss_output_enable_ctrl() function.  
  
        11. To use the CRC Hardware calculation feature refer to the Peripheral 
            CRC hardware Calculation subsection.
  
    @endverbatim 
  *
  */

#include "cs32f0xx_spi.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */


/* SPI registers Masks */
#define SPI_CTR1_CLEAR_MASK  ((uint16_t)0x3040)
#define CR1_CLEAR_MASK2      ((uint16_t)0xFFFB)
#define CR2_LDMA_MASK        ((uint16_t)0x9FFF)

#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)



/**
  * @fn void spi_i2s_def_init(spi_reg_t* ptr_spi)
  * @brief  Deinitializes the SPIx peripheral registers to their default reset values.      
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @note   I2S mode is only applicable for SPI1.   
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @return None
  */
void spi_i2s_def_init(spi_reg_t* ptr_spi)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));

    if (ptr_spi == SPI1)
    {
        /* Reset SPI1 */
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_SPI1, ENABLE);
        /* Release SPI1 from reset */
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_SPI1, DISABLE);
    }
    else if (ptr_spi == SPI2)
    {
        /* Reset SPI2 */
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_SPI2, ENABLE);
        /* Release SPI2 from reset */
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_SPI2, DISABLE);
    }
}

/**
  * @fn void spi_config_struct_init(spi_config_t* ptr_config)
  * @brief  Fills each spi_config_t member with its default value.
  * @param  ptr_config: pointer to a spi_config_t structure which will be initialized.
  * @return None
  */
void spi_config_struct_init(spi_config_t* ptr_config)
{
    ptr_config->spi_direct = SPI_DIR_2LINES_FULL_DUPLEX;
    ptr_config->spi_mode = SPI_MODE_SLAVE;
    ptr_config->data_width = SPI_DATA_WIDTH_8BIT;
    
    ptr_config->spi_cpol = SPI_CPOL_LOW;
    ptr_config->spi_cpha = SPI_CPHA_1EDGE;
    ptr_config->spi_nss = SPI_SSM_HW;
  
    ptr_config->spi_predivid = SPI_BAUD_RATE_PDIV_2;
    ptr_config->first_bit = SPI_FIRST_BIT_MSB;
    ptr_config->crc_polynomial = 7;
}

/**
  * @fn void spi_init(spi_reg_t* ptr_spi, spi_config_t* ptr_config)
  * @brief  Initializes the SPIx peripheral according to the ptr_config.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  ptr_config: pointer to a spi_config_t structure that
  *         contains the configuration for the specified SPI peripheral.
  * @return None
  */
void spi_init(spi_reg_t* ptr_spi, spi_config_t* ptr_config)
{
    uint16_t reg = 0;

    ASSERT(SPI_LIST_CHECK(ptr_spi));

    ASSERT(SPI_DIR_LINES_CHECK(ptr_config->spi_direct));
    ASSERT(SPI_MODE_CHECK(ptr_config->spi_mode));
    ASSERT(IS_SPI_DATA_SIZE(ptr_config->data_width));
    ASSERT(SPI_CPOL_CHECK(ptr_config->spi_cpol));
    ASSERT(SPI_CPHA_CHECK(ptr_config->spi_cpha));
    ASSERT(SPI_SSM_CHECK(ptr_config->spi_nss));
    ASSERT(SPI_BAUD_RATE_PDIV_CHECK(ptr_config->spi_predivid));
    ASSERT(SPI_FIRST_BIT_CHECK(ptr_config->first_bit));
    ASSERT(SPI_CRC_POLYNOMIAL_CHECK(ptr_config->crc_polynomial));

    reg = ptr_spi->CTR1;
  
    reg &= SPI_CTR1_CLEAR_MASK;
    reg |= (uint16_t)((uint32_t)ptr_config->spi_direct | ptr_config->first_bit |
                      ptr_config->spi_cpol | ptr_config->spi_cpha |
                      ptr_config->spi_nss | ptr_config->spi_predivid);  
  
    ptr_spi->CTR1 = reg;
    reg = ptr_spi->CTR2;  
    reg &=(uint16_t)~SPI_CTR2_DLEN;
  
    reg |= (uint16_t)(ptr_config->data_width);
    ptr_spi->CTR2 = reg; 
    ptr_spi->CRCPOLYR = ptr_config->crc_polynomial;
    
    reg = ptr_spi->CTR1;  
    reg &= CR1_CLEAR_MASK2;  
    reg |= (uint16_t)((uint32_t)ptr_config->spi_mode);  
  
    ptr_spi->CTR1 = reg;    
    ptr_spi->I2SCTR &= (uint16_t)~((uint16_t)SPI_I2SCTR_I2SMS);
}

/**
  * @fn void i2s_config_struct_init(i2s_config_t* ptr_i2s_config)
  * @brief  Fills each I2S_InitStruct member with its default value.
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @param  ptr_i2s_config: pointer to a i2s_config_t structure which will be initialized.
  * @return None
  */
void i2s_config_struct_init(i2s_config_t* ptr_i2s_config)
{
    ptr_i2s_config->i2s_mode = I2S_MODE_SLAVE_TX;

    ptr_i2s_config->i2s_standard = I2S_STANDARD_HP;

    ptr_i2s_config->i2s_data_len = I2S_DATA_LEN_16BIT;

    ptr_i2s_config->i2s_mck_output = I2S_MCK_OUTPUT_DISABLE;
    
    ptr_i2s_config->audio_freq = I2S_AUDIO_FREQ_DEFAULT;

    ptr_i2s_config->i2s_cpol = I2S_CPOL_LOW;
}

/**
  * @fn void i2s_init(spi_reg_t* ptr_spi, i2s_config_t* ptr_i2s_config)
  * @brief  Initializes the SPIx peripheral according to the specified parameters
  *         in the ptr_i2s_config.
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @param  ptr_spi: pointer to SPIx (x can be 1) peripheral (configured in I2S mode).
  * @param  ptr_i2s_config: pointer to an i2s_config_t structure that contains the configuration
  *         information for the specified SPI peripheral configured in I2S mode.
  * @return None
  */
void i2s_init(spi_reg_t* ptr_spi, i2s_config_t* ptr_i2s_config)
{
    rcu_clock_t rcu_clock;
    uint16_t reg = 0;
    uint16_t div = 2;
    uint16_t odd = 0;  
    uint16_t packet = 1;  
    uint32_t clock_freq = 0;
    uint32_t temp = 0;

    ASSERT(SPI_SPI1_CHECK(ptr_spi));
    ASSERT(I2S_MODE_CHECK(ptr_i2s_config->i2s_mode));
    ASSERT(I2S_STANDARD_CHECK(ptr_i2s_config->i2s_standard));
    ASSERT(I2S_DATA_LEN_CHECK(ptr_i2s_config->i2s_data_len));
    ASSERT(I2S_MCK_OUTPUT_CHECK(ptr_i2s_config->i2s_mck_output));
    ASSERT(IS_I2S_AUDIO_FREQ(ptr_i2s_config->audio_freq));
    ASSERT(I2S_CPOL_CHECK(ptr_i2s_config->i2s_cpol));  

    /* Clear I2SMS, I2SEN, I2SOM, PCMLONG, I2SSTD, CKPIS, I2SDL and I2SCL bits */
    ptr_spi->I2SCTR &= I2SCFGR_CLEAR_Mask; 
    ptr_spi->I2SPDIV = 0x0002;

    reg = ptr_spi->I2SCTR;

    if(ptr_i2s_config->audio_freq == I2S_AUDIO_FREQ_DEFAULT)
    {
        odd = (uint16_t)0;
        div = (uint16_t)2;   
    }
    else
    {
        if(ptr_i2s_config->i2s_data_len == I2S_DATA_LEN_16BIT)
        {
            /* Packet length is 16 bits */
            packet = 1;
        }
        else
        {
            /* Packet length is 32 bits */
            packet = 2;
        }

        rcu_clk_freq_get(&rcu_clock);      
        clock_freq = rcu_clock.system_clk_freq;    

        if(ptr_i2s_config->i2s_mck_output == I2S_MCK_OUTPUT_ENABLE)
        {
            /* MCK output is enabled */
            temp = (uint16_t)(((((clock_freq / 256) * 10) / ptr_i2s_config->audio_freq)) + 5);
        }
        else
        {
            /* MCLK output is disabled */
            temp = (uint16_t)(((((clock_freq / (32 * packet)) *10 ) / ptr_i2s_config->audio_freq)) + 5);
        }
    
        temp = temp / 10;

        /* Check the parity of the divider */
        odd = (uint16_t)(temp & (uint16_t)0x0001);
        div = (uint16_t)((temp - odd) / 2);

        /* Get the Mask for the Odd bit (SPI_I2SPDIV[8]) register */
        odd = (uint16_t) (odd << 8);
    }
    
    if ((div < 2) || (div > 0xFF))
    {
        /* Set the default values */
        div = 2;
        odd = 0;
    }

    ptr_spi->I2SPDIV = (uint16_t)(div | (uint16_t)(odd | (uint16_t)ptr_i2s_config->i2s_mck_output));

    /* Configure the I2S */
    reg |= (uint16_t)(SPI_I2SCTR_I2SMS | (uint16_t)(ptr_i2s_config->i2s_mode | (uint16_t)(ptr_i2s_config->i2s_standard \
                  | (uint16_t)(ptr_i2s_config->i2s_data_len | (uint16_t)ptr_i2s_config->i2s_cpol))));                  
    ptr_spi->I2SCTR = reg;
}

/**
  * @fn void spi_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  enable_flag: ENABLE or DISABLE the SPIx peripheral. 
  * @return None
  */
void spi_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable SPI */
        ptr_spi->CTR1 |= SPI_CTR1_SPIEN;
    }
    else
    {
        /* Disable SPI */
        ptr_spi->CTR1 &= (uint16_t)~((uint16_t)SPI_CTR1_SPIEN);
    }
}

/**
  * @fn void spi_ti_mode_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the TI Mode.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the selected SPI TI communication mode.
  * @return None
  */
void spi_ti_mode_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the TI mode */
        ptr_spi->CTR2 |= SPI_CTR2_TIEN;
    }
    else
    {
        /* Disable the TI mode */
        ptr_spi->CTR2 &= (uint16_t)~((uint16_t)SPI_CTR2_TIEN);
    }
}

/**
  * @fn void i2s_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the specified SPI peripheral (in I2S mode).
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @param  ptr_spi: pointer to SPIx (x can be 1) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx peripheral. 
  * @return None
  */
void i2s_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_SPI1_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        /* Enable SPI using in I2S mode */
        ptr_spi->I2SCTR |= SPI_I2SCTR_I2SEN;
    }
    else
    {
        /* Disable SPI using in in I2S mode */
        ptr_spi->I2SCTR &= (uint16_t)~((uint16_t)SPI_I2SCTR_I2SEN);
    }
}

/**
  * @fn void spi_data_width_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the data width for the selected SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the SPI data size, it can be one of the following values:
  *          @arg SPI_DATA_WIDTH_4BIT    
  *          @arg SPI_DATA_WIDTH_5BIT
  *          @arg SPI_DATA_WIDTH_6BIT     
  *          @arg SPI_DATA_WIDTH_7BIT
  *          @arg SPI_DATA_WIDTH_8BIT     
  *          @arg SPI_DATA_WIDTH_9BIT
  *          @arg SPI_DATA_WIDTH_10BIT 
  *          @arg SPI_DATA_WIDTH_11BIT
  *          @arg SPI_DATA_WIDTH_12BIT  
  *          @arg SPI_DATA_WIDTH_13BIT
  *          @arg SPI_DATA_WIDTH_14BIT  
  *          @arg SPI_DATA_WIDTH_15BIT
  *          @arg SPI_DATA_WIDTH_16BIT
  * @return None
  */
void spi_data_width_set(spi_reg_t* ptr_spi, uint16_t value)
{
    uint16_t reg = 0;
  
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(IS_SPI_DATA_SIZE(value));
  
    reg = ptr_spi->CTR2;  
    reg &= (uint16_t)~SPI_CTR2_DLEN;
  
    reg |= value;
    ptr_spi->CTR2 = reg;
}

/**
  * @fn void spi_fifo_recv_threshold_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the FIFO reception threshold for the selected SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  value: specifies the FIFO reception threshold, it can be one of the following values:
  *          @arg SPI_FIFO_RX_THRESHOLD_HF (event is generated if the FIFO level is greater or equal to 1/2)        
  *          @arg SPI_FIFO_RX_THRESHOLD_QF (event is generated if the FIFO  level is greater or equal to 1/4)                           
  * @return None
  */
void spi_fifo_recv_threshold_set(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_FIFO_RX_THRESHOLD_CHECK(value));

    /* Clear RXNE8 bit */
    ptr_spi->CTR2 &= (uint16_t)~((uint16_t)SPI_CTR2_RXNE8);
    ptr_spi->CTR2 |= value;
}

/**
  * @fn void spi_transmit_direct_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  config the data transfer direction.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the data transfer direction. It can be one of the
  *         following values:
  *          @arg SPI_TRANSMIT_DIR_TX
  *          @arg SPI_TRANSMIT_DIR_RX
  * @return None
  */
void spi_transmit_direct_set(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_TRANSMIT_DIR_CHECK(value));
  
    if (value == SPI_TRANSMIT_DIR_TX)
    {
        ptr_spi->CTR1 |= SPI_TRANSMIT_DIR_TX;
    }
    else
    {
        ptr_spi->CTR1 &= SPI_TRANSMIT_DIR_RX;
    }
}

/**
  * @fn void spi_software_nss_config(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures NSS value in software NSS mode. It can be called
  *         only after the spi_init() function has  been called.     
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the SPI NSS internal state, it can be:
  *          @arg SPI_SOFTWARE_NSS_SET (Set NSS pin internally) 
  *          @arg SPI_SOFTWARE_NSS_RESET (Reset NSS pin internally)
  * @return None
  */
void spi_software_nss_config(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_SOFTWARE_NSS_CHECK(value));

    if (value != SPI_SOFTWARE_NSS_RESET)
    {
        /* Set NSS pin */
        ptr_spi->CTR1 |= SPI_SOFTWARE_NSS_SET;
    }
    else
    {
        /* Reset NSS  */
        ptr_spi->CTR1 &= SPI_SOFTWARE_NSS_RESET;
    }
}

/**
  * @fn void spi_nss_output_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the SS output , It can be called only after the spi_init() 
  *         has been called and the NSS hardware management mode is selected. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx SS output. 
  * @return None
  */
void spi_nss_output_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the selected SPI SS output */
        ptr_spi->CTR2 |= SPI_CTR2_NSSOEN;
    }
    else
    {
        /* Disable the selected SPI SS output */
        ptr_spi->CTR2 &= (uint16_t)~((uint16_t)SPI_CTR2_NSSOEN);
    }
}   

/**
  * @fn void spi_nss_pulse_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the NSS pulse  mode, and It can be called only
  *         after the spi_init() function has been called.When TI mode is selected,
  *         the control bits NSSP is not taken into consideration and are configured
  *         by hardware respectively to the TI mode requirements.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  enable_flag: ENABLE or DISABLE the NSS pulse management mode.
  * @return None
  */
void spi_nss_pulse_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable the NSS pulse mode */
        ptr_spi->CTR2 |= SPI_CTR2_NSSPM;
    }
    else
    {
        /* Disable the NSS pulse mode */
        ptr_spi->CTR2 &= (uint16_t)~((uint16_t)SPI_CTR2_NSSPM);    
    }
}

/**
  * @fn void spi_8bits_data_send(spi_reg_t* ptr_spi, uint8_t value)
  * @brief  Transmits a byte data through the SPIx/I2Sx.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: value to be transmitted.
  * @return None
  */
void spi_8bits_data_send(spi_reg_t* ptr_spi, uint8_t value)
{
    uint32_t address = 0x00;

    ASSERT(SPI_LIST_CHECK(ptr_spi));

    address = (uint32_t)ptr_spi; 
    address += 0x0C;
  
    *(__IO uint8_t *) address = value;
}

/**
  * @fn void spi_i2s_16bits_data_send(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Transmits a 16bits data through the SPIx/I2Sx.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: Data to be transmitted.
  * @return None
  */
void spi_i2s_16bits_data_send(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
  
    ptr_spi->DATA = (uint16_t)value;
}

/**
  * @fn uint8_t spi_8bits_data_receive(spi_reg_t* ptr_spi)
  * @brief  Returns received a 8bits data by the SPIx/I2Sx. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @return The value of the received data.
  */
uint8_t spi_8bits_data_receive(spi_reg_t* ptr_spi)
{
    uint32_t reg_addr = 0x00;
  
    reg_addr = (uint32_t)ptr_spi; 
    reg_addr += 0x0C;
  
    return *(__IO uint8_t *) reg_addr;
}

/**
  * @fn uint16_t spi_i2s_16bits_data_receive(spi_reg_t* ptr_spi)
  * @brief  Returns received a 16bits data by the SPIx/I2Sx. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @return The value of the received data.
  */
uint16_t spi_i2s_16bits_data_receive(spi_reg_t* ptr_spi)
{
    return ptr_spi->DATA;
}

/**
  * @fn void spi_crc_length_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the CRC calculation length. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the SPI CRC calculation length.It can be 
  *          @arg SPI_CRC_LEN_8BIT (Set CRC Calculation to 8 bits)
  *          @arg SPI_CRC_LEN_16BIT (Set CRC Calculation to 16 bits)
  * @return None
  */
void spi_crc_length_set(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_CRC_LEN_CHECK(value));
  
    /* Clear CRC16 bit */
    ptr_spi->CTR1 &= (uint16_t)~((uint16_t)SPI_CTR1_CRC16);
    /* Set CRC16 bit */
    ptr_spi->CTR1 |= value;
}

/**
  * @fn void spi_crc_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the CRC value calculation of the transferred bytes. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx CRC value calculation.
  * @return None
  */
void spi_crc_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        /* Enable the SPI CRC calculation */
        ptr_spi->CTR1 |= SPI_CTR1_CRCEN;
    }
    else
    {
        /* Disable the SPI CRC calculation */
        ptr_spi->CTR1 &= (uint16_t)~((uint16_t)SPI_CTR1_CRCEN);
    }
}

/**
  * @fn void spi_crc_transmit(spi_reg_t* ptr_spi)
  * @brief  Transmit the SPIx CRC value.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @return None
  */
void spi_crc_transmit(spi_reg_t* ptr_spi)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));

    /* Enable SPI CRC transmission */
    ptr_spi->CTR1 |= SPI_CTR1_NXTCRC;
}

/**
  * @fn uint16_t spi_crc_value_get(spi_reg_t* ptr_spi, uint8_t direct)
  * @brief Returns the transmit or the receive CRC register value for the specified SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  direct: specifies the CRC register to be read. It can be
  *          @arg SPI_CRC_DIR_TX (Selects Tx CRC register) 
  *          @arg SPI_CRC_DIR_RX (Selects Rx CRC register)
  * @return The selected CRC register value.
  */
uint16_t spi_crc_value_get(spi_reg_t* ptr_spi, uint8_t direct)
{
    uint16_t reg_value = 0;
  
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_CRC_DIR_CHECK(direct));
    
    if (direct != SPI_CRC_DIR_RX)
    {
        /* Get the Tx CRC value */
        reg_value = ptr_spi->TCRC;
    }
    else
    {
        /* Get the Rx CRC value */
        reg_value = ptr_spi->RCRC;
    }
  
    return reg_value;
}

/**
  * @fn uint16_t spi_crc_polynomial_get(spi_reg_t* ptr_spi)
  * @brief  Returns the CRC Polynomial register value for the specified SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @return The CRC Polynomial register value.
  */
uint16_t spi_crc_polynomial_get(spi_reg_t* ptr_spi)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));

    return ptr_spi->CRCPOLYR;
}

/**
  * @fn void spi_i2s_dma_enable_ctrl(spi_reg_t* ptr_spi, uint16_t value, enable_state_t enable_flag)
  * @brief  Enable or disable the SPIx/I2Sx DMA interface.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: specifies the SPI DMA transfer request to be enabled or disabled, it can be: 
  *          @arg SPI_I2S_DMA_ENABLE_TX (Tx buffer DMA transfer request)
  *          @arg SPI_I2S_DMA_ENABLE_RX (Rx buffer DMA transfer request)
  * @param  enable_flag: ENABLE or DISABLE the selected SPI DMA transfer request.
  * @return None
  */
void spi_i2s_dma_enable_ctrl(spi_reg_t* ptr_spi, uint16_t value, enable_state_t enable_flag)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    ASSERT(SPI_I2S_DMA_ENABLE_CHECK(value));
    
    if (enable_flag != DISABLE)
    {
        /* Enable the SPI DMA requests */
        ptr_spi->CTR2 |= value;
    }
    else
    {
        /* Disable the SPI DMA requests */
        ptr_spi->CTR2 &= (uint16_t)~value;
    }
}

/**
  * @fn void spi_dma_odd_even_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the number of data Even/Odd for the DMA transfers.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the SPI last DMA transfers state, it can be:
  *          @arg SPI_DMA_TX_EVEN_RX_EVEN (TX and RX transmission data Number are all Even)                                                   .
  *          @arg SPI_DMA_TX_ODD_RX_EVEN (TX data number is Odd, RX data number is Even)
  *          @arg SPI_DMA_TX_EVEN_RX_ODD (TX data number is Even, RX data number is Odd)
  *          @arg SPI_DMA_TX_ODD_RX_ODD (TX data number is Odd, RX data number is Odd)
  * @return None
  */
void spi_dma_odd_even_set(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_DMA_TX_RX_NUMBER_CHECK(value));

    /* Clear DMATXODD and DMARXODD bits */
    ptr_spi->CTR2 &= CR2_LDMA_MASK;

    /* Set new DMATXODD and DMARXODD bits value */
    ptr_spi->CTR2 |= value; 
}

/**
  * @fn void spi_i2s_interrupt_set(spi_reg_t* ptr_spi, uint8_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the SPI/I2S interrupts.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  set_value: specifies the SPI interrupt source to be enabled or disabled. It can be:
  *          @arg SPI_I2S_INTR_SET_TXE(Tx buffer empty interrupt mask)        
  *          @arg SPI_I2S_INTR_SET_RXNE (Rx buffer not empty interrupt mask)
  *          @arg SPI_I2S_INTR_SET_ERR (Error interrupt mask)
  * @param  enable_flag: ENABLE or DISABLE the specified SPI interrupt.
  * @return None
  */
void spi_i2s_interrupt_set(spi_reg_t* ptr_spi, uint8_t set_value, enable_state_t enable_flag)
{
    uint16_t itpos = 0, itmask = 0 ;

    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    ASSERT(SPI_I2S_INTR_SET_CHECK(set_value));

    /* Get the SPI INTR index */
    itpos = set_value >> 4;
    
    /* Set the INTR mask */
    itmask = (uint16_t)((uint16_t)1 << (uint16_t)itpos);

    if (enable_flag != DISABLE)
    {
        /* Enable the selected SPI interrupt */
        ptr_spi->CTR2 |= itmask;
    }
    else
    {
        /* Disable the selected SPI interrupt */
        ptr_spi->CTR2 &= (uint16_t)~itmask;
    }
}

/**
  * @fn uint16_t spi_transfer_fifo_status_get(spi_reg_t* ptr_spi)
  * @brief  Get the current SPIx Transmission FIFO filled level.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @return The Transmission FIFO filling state.
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_EMPTY (FIFO is empty)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_1_4_FULL (more than 1/4 full)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_1_2_FULL (more than 1/2 full)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_FULL ( FIFO is full)
  */
uint16_t spi_transfer_fifo_status_get(spi_reg_t* ptr_spi)
{
    return (uint16_t)((ptr_spi->STS & SPI_STS_TXFIFOS));
}

/**
  * @fn uint16_t spi_receive_fifo_status_get(spi_reg_t* ptr_spi)
  * @brief  Returns the current SPIx Reception FIFO filled level.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @return The Reception FIFO filling state.
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_EMPTY (FIFO is empty)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_1_4_FULL (more than 1/4 full)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_1_2_FULL (more than 1/2 full)
  *          @arg SPI_TX_FIFO_LEVEL_STATUS_FULL ( FIFO is full)
  */
uint16_t spi_receive_fifo_status_get(spi_reg_t* ptr_spi)
{
    return (uint16_t)((ptr_spi->STS & SPI_STS_RXFIFOS));
}

/**
  * @fn bit_status_t spi_i2s_flag_status_get(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: specifies the SPI flag to check. 
  *         It can be one of the following values:
  *          @arg SPI_I2S_FLAG_TXE (Transmit buffer empty flag)
  *          @arg SPI_I2S_FLAG_RXNE (Receive buffer not empty flag)
  *          @arg SPI_I2S_FLAG_BUSY (Busy flag)
  *          @arg SPI_I2S_FLAG_RXOFERR (Receiver data overflow error)
  *          @arg SPI_I2S_FLAG_MMERR (Multi-master error)
  *          @arg SPI_I2S_FLAG_CRCERR (CRC Error flag)
  *          @arg SPI_I2S_FLAG_NWERR (Error of SPI NSS or I2S WS)
  *          @arg SPI_I2S_FLAG_TXUFERR (Transmitter data underflow error)
  *          @arg SPI_I2S_FLAG_I2SCSF (I2S Channel side flag)   
  * @return  state SET or RESET.
  */
bit_status_t spi_i2s_flag_status_get(spi_reg_t* ptr_spi, uint16_t value)
{
    bit_status_t status = RESET;
  
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_I2S_GET_FLAG_CHECK(value));

    /* Check the status of the specified SPI flag */
    if ((ptr_spi->STS & value) != (uint16_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    
    return  status;
}

/**
  * @fn void spi_i2s_flag_clear(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: specifies the SPI flag to clear,only CRCERR flag.
  * @return None
  */
void spi_i2s_flag_clear(spi_reg_t* ptr_spi, uint16_t value)
{
    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_I2S_CLEAR_FLAG_CHECK(value));

    /* Clear SPI CRCERR flag */
    ptr_spi->STS = (uint16_t)~value;
}

/**
  * @fn bit_status_t spi_i2s_interrupt_status_get(spi_reg_t* ptr_spi, uint8_t value)
  * @brief  Checks whether the specified SPI/I2S interrupt has occurred or not.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: specifies the SPI interrupt source to check. It can be: 
  *          @arg SPI_I2S_INTR_SET_TXE (Tx buffer empty interrupt)          
  *          @arg SPI_I2S_INTR_SET_RXNE (Rx buffer not empty interrupt)
  *          @arg SPI_I2S_INTR_MMERR (Multi-master error interrupt)
  *          @arg SPI_I2S_INTR_RXOFERR (Receiver data overflow error interrupt)
  *          @arg SPI_I2S_INTR_TXUFERR (Transmitter data underflow error interrupt) 
  *          @arg SPI_I2S_INTR_NWERR (Error of SPI NSS or I2S WS interrupt) 
  * @return The  state SET or RESET.
  */
bit_status_t spi_i2s_interrupt_status_get(spi_reg_t* ptr_spi, uint8_t value)
{
    bit_status_t status = RESET;
    uint16_t temp  = 0;
    uint16_t mask = 0;
    uint16_t flag = 0;

    ASSERT(SPI_LIST_CHECK(ptr_spi));
    ASSERT(SPI_I2S_GET_INTR_CHECK(value));

    /* Get the SPI/I2S INTR index */
    temp = (uint16_t)(0x01 << (value & 0x0F));

    /* Get the SPI/I2S INTR mask */
    mask = value >> 4;

    /* Set the INTR mask */
    mask = (uint16_t)(0x01 << mask);

    /* Get the SPI/I2S INTR enable bit status */
    flag = (ptr_spi->CTR2 & mask) ;

    /* Check the status of the specified SPI interrupt */
    if (((ptr_spi->STS & temp) != (uint16_t)RESET) && flag)
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
  * @}
  */ 

/**
  * @}
  */

