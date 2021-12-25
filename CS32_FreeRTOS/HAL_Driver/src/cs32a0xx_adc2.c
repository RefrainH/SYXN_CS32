/**
  * @file    cs32a0xx_adc2.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Analog to Digital Convertor (ADC) peripheral:
  *           + Initialization and Configuration
  *           + Power saving
  *           + Analog Watchdog configuration
  *           + Temperature Sensor, Vrefint (Internal Reference Voltage) and 
  *             Vbat (Voltage battery) management 
  *           + ADC Channels Configuration
  *           + ADC Channels DMA Configuration
  *           + Interrupts and flags management
  *
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "cs32a0xx_adc2.h"
#include "cs32f0xx_rcu.h"
#include "cs32f0xx_spi.h"
#include "cs32f0xx_gpio.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup ADC2
  * @{
  */
  
#define SPI_ADC2_CS_LOW()        gpio_bits_reset(GPIOB, GPIO_PIN_3)
#define SPI_ADC2_CS_HIGH()       gpio_bits_set(GPIOB, GPIO_PIN_3)

/**
  * @fn void adc2_init(void)
  * @brief  Deinitializes ADC1 peripheral registers to their default reset values.
  * @param  ptr_adc: pointer to ADCx where x can be 1.
  * @return None
  */
void adc2_init(void)
{
    gpio_config_t  gpio_config_struct; 
	spi_config_t   spi_config_struct;  

	spi_i2s_def_init(SPI2);

	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB, ENABLE);  
	rcu_apb1_periph_clock_enable_ctrl(RCU_APB1_PERI_SPI2, ENABLE);
    
    gpio_config_struct.gpio_pin = GPIO_PIN_3;  //SPI_CS
	gpio_config_struct.gpio_mode = GPIO_MODE_OUTPUT;  
	gpio_config_struct.gpio_speed = GPIO_SPEED_LOW;
	gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
	gpio_config_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOB, &gpio_config_struct);	
    
    gpio_config_struct.gpio_pin = GPIO_PIN_13|GPIO_PIN_15; //PB13 PB15
    gpio_config_struct.gpio_mode = GPIO_MODE_MF;
    gpio_config_struct.gpio_speed = GPIO_SPEED_LOW;
    gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
    gpio_config_struct.gpio_pull = GPIO_PULL_UP; 
    gpio_init(GPIOB, &gpio_config_struct);    
			
    gpio_mf_config(GPIOB, GPIO_PIN_NUM13, GPIO_MF_SEL0); //SPI_SCL
    gpio_mf_config(GPIOB, GPIO_PIN_NUM15, GPIO_MF_SEL0); //SPI_SDA

    spi_config_struct.spi_direct = SPI_DIR_1LINES_TX;
	spi_config_struct.spi_mode = SPI_MODE_MASTER;
	spi_config_struct.data_width = SPI_DATA_WIDTH_8BIT;
	spi_config_struct.spi_cpol = SPI_CPOL_LOW;
	spi_config_struct.spi_cpha = SPI_CPHA_2EDGE;
	spi_config_struct.spi_nss = SPI_SSM_SW;
	spi_config_struct.spi_predivid = SPI_BAUD_RATE_PDIV_32;
	spi_config_struct.first_bit = SPI_FIRST_BIT_MSB;
	spi_config_struct.crc_polynomial = 7;
	spi_init(SPI2, &spi_config_struct);

	spi_enable_ctrl(SPI2, ENABLE);
    SPI_ADC2_CS_HIGH();
}

/**
  * @fn int32_t adc2_io_read(uint8_t addr, uint8_t* buf)
  * @brief  Read the sigma-delta ADC register value with address addr and store its value in the buf.
  * @param  addr:8-bit sigma-delta ADC register address value
  * @param  buf: 8-bit sigma-delta ADC register address value output value
  * @return 0:OK 
  */
int32_t adc2_io_read(uint8_t addr, uint8_t* buf)
{
    SPI_ADC2_CS_LOW();
    
    while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    spi_8bits_data_send(SPI2, addr&0x7F);
    
    while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    while (spi_i2s_flag_status_get(SPI2, SPI_STS_BUSY) != RESET);
    
    spi_transmit_direct_set(SPI2, SPI_TRANSMIT_DIR_RX);
    
    if (addr == ADC2_ADO)
    {
        while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
        buf[0] = (uint8_t)spi_8bits_data_receive(SPI2);
        while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
        buf[1] = (uint8_t)spi_8bits_data_receive(SPI2);
        while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
        buf[2] = (uint8_t)spi_i2s_16bits_data_receive(SPI2);
    }
    else {
        while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
        buf[0] = (uint8_t)spi_i2s_16bits_data_receive(SPI2);
    }
        
    SPI_ADC2_CS_HIGH();
    spi_enable_ctrl(SPI2, DISABLE);
    spi_transmit_direct_set(SPI2, SPI_TRANSMIT_DIR_TX);
    spi_enable_ctrl(SPI2, ENABLE);
    while (spi_i2s_flag_status_get(SPI2, SPI_STS_BUSY) != RESET);
    return 0;
}

/**
  * @fn int32_t adc2_io_write(uint8_t addr, uint8_t value)
  * @brief  Write the sigma-delta ADC register value with address addr
  * @param  addr:8-bit sigma-delta ADC register address value
  * @param  value: 8-bit sigma-delta ADC register address value output value
  * @return 0:OK 
  */
int32_t adc2_io_write(uint8_t addr, uint8_t value)
{
    SPI_ADC2_CS_LOW();
    while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    spi_8bits_data_send(SPI2, addr|0x80);
    while (spi_i2s_flag_status_get(SPI2, SPI_I2S_FLAG_TXE) == RESET);
    spi_8bits_data_send(SPI2, value);
    while (spi_i2s_flag_status_get(SPI2, SPI_STS_BUSY) != RESET);
    
//    ndelay(0x2FF);
    SPI_ADC2_CS_HIGH();
    return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

