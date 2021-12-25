/**
  * @file    cs32f0xx_flash.c  
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the FLASH peripheral:
  *            - FLASH Interface configuration
  *            - FLASH Memory Programming
  *            - Option Bytes Programming
  *            - Interrupts and flags management
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
    The driver provides functions to configure and program the Flash memory of  CS32F0xx devices.          
         1. FLASH Interface configuration functions: 
           a.Set the wait conuter. 
           b.Option enable or disable wait.
           
         2. FLASH Memory Programming functions: 
             a. Lock and Unlock the Flash.
             b. Erase function: Erase Page, erase all pages.
             c.Program functions: Half Word and Word write.

         3. FLASH Option Bytes Programming functions: 
             a. Lock and Unlock the Flash Option bytes.
             b. Update the Option Bytes.
             c. Erase the Option Bytes.
             d.Set or Reset the write protection.
             e.Set the Read protection Level
             f. Program the user option Bytes.
             g.Set or Reset the BOOT1 bit.
             h. Enable or Disable the VDDA detect.
             i. Get the user option bytes.
             j. Get the Write protection.
             k. Get the read protection status.

        4. FLASH Interrupts and flag management functions: 
             a. Enable or Disable the flash interrupt sources
             b. Get flags status
             c. Clear flags
             d. Get Flash operation status
             e. Wait for last flash operation

    @endverbatim
  *
  */  

#include "cs32f0xx_flash.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup FLASH
  * @brief FLASH driver modules
  * @{
  */ 


/**
  * @fn void flash_wait_counter_set(uint32_t counter)
  * @brief  Sets WCNT (wait conuter).
  * @param  counter: set wait count value. It can be one of the following values:
  *          @arg FMC_WCR_WCNT_0(0 cycle)
  *          @arg FMC_WCR_WCNT_1(1 cycle)
  * @note   For reading data from Flash memory, the number of wait states
  * @note   must be correctly programmed according to  the CPU clock (HCLK) 
  * @note   -------------------------------------------------
  *         |  Wait states      |   HCLK  frequency (MHz)   |
  *         |-------------------|---------------------------|
  *         |0WS(1CPU cycle)    |    0 < HCLK <= 24         |
  *         |1WS(2CPU cycle)    |    24 < HCLK <= 48        |
  *         -------------------------------------------------
  * @return None
  */
void flash_wait_counter_set(uint32_t counter)
{
    uint32_t reg = 0;

    ASSERT(FMC_WCR_WCNT_CHECK(counter));

    /* Read the WCR register */
    reg = FLASH->WCR;  

    /* Sets the wait count value */
    reg &= (uint32_t) (~((uint32_t)FMC_WCR_WCNT));
    reg |= counter;

    /* Write the WCR register */
    FLASH->WCR = reg;
}

/**
  * @fn void flash_wait_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the FMC wait.
  * @param  enable_flag: It can be ENABLE or DISABLE .          
  * @return None
  */
void flash_wait_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if(enable_flag != DISABLE)
    {
        FLASH->WCR |= FMC_WCR_WE;
    }
    else
    {
        FLASH->WCR &= (uint32_t)(~((uint32_t)FMC_WCR_WE));
    }
}

/**
  * @fn bit_status_t flash_wait_status_get(void)
  * @brief  Checks whether the FLASH wait status is set or not.
  * @param  None
  * @return Flash wait Status (SET or RESET).
  */
bit_status_t flash_wait_status_get(void)
{
    bit_status_t bit_status = RESET;

    if ((FLASH->WCR & FMC_WCR_WS) != (uint32_t)RESET)
    {
        bit_status = SET;
    }
    else
    {
        bit_status = RESET;
    }
    /* Return wait status (SET or RESET) */
    return bit_status; 
}


/**
  * @fn void flash_unlock(void)
  * @brief  Unlock the flash control register and program memory access.
  * @param  None
  * @return None
  */
void flash_unlock(void)
{
    if((FLASH->CTR & FMC_CTR_LOCK) != RESET)
    {
        /* Unlocking the program memory access */
        FLASH->CCODE = FMC_CCODE_VALUE1;
        FLASH->CCODE = FMC_CCODE_VALUE2;
    }
}

/**
  * @fn void flash_lock(void)
  * @brief  Lock the Program memory access.
  * @param  None
  * @return None
  */
void flash_lock(void)
{
    /* Set the lock bit*/
    FLASH->CTR |= FMC_CTR_LOCK;
}

/**
  * @fn flash_status_t flash_page_erase(uint32_t page_addr)
  * @brief  Erases a specified page in program memory. 
  *         The flash_unlock() function must be called before runing this function.
  * @param  page_addr: The page address in program memory to be erased.       
  * @return flash current status.         
  */
flash_status_t flash_page_erase(uint32_t page_addr)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;

    ASSERT(FLASH_PROGRAM_ADDRESS_CHECK(page_addr));
    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
  
    if(status == FLASH_STATUS_COMPLETE)
    { 
        /* If the previous operation is completed, proceed to erase the page */
        FLASH->CTR |= FMC_CTR_PERS;
        FLASH->ERSADR  = page_addr;
        FLASH->CTR |= FMC_CTR_START;
        
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
    
        /* Disable the PERS Bit */
        FLASH->CTR &= ~FMC_CTR_PERS;
    }
    
    /* Return the Erase Status */
    return status;
}

/**
  * @fn flash_status_t flash_all_pages_erase(void)
  * @brief  Erases all flash pages.
  *         The flash_unlock() function must be called before runing this function.
  * @param  None
  * @return flash status.
  */
flash_status_t flash_all_pages_erase(void)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
  
    if(status == FLASH_STATUS_COMPLETE)
    {
        /* if the previous operation is completed, proceed to erase all pages */
        FLASH->CTR |= FMC_CTR_CHIPERS;
        FLASH->CTR |= FMC_CTR_START;
        
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        /* Disable the CHIPERS Bit */
        FLASH->CTR &= ~FMC_CTR_CHIPERS;
    }

    /* Return the Erase Status */
    return status;
}

/**
  * @fn flash_status_t flash_word_program(uint32_t addr, uint32_t value)
  * @brief  Programs a word at a specified address.
  *         The flash_unlock() function must be called before runing this function.
  * @param  addr: specifies the address to be programmed.
  * @param  value: specifies the data to be programmed.
  * @return flash status. 
  */
flash_status_t flash_word_program(uint32_t addr, uint32_t value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;
    __IO uint32_t temp = 0;

    ASSERT(FLASH_PROGRAM_ADDRESS_CHECK(addr));
  
    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

    if(status == FLASH_STATUS_COMPLETE)
    {
        /* Proceed to program the new first half word */    
        FLASH->CTR |= FMC_CTR_PG;
  
        *(__IO uint16_t*)addr = (uint16_t)value;
    
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
 
        if(status == FLASH_STATUS_COMPLETE)
        {
            /*Proceed to program the new second half word */      
            temp = addr + 2;

            *(__IO uint16_t*) temp = value >> 16;
    
            /* Wait for last operation to be completed */
            status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
        
            /* Disable the PG Bit */
            FLASH->CTR &= ~FMC_CTR_PG;
        }
        else
        {
            /* Disable the PG Bit */
            FLASH->CTR &= ~FMC_CTR_PG;
        }   
    }
   
    /* Return the Program Status */
    return status;
}

/**
  * @fn flash_status_t flash_half_word_program(uint32_t addr, uint16_t value)
  * @brief  Programs a half word at a specified address.
  *         The flash_unlock() function must be called before runing this function.
  * @param  addr: specifies the address to be programmed.
  * @param  value: specifies the data to be programmed.
  * @return flash status. 
  */
flash_status_t flash_half_word_program(uint32_t addr, uint16_t value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;

    ASSERT(FLASH_PROGRAM_ADDRESS_CHECK(addr));

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);  
    if(status == FLASH_STATUS_COMPLETE)
    {
        /*Proceed to program the new data */
        FLASH->CTR |= FMC_CTR_PG;
  
        *(__IO uint16_t*)addr = value;

        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
    
        /* Disable the PG Bit */
        FLASH->CTR &= ~FMC_CTR_PG;
    } 
  
    /* Return the Program Status */
    return status;
}

/**
  * @fn void flash_option_bytes_unlock(void)
  * @brief  Unlocks option bytes block access.
  * @param  None
  * @return None
  */
void flash_option_bytes_unlock(void)
{
    if((FLASH->CTR & FMC_CTR_OBWEN) == RESET)
    { 
        /* Unlocking the option bytes block access */
        FLASH->OBCCODE = FMC_OBCCODE_VALUE1;
        FLASH->OBCCODE = FMC_OBCCODE_VALUE2;
    }
}

/**
  * @fn void flash_option_bytes_lock(void)
  * @brief  Locks option bytes block access
  * @param  None
  * @return None
  */
void flash_option_bytes_lock(void)
{
    /* Set the OBWEN bit to lock the option bytes block access */
    FLASH->CTR &= ~FMC_CTR_OBWEN;
}

/**
  * @fn void flash_option_bytes_update(void)
  * @brief  Update option bytes.
  * @param  None
  * @return None
  */
void flash_option_bytes_update(void)
{
    /* Set the OBUPDATE bit to update the option byte */
    FLASH->CTR |= FMC_CTR_OBUPDATE;
}

/**
  * @fn flash_status_t flash_option_bytes_erase(void)
  * @brief  Erases all option bytes except the Read protection (RPROT).
  *         The flash_option_bytes_unlock() function must be called before runing this function.   
  * @param  None
  * @return flash status.
  */
flash_status_t flash_option_bytes_erase(void)
{
    uint16_t temp = OB_RPROT_LEVEL_0;
    
    flash_status_t status = FLASH_STATUS_COMPLETE;

    /* Get the actual read protection Option Byte value */ 
    if(flash_option_bytes_read_protect_get() != RESET)
    {
        temp = 0x00;  
    }
    
    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

    if(status == FLASH_STATUS_COMPLETE)
    {   
        /* Proceed to erase the option bytes */
        FLASH->CTR |= FMC_CTR_OBERS;
        FLASH->CTR |= FMC_CTR_START;

        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        if(status == FLASH_STATUS_COMPLETE)
        {
            /* Disable the OBERS bit if the erase operation is complete */
            FLASH->CTR &= ~FMC_CTR_OBERS;
       
            /* Enable the Option Bytes Programming operation */
            FLASH->CTR |= FMC_CTR_OBPG;

            /* Restore the last read protection Option Byte value */
            OB->RPROT = (uint16_t)temp; 

            /* Wait for last operation to be completed */
            status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
            if(status != FLASH_STATUS_TIMEOUT)
            {
                /* Disable the OBPG bit if the program operation is complete */
                FLASH->CTR &= ~FMC_CTR_OBPG;
            }
        }
        else
        {
            if (status != FLASH_STATUS_TIMEOUT)
            {
                /* Disable the OBPG bit */
                FLASH->CTR &= ~FMC_CTR_OBPG;
            }
        }  
    }
    /* Return the erase status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_write_protect_enable(uint32_t set_value)
  * @brief  Write protects (WPROT) the desired pages.
  *         The flash_option_bytes_unlock() function must be called before runing this function.  
  * @param  set_value: specifies the address of the pages to be write protected.
  *          It can be :
  *           @arg OB_WPROT_PAGE0_TO_PAGE3
  *           @arg OB_WPROT_PAGE60_TO_PAGE63
  *           @arg OB_WPROT_ALL_PAGE
  * @return flash status. 
  */
flash_status_t flash_option_bytes_write_protect_enable(uint32_t set_value)
{
    uint16_t value_wport0 = 0xFFFF;
    uint16_t value_wport1 = 0xFFFF;

    flash_status_t status = FLASH_STATUS_COMPLETE;

    ASSERT(OB_WPROT_CHECK(set_value));

    set_value = (uint32_t)(~set_value);
    value_wport0 = (uint16_t)(set_value & OB_WPROT0_WPROT0);
    value_wport1 = (uint16_t)((set_value >> 8) & OB_WPROT0_WPROT0);
    
    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

    if(status == FLASH_STATUS_COMPLETE)
    {
        FLASH->CTR |= FMC_CTR_OBPG;

        if(value_wport0 != 0xFF)
        {
            OB->WPROTOB0 = (uint16_t)((uint32_t)value_wport0 | (((uint32_t)value_wport1 << 16)));
      
            /* Wait for last operation to be completed */
            status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
        }
        if((status == FLASH_STATUS_COMPLETE) && (value_wport1 != 0xFF))
        {
            OB->WPROTOB1 = value_wport1;
      
            /* Wait for last operation to be completed */
            status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
        }
        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG Bit */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    } 
    /* Return the write protection operation Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_read_protect_config(uint8_t set_level)
  * @brief  Enables or disables RPROT (the read out protection).
  *         The flash_option_bytes_unlock() function must be called before runing this function. 
  * @param  set_level: flash read protection level, it can be:
  *          @arg OB_RPROT_LEVEL_0(No protection)
  *          @arg OB_RPROT_LEVEL_1(Read protection of the memory)
  *          @arg OB_RPROT_LEVEL_2(Chip protection)
  * @note   When enabling set level 2, it's no more possible to go back to level 1 or 0
  * @return flash status. 
  */
flash_status_t flash_option_bytes_read_protect_config(uint8_t set_level)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;
  
    ASSERT(OB_RPROT_LEVEL_CHECK(set_level));

    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
  
    if(status == FLASH_STATUS_COMPLETE)
    {
        FLASH->CTR |= FMC_CTR_OBERS;
        FLASH->CTR |= FMC_CTR_START;
    
        /* Wait for last operation to be completed*/
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
        
        if(status == FLASH_STATUS_COMPLETE)
        {
            /* Disable the OBERS bit if the erase operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBERS;
        
            /* Enable the Option Bytes Programming operation */
            FLASH->CTR |= FMC_CTR_OBPG;
       
            OB->RPROT = set_level;

            /* Wait for last operation to be completed */
            status = flash_operation_wait(FLASH_OPERATE_TIMEOUT); 
    
            if(status != FLASH_STATUS_TIMEOUT)
            {
                /* Disable the OBPG bit if the program operation is completed */
                FLASH->CTR &= ~FMC_CTR_OBPG;
            }
        }
        else 
        {
            if(status != FLASH_STATUS_TIMEOUT)
            {
                /* Disable the OBERS Bit */
                FLASH->CTR &= ~FMC_CTR_OBERS;
            }
        }
    }
    /* Return the protection operation Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_user_config(uint8_t fwdt_value, uint8_t dsm_value, uint8_t pwd_value) 
  * @brief  Programs the FLASH User Option Byte: FWDT_SW / RST_STOP / RST_STDBY.
  *         The flash_option_bytes_unlock() function must be called before runing this function. 
  *         Call the flash_option_bytes_lock() to disable the flash control register access and the option bytes.
  * @param  fwdt_value: Selects the FWDT mode, it can be:
  *          @arg OB_FWDT_SW(Software FWDT selected)
  *          @arg OB_FWDT_HW(Hardware FWDT selected)          
  * @param  dsm_value: Reset event when entering DSM mode. It can be: 
  *          @arg OB_DSM_NO_RST(No reset generated when entering in DSM)
  *          @arg OB_DSM_RST(Reset generated when entering in DSM)
  * @param  pwd_value: Reset event when entering PWD mode. It can be: 
  *          @arg OB_PWD_NO_RST(No reset generated when entering in PWD)
  *          @arg OB_PWD_RST(Reset generated when entering in PWD)
  * @return Flash status.
  */
flash_status_t flash_option_bytes_user_config(uint8_t fwdt_value, uint8_t dsm_value, uint8_t pwd_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE; 

    ASSERT(OB_FWDT_SELECT_CHECK(fwdt_value));
    ASSERT(OB_DSM_RST_SELECT_CHECK(dsm_value));
    ASSERT(OB_PWD_RST_SELECT_CHECK(pwd_value));
    
    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);  
    if(status == FLASH_STATUS_COMPLETE)
    {
        /* Enable the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG;     

        OB->USER = (uint16_t)((uint16_t)(fwdt_value | dsm_value) | (uint16_t)(pwd_value | 0xF8));
  
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG Bit if the program operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }    
    /* Return the Option Byte program Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_boot_config(uint8_t set_value)
  * @brief  Sets or resets the BOOT1 option bit.
  * @param  set_value: it can be:  
  *          @arg FLASH_OB_BOOT1_RESET(BOOT1 option bit reset)
  *          @arg FLASH_OB_BOOT1_SET(BOOT1 option bit set)
  * @return flash status.
  */
flash_status_t flash_option_bytes_boot_config(uint8_t set_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE; 

    ASSERT(FLASH_OB_BOOT1_CHECK(set_value));

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

    if(status == FLASH_STATUS_COMPLETE)
    {  
        /* Enable the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG;
    
        OB->USER = set_value | 0xEF;
  
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
    
        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG bit if the program operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }
    /* Return the Option Byte program Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_vdda_config(uint8_t set_value)
  * @brief  Sets or resets the analogue detection on VDDA Power source.
  * @param  set_value: it can be:
  *          @arg FLASH_OB_VDDA_ON(VDDA Power source ON)
  *          @arg FLASH_OB_VDDA_OFF(VDDA Power source OFF)
  * @return flash status.
  */
flash_status_t flash_option_bytes_vdda_config(uint8_t set_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE; 
    
    ASSERT(FLASH_OB_VDDA_CHECK(set_value));

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
    
    if(status == FLASH_STATUS_COMPLETE)
    {  
        /* Enable the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG; 

        OB->USER = set_value | 0xDF;
  
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG bit if the program operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }
    /* Return the Option Byte program status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_sram_parity_config(uint8_t set_value)
  * @brief  Sets or resets the SRAM parity.
  * @param  set_value: Set or Reset the SRAM parity enable bit.
  *         It can be one of the following values:
  *          @arg FLASH_OB_SRAM_PARITY_SET(Set SRAM parity)
  *          @arg FLASH_OB_SRAM_PARITY_RESET(Reset SRAM parity)
  * @return flash status.
  */
flash_status_t flash_option_bytes_sram_parity_config(uint8_t set_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE; 
    
    ASSERT(FLASH_OB_SRAM_PARITY_CHECK(set_value));

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT); 

    if(status == FLASH_STATUS_COMPLETE)
    {  
        /* Enable the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG; 

        OB->USER = set_value | 0xBF;
  
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG bit if the program operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }
    /* Return the Option Byte program Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_user_write(uint8_t set_value)
  * @brief  Programs the FLASH User Option Byte, It can be FWDT_SW, RST_DSM, RST_PWD,
  *         BOOT1 and VDDA detection. The flash_option_bytes_unlock() function must be called before runing
  *         this function.
  * @param  set_value: Selects all user option bytes,It can be the following values:
  *          @arg OB_FWDT_SW(Software WDG selected)
  *          @arg OB_FWDT_HW(Hardware WDG selected)
  *          @arg OB_DSM_NO_RST(No reset generated when entering in DSM)
  *          @arg OB_DSM_RST(reset generated when entering in DSM)
  *          @arg OB_PWD_NO_RST(reset generated when entering in PWD)
  *          @arg OB_PWD_RST(reset generated when entering in PWD)
  *          @arg FLASH_OB_BOOT1_RESET(BOOT1 reset)
  *          @arg FLASH_OB_BOOT1_SET(BOOT1 set)
  *          @arg FLASH_OB_VDDA_ON(Analog monitoring on VDDA Power source ON)
  *          @arg FLASH_OB_VDDA_OFF(Analog monitoring on VDDA Power source OFF)
  *          @arg FLASH_OB_SRAM_PARITY_SET(SRAM Parity SET)
  *          @arg FLASH_OB_SRAM_PARITY_RESET(SRAM Parity RESET)
  * @return flash status.
  */
flash_status_t flash_option_bytes_user_write(uint8_t set_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE; 

    /* Wait for last operation to be completed */
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT); 

    if(status == FLASH_STATUS_COMPLETE)
    {
        /* Enable the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG; 

        OB->USER = set_value;
  
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG bit if the program operation is completed */
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }    
    /* Return the Option Byte program Status */
    return status;
}

/**
  * @fn flash_status_t flash_option_bytes_data_program(uint32_t addr, uint8_t set_value) 
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   The flash_option_bytes_unlock() function must be called before runing this function.
  * @note   Call the flash_option_bytes_lock() to disable the flash register access and the option bytes.   
  * @param  addr: specifies the address to be programmed can be 0x1FFFF804 or 0x1FFFF806.
  * @param  set_value: specifies the data to be programmed.
  * @return flash status.
  */
flash_status_t flash_option_bytes_data_program(uint32_t addr, uint8_t set_value)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;
  
    ASSERT(FLASH_OB_DATA_ADDRESS_CHECK(addr));
  
    status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);

    if(status == FLASH_STATUS_COMPLETE)
    {
        /* Enables the Option Bytes Programming operation */
        FLASH->CTR |= FMC_CTR_OBPG; 
        *(__IO uint16_t*)addr = set_value;
    
        /* Wait for last operation to be completed */
        status = flash_operation_wait(FLASH_OPERATE_TIMEOUT);
    
        if(status != FLASH_STATUS_TIMEOUT)
        {
            /* Disable the OBPG bit if tht program operation is completed*/
            FLASH->CTR &= ~FMC_CTR_OBPG;
        }
    }
    /* Return the Option Byte Data Program Status */
    return status;
}

/**
  * @fn uint8_t flash_option_bytes_user_get(void)
  * @brief  Get the FLASH User Option Bytes values.
  * @param  None
  * @return The flash User Option Bytes .
  */
uint8_t flash_option_bytes_user_get(void)
{
    /* Return the User Option Byte */
    return (uint8_t)(FLASH->OBSTS >> 8);
}

/**
  * @fn uint32_t flash_option_bytes_write_protect_get(void)
  * @brief  Get the flash Write Protection Option Bytes value.
  * @param  None
  * @return Flash Write Protection Option Bytes register value (WPSTS).
  */
uint32_t flash_option_bytes_write_protect_get(void)
{
    /* Return the FLASH write protection Register value */
    return (uint32_t)(FLASH->WPSTS);
}

/**
  * @fn  bit_status_t flash_option_bytes_read_protect_get(void)
  * @brief  Checks whether the FLASH Read out Protection Status is set or not.
  * @param  None
  * @return status(SET or RESET)
  */
bit_status_t flash_option_bytes_read_protect_get(void)
{
    bit_status_t readstatus = RESET;
  
    if ((uint8_t)(FLASH->OBSTS & (FMC_OBSTS_RPROT1 | FMC_OBSTS_RPROT2)) != RESET)
    {
        readstatus = SET;
    }
    else
    {
        readstatus = RESET;
    }
    return readstatus;
}

/**
  * @fn void flash_interrupt_config(uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified flash interrupts.
  * @param  set_value: specifies the FLASH interrupt sources.
  *         It can be one of the following values:
  *          @arg FLASH_INT_END(end of programming Interrupt)
  *          @arg FLASH_INT_ERR(Error Interrupt)
  * @param  enable_flag: DISABLE or ENABLE be selected.         
  * @return None 
  */
void flash_interrupt_config(uint32_t set_value, enable_state_t enable_flag)
{
    ASSERT(FLASH_INT_CHECK(set_value)); 
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if(enable_flag != DISABLE)
    {
        /* Enable the interrupt sources */
        FLASH->CTR |= set_value;
    }
    else
    {
        /* Disable the interrupt sources */
        FLASH->CTR &= ~(uint32_t)set_value;
    }
}

/**
  * @fn bit_status_t flash_flag_status_get(uint32_t status_flag)
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  status_flag: specifies the FLASH flag to check. It can be the following values:
  *          @arg FLASH_FLAG_BUSY(FLASH write/erase operations in progress flag)
  *          @arg FLASH_FLAG_PGERR(FLASH Programming error flag flag)
  *          @arg FLASH_FLAG_WPERR(FLASH Write protected error flag)
  *          @arg FLASH_FLAG_ENDF(FLASH End of Programming flag)
  * @return The new state of status_flag (SET or RESET).
  */
bit_status_t flash_flag_status_get(uint32_t status_flag)
{
    bit_status_t bit_status = RESET;

    ASSERT(FLASH_GET_FLAG_CHECK(status_flag));

    if((FLASH->STS & status_flag) != (uint32_t)RESET)
    {
        bit_status = SET;
    }
    else
    {
        bit_status = RESET;
    }
    /* Return bit status (SET or RESET) */
    return bit_status; 
}

/**
  * @fn void flash_flag_clear(uint32_t status_flag)
  * @brief  Clears the FLASH's flags.
  * @param  status_flag: specifies the FLASH flags to clear.It can be 
  *          @arg FLASH_FLAG_PGERR(FLASH Programming error flag flag)
  *          @arg FLASH_FLAG_WPERR(FLASH Write protected error flag)
  *          @arg FLASH_FLAG_ENDF(FLASH End of Programming flag)
  * @return None
  */
void flash_flag_clear(uint32_t status_flag)
{
    ASSERT(FLASH_CLEAR_FLAG_CHECK(status_flag));
  
    /* Clear the flags */
    FLASH->STS = status_flag;
}

/**
  * @fn flash_status_t flash_status_get(void)
  * @brief  Returns the flash Status.
  * @param  None
  * @return flash status.
  */
flash_status_t flash_status_get(void)
{
    flash_status_t status = FLASH_STATUS_COMPLETE;
  
    if((FLASH->STS & FLASH_FLAG_BUSY) == FLASH_FLAG_BUSY) 
    {
        status = FLASH_STATUS_BUSY;
    }
    else 
    {  
        if((FLASH->STS & (uint32_t)FLASH_FLAG_WPERR)!= (uint32_t)0x00)
        { 
            status = FLASH_STATUS_ERROR_WRP;
        }
        else 
        {
            if((FLASH->STS & (uint32_t)(FMC_STS_PGERR)) != (uint32_t)0x00)
            {
                status = FLASH_STATUS_ERROR_PROGRAM; 
            }
            else
            {
                status = FLASH_STATUS_COMPLETE;
            }
        }
    }
    /* Return the FLASH status */
    return status;
}


/**
  * @fn flash_status_t flash_operation_wait(uint32_t counter)
  * @brief  Waits for a FLASH operation to complete or  delay timeout.
  * @param  counter: FLASH programming Timeout
  * @return flash status.
  */
flash_status_t flash_operation_wait(uint32_t counter)
{ 
    flash_status_t status = FLASH_STATUS_COMPLETE;
   
    /* Check for the FLASH Status */
    status = flash_status_get();
  
    /* Wait for a FLASH operation to complete or delay timeout */
    while((status == FLASH_STATUS_BUSY) && (counter != 0x00))
    {
        status = flash_status_get();
        counter--;
    }
  
    if(counter == 0x00 )
    {
        status = FLASH_STATUS_TIMEOUT;
    }
    /* Return the Flash status */
    return status;
}
 
  /**
  * @}
  */ 

/**
  * @}
  */ 

