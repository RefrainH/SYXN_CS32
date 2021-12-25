/**
  * @file    cs32f0xx_i2c.c  
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Inter-Integrated circuit (I2C):
  *           + Initialization and Configuration
  *           + Communications handling
  *           + SMBUS management
  *           + I2C registers management
  *           + Data transfers management
  *           + DMA transfers management
  *           + Interrupts and flags management
  * @note     SMBUS is not supported for CS32F036)
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2020 ChipSea</center></h2>
  *
  * @verbatim  
  *
  ----------------------- this driver instruction for use--------------------------- 
   1. Enable peripheral clock using  for I2C1 or I2C2.       
   2. Enable SDA, SCL  and SMBA (when used) GPIO clocks.        
   3. Peripherals  multi-function: 
       a. Connect the pin to the desired peripherals' multi-function (MF) .            
       b. Configure the desired pin in  multi-function. 
       c.Select the type, OpenDrain and speed.
       d. Call gpio_init() function.
   4. Config the Mode, Timing , slave address, Ack and Ack Address using the i2c_init().       
   5. Optionally you can enable/configure the following parameters without
       a. Enable the acknowledge feature using i2c_ack_config() function.
       b. Enable the dual addressing mode using i2c_slave_address2_enable_ctrl() function.
       c. Enable the general call using the i2c_call_enable_ctrl() function.
       d. Enable the clock stretching using i2c_stretch_clock_enable_ctrl() function.
       e. Enable the PEC Calculation using i2c_pec_enable_ctrl() function.
       f.  Enable the SMBusAlert pin using i2c_smbus_alert_enable_ctrl() function.
   6. Enable the NVIC and the corresponding interrupt using the function
       i2c_interrupt_config() if you need to use interrupt mode.
   7. When using the DMA mode 
      a. Configure the DMA using dma_init() function.
      b. Active the needed channel Request using i2c_dma_enable_ctrl() function.
   8. Enable the I2C using the i2c_enable_ctrl() function.
   9. Enable the DMA using the dma_enable_ctrl() function when using DMA mode in the  transfers.       
    @endverbatim
  *
  */

#include "cs32f0xx_i2c.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup I2C 
  * @{
  */

#define I2C_CTR1_CLEAR_MASK          ((uint32_t)0x00CFE0FF)  //<! I2C CTR1 clear register Mask 
#define I2C_CTR2_CLEAR_MASK          ((uint32_t)0x07FF7FFF)  //<! I2C CTR2 clear register Mask 
#define I2C_TMR_CLEAR_MASK           ((uint32_t)0xF0FFFFFF)  //<! I2C TMR clear register Mask 
#define I2C_ERROR_INTERRUPT_MASK     ((uint32_t)0x00003F00)  //<! I2C Error interrupt Mask 
#define I2C_CMPF_INTERRUPT_MASK      ((uint32_t)0x000000C0)  //<! I2C  CMPF interrupt Mask 


/**
  * @fn  void i2c_struct_init(i2c_config_t* ptr_struct)
  * @brief Config each ptr_struct member with its default value.
  * @param  ptr_struct: pointer to an i2c_config_t structure which will be initialized.
  * @return None
  */
void i2c_struct_init(i2c_config_t* ptr_struct)
{
    ptr_struct->tmr_value = 0;
    ptr_struct->analog_flag = I2C_ANALOG_FILTER_ENABLE;
    ptr_struct->digital_value = 0;
    ptr_struct->mode = I2C_BUS_MODE_I2C;
    ptr_struct->slave_addr1 = 0;
    ptr_struct->ack_flag = I2C_ACK_DISABLE;
    ptr_struct->ack_addr = I2C_ACK_ADDR_7BITS;
}

/**
  * @fn void i2c_def_init(i2c_reg_t* ptr_i2c)
  * @brief config the ptr_i2c  registers to their default reset values.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return None
  */
void i2c_def_init(i2c_reg_t* ptr_i2c)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    
    if (ptr_i2c == I2C1)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_I2C1, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_I2C1, DISABLE);
    }
    else
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_I2C2, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_I2C2, DISABLE);
    }
}

/**
  * @fn void i2c_init(i2c_reg_t* ptr_i2c, i2c_config_t* ptr_struct)
  * @brief  Initializes the ptr_i2c peripheral according to  the ptr_struct.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  ptr_struct: pointer to a i2c_config_t structure containing  configuration value.          
  * @return None
  */
void i2c_init(i2c_reg_t* ptr_i2c, i2c_config_t* ptr_struct)
{
    uint32_t reg = 0;

    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_ANALOG_FILTER_CHECK(ptr_struct->analog_flag));
    ASSERT(I2C_DIGITAL_FILTER_CHECK(ptr_struct->digital_value));
    ASSERT(I2C_BUS_MODE_CHECK(ptr_struct->mode));
    ASSERT(I2C_SLAVE_ADDR1_CHECK(ptr_struct->slave_addr1));
    ASSERT(I2C_ACK_CONFIG_CHECK(ptr_struct->ack_flag));
    ASSERT(I2C_ACK_ADDR_CHECK(ptr_struct->ack_addr));

    /* Disable I2C function */
    ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_I2CEN);
    reg = ptr_i2c->CTR1;
    reg &= I2C_CTR1_CLEAR_MASK;
  
    /* Configure  analog and digital filter */ 
    reg |= (uint32_t)ptr_struct->analog_flag |(ptr_struct->digital_value << 8);
    ptr_i2c->CTR1 = reg;
    
    /* Set Timing */
    ptr_i2c->TMR = ptr_struct->tmr_value & I2C_TMR_CLEAR_MASK;
    
    /* Enable I2C  */
    ptr_i2c->CTR1 |= I2C_CTR1_I2CEN;

    /* Clear reg */
    reg = 0;
    ptr_i2c->SADR1 = (uint32_t)reg;  
    ptr_i2c->SADR2 = (uint32_t)reg;
  
    /* set I2Cx slave Address1 and acknowledged address */
    reg = (uint32_t)((uint32_t)ptr_struct->ack_addr | (uint32_t)ptr_struct->slave_addr1);
                        
    ptr_i2c->SADR1 = reg;
    /* Enable slave Address1 acknowledgement */
    ptr_i2c->SADR1 |= I2C_SADR1_SADR1EN;
    
    /* Configure I2C: mode */
    reg = ptr_struct->mode;
    ptr_i2c->CTR1 |= reg;

    reg = ptr_i2c->CTR2;
    reg &= I2C_CTR2_CLEAR_MASK;
    
    /* Configure I2C ACK */
    reg |= ptr_struct->ack_flag;
    ptr_i2c->CTR2 = reg;
}

/**
  *@fn void i2c_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the specified I2C peripheral.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    if (enable_flag != DISABLE)
    {
        ptr_i2c->CTR1 |= I2C_CTR1_I2CEN;
    }
    else
    {
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_I2CEN);
    }
}

/**
  * @fn void i2c_software_reset(i2c_reg_t* ptr_i2c)
  * @brief  Active the specified I2C software reset.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return None
  */
void i2c_software_reset(i2c_reg_t* ptr_i2c)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    
    /* Disable bus */
    ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_I2CEN);

    /* Wait for 3 APB clock cycles to perform the software reset  */     
    *(__IO uint32_t *)(uint32_t)ptr_i2c; 

    /* Enable bus */
    ptr_i2c->CTR1 |= I2C_CTR1_I2CEN;
}

/**
  * @fn void i2c_interrupt_config(i2c_reg_t* ptr_i2c, uint32_t flag, enable_state_t enable_flag)
  * @brief  Enables or disables the specified I2C interrupts.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  flag: specifies the I2C interrupts sources to be enabled or disabled. 
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_interrupt_config(i2c_reg_t* ptr_i2c, uint32_t flag, enable_state_t enable_flag)
{
    /* Check the parameters */
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    ASSERT(I2C_INT_CONFIG_CHECK(flag));
  
    if (enable_flag != DISABLE)
    {
        ptr_i2c->CTR1 |= flag;
    }
    else
    {
        /* Disable the selected I2C interrupts */
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)flag);
    }
}

/**
  * @fn void i2c_stretch_clock_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C Clock stretching.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_stretch_clock_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable clock stretching */
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_DISSTRETCH);    
    }
    else
    {
        /* Disable clock stretching  */
        ptr_i2c->CTR1 |= I2C_CTR1_DISSTRETCH;
    }
}

/**
  * @fn void i2c_wakeup_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C wakeup from stop mode.
  *         It is not applicable for CS32F030 devices.  
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_wakeup_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable wakeup from stop mode */
        ptr_i2c->CTR1 |= I2C_CTR1_WKUPEN;   
    }
    else
    {
        /* Disable wakeup from stop mode */    
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_WKUPEN); 
    }
}

/**
  *@fn void i2c_slave_address2_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C slave address 2.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_slave_address2_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable slave address 2 */
        ptr_i2c->SADR2 |= I2C_SADR2_SADR2EN;
    }
    else
    {
        /* Disable slave address 2 */
        ptr_i2c->SADR2 &= (uint32_t)~((uint32_t)I2C_SADR2_SADR2EN);
    }
}    

/**
  * @fn void i2c_slave_address2_config(i2c_reg_t* ptr_i2c, uint16_t addr, uint8_t mask)
  * @brief  Configures the I2C slave  address 2 and mask.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  addr: specifies the slave address.
  * @param  mask: specifies own address 2 mask to be programmed.
  * @return None
  */
void i2c_slave_address2_config(i2c_reg_t* ptr_i2c, uint16_t addr, uint8_t mask)
{
    uint32_t reg = 0;

    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_SLAVE_ADDR2_CHECK(addr));
    ASSERT(I2C_SLAVE_ADDR2_MASK_CHECK(mask));
  
    reg = ptr_i2c->SADR2;

    /* Reset I2Cx SADR2 bit [7:1] and SADR2MSK bit [10:8]  */
    reg &= (uint32_t)~((uint32_t)(I2C_SADR2_SADR2 | I2C_SADR2_SADR2MSK));
    reg |= (uint32_t)(((uint32_t)addr & I2C_SADR2_SADR2) | (((uint32_t)mask << 8) & I2C_SADR2_SADR2MSK)); 

    /* modify the new register value */
    ptr_i2c->SADR2 = reg;
}

/**
  * @fn void i2c_call_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C general call mode.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag:It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_call_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
  ASSERT(I2C_BUS_CHECK(ptr_i2c));
  ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
  if (enable_flag != DISABLE)
  {
    /* Enable general call mode */
    ptr_i2c->CTR1 |= I2C_CTR1_GCEN;
  }
  else
  {
    /* Disable general call mode */
    ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_GCEN);
  }
} 

/**
  * @fn void i2c_slave_response_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C slave response control.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_slave_response_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable control */
        ptr_i2c->CTR1 |= I2C_CTR1_SLVRC;
    }
    else
    {
        /* Disable control */
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_SLVRC);
    }
}

/**
  * @fn  void i2c_slave_address_config(i2c_reg_t* ptr_i2c, uint32_t addr)
  * @brief  Configures the slave address to be transmitted after start generation.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral. 
  * @param  addr: specifies the slave address.
  * @note   This function should be called before generating start condition.
  * @return None
  */
void i2c_slave_address_config(i2c_reg_t* ptr_i2c, uint32_t addr)
{
    uint32_t reg = 0;

    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_SLAVE_ADDR1_CHECK(addr));
               
    reg = ptr_i2c->CTR2;

    /* Reset I2Cx MTADR bit [9:0] */
    reg &= (uint32_t)~((uint32_t)I2C_CTR2_MTADR);

    /* modify I2Cx MTADR */
    reg |= (uint32_t)(addr & I2C_CTR2_MTADR);

    ptr_i2c->CTR2 = reg;
}
  
/**
  * @fn  void i2c_transfer_end_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C transfer end mode. 
  *         It has effect if Reload mode is disabled.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_transfer_end_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable transfer end mode */
        ptr_i2c->CTR2 |= I2C_CTR2_TENDSEL;
    }
    else
    {
        /* Disable transfer end mode */
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_TENDSEL);
    }
} 

#ifdef CS32F036
/**
  * @fn void i2c_reload_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables the I2C Bytes number reload mode.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_reload_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable Auto Reload mode */
        ptr_i2c->CTR2 |= I2C_CTR2_RELOADM;
    }
    else
    {
        /* Disable Auto Reload mode */
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_RELOADM);
    }
}
#endif

/**
  * @fn void i2c_bytes_number_config(i2c_reg_t* ptr_i2c, uint8_t counter)
  * @brief Configures the number of bytes to be transmitted/received.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  counter: specifies the number of bytes.
  * @return None
  */
void i2c_bytes_number_config(i2c_reg_t* ptr_i2c, uint8_t counter)
{
    uint32_t reg = 0;

    ASSERT(I2C_BUS_CHECK(ptr_i2c));

    reg = ptr_i2c->CTR2;
    reg &= (uint32_t)~((uint32_t)I2C_CTR2_BNUM);

    /* Set bytes number */
    reg |= (uint32_t)(((uint32_t)counter << 16 ) & I2C_CTR2_BNUM);

    ptr_i2c->CTR2 = reg;
}  
  
/**
  * @fn void i2c_request_direction_config(i2c_reg_t* ptr_i2c, uint16_t direction)
  * @brief  Configures the type of transfer request for the master.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  direction: specifies the transfer request direction,it can be one of the following values
  *            @arg I2C_DIRECTION_TX(Master request a write transfer)
  *            @arg I2C_DIRECTION_RX(Master request a read transfer)  
  * @return None
  */
void i2c_request_direction_config(i2c_reg_t* ptr_i2c, uint16_t direction)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_DIRECTION_CHECK(direction));
  
    /* Test on the direction to set/reset the read/write bit */
    if (direction == I2C_DIRECTION_TX)
    {
        /* Request a write Transfer */
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_TDIR);
    }
    else
    {
        /* Request a read Transfer */
        ptr_i2c->CTR2 |= I2C_CTR2_TDIR;
    }
}  
  
/**
  * @fn void i2c_start_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2Cx communication START condition.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_start_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Generate a START  */
        ptr_i2c->CTR2 |= I2C_CTR2_STARTGEN;
    }
    else
    {
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_STARTGEN);
    }
}  
  
/**
  * @fn  void i2c_stop_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2Cx communication STOP condition.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_stop_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Generate a STOP condition */
        ptr_i2c->CTR2 |= I2C_CTR2_STOPGEN;
    }
    else
    {
        /* Disable the STOP condition generation */
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_STOPGEN);
    }
}     

/**
  * @fn void i2c_ack_config(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Generates I2C communication Acknowledge.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_ack_config(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable ACK generation */
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_NACKGEN);    
    }
    else
    {
        /* Enable NACK generation */
        ptr_i2c->CTR2 |= I2C_CTR2_NACKGEN;
    }
}   

/**
  * @fn uint8_t i2c_match_address_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C  matched slave address .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the slave matched address .
  */
uint8_t i2c_match_address_get(i2c_reg_t* ptr_i2c)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
  
    /* Return the matched  slave address in the STS register */
    return (uint8_t)(((uint32_t)ptr_i2c->STS & I2C_STS_MSLVA) >> 16) ;
}   

/**
  * @fn uint16_t i2c_transfer_direction_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C slave received request.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the received request.
  */
uint16_t i2c_transfer_direction_get(i2c_reg_t* ptr_i2c)
{
    uint32_t reg = 0;
    uint16_t direction = 0;
  
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
  
    reg = (uint32_t)(ptr_i2c->STS & I2C_STS_DIRF);  
    if (reg == 0)
    {
        direction = I2C_DIRECTION_TX;
    }
    else
    {
        direction = I2C_DIRECTION_RX;
    }  
    return direction;
}

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
void i2c_transfer_handle(i2c_reg_t* ptr_i2c, uint32_t addr, uint8_t counter, uint32_t end_select, uint32_t operate_set)
{
    uint32_t reg = 0;
  
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_SLAVE_ADDR1_CHECK(addr));  
    ASSERT(I2C_END_MODE_CHECK(end_select));
    ASSERT(I2C_OPERATE_CONFIG_CHECK(operate_set));
    
    reg = ptr_i2c->CTR2;
  
    if (((ptr_i2c->CTR1 & I2C_BUS_MODE_SMBUS_DEVICE) == I2C_BUS_MODE_SMBUS_DEVICE) && (counter == 1) && (end_select == I2C_RELOAD_MODE))
    {

    }
    else {
        /* clear reg specific bits */
        reg &= (uint32_t)~((uint32_t)(I2C_CTR2_MTADR | I2C_CTR2_BNUM | I2C_CTR2_RELOADM |\
                                I2C_CTR2_TENDSEL | I2C_CTR2_TDIR | I2C_CTR2_STARTGEN | I2C_CTR2_STOPGEN));
  	
    
        /* modify reg value */
        reg |= (uint32_t)(((uint32_t)addr & I2C_CTR2_MTADR) | (((uint32_t)counter << 16 ) & I2C_CTR2_BNUM) | \
                (uint32_t)end_select | (uint32_t)operate_set);
  
	
        /* set CTR2 register */
        ptr_i2c->CTR2 = reg;  
    }
}   

/**
  * @fn  void i2c_overtime_a_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C timer A for overtime control (SCL overtime detection).
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_overtime_a_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable  timer A  for overtime detection */
        ptr_i2c->OVRT |= I2C_OVRT_TIMAEN;   
    }
    else
    {
        /* Disable timer A for overtime detection*/    
        ptr_i2c->OVRT &= (uint32_t)~((uint32_t)I2C_OVRT_TIMAEN); 
    }
}

/**
  * @fn  void i2c_overtime_b_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C timer B for overtime control (SCL overtime detection).
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_overtime_b_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable  timer B  for overtime detection */
        ptr_i2c->OVRT |= I2C_OVRT_TIMBEN;   
    }
    else
    {
        /* Disable timer B for overtime detection*/  
        ptr_i2c->OVRT &= (uint32_t)~((uint32_t)I2C_OVRT_TIMBEN); 
    }
}

/**
  * @fn void i2c_idle_overtime_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief Enables /disables I2C Idle clock overtime detection (Bus idle SCL and SDA high detection).     
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_idle_overtime_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable Idle clock overtime detection */
        ptr_i2c->OVRT |= I2C_OVRT_IDLEDEN;   
    }
    else
    {
        /* Disable Idle clock overtime detection */    
        ptr_i2c->OVRT &= (uint32_t)~((uint32_t)I2C_OVRT_IDLEDEN); 
    }
}

/**
  * @fn  void i2c_overtime_a_config(i2c_reg_t* ptr_i2c, uint16_t value)
  * @brief  Configures I2C timer A  for overtime control .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: specifies the overtime value. 
  * @return None
  */
void i2c_overtime_a_config(i2c_reg_t* ptr_i2c, uint16_t value)
{
    uint32_t reg = 0;

    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_OVERTIME_CHECK(value));
    
    reg = ptr_i2c->OVRT;

    /* clear I2Cx TIMA[11:0] */
    reg &= (uint32_t)~((uint32_t)I2C_OVRT_TIMA);

    /* Set I2Cx TIMA[11:0] */
    reg |= (uint32_t)((uint32_t)value & I2C_OVRT_TIMA) ;

    ptr_i2c->OVRT = reg;
}

/**
  * @fn  void i2c_overtime_b_config(i2c_reg_t* ptr_i2c, uint16_t value)
  * @brief  Configures I2C timer B  for overtime control .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: specifies the TimeoutB value. 
  * @return None
  */
void i2c_overtime_b_config(i2c_reg_t* ptr_i2c, uint16_t value)
{
    uint32_t reg = 0;

    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_OVERTIME_CHECK(value));

    reg = ptr_i2c->OVRT;

    /* clear I2Cx TIMB[27:16] */
    reg &= (uint32_t)~((uint32_t)I2C_OVRT_TIMB);

    /* Set I2Cx TIMB[27:16] */
    reg |= (uint32_t)(((uint32_t)value << 16) & I2C_OVRT_TIMB) ;

    ptr_i2c->OVRT = reg;
}

/**
  * @fn void i2c_pec_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C PEC calculation.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_pec_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{   
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_NACKGEN);   
        if (((ptr_i2c->CTR1 & I2C_BUS_MODE_I2C) == I2C_BUS_MODE_I2C) && (ptr_i2c->SADR1 != 0x00))
        {
            ptr_i2c->CTR2 |= I2C_CTR2_STOPGEN;
        }
        /* Enable PEC calculation */
        ptr_i2c->CTR1 |= I2C_CTR1_PECMEN;   
    }
    else
    {
        /* Disable PEC calculation */    
        ptr_i2c->CTR1 &= (uint32_t)~((uint32_t)I2C_CTR1_PECMEN); 
    }
}

/**
  * @fn void i2c_pec_byte_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
  * @brief  Enables or disables I2C PEC  byte control.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
void i2c_pec_byte_enable_ctrl(i2c_reg_t* ptr_i2c, enable_state_t enable_flag)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable PEC  byte control. */
        ptr_i2c->CTR2 |= I2C_CTR2_PECCTR;   
    }
    else
    {
        /* Disable  PEC  byte control. */    
        ptr_i2c->CTR2 &= (uint32_t)~((uint32_t)I2C_CTR2_PECCTR); 
    }
}

/**
  * @fn uint8_t i2c_pec_code_get(i2c_reg_t* ptr_i2c)
  * @brief  Returns the I2C received PEC value .
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the PEC .
  */
uint8_t i2c_pec_code_get(i2c_reg_t* ptr_i2c)
{
    ASSERT(I2C1_BUS_CHECK(ptr_i2c));
  
    /* Return the slave matched address in the PECCODE register */
    return (uint8_t)((uint32_t)ptr_i2c->PECCODE & I2C_PECCODE_PECCODE);
}

/**
  * @fn uint32_t i2c_register_read(i2c_reg_t* ptr_i2c, uint8_t reg)
  * @brief  Reads the specified I2C register and returns its value.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  reg: specifies the register to read.
  * @return The value of the read register.
  */
uint32_t i2c_register_read(i2c_reg_t* ptr_i2c, uint8_t reg)
{
    __IO uint32_t temp = 0;
    
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_REG_CHECK(reg));

    temp = (uint32_t)ptr_i2c;
    temp += reg;

    /* Return the specifies register value */
    return (*(__IO uint32_t *) temp);
}

/**
  * @fn void i2c_data_send(i2c_reg_t* ptr_i2c, uint8_t value)
  * @brief  Sends a data byte through the I2C bus.
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @param  value: Byte to be transmitted..
  * @return None
  */
void i2c_data_send(i2c_reg_t* ptr_i2c, uint8_t value)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
  
    ptr_i2c->TXBUF = (uint8_t)value;
}

/**
  * @fn uint8_t i2c_data_receive(i2c_reg_t* ptr_i2c)
  * @brief  Returns the most recent received data by the I2C bus..
  * @param  ptr_i2c: pointer to I2Cx where x can be 1 or 2 to select I2C peripheral.
  * @return The value of the received data.
  */
uint8_t i2c_data_receive(i2c_reg_t* ptr_i2c)
{
    uint8_t reg_val;
	
    ASSERT(I2C_BUS_CHECK(ptr_i2c));

    reg_val = (uint8_t)ptr_i2c->RXBUF;
    return reg_val;
}  
    
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
void i2c_dma_enable_ctrl(i2c_reg_t* ptr_i2c, uint32_t request, enable_state_t enable_flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    ASSERT(I2C_DMA_REQUEST_CHECK(request));

    if (enable_flag != DISABLE)
    {
        /* Enable the I2C DMA requests */
        ptr_i2c->CTR1 |= request;
    }
    else
    {
        /* Disable the selected I2C DMA requests */
        ptr_i2c->CTR1 &= (uint32_t)~request;
    }
}

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
bit_status_t i2c_flag_status_get(i2c_reg_t* ptr_i2c, uint32_t flag)
{
    uint32_t reg = 0;
    bit_status_t bit_status = RESET;
  
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_GET_FLAG_CHECK(flag));
  
    reg = ptr_i2c->STS;  
    reg &= flag;
  
    if(reg != 0)
    {
        bit_status = SET;
    }
    else
    {
        bit_status = RESET;
    }
    return bit_status;
} 

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
void i2c_flag_clear(i2c_reg_t* ptr_i2c, uint32_t flag)
{ 
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_CLEAR_FLAG_CHECK(flag));

    ptr_i2c->STSC = flag;
}

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
bit_status_t i2c_interrupt_status_get(i2c_reg_t* ptr_i2c, uint32_t flag)
{
    uint32_t reg = 0;
    bit_status_t bits_tatus = RESET;
    uint32_t enable_flag = 0;

    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_GET_INT_CHECK(flag));

    /*check Error interrupt */
    if ((uint32_t)(flag & I2C_ERROR_INTERRUPT_MASK))
    {
        enable_flag = (uint32_t)((I2C_CTR1_ERRDIE) & (ptr_i2c->CTR1));
    }
    else if ((uint32_t)(flag & I2C_CMPF_INTERRUPT_MASK))
    {
        enable_flag = (uint32_t)((I2C_CTR1_CMPIE) & (ptr_i2c->CTR1));
    }
    else
    {
        enable_flag = (uint32_t)((flag) & (ptr_i2c->CTR1));
    }
  
    reg = ptr_i2c->STS;
    
    /* Get flag status */
    reg &= flag;

    /* Check the status of the specified I2C flag */
    if((reg != RESET) && enable_flag)
    {
        bits_tatus = SET;
    }
    else
    {
        bits_tatus = RESET;
    }

    return bits_tatus;
}

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
void i2c_interrupt_bit_clear(i2c_reg_t* ptr_i2c, uint32_t flag)
{
    ASSERT(I2C_BUS_CHECK(ptr_i2c));
    ASSERT(I2C_CLEAR_INT_CHECK(flag));

    /* Clear the selected flag */
    ptr_i2c->STSC = flag;
}

/**
  * @}
  */

/**
  * @}
  */

