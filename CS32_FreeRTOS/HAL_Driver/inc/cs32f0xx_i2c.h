/**
  * @file    cs32f0xx_i2c.h
  * @brief   This file contains all the functions prototypes for the I2C firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2020 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_I2C_H__
#define __CS32F0XX_I2C_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */


/**
  * @struct i2c_config_t
  * @brief I2C Init structure definition
  */

typedef struct
{
    uint32_t tmr_value;       //!< Specifies the I2C_TMR register value(referring to I2C Timing).                                         
    uint32_t analog_flag;     //!<  Enables or disables analog noise filter.                                      
    uint32_t digital_value;   //!< set the digital noise filter. range in 0x00 ~ 0x0F.                                        
    uint32_t mode;            //!< set the I2C mode.                                        
    uint32_t slave_addr1;     //!< set slave address 1.It can be a 7bits or 10bits address.                                       
    uint32_t ack_flag;        //!< Enables or disables the acknowledgement.                                    
    uint32_t ack_addr;        //!<  set acknowledge address.It can be a 7bits or 10bits address.                                       
}i2c_config_t;



/**
  * @brief I2C bus check definition
  */
#define I2C_BUS_CHECK(bus)     (((bus) == I2C1) || ((bus) == I2C2))                                                                                  
#define I2C1_BUS_CHECK(bus)    ((bus) == I2C1) 

/** 
  * @ brief I2C analog Filter definition
  */
#define I2C_ANALOG_FILTER_ENABLE         ((uint32_t)0x00000000) //!< enable analog Filter 
#define I2C_ANALOG_FILTER_DISABLE        I2C_CTR1_AFDIS           //!< disable analog Filter

#define I2C_ANALOG_FILTER_CHECK(flag)    (((flag) == I2C_ANALOG_FILTER_ENABLE) || \
                                         ((flag) == I2C_ANALOG_FILTER_DISABLE))
/**
  * @ brief I2C digital filter value check
  */
#define I2C_DIGITAL_FILTER_CHECK(filter)   ((filter) <= 0x0000000F)


/** 
  * @ brief I2C bus mode definition
  */
#define I2C_BUS_MODE_I2C                  ((uint32_t)0x00000000) //!<   I2C mode
#define I2C_BUS_MODE_SMBUS_DEVICE         I2C_CTR1_SMBDDAEN       //!< Enable bit for SMBus Device Default address (SMBUS is not supported for CS32F036 devices)
#define I2C_BUS_MODE_SMBUS_HOST           I2C_CTR1_SMBHAEN        //!< Enable bit for SMBus Host address (SMBUS is not supported for CS32F036 devices)

#define I2C_BUS_MODE_CHECK(mode)       (((mode) == I2C_BUS_MODE_I2C) || (((mode)) == I2C_BUS_MODE_SMBUS_DEVICE)\
                                        || (((mode)) == I2C_BUS_MODE_SMBUS_HOST))

/** 
  * @ brief I2C ACK and check definition
  */
#define I2C_ACK_ENABLE                ((uint32_t)0x00000000) //!< enable ACK
#define I2C_ACK_DISABLE               I2C_CTR2_NACKGEN        //!< disable ACK
#define I2C_ACK_CONFIG_CHECK(ACK)      (((ACK) == I2C_ACK_ENABLE) || ((ACK) == I2C_ACK_DISABLE))                                        

/** 
  * @ brief I2C Addressing format definition
  */
#define I2C_ACK_ADDR_7BITS    ((uint32_t)0x00000000)  //!< Slave Address 7bits
#define I2C_ACK_ADDR_10BITS   I2C_SADR1_SADR1MODE     //!< Slave Address 10bits

#define I2C_ACK_ADDR_CHECK(addr)  (((addr) == I2C_ACK_ADDR_7BITS) || ((addr) == I2C_ACK_ADDR_10BITS))
                                             
/** 
  * @ brief I2C slave addr1 check definition
  */
#define I2C_SLAVE_ADDR1_CHECK(addr)   ((addr) <= (uint32_t)0x000003FF) //!< slave addr1 range 

/** 
  * @ brief I2C slave addr2 check definition
  */
#define I2C_SLAVE_ADDR2_CHECK(addr)   ((addr) <= (uint16_t)0x00FF) //!< slave addr2 range

/** 
  * @ brief I2C data Tx/Rx direction definition
  */
#define I2C_DIRECTION_TX       ((uint16_t)0x0000)  //!<set data Transfer direction for writing 
#define I2C_DIRECTION_RX       ((uint16_t)0x0400)  //!<set data Transfer direction for reading 
#define I2C_DIRECTION_CHECK(direct)     (((direct) == I2C_DIRECTION_TX) || ((direct) == I2C_DIRECTION_RX))
                                         
/**
  * @ brief I2C DMA transfer requests definition
  */
#define I2C_TX_DMA_REQUEST          I2C_CTR1_TXDMAREQEN  //!< Enable  transmission DMA requests
#define I2C_RX_DMA_REQUEST          I2C_CTR1_RXDMAREQEN  //!< Enable bit for reception DMA requests
#define I2C_DMA_REQUEST_CHECK(flag)       ((((flag) & (uint32_t)0xFFFF3FFF) == 0x00) && ((flag) != 0x00))

/** 
  * @ brief check  I2c slave address2 mask value definition
  */
#define I2C_SLAVE_ADDR2_MASK_CHECK(mask)  ((mask) < ((uint8_t)0x08)) 
                                         
/** 
  * @ brief I2C  timer overtime value check definition
  */
#define I2C_OVERTIME_CHECK(value)   ((value) <= (uint16_t)0x0FFF)

/** 
  * @ brief I2C registers name definition
  */
#define I2C_CTR1_REG            ((uint8_t)0x00)  //!< I2C registers CTR1
#define I2C_CTR2_REG            ((uint8_t)0x04) //!< I2C registers CTR2
#define I2C_SADR1_REG           ((uint8_t)0x08)  //!< I2C registers SADR1
#define I2C_SADR2_REG           ((uint8_t)0x0C) //!< I2C registers SADR2
#define I2C_TMR_REG             ((uint8_t)0x10)  //!< I2C registers TMR
#define I2C_OVRT_REG            ((uint8_t)0x14)  //!< I2C registers OVRT
#define I2C_STS_REG             ((uint8_t)0x18)  //!< I2C registers STS
#define I2C_STSC_REG             ((uint8_t)0x1C) //!< I2C registers STSC
#define I2C_PECCODE_REG          ((uint8_t)0x20) //!< I2C registers PECCODE 
#define I2C_RXBUF_REG            ((uint8_t)0x24) //!< I2C registers RXBUF
#define I2C_TXBUF_REG            ((uint8_t)0x28) //!< I2C registers TXBUF

#define I2C_REG_CHECK(REGISTER)      (((REGISTER) == I2C_CTR1_REG) || ((REGISTER) == I2C_CTR2_REG)  \
                                     || ((REGISTER) == I2C_SADR1_REG) || ((REGISTER) == I2C_SADR2_REG)\
                                     || ((REGISTER) == I2C_TMR_REG) || ((REGISTER) == I2C_OVRT_REG)\
                                     || ((REGISTER) == I2C_STS_REG) || ((REGISTER) == I2C_STSC_REG)\
                                     || ((REGISTER) == I2C_PECCODE_REG) || ((REGISTER) == I2C_RXBUF_REG)\
                                     || ((REGISTER) == I2C_TXBUF_REG))                                     
/** 
  * @brief I2C interrupts enable config check definition
  */
#define I2C_INT_CONFIG_CHECK(flag)      ((((flag) & (uint32_t)0xFFFFFF01) == 0x00) && ((flag) != 0x00))

/** 
  * @brief I2C status bit check definition
  */
#define I2C_CLEAR_FLAG_CHECK(flag)       ((((flag) & (uint32_t)0xFFFF4000) == 0x00) && ((flag) != 0x00))
#define I2C_GET_FLAG_CHECK(flag)         (((flag) == I2C_STS_TXEF) || ((flag) == I2C_STS_TXINTF) || \
                                         ((flag) == I2C_STS_RXNEF) || ((flag) == I2C_STS_ADRF) || \
                                         ((flag) == I2C_STS_NACKF) || ((flag) == I2C_STS_STOPF) || \
                                         ((flag) == I2C_STS_CMPF) || ((flag) == I2C_STS_RLDF) || \
                                         ((flag) == I2C_STS_BUSERRF) || ((flag) == I2C_STS_ARBLOF) || \
                                         ((flag) == I2C_STS_OVRF) || ((flag) == I2C_STS_PECERRF) || \
                                         ((flag) == I2C_STS_OVRTF) || ((flag) == I2C_STS_SMBAF) || \
                                         ((flag) == I2C_STS_BUSYF))

/** 
  * @brief I2C interrupts flag bit check  definition
  */
#define I2C_CLEAR_INT_CHECK(flag)     ((((flag) & (uint32_t)0xFFFFC001) == 0x00) && ((flag) != 0x00))                             
#define I2C_GET_INT_CHECK(flag)      (((flag) == I2C_STS_TXINTF) || ((flag) == I2C_STS_RXNEF) || \
                                           ((flag) == I2C_STS_ADRF) || ((flag) == I2C_STS_NACKF) || \
                                           ((flag) == I2C_STS_STOPF) || ((flag) == I2C_STS_CMPF) || \
                                           ((flag) == I2C_STS_RLDF) || ((flag) == I2C_STS_BUSERRF) || \
                                           ((flag) == I2C_STS_ARBLOF) || ((flag) == I2C_STS_OVRF) || \
                                           ((flag) == I2C_STS_PECERRF) || ((flag) == I2C_STS_OVRTF) || \
                                           ((flag) == I2C_STS_SMBAF))
                               

/** 
  * @brief I2C transfer end mode definition
  */
#define  I2C_RELOAD_MODE             I2C_CTR2_RELOADM       //!<  bytes number reload mode
#define  I2C_TRANFER_END_MODE        I2C_CTR2_TENDSEL        //!< tranfer end mode 
#define  I2C_SOFTWARE_END_MODE      ((uint32_t)0x00000000)   //!< software end mode
                              
#define I2C_END_MODE_CHECK(mode)    (((mode) == I2C_RELOAD_MODE) || ((mode) == I2C_TRANFER_END_MODE)\
                                    || ((mode) == I2C_SOFTWARE_END_MODE)) 
                                                                       
/** 
  * @brief I2C bus operation config definition
  */
#define  I2C_RELEASE_CONFIG       ((uint32_t)0x00000000)   //!< I2C BUS release
#define  I2C_STOP_CONFIG          I2C_CTR2_STOPGEN         //!< I2C BUS stop config
#define  I2C_WRITE_CONFIG         I2C_CTR2_STARTGEN        //!< I2C BUS  write config                 
#define  I2C_READ_CONFIG          (uint32_t)(I2C_CTR2_STARTGEN | I2C_CTR2_TDIR)  //!<I2C BUS read config
                              
#define I2C_OPERATE_CONFIG_CHECK(MODE)     (((MODE) == I2C_STOP_CONFIG) || ((MODE) == I2C_READ_CONFIG)  \
                                         ||((MODE) == I2C_WRITE_CONFIG) || ((MODE) == I2C_RELEASE_CONFIG))


/**
  * @fn  void i2c_struct_init(i2c_config_t* ptr_struct)
  * @brief Config each ptr_struct member with its default value.
  * @param  ptr_struct: pointer to an i2c_config_t structure which will be initialized.
  * @return None
  */
void i2c_struct_init(i2c_config_t* ptr_struct);

/**
  * @fn void i2c_def_init(i2c_reg_t* ptr_i2c)
  * @brief config the ptr_i2c  registers to their default reset values.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return None
  */
void i2c_def_init(i2c_reg_t* ptr_i2c);

/**
  * @fn void i2c_init(i2c_reg_t* ptr_i2c, i2c_config_t* ptr_struct)
  * @brief  Initializes the ptr_i2c peripheral according to  the ptr_struct.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  ptr_struct: pointer to a i2c_config_t structure containing  configuration value.          
  * @return None
  */
void i2c_init(i2c_reg_t* ptr_i2c, i2c_config_t* ptr_struct);

/**
  *@fn void i2c_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_software_reset(i2c_reg_t* ptr_i2c)
  * @brief  Active the specified I2C software reset.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return None
  */
void i2c_software_reset(i2c_reg_t* ptr_i2c);

/**
  * @fn void i2c_interrupt_config(i2c_reg_t* ptr_i2c, uint32_t flag, enable_state_t enable_flag)
  * @brief  Enables or disables the specified I2C interrupts.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the I2C interrupts sources to be enabled or disabled. 
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_interrupt_config(i2c_reg_t* ptr_i2c, uint32_t flag, enable_state_t enable_flag);

/**
  * @fn void i2c_stretch_clock_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C Clock stretching.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_stretch_clock_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_wakeup_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C wakeup from stop mode.
  *         It is not applicable for CS32F030 devices.  
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_wakeup_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  *@fn void i2c_slave_address2_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C slave address 2.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_slave_address2_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_slave_address2_config(i2c_reg_t* ptr_i2c, uint16_t addr, uint8_t mask)
  * @brief  Configures the I2C slave  address 2 and mask.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  addr: specifies the slave address.
  * @param  mask: specifies own address 2 mask to be programmed.
  * @return None
  */
void i2c_slave_address2_config(i2c_reg_t* ptr_i2c, uint16_t addr, uint8_t mask);

/**
  * @fn void i2c_call_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C general call mode.
  * @param  ptr_i2c: pointer to the  selected I2C bus.
  * @param  enable_flag:It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_call_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_slave_response_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C slave response control.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_slave_response_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn  void i2c_slave_address_config(i2c_reg_t* ptr_i2c, uint32_t addr)
  * @brief  Configures the slave address to be transmitted after start generation.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  addr: specifies the slave address.
  * @note   This function should be called before generating start condition.
  * @return None
  */
void i2c_slave_address_config(i2c_reg_t* ptr_i2c, uint32_t addr);

/**
  * @fn  void i2c_transfer_end_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C transfer end mode. 
  *         It has effect if Reload mode is disabled.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_transfer_end_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

#ifdef CS32F036
/**
  * @fn void i2c_reload_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C Bytes number reload mode.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_reload_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

#endif

/**
  * @fn void i2c_bytes_number_config(i2c_reg_t* ptr_i2c, uint8_t counter)
  * @brief Configures the number of bytes to be transmitted/received.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  counter: specifies the number of bytes.
  * @return None
  */
void i2c_bytes_number_config(i2c_reg_t* ptr_i2c, uint8_t counter);

/**
  * @fn void i2c_request_direction_config(i2c_reg_t* ptr_i2c, uint16_t direction)
  * @brief  Configures the type of transfer request for the master.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  direction: specifies the transfer request direction,it can be one of the following values
  *            @arg I2C_DIRECTION_TX(Master request a write transfer)
  *            @arg I2C_DIRECTION_RX(Master request a read transfer)  
  * @return None
  */
void i2c_request_direction_config(i2c_reg_t* ptr_i2c, uint16_t direction);

/**
  * @fn void i2c_start_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2Cx communication START condition.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_start_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn  void i2c_stop_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2Cx communication STOP condition.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_stop_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_ack_config(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2C communication Acknowledge.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_ack_config(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn uint8_t i2c_match_address_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C  matched slave address .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the slave matched address .
  */
uint8_t i2c_match_address_get(i2c_reg_t* ptr_i2c);

/**
  * @fn uint16_t i2c_transfer_direction_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C slave received request.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the received request.
  */
uint16_t i2c_transfer_direction_get(i2c_reg_t* ptr_i2c);

/**
  * @fn void i2c_transfer_handle(i2c_reg_t* ptr_i2c, uint32_t addr, uint8_t counter, uint32_t end_select, uint32_t operate_set)
  * @brief  Handles I2C communication when starting transfer or during transfer.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  addr: specifies the slave address to be programmed.
  * @param  counter: specifies the number of bytes between 0 and 255.
  * @param  end_select: It can be one of the following values:
  *            @arg I2C_RELOAD_MODE(Enable Reload mode)
  *            @arg I2C_TRANFER_END_MODE(Enable Automatic end mode)
  *            @arg I2C_SOFTWARE_END_MODE(Enable Software end mode)
  * @param  operate_set: It can be one of the following values:r 
  *            @arg I2C_RELEASE_CONFIG(Don't Generate stop and start condition)
  *            @arg I2C_STOP_CONFIG(Generate stop condition ,Number_Bytes should be set to 0)
  *            @arg I2C_READ_CONFIG(Generate Restart for read request)
  *            @arg I2C_WRITE_CONFIG(Generate Restart for write request).
  * @return None
  */
void i2c_transfer_handle(i2c_reg_t* ptr_i2c, uint32_t addr, uint8_t counter, uint32_t end_select, uint32_t operate_set);

/**
  * @fn  void i2c_overtime_a_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C timer A for overtime control (SCL overtime detection).
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_overtime_a_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn  void i2c_overtime_b_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C timer B for overtime control (SCL overtime detection).
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_overtime_b_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_idle_overtime_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief Enables /disables I2C Idle clock overtime detection (Bus idle SCL and SDA high detection).     
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_idle_overtime_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn  void i2c_overtime_a_config(i2c_reg_t* ptr_i2c, uint16_t value)
  * @brief  Configures I2C timer A  for overtime control .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: specifies the overtime value. 
  * @return None
  */
void i2c_overtime_a_config(i2c_reg_t* ptr_i2c, uint16_t value);

/**
  * @fn  void i2c_overtime_b_config(i2c_reg_t* ptr_i2c, uint16_t value)
  * @brief  Configures I2C timer B  for overtime control .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: specifies the TimeoutB value. 
  * @return None
  */
void i2c_overtime_b_config(i2c_reg_t* ptr_i2c, uint16_t value);

/**
  * @fn void i2c_pec_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C PEC calculation.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_pec_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn void i2c_pec_byte_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C PEC  byte control.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_pec_byte_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag);

/**
  * @fn uint8_t i2c_pec_code_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C received PEC value .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the PEC .
  */
uint8_t i2c_pec_code_get(i2c_reg_t* ptr_i2c);

/**
  * @fn uint32_t i2c_register_read(i2c_reg_t* ptr_i2c, uint8_t reg)
  * @brief  Reads the specified I2C register and returns its value.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  reg: specifies the register to read.
  * @return The value of the read register.
  */
uint32_t i2c_register_read(i2c_reg_t* ptr_i2c, uint8_t reg);

/**
  * @fn void i2c_data_send(i2c_reg_t* ptr_i2c, uint8_t value)
  * @brief  Sends a data byte through the I2C bus.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: Byte to be transmitted..
  * @return None
  */
void i2c_data_send(i2c_reg_t* ptr_i2c, uint8_t value);

/**
  * @fn uint8_t i2c_data_receive(i2c_reg_t* ptr_i2c)
  * @brief  Returns the most recent received data by the I2C bus..
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the received data.
  */
uint8_t i2c_data_receive(i2c_reg_t* ptr_i2c);

/**
  * @fn void i2c_dma_enable_ctrl(i2c_reg_t* ptr_i2c, uint32_t request, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C DMA interface.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  request: specifies the I2C DMA transfer request to be enabled or disabled. 
  *          This parameter can be any combination of the following values:
  *            @arg I2C_TX_DMA_REQUEST(Tx DMA transfer request)
  *            @arg I2C_RX_DMA_REQUEST(Rx DMA transfer request)
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_dma_enable_ctrl(i2c_reg_t* ptr_i2c, uint32_t request, enable_state_t enable_flag);

/**
  * @fn bit_status_t i2c_flag_status_get(i2c_reg_t* ptr_i2c, uint32_t flag)
  * @brief  Checks whether the specified I2C flag is set or not.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the flag to check. It can be one of the following values:
  *            @arg I2C_STS_TXEF(Data register empty flag in transmitting mode)
  *            @arg I2C_STS_TXINTF(Transmit interrupt status flag)
  *            @arg I2C_STS_RXNEF(Receive data register not empty)
  *            @arg I2C_STS_ADRF(Slave mode Address matched flag)
  *            @arg I2C_STS_NACKF(NACK received flag)
  *            @arg I2C_STS_STOPF(STOP detection flag)
  *            @arg I2C_STS_CMPF(Transfer complete, master mode)
  *            @arg I2C_STS_RLDF(Transfer complete reload)
  *            @arg I2C_STS_BUSERRF(Bus error)
  *            @arg I2C_STS_ARBLOF(Arbitration lost)
  *            @arg I2C_STS_OVRF(Slave Overflow/Underflow)
  *            @arg I2C_STS_PECERRF(PEC error in reception)
  *            @arg I2C_STS_OVRTF(Overtime or tLOW detection flag)
  *            @arg I2C_STS_SMBAF(SMBus Alert)(SMBUS is not supported for CS32F036)
  *            @arg I2C_STS_BUSYF(Bus busy)
  * @return The state (SET or RESET).
  */
bit_status_t i2c_flag_status_get(i2c_reg_t* ptr_i2c, uint32_t flag);

/**
  * @fn void i2c_flag_clear(i2c_reg_t* ptr_i2c, uint32_t flag)
  * @brief  Clears the I2Cx's pending flags.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the flag to clear.,it can be any combination of the following values:
  *            @arg I2C_STS_ADRF(Slave mode Address matched flag)
  *            @arg I2C_STS_NACKF(NACK received flag)
  *            @arg I2C_STS_STOPF(STOP detection flag)
  *            @arg I2C_STS_BUSERRF(Bus error)
  *            @arg I2C_STS_ARBLOF(Arbitration lost)
  *            @arg I2C_STS_OVRF(Slave Overflow/Underflow flag)
  *            @arg I2C_STS_PECERRF(PEC error in reception)
  *            @arg I2C_STS_OVRTF(Overtime or tLOW detection flag)
  *            @arg I2C_STS_SMBAF(SMBus Alert)(SMBUS is not supported for CS32F036)
  * @return The new state of I2C_FLAG (SET or RESET).
  */
void i2c_flag_clear(i2c_reg_t* ptr_i2c, uint32_t flag);

/**
  * @fn bit_status_t i2c_interrupt_status_get(i2c_reg_t* ptr_i2c, uint32_t flag)
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the interrupt source to check.,itcan be one of the following values:
  *            @arg I2C_STS_TXINTF(Transmit interrupt status)
  *            @arg I2C_STS_RXNEF(Receive data register not empty)
  *            @arg I2C_STS_ADRF(Slave mode Address matched flag)
  *            @arg I2C_STS_NACKF(NACK received flag)
  *            @arg I2C_STS_STOPF(STOP detection flag)
  *            @arg I2C_STS_CMPF(Transfer complete master mode)
  *            @arg I2C_STS_RLDF(Transfer complete reload)
  *            @arg I2C_STS_BUSERRF(Bus error)
  *            @arg I2C_STS_ARBLOF(Arbitration lost)
  *            @arg I2C_STS_OVRF(Slave Overflow/Underflow flag)
  *            @arg I2C_STS_PECERRF(PEC error in reception)
  *            @arg I2C_STS_OVRTF(Overtime or tLOW detection flag)
  *            @arg I2C_STS_SMBAF(SMBus Alert)(SMBUS is not supported for CS32F036)
  * @return The new state of flag (SET or RESET).
  */
bit_status_t i2c_interrupt_status_get(i2c_reg_t* ptr_i2c, uint32_t flag);

/**
  * @fn void i2c_interrupt_bit_clear(i2c_reg_t* ptr_i2c, uint32_t flag)
  * @brief  Clears the I2Cx's interrupt pending bits.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the interrupt pending bit to clear,it can be any combination of the following values:
  *            @arg I2C_STS_ADRF(Slave mode Address matched flag)
  *            @arg I2C_STS_NACKF(NACK received flag)
  *            @arg I2C_STS_STOPF(STOP detection flag)
  *            @arg I2C_STS_BUSERRF(Bus error)
  *            @arg I2C_STS_ARBLOF(Arbitration lost)
  *            @arg I2C_STS_OVRF(Slave Overflow/Underflow flag)
  *            @arg I2C_STS_PECERRF(PEC error in reception)
  *            @arg I2C_STS_OVRTF(Overtime or tLOW detection flag)
  *            @arg I2C_STS_SMBAF(SMBus Alert)(SMBUS is not supported for CS32F036)
  * @return The new state of flag (SET or RESET).
  */
void i2c_interrupt_bit_clear(i2c_reg_t* ptr_i2c, uint32_t flag);

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

