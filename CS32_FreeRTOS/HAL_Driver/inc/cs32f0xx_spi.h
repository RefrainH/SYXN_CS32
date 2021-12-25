/**
  * @file    cs32f0xx_spi.h
  * @brief   This file contains all the functions prototypes for the SPI  firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_SPI_H__
#define __CS32F0XX_SPI_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */


/** 
  * @struct spi_config_t
  * @brief  SPI Init structure definition  
  */
typedef struct
{
    uint16_t spi_direct;           //!< Specifies the SPI unidirectional or bidirectional data mode
    uint16_t spi_mode;             //!< Specifies the SPI mode (Master/Slave)
    uint16_t data_width;           //!< Specifies the SPI data size
    uint16_t spi_cpol;             //!< Specifies the serial clock steady state
    uint16_t spi_cpha;             //!< Specifies the clock active edge for the bit capture
    uint16_t spi_nss;              //!< Specifies whether the NSS signal is managed by NSS pin
    uint16_t spi_predivid;         //!< Specifies the Baud Rate prescaler value       
    uint16_t first_bit;            //!< Specifies whether data transfers start from MSB or LSB bit
    uint16_t crc_polynomial;       //!< Specifies the polynomial used for the CRC calculation 
}spi_config_t;


/** 
  * @struct i2s_config_t
  * @brief  I2S Init structure definition (not  for CS32F030, CS32F036 devices)
  */
typedef struct
{
    uint16_t i2s_mode;         //!< Specifies the I2S operating mode
    uint16_t i2s_standard;     //!< Specifies the standard used for the I2S communication
    uint16_t i2s_data_len;     //!< Specifies I2S data length
    uint16_t i2s_mck_output;   //!< Specifies whether the I2S MCLK output is enabled or not
    uint32_t audio_freq;       //!< Specifies the frequency selected for the I2S communication
    uint16_t i2s_cpol;         //!< Specifies the idle state of the I2S clock
    uint16_t resrved0;
}i2s_config_t;



/** 
   * @brief Check SPI definition  
  */  
#define SPI_SPI1_CHECK(bus) (((bus) == SPI1))
#define SPI_LIST_CHECK(bus) (((bus) == SPI1) || \
                             ((bus) == SPI2))
                                                                     
/** 
  * @brief  SPI direction and lines definition
  */
#define SPI_DIR_2LINES_FULL_DUPLEX ((uint16_t)0x0000) //!< SPI 2 lines full duplex
#define SPI_DIR_2LINES_ONLY_RX     ((uint16_t)0x0400) //!< SPI 2 lines read only 
#define SPI_DIR_1LINES_RX          ((uint16_t)0x8000) //!< SPI 1 line read 
#define SPI_DIR_1LINES_TX          ((uint16_t)0xC000) //!< SPI 1 line Write
#define SPI_DIR_LINES_CHECK(value) (((value) == SPI_DIR_2LINES_FULL_DUPLEX) || \
                                    ((value) == SPI_DIR_2LINES_ONLY_RX) || \
                                    ((value) == SPI_DIR_1LINES_RX) || \
                                    ((value) == SPI_DIR_1LINES_TX))  
/** 
  * @brief SPI mode definition
  */
#define SPI_MODE_MASTER         ((uint16_t)0x0104) //!< SPI master mode 
#define SPI_MODE_SLAVE          ((uint16_t)0x0000) //!< SPI slave mode 
#define SPI_MODE_CHECK(mode) (((mode) == SPI_MODE_MASTER) || \
                              ((mode) == SPI_MODE_SLAVE))
                           
/** 
  * @brief SPI data width definition
  */
#define SPI_DATA_WIDTH_4BIT                 ((uint16_t)0x0300) //!< Specifies data width is 4 bits
#define SPI_DATA_WIDTH_5BIT                 ((uint16_t)0x0400) //!< Specifies data width is 5 bits
#define SPI_DATA_WIDTH_6BIT                 ((uint16_t)0x0500) //!< Specifies data width is 6 bits
#define SPI_DATA_WIDTH_7BIT                 ((uint16_t)0x0600) //!< Specifies data width is 7 bits
#define SPI_DATA_WIDTH_8BIT                 ((uint16_t)0x0700) //!< Specifies data width is 8 bits
#define SPI_DATA_WIDTH_9BIT                 ((uint16_t)0x0800) //!< Specifies data width is 9 bits
#define SPI_DATA_WIDTH_10BIT                ((uint16_t)0x0900) //!< Specifies data width is 10 bits
#define SPI_DATA_WIDTH_11BIT                ((uint16_t)0x0A00) //!< Specifies data width is 11 bits
#define SPI_DATA_WIDTH_12BIT                ((uint16_t)0x0B00) //!< Specifies data width is 12 bits
#define SPI_DATA_WIDTH_13BIT                ((uint16_t)0x0C00) //!< Specifies data width is 13 bits
#define SPI_DATA_WIDTH_14BIT                ((uint16_t)0x0D00) //!< Specifies data width is 14 bits
#define SPI_DATA_WIDTH_15BIT                ((uint16_t)0x0E00) //!< Specifies data width is 15 bits
#define SPI_DATA_WIDTH_16BIT                ((uint16_t)0x0F00) //!< Specifies data width is 16 bits
#define IS_SPI_DATA_SIZE(size) (((size) == SPI_DATA_WIDTH_4BIT)  || \
                                ((size) == SPI_DATA_WIDTH_5BIT)  || \
                                ((size) == SPI_DATA_WIDTH_6BIT)  || \
                                ((size) == SPI_DATA_WIDTH_7BIT)  || \
                                ((size) == SPI_DATA_WIDTH_8BIT)  || \
                                ((size) == SPI_DATA_WIDTH_9BIT)  || \
                                ((size) == SPI_DATA_WIDTH_10BIT) || \
                                ((size) == SPI_DATA_WIDTH_11BIT) || \
                                ((size) == SPI_DATA_WIDTH_12BIT) || \
                                ((size) == SPI_DATA_WIDTH_13BIT) || \
                                ((size) == SPI_DATA_WIDTH_14BIT) || \
                                ((size) == SPI_DATA_WIDTH_15BIT) || \
                                ((size) == SPI_DATA_WIDTH_16BIT))

/**
  * @brief SPI CRC length definition
  */
#define SPI_CRC_LEN_8BIT                ((uint16_t)0x0000) //!< Specifies CRC calculation length is 8 bits
#define SPI_CRC_LEN_16BIT               SPI_CTR1_CRC16     //!< Specifies CRC calculation length is 16 bits
#define SPI_CRC_LEN_CHECK(value) (((value) == SPI_CRC_LEN_8BIT) || \
                                  ((value) == SPI_CRC_LEN_16BIT))
                                   
/** 
  * @brief SPI Clock Polarity definition
  */
#define SPI_CPOL_LOW                    ((uint16_t)0x0000) //!< SPI CPOL output low level 
#define SPI_CPOL_HIGH                   SPI_CTR1_CPOL      //!< SPI CPOL output high level
#define SPI_CPOL_CHECK(value) (((value) == SPI_CPOL_LOW) || \
                               ((value) == SPI_CPOL_HIGH))
                           
/**  
  * @brief SPI Clock Phase definition
  */
#define SPI_CPHA_1EDGE                  ((uint16_t)0x0000) //!< SPI smaple data at 1st edge
#define SPI_CPHA_2EDGE                  SPI_CTR1_CPHA      //!< SPI sample data at 2nd edge
#define SPI_CPHA_CHECK(value) (((value) == SPI_CPHA_1EDGE) || \
                               ((value) == SPI_CPHA_2EDGE))
                           
/**   
  * @brief SPI SSM (Slave Select management) definition
  */
#define SPI_SSM_SW                    SPI_CTR1_SWNSSM    //!< SPI software NSS management is selected
#define SPI_SSM_HW                    ((uint16_t)0x0000) //!< SPI hardware NSS management is selected
#define SPI_SSM_CHECK(NSS) (((NSS) == SPI_SSM_SW) || \
                            ((NSS) == SPI_SSM_HW))

/** 
  * @brief SPI baud rate pre divide definition
  */
#define SPI_BAUD_RATE_PDIV_2         ((uint16_t)0x0000) //!< SPI baud rate is pre-divided by 2
#define SPI_BAUD_RATE_PDIV_4         ((uint16_t)0x0008) //!< SPI baud rate is pre-divided by 4
#define SPI_BAUD_RATE_PDIV_8         ((uint16_t)0x0010) //!< SPI baud rate is pre-divided by 8
#define SPI_BAUD_RATE_PDIV_16        ((uint16_t)0x0018) //!< SPI baud rate is pre-divided by 16
#define SPI_BAUD_RATE_PDIV_32        ((uint16_t)0x0020) //!< SPI baud rate is pre-divided by 32
#define SPI_BAUD_RATE_PDIV_64        ((uint16_t)0x0028) //!< SPI baud rate is pre-divided by 64
#define SPI_BAUD_RATE_PDIV_128       ((uint16_t)0x0030) //!< SPI baud rate is pre-divided by 128
#define SPI_BAUD_RATE_PDIV_256       ((uint16_t)0x0038) //!< SPI baud rate is pre-divided by 256
#define SPI_BAUD_RATE_PDIV_CHECK(value) (((value) == SPI_BAUD_RATE_PDIV_2)   || \
                                         ((value) == SPI_BAUD_RATE_PDIV_4)   || \
                                         ((value) == SPI_BAUD_RATE_PDIV_8)   || \
                                         ((value) == SPI_BAUD_RATE_PDIV_16)  || \
                                         ((value) == SPI_BAUD_RATE_PDIV_32)  || \
                                         ((value) == SPI_BAUD_RATE_PDIV_64)  || \
                                         ((value) == SPI_BAUD_RATE_PDIV_128) || \
                                         ((value) == SPI_BAUD_RATE_PDIV_256))
/**   
  * @brief SPI MSB /LSB first transmission definition
  */
#define SPI_FIRST_BIT_MSB                ((uint16_t)0x0000) //!< SPI MSB transmitted first
#define SPI_FIRST_BIT_LSB                SPI_CTR1_LSBF      //!< SPI LSB transmitted first 
#define SPI_FIRST_BIT_CHECK(flag) (((flag) == SPI_FIRST_BIT_MSB) || \
                                   ((flag) == SPI_FIRST_BIT_LSB))

/** 
  * @brief Check SPI CRC polynomial value
  */
#define SPI_CRC_POLYNOMIAL_CHECK(value) ((value) >= 0x1)


/** 
  * @brief I2S Mode definition
  */
#define I2S_MODE_SLAVE_TX                ((uint16_t)0x0000) //!< I2S slave mode transmit data
#define I2S_MODE_SLAVE_RX                ((uint16_t)0x0100) //!< I2S slave mode receive data
#define I2S_MODE_MASTER_TX               ((uint16_t)0x0200) //!< I2S master mode transmit data 
#define I2S_MODE_MASTER_RX               ((uint16_t)0x0300) //!< I2S master mode receive data 
#define I2S_MODE_CHECK(mode) (((mode) == I2S_MODE_SLAVE_TX) || \
                              ((mode) == I2S_MODE_SLAVE_RX) || \
                              ((mode) == I2S_MODE_MASTER_TX)|| \
                              ((mode) == I2S_MODE_MASTER_RX)) 
                                
/**  
  * @brief I2S standard definition
  */
#define I2S_STANDARD_HP           ((uint16_t)0x0000)
#define I2S_STANDARD_MSB          ((uint16_t)0x0010)
#define I2S_STANDARD_LSB          ((uint16_t)0x0020)
#define I2S_STANDARD_PCM_SHORT    ((uint16_t)0x0030)
#define I2S_STANDARD_PCM_LONG     ((uint16_t)0x00B0)
#define I2S_STANDARD_CHECK(value) (((value) == I2S_STANDARD_HP)  || \
                                   ((value) == I2S_STANDARD_MSB) || \
                                   ((value) == I2S_STANDARD_LSB) || \
                                   ((value) == I2S_STANDARD_PCM_SHORT)|| \
                                   ((value) == I2S_STANDARD_PCM_LONG))
/**  
  * @brief I2S Data length definition
  */
#define I2S_DATA_LEN_16BIT              ((uint16_t)0x0000) //!< I2S data length is 16 bits 
#define I2S_DATA_LEN_16BIT_EX           ((uint16_t)0x0001) //!< I2S data length is extended 16 bits 
#define I2S_DATA_LEN_24BIT              ((uint16_t)0x0003) //!< I2S data length is 24 bits 
#define I2S_DATA_LEN_32BIT              ((uint16_t)0x0005) //!< I2S data length is 32 bits 
#define I2S_DATA_LEN_CHECK(len) (((len) == I2S_DATA_LEN_16BIT)   || \
                                 ((len) == I2S_DATA_LEN_16BIT_EX)||\
                                 ((len) == I2S_DATA_LEN_24BIT)   || \
                                 ((len) == I2S_DATA_LEN_32BIT))
                                    
/** 
  * @brief I2S MCK output definition
  */
#define I2S_MCK_OUTPUT_ENABLE           SPI_I2SPDIV_MCKOEN //!< I2S MCK output enabled
#define I2S_MCK_OUTPUT_DISABLE          ((uint16_t)0x0000) //!< I2S MCK output disabled 
#define I2S_MCK_OUTPUT_CHECK(flag) (((flag) == I2S_MCK_OUTPUT_ENABLE) ||\
                                    ((flag) == I2S_MCK_OUTPUT_DISABLE)) 
                                    
/**  
  * @brief I2S Audio Frequency definition
  */
#define I2S_AUDIO_FREQ_192KHZ               ((uint32_t)192000) //!< I2S audio frequency is 192 khz
#define I2S_AUDIO_FREQ_96KHZ                ((uint32_t)96000)  //!< I2S audio frequency is 96 khz
#define I2S_AUDIO_FREQ_48KHZ                ((uint32_t)48000)  //!< I2S audio frequency is 48 khz
#define I2S_AUDIO_FREQ_44KHZ                ((uint32_t)44100)  //!< I2S audio frequency is 44 khz
#define I2S_AUDIO_FREQ_32KHZ                ((uint32_t)32000)  //!< I2S audio frequency is 32 khz
#define I2S_AUDIO_FREQ_22KHZ                ((uint32_t)22050)  //!< I2S audio frequency is 22 khz
#define I2S_AUDIO_FREQ_16KHZ                ((uint32_t)16000)  //!< I2S audio frequency is 16 khz
#define I2S_AUDIO_FREQ_11KHZ                ((uint32_t)11025)  //!< I2S audio frequency is 11 khz
#define I2S_AUDIO_FREQ_8KHZ                 ((uint32_t)8000)   //!< I2S audio frequency is 8 khz
#define I2S_AUDIO_FREQ_DEFAULT              ((uint32_t)2)      //!< I2S audio default frequency

#define IS_I2S_AUDIO_FREQ(freq) ((((freq) >= I2S_AUDIO_FREQ_8KHZ)    && \
                                  ((freq) <= I2S_AUDIO_FREQ_192KHZ)) || \
                                  ((freq) == I2S_AUDIO_FREQ_DEFAULT))

/**  
  * @brief I2S clock polarity definition
  */
#define I2S_CPOL_LOW                    ((uint16_t)0x0000) //!< I2S CPOL output low level 
#define I2S_CPOL_HIGH                   SPI_I2SCTR_CKPIS   //!< I2S CPOL output high level 
#define I2S_CPOL_CHECK(flag) (((flag) == I2S_CPOL_LOW) || \
                              ((flag) == I2S_CPOL_HIGH))

/**  
  * @brief SPI FIFO reception threshold definition
  */
#define SPI_FIFO_RX_THRESHOLD_HF          ((uint16_t)0x0000) //!< RXNE is occurred if FIFO level is grater or equal to 0.5
#define SPI_FIFO_RX_THRESHOLD_QF          SPI_CTR2_RXNE8     //!< RXNE is occurred if FIFO level is grater or equal to 0.25
#define SPI_FIFO_RX_THRESHOLD_CHECK(value) (((value) == SPI_FIFO_RX_THRESHOLD_HF) || \
                                            ((value) == SPI_FIFO_RX_THRESHOLD_QF))

/** 
  * @brief SPI_I2S_DMA_transfer_requests
  */

#define SPI_I2S_DMA_ENABLE_TX               SPI_CTR2_DMATXEN //!< SPI Tx buffer DMA transfer request 
#define SPI_I2S_DMA_ENABLE_RX               SPI_CTR2_DMARXEN //!< SPI Rx buffer DMA transfer request 
#define SPI_I2S_DMA_ENABLE_CHECK(flag) ((((flag) & (uint16_t)0xFFFC) == 0x00) && ((flag) != 0x00))


/** 
  * @brief SPI DMA  transmit number odd/even  definition
  */
#define SPI_DMA_TX_EVEN_RX_EVEN   ((uint16_t)0x0000) //!< SPI number of data for Tx/Rx is even 
#define SPI_DMA_TX_ODD_RX_EVEN    ((uint16_t)0x4000) //!< SPI number of data for Tx is odd and for Rx is even 
#define SPI_DMA_TX_EVEN_RX_ODD    ((uint16_t)0x2000) //!< SPI number of data for Tx is even and for Rx is odd
#define SPI_DMA_TX_ODD_RX_ODD     ((uint16_t)0x6000) //!< SPI number of data for Tx/Rx is odd 
#define SPI_DMA_TX_RX_NUMBER_CHECK(value) (((value) == SPI_DMA_TX_EVEN_RX_EVEN) || \
                                           ((value) == SPI_DMA_TX_ODD_RX_EVEN)  || \
                                           ((value) == SPI_DMA_TX_EVEN_RX_ODD)  || \
                                           ((value) == SPI_DMA_TX_ODD_RX_ODD))

/** 
  * @brief NSS value (in software NSS mode) definition
  */
#define SPI_SOFTWARE_NSS_SET          SPI_CTR1_NVSWNSSM  //!< SPI set NSS pin internally
#define SPI_SOFTWARE_NSS_RESET        ((uint16_t)0xFEFF) //!< SPI reset NSS pin internally
#define SPI_SOFTWARE_NSS_CHECK(value) (((value) == SPI_SOFTWARE_NSS_SET) || \
                                       ((value) == SPI_SOFTWARE_NSS_RESET))
                                       
/**
  * @brief SPI CRC  direction definition  
  */
#define SPI_CRC_DIR_TX             ((uint8_t)0x00) //!< SPI select Tx CRC register 
#define SPI_CRC_DIR_RX             ((uint8_t)0x01) //!< SPI select Rx CRC register 
#define SPI_CRC_DIR_CHECK(flag) (((flag) == SPI_CRC_DIR_TX) || \
                                 ((flag) == SPI_CRC_DIR_RX))

/**   
  * @brief SPI transmit direction definition  
  */
#define SPI_TRANSMIT_DIR_RX                ((uint16_t)0xBFFF) //!< SPI selects Tx transmit direction 
#define SPI_TRANSMIT_DIR_TX                ((uint16_t)0x4000) //!< SPI selects Rx receive direction 
#define SPI_TRANSMIT_DIR_CHECK(value) (((value) == SPI_TRANSMIT_DIR_RX) || \
                                       ((value) == SPI_TRANSMIT_DIR_TX))
                                   
/** 
  * @brief SPI/I2S interrupts definition
  */
#define SPI_I2S_INTR_SET_TXE                  ((uint8_t)0x71) //!< SPI Tx buffer empty interrupt mask 
#define SPI_I2S_INTR_SET_RXNE                 ((uint8_t)0x60) //!< SPI Rx buffer not empty interrupt mask
#define SPI_I2S_INTR_SET_ERR                  ((uint8_t)0x50) //!< SPI Error interrupt mask 

#define SPI_I2S_INTR_SET_CHECK(flag) (((flag) == SPI_I2S_INTR_SET_TXE)  || \
                                      ((flag) == SPI_I2S_INTR_SET_RXNE) || \
                                      ((flag) == SPI_I2S_INTR_SET_ERR))

#define SPI_I2S_INTR_TXUFERR                ((uint8_t)0x53) //!< SPI transmit data underflow error 
#define SPI_I2S_INTR_MMERR                  ((uint8_t)0x55) //!< SPI multi-master error 
#define SPI_I2S_INTR_RXOFERR                ((uint8_t)0x56) //!< SPI receive data overflow error 
#define SPI_I2S_INTR_NWERR                  ((uint8_t)0x58) //!< SPI NSS or I2S WS error 

#define SPI_I2S_GET_INTR_CHECK(flag) (((flag) == SPI_I2S_INTR_SET_RXNE)|| \
                                      ((flag) == SPI_I2S_INTR_SET_TXE) || \
                                      ((flag) == SPI_I2S_INTR_RXOFERR) || \
                                      ((flag) == SPI_I2S_INTR_MMERR) || \
                                      ((flag) == SPI_I2S_INTR_NWERR) || \
                                      ((flag) == SPI_I2S_INTR_TXUFERR))



/** 
  * @brief SPI transmission fifo level status  definition
  */ 
#define SPI_TX_FIFO_LEVEL_STATUS_EMPTY           ((uint16_t)0x0000) //!< SPI Tx FIFO is empty 
#define SPI_TX_FIFO_LEVEL_STATUS_1_4_FULL        ((uint16_t)0x0800) //!< SPI Tx FIIO more than 0.25 full
#define SPI_TX_FIFO_LEVEL_STATUS_1_2_FULL        ((uint16_t)0x1000) //!< SPI Tx FIIO more than 0.5 full 
#define SPI_TX_FIFO_LEVEL_STATUS_FULL            ((uint16_t)0x1800) //!< SPI Tx FIIO is full 
 

/** @defgroup SPI_reception_fifo_status_level 
  * @{
  */ 
#define SPI_RX_FIFO_LEVEL_STATUS_EMPTY           ((uint16_t)0x0000) //!< SPI Rx FIFO is empty 
#define SPI_RX_FIFO_LEVEL_STATUS_1_4_FULL        ((uint16_t)0x0200) //!< SPI Rx FIIO more than 0.25 full
#define SPI_RX_FIFO_LEVEL_STATUS_1_2_FULL        ((uint16_t)0x0400) //!< SPI Rx FIIO more than 0.5 full
#define SPI_RX_FIFO_LEVEL_STATUS_FULL            ((uint16_t)0x0600) //!< SPI Rx FIIO is full

/**
  * @}
  */ 

/**  
  * @brief SPI/ I2S flags definition
  */
#define SPI_I2S_FLAG_RXNE               SPI_STS_RXNE   //!< Receive buffer not empty flag 
#define SPI_I2S_FLAG_TXE                SPI_STS_TXE    //!< Transmit buffer not empty flag 
#define SPI_I2S_FLAG_I2SCSF             SPI_STS_I2SCSF //!< I2S channel side flag 
#define SPI_I2S_FLAG_TXUFERR            SPI_STS_TXUFERR//!< Transmit data underflow error flag 
#define SPI_I2S_FLAG_CRCERR             SPI_STS_CRCERR //!< CRC error flag 
#define SPI_I2S_FLAG_MMERR              SPI_STS_MMERR  //!< Multi-master error flag 
#define SPI_I2S_FLAG_RXOFERR            SPI_STS_RXOFERR//!< Receive data overflow error flag 
#define SPI_I2S_FLAG_BUSY               SPI_STS_BUSY   //!< Busy flag 
#define SPI_I2S_FLAG_NWERR              SPI_STS_NWERR  //!< Error of SPI NSS or I2S WS 

#define SPI_I2S_CLEAR_FLAG_CHECK(flag) (((flag) == SPI_I2S_FLAG_CRCERR))
#define SPI_I2S_GET_FLAG_CHECK(flag) (((flag) == SPI_I2S_FLAG_BUSY)   || \
                                      ((flag) == SPI_I2S_FLAG_RXOFERR)|| \
                                      ((flag) == SPI_I2S_FLAG_MMERR)  || \
                                      ((flag) == SPI_I2S_FLAG_CRCERR) || \
                                      ((flag) == SPI_I2S_FLAG_TXE)    || \
                                      ((flag) == SPI_I2S_FLAG_RXNE)   || \
                                      ((flag) == SPI_I2S_FLAG_NWERR)  || \
                                      ((flag) == SPI_I2S_FLAG_I2SCSF) || \
                                      ((flag) == SPI_I2S_FLAG_TXUFERR))
/**
  * @}
  */ 



/**
  * @fn void spi_i2s_def_init(spi_reg_t* ptr_spi)
  * @brief  Deinitializes the SPIx peripheral registers to their default reset values.      
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @note   I2S mode is only applicable for SPI1. 
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @return None
  */
void spi_i2s_def_init(spi_reg_t* ptr_spi);

/**
  * @fn void spi_config_struct_init(spi_config_t* ptr_config)
  * @brief  Fills each spi_config_t member with its default value.
  * @param  ptr_config: pointer to a spi_config_t structure which will be initialized.
  * @return None
  */
void spi_config_struct_init(spi_config_t* ptr_config);

/**
  * @fn void spi_init(spi_reg_t* ptr_spi, spi_config_t* ptr_config)
  * @brief  Initializes the SPIx peripheral according to the ptr_config.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  ptr_config: pointer to a spi_config_t structure that
  *         contains the configuration for the specified SPI peripheral.
  * @return None
  */
void spi_init(spi_reg_t* ptr_spi, spi_config_t* ptr_config);

/**
  * @fn void i2s_config_struct_init(i2s_config_t* ptr_i2s_config)
  * @brief  Fills each I2S_InitStruct member with its default value,
  * @param  ptr_i2s_config: pointer to a i2s_config_t structure which will be initialized.
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @return None
  */
void i2s_config_struct_init(i2s_config_t* ptr_i2s_config);

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
void i2s_init(spi_reg_t* ptr_spi, i2s_config_t* ptr_i2s_config);

/**
  * @fn void spi_ti_mode_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the TI Mode.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the selected SPI TI communication mode.
  * @return None
  */
void spi_ti_mode_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

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
void spi_nss_pulse_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

/**
  * @fn void spi_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  enable_flag: ENABLE or DISABLE the SPIx peripheral. 
  * @return None
  */
void spi_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

/**
  * @fn void i2s_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the specified SPI peripheral (in I2S mode).
  * @note   I2S mode is not supported for CS32F030, CS32F036 devices.   
  * @param  ptr_spi: pointer to SPIx (x can be 1) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx peripheral. 
  * @return None
  */
void i2s_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

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
void spi_data_width_set(spi_reg_t* ptr_spi, uint16_t value);

/**
  * @fn void spi_fifo_recv_threshold_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the FIFO reception threshold for the selected SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @param  value: specifies the FIFO reception threshold, it can be one of the following values:
  *          @arg SPI_FIFO_RX_THRESHOLD_HF (event is generated if the FIFO level is greater or equal to 1/2)        
  *          @arg SPI_FIFO_RX_THRESHOLD_QF (event is generated if the FIFO  level is greater or equal to 1/4)                           
  * @return None
  */
void spi_fifo_recv_threshold_set(spi_reg_t* ptr_spi, uint16_t value);

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
void spi_transmit_direct_set(spi_reg_t* ptr_spi, uint16_t value);

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
void spi_software_nss_config(spi_reg_t* ptr_spi, uint16_t value);

/**
  * @fn void spi_nss_output_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the SS output, It can be called only after the spi_init() 
  *         has been called and the NSS hardware management mode is selected. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx SS output. 
  * @return None
  */
void spi_nss_output_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

/* Data transfers functions ***************************************************/

/**
  * @fn void spi_8bits_data_send(spi_reg_t* ptr_spi, uint8_t value)
  * @brief  Transmits a byte data through the SPIx/I2Sx.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: value to be transmitted.
  * @return None
  */
void spi_8bits_data_send(spi_reg_t* ptr_spi, uint8_t value);

/**
  * @fn void spi_i2s_16bits_data_send(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Transmits a 16bits data through the SPIx/I2Sx.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: Data to be transmitted.
  * @return None
  */
void spi_i2s_16bits_data_send(spi_reg_t* ptr_spi, uint16_t value);

/**
  * @fn uint8_t spi_8bits_data_receive(spi_reg_t* ptr_spi)
  * @brief  Returns received a 8bits data by the SPIx/I2Sx. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @return The value of the received data.
  */
uint8_t spi_8bits_data_receive(spi_reg_t* ptr_spi);

/**
  * @fn uint16_t spi_i2s_16bits_data_receive(spi_reg_t* ptr_spi)
  * @brief  Returns received a 16bits data by the SPIx/I2Sx. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @return The value of the received data.
  */
uint16_t spi_i2s_16bits_data_receive(spi_reg_t* ptr_spi);

/* Hardware CRC Calculation functions *****************************************/

/**
  * @fn void spi_crc_length_set(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Configures the CRC calculation length. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  value: specifies the SPI CRC calculation length.It can be 
  *          @arg SPI_CRC_LEN_8BIT (Set CRC Calculation to 8 bits)
  *          @arg SPI_CRC_LEN_16BIT (Set CRC Calculation to 16 bits)
  * @return None
  */
void spi_crc_length_set(spi_reg_t* ptr_spi, uint16_t value);

/**
  * @fn void spi_crc_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag)
  * @brief  Enables or disables the CRC value calculation of the transferred bytes. 
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  enable_flag: ENABLE or DISABLE the SPIx CRC value calculation.
  * @return None
  */
void spi_crc_enable_ctrl(spi_reg_t* ptr_spi, enable_state_t enable_flag);

/**
  * @fn void spi_crc_transmit(spi_reg_t* ptr_spi)
  * @brief  Transmit the SPIx CRC value.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @return None
  */
void spi_crc_transmit(spi_reg_t* ptr_spi);

/**
  * @fn uint16_t spi_crc_value_get(spi_reg_t* ptr_spi, uint8_t direct)
  * @brief Returns the transmit or the receive CRC register value for the specified SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral.
  * @param  direct: specifies the CRC register to be read. It can be
  *          @arg SPI_CRC_DIR_TX (Selects Tx CRC register) 
  *          @arg SPI_CRC_DIR_RX (Selects Rx CRC register)
  * @return The selected CRC register value.
  */
uint16_t spi_crc_value_get(spi_reg_t* ptr_spi, uint8_t direct);

/**
  * @fn uint16_t spi_crc_polynomial_get(spi_reg_t* ptr_spi)
  * @brief  Returns the CRC Polynomial register value for the specified SPI.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral .
  * @return The CRC Polynomial register value.
  */
uint16_t spi_crc_polynomial_get(spi_reg_t* ptr_spi);

/* DMA transfers management functions *****************************************/

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
void spi_i2s_dma_enable_ctrl(spi_reg_t* ptr_spi, uint16_t value, enable_state_t enable_flag);

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
void spi_dma_odd_even_set(spi_reg_t* ptr_spi, uint16_t value);

/* Interrupts and flags management functions **********************************/

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
void spi_i2s_interrupt_set(spi_reg_t* ptr_spi, uint8_t set_value, enable_state_t enable_flag);

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
uint16_t spi_transfer_fifo_status_get(spi_reg_t* ptr_spi);

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
uint16_t spi_receive_fifo_status_get(spi_reg_t* ptr_spi);

/**
  * @fn bit_status_t spi_i2s_flag_status_get(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Checks whether the specified SPI flag is set or not.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral  in SPI/I2S mode.
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
bit_status_t spi_i2s_flag_status_get(spi_reg_t* ptr_spi, uint16_t value);

/**
  * @fn void spi_i2s_flag_clear(spi_reg_t* ptr_spi, uint16_t value)
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  ptr_spi: pointer to SPIx (x can be 1 or 2) peripheral in SPI/I2S mode.
  * @param  value: specifies the SPI flag to clear,only CRCERR flag.
  * @return None
  */
void spi_i2s_flag_clear(spi_reg_t* ptr_spi, uint16_t value);

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
bit_status_t spi_i2s_interrupt_status_get(spi_reg_t* ptr_spi, uint8_t value);

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

