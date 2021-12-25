/**
  * @file    cs32f0xx_flash.h
  * @brief   This file contains all the functions prototypes for the FLASH 
  *          firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_FLASH_H__
#define __CS32F0XX_FLASH_H__


#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup FLASH
  * @{
  */


/** 
  * @struct flash_status_t
  * @brief Flash operate Status 
  */ 
typedef enum
{
    FLASH_STATUS_BUSY = 1,        //!< Flash current status busy
    FLASH_STATUS_ERROR_WRP,       //!< Flash erase/program protection error
    FLASH_STATUS_ERROR_PROGRAM,   //!< Flash program error 
    FLASH_STATUS_COMPLETE,        //!< Flash Program complete
    FLASH_STATUS_TIMEOUT          //!< Flash operate timeout
}flash_status_t;

  
/**   
  * @brief FMC_WCR_WCNT
  */ 
#define FMC_WCR_WCNT_0                ((uint32_t)0x00000000)  //!< FLASH wait 0 cycle 
#define FMC_WCR_WCNT_1                FMC_WCR_WCNT            //!< FLASH wati 1 cycle

#define FMC_WCR_WCNT_CHECK(wcnt) (((wcnt) == FMC_WCR_WCNT_0) || \
                                  ((wcnt) == FMC_WCR_WCNT_1))
 
/**   
  * @brief  FLASH_Interrupts
  */   
#define FLASH_INT_END                   FMC_CTR_ENDIE  //!< End of operation interrupt
#define FLASH_INT_ERR                   FMC_CTR_ERRIE  //!< Error interrupt 
#define FLASH_INT_CHECK(flag) ((((flag) & (uint32_t)0xFFFFEBFF) == 0x00000000) && (((flag) != 0x00000000)))


/** @defgroup Flash_memory_address 
  * @{
  */
#if defined(CS32F031)      /*32K Flash devices */
    #define FLASH_PROGRAM_ADDRESS_CHECK(address) (((address) >= 0x08000000) && ((address) <= 0x08007FFF))
#elif defined(CS32F030)                           /*64K Flash devices */
    #define FLASH_PROGRAM_ADDRESS_CHECK(address) (((address) >= 0x08000000) && ((address) <= 0x0800FFFF))
#elif defined(CS32F036)                           /*32K Flash devices */
    #define FLASH_PROGRAM_ADDRESS_CHECK(address) (((address) >= 0x08000000) && ((address) <= 0x08007FFF))	
#else                         /*64K Flash Flash devices */
    #define FLASH_PROGRAM_ADDRESS_CHECK(address) (((address) >= 0x08000000) && ((address) <= 0x0800FFFF))
#endif 
/**
  * @}
  */

/** 
  * @brief Flash OB data address check
  */  
#define FLASH_OB_DATA_ADDRESS_CHECK(address) (((address) == 0x1FFFF804) || ((address) == 0x1FFFF806)) 

/** @defgroup FLASH_Option_Bytes_Write_Protection 
  * @{
  */  
#define OB_WPROT_PAGE0_TO_PAGE3               ((uint32_t)0x00000001) //!< Write protection of page 0 to 3 
#define OB_WPROT_PAGE4_TO_PAGE7               ((uint32_t)0x00000002) //!< Write protection of page 4 to 7 
#define OB_WPROT_PAGE8_TO_PAGE11              ((uint32_t)0x00000004) //!< Write protection of page 8 to 11 
#define OB_WPROT_PAGE12_TO_PAGE15             ((uint32_t)0x00000008) //!< Write protection of page 12 to 15 
#define OB_WPROT_PAGE16_TO_PAGE19             ((uint32_t)0x00000010) //!< Write protection of page 16 to 19 
#define OB_WPROT_PAGE20_TO_PAGE23             ((uint32_t)0x00000020) //!< Write protection of page 20 to 23 
#define OB_WPROT_PAGE24_TO_PAGE27             ((uint32_t)0x00000040) //!< Write protection of page 24 to 27 
#define OB_WPROT_PAGE28_TO_PAGE31             ((uint32_t)0x00000080) //!< Write protection of page 28 to 31 
#define OB_WPROT_PAGE32_TO_PAGE35             ((uint32_t)0x00000100) //!< Write protection of page 32 to 35 
#define OB_WPROT_PAGE36_TO_PAGE39             ((uint32_t)0x00000200) //!< Write protection of page 36 to 39 
#define OB_WPROT_PAGE40_TO_PAGE43             ((uint32_t)0x00000400) //!< Write protection of page 40 to 43 
#define OB_WPROT_PAGE44_TO_PAGE47             ((uint32_t)0x00000800) //!< Write protection of page 44 to 47 
#define OB_WPROT_PAGE48_TO_PAGE51             ((uint32_t)0x00001000) //!< Write protection of page 48 to 51 
#define OB_WPROT_PAGE52_TO_PAGE55             ((uint32_t)0x00002000) //!< Write protection of page 52 to 55 
#define OB_WPROT_PAGE56_TO_PAGE59             ((uint32_t)0x00004000) //!< Write protection of page 56 to 59 
#define OB_WPROT_PAGE60_TO_PAGE63             ((uint32_t)0x00008000) //!< Write protection of page 60 to 63 

#define OB_WPROT_ALL_PAGE                     ((uint32_t)0x0000FFFF) //!< Write protection of all Sectors 

#define OB_WPROT_CHECK(page) (((page) != 0x0000000))

/**
  * @}
  */


/** @defgroup FLASH_WPROT 
  * @{
  */
#define FLASH_WPROT_PAGE0_TO_3	         OB_WPROT_PAGE0_TO_PAGE3    //!< Write protection of page 0 to 3 
#define FLASH_WPROT_PAGE4_TO_7	         OB_WPROT_PAGE4_TO_PAGE7    //!< Write protection of page 4 to 7 
#define FLASH_WPROT_PAGE8_TO_11	         OB_WPROT_PAGE8_TO_PAGE11   //!< Write protection of page 8 to 11 
#define FLASH_WPROT_PAGE12_TO_15	     OB_WPROT_PAGE12_TO_PAGE15  //!< Write protection of page 12 to 15 
#define FLASH_WPROT_PAGE16_TO_19	     OB_WPROT_PAGE16_TO_PAGE19  //!< Write protection of page 16 to 19 
#define FLASH_WPROT_PAGE20_TO_23	     OB_WPROT_PAGE20_TO_PAGE23  //!< Write protection of page 20 to 23 
#define FLASH_WPROT_PAGE24_TO_27	     OB_WPROT_PAGE24_TO_PAGE27  //!< Write protection of page 24 to 27 
#define FLASH_WPROT_PAGE28_TO_31	     OB_WPROT_PAGE28_TO_PAGE31  //!< Write protection of page 28 to 31 
#define FLASH_WPROT_PAGE32_TO_35	     OB_WPROT_PAGE32_TO_PAGE35  //!< Write protection of page 32 to 35 
#define FLASH_WPROT_PAGE36_TO_39	     OB_WPROT_PAGE36_TO_PAGE39  //!< Write protection of page 36 to 39 
#define FLASH_WPROT_PAGE40_TO_43	     OB_WPROT_PAGE40_TO_PAGE21  //!< Invalid   
#define FLASH_WPROT_PAGE44_TO_47	     OB_WPROT_PAGE44_TO_PAGE23  //!< Invalid
#define FLASH_WPROT_PAGE48_TO_51	     OB_WPROT_PAGE48_TO_PAGE51  //!< Write protection of page 48 to 51 
#define FLASH_WPROT_PAGE52_TO_55	     OB_WPROT_PAGE52_TO_PAGE55  //!< Write protection of page 52 to 55 
#define FLASH_WPROT_PAGE56_TO_59	     OB_WPROT_PAGE56_TO_PAGE59  //!< Write protection of page 56 to 59 
#define FLASH_WPROT_PAGE60_TO_63	     OB_WPROT_PAGE60_TO_PAGE63  //!< Write protection of page 60 to 63 

#define FLASH_WPROT_ALL_PAGE             OB_WPROT_ALL_PAGE          //!< Write protection of all Sectors     
/**
  * @}
  */


/** 
  * @brief FLASH_Read Protection Level  
  */ 
#define OB_RPROT_LEVEL_0   ((uint8_t)0xAA)  //!< Flash read protection level 0
#define OB_RPROT_LEVEL_1   ((uint8_t)0xBB)  //!< Flash read protection level 1
#define OB_RPROT_LEVEL_2   ((uint8_t)0xCC)  //!< Flash read protection level 1
#define OB_RPROT_LEVEL_CHECK(level) ((((level) == OB_RPROT_LEVEL_0) || ((level) == OB_RPROT_LEVEL_1)) || ((level) == OB_RPROT_LEVEL_2))
                          

/** 
  * @brief Flash Option Bytes FWDT
  */   
#define OB_FWDT_SW                     ((uint8_t)0x01)  //!< Software FWDT selected 
#define OB_FWDT_HW                     ((uint8_t)0x00)  //!< Hardware FWDT selected 
#define OB_FWDT_SELECT_CHECK(source) (((source) == OB_FWDT_SW) || ((source) == OB_FWDT_HW))


/** 
  * @brief Flash Option Bytes STOP control by  reset select. 
  */    
#define OB_DSM_NO_RST                  ((uint8_t)0x02) //!< No reset generated when entering in DSM 
#define OB_DSM_RST                     ((uint8_t)0x00) //!< Reset generated when entering in DSM 
#define OB_DSM_RST_SELECT_CHECK(source) (((source) == OB_DSM_NO_RST) || ((source) == OB_DSM_RST))


/**  
  * @brief Flash Option Bytes STDBY control by  reset select.
  */
#define OB_PWD_NO_RST                 ((uint8_t)0x04) //!< No reset generated when entering in PWD
#define OB_PWD_RST                 ((uint8_t)0x00) //!< Reset generated when entering in PWD
#define OB_PWD_RST_SELECT_CHECK(source) (((source) == OB_PWD_NO_RST) || ((source) == OB_PWD_RST))


/** 
  * @brief Flash Option Bytes BOOT1 
  */
#define FLASH_OB_BOOT1_RESET                 ((uint8_t)0x00) //!< BOOT1 Reset 
#define FLASH_OB_BOOT1_SET                   ((uint8_t)0x10) //!< BOOT1 Set 
#define FLASH_OB_BOOT1_CHECK(boot1) (((boot1) == FLASH_OB_BOOT1_RESET) || ((boot1) == FLASH_OB_BOOT1_SET))

/** 
  * @brief Flash Option Bytes VDD detect 
  */
#define FLASH_OB_VDDA_ON              ((uint8_t)0x20) //!< Analog detecting on VDDA Power source ON 
#define FLASH_OB_VDDA_OFF             ((uint8_t)0x00) //!< Analog detecting on VDDA Power source OFF

#define FLASH_OB_VDDA_CHECK(analog) (((analog) == FLASH_OB_VDDA_ON) || ((analog) == FLASH_OB_VDDA_OFF))


/** 
  * @brief Flash Option Bytes SRAM Parity enable Set 
  */
#define FLASH_OB_SRAM_PARITY_SET              ((uint8_t)0x00) //!< SRAM parity enable Set 
#define FLASH_OB_SRAM_PARITY_RESET            ((uint8_t)0x40) //!< SRAM parity enable reset 


/** 
  * @brief Flash Option Bytes SRAM Parity check
  */
#define FLASH_OB_SRAM_PARITY_CHECK(parity) (((parity) == FLASH_OB_SRAM_PARITY_SET) || ((parity) == FLASH_OB_SRAM_PARITY_RESET))


/** 
  * @brief Flash status flag and status check
  */
#define FLASH_FLAG_BUSY               FMC_STS_BUSY   //!< FLASH busy flag 
#define FLASH_FLAG_PGERR              FMC_STS_PGERR  //!< FLASH programming error flag
#define FLASH_FLAG_WPERR              FMC_STS_WPERR  //!< FLASH write protected error flag 
#define FLASH_FLAG_ENDF               FMC_STS_ENDF   //!< FLASH end of programming flag


/** 
  * @brief Flash clear status flag check
  */ 
#define FLASH_CLEAR_FLAG_CHECK(flag) ((((flag) & (uint32_t)0xFFFFFFCB) == 0x00000000) && ((flag) != 0x00000000))

/** 
  * @brief Flash get status flag check
  */
#define FLASH_GET_FLAG_CHECK(flag)  (((flag) == FLASH_FLAG_BUSY) || ((flag) == FLASH_FLAG_PGERR) || \
                                     ((flag) == FLASH_FLAG_WPERR) || ((flag) == FLASH_FLAG_ENDF))

/** 
  * @brief Flash operate timeou value
  */ 
#define FLASH_OPERATE_TIMEOUT         ((uint32_t)0x000B0000)  //!< FLASH operate timeou 



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
void flash_wait_counter_set(uint32_t counter);

/**
  * @fn void flash_wait_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the FMC wait.
  * @param  enable_flag: It can be ENABLE or DISABLE .          
  * @return None
  */
void flash_wait_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn bit_status_t flash_wait_status_get(void)
  * @brief  Checks whether the FLASH wait status is set or not.
  * @param  None
  * @return Flash wait status (SET or RESET).
  */
bit_status_t flash_wait_status_get(void);


/**
  * @fn void flash_unlock(void)
  * @brief  Unlock the flash control register and program memory access.
  * @param  None
  * @return None
  */
void flash_unlock(void);

/**
  * @fn void flash_lock(void)
  * @brief  Lock the Program memory access.
  * @param  None
  * @return None
  */
void flash_lock(void);

/**
  * @fn flash_status_t flash_page_erase(uint32_t page_addr)
  * @brief  Erases a specified page in program memory. 
  *         The flash_unlock() function must be called before runing this function.
  * @param  page_addr: The page address in program memory to be erased.       
  * @return flash current status.         
  */
flash_status_t flash_page_erase(uint32_t page_addr);

/**
  * @fn flash_status_t flash_all_pages_erase(void)
  * @brief  Erases all flash pages.
  *         The flash_unlock() function must be called before runing this function.
  * @param  None
  * @return flash status.
  */
flash_status_t flash_all_pages_erase(void);

/**
  * @fn flash_status_t flash_word_program(uint32_t addr, uint32_t value)
  * @brief  Programs a word at a specified address.
  *         The flash_unlock() function must be called before runing this function.
  * @param  addr: specifies the address to be programmed.
  * @param  value: specifies the data to be programmed.
  * @return flash status. 
  */
flash_status_t flash_word_program(uint32_t addr, uint32_t value);

/**
  * @fn flash_status_t flash_half_word_program(uint32_t addr, uint16_t value)
  * @brief  Programs a half word at a specified address.
  *         The flash_unlock() function must be called before runing this function.
  * @param  addr: specifies the address to be programmed.
  * @param  value: specifies the data to be programmed.
  * @return flash status. 
  */
flash_status_t flash_half_word_program(uint32_t addr, uint16_t value);


/**
  * @fn void flash_option_bytes_unlock(void)
  * @brief  Unlocks OB (option bytes) block access.
  * @param  None
  * @return None
  */
void flash_option_bytes_unlock(void);

/**
  * @fn void flash_option_bytes_lock(void)
  * @brief  Locks OB (option bytes) block access
  * @param  None
  * @return None
  */
void flash_option_bytes_lock(void);

/**
  * @fn void flash_option_bytes_update(void)
  * @brief  Update OB (option bytes).
  * @param  None
  * @return None
  */
void flash_option_bytes_update(void);

/**
  * @fn flash_status_t flash_option_bytes_erase(void)
  * @brief  Erases all OB (option bytes) except the Read protection (RPROT).
  *         The flash_option_bytes_unlock() function must be called before runing this function.   
  * @param  None
  * @return flash status.
  */
flash_status_t flash_option_bytes_erase(void);

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
flash_status_t flash_option_bytes_write_protect_enable(uint32_t set_value);

/**
  * @fn flash_status_t flash_option_bytes_read_protect_config(uint8_t set_level)
  * @brief  Enables or disables RPROT (the read out protection).
  *         The flash_option_bytes_unlock() function must be called before runing this function. 
  * @param  set_level: flash read protection level, it can be:
  *          @arg OB_RPROT_LEVEL_0(No protection)
  *          @arg OB_RPROT_LEVEL_1(Read protection of the memory)
  *          @arg OB_RDP_Level_2(Chip protection)
  * @note   When enabling set level 2, it's no more possible to go back to level 1 or 0
  * @return flash status. 
  */
flash_status_t flash_option_bytes_read_protect_config(uint8_t set_level);

/**
  * @fn flash_status_t flash_option_bytes_user_config(uint8_t fwdt_value, uint8_t dsm_value, uint8_t pwd_value) 
  * @brief  Programs the FLASH User Option Byte: FWDT_SW / RST_STOP / RST_STDBY.
  *         The flash_option_bytes_unlock() function must be called before runing this function. 
  *         Call the flash_option_bytes_lock() to disable the flash control register access and the option bytes.
  * @param  fwdt_value: Selects the FWDT mode, it can be:
  *          @arg OB_FWDT_SW(Software FWDT selected)
  *          @arg OB_FWDT_HW(Hardware FWDT selected)          
  * @param  dsm_value: Reset event when entering DSM mode. It can be: 
  *          @arg OB_DSM_NO_RST(No reset generated when entering in STOP)
  *          @arg OB_DSM_RST(Reset generated when entering in STOP)
  * @param  pwd_value: Reset event when entering PWD mode. It can be: 
  *          @arg OB_PWD_NO_RST(No reset generated when entering in STANDBY)
  *          @arg OB_PWD_RST(Reset generated when entering in STANDBY)
  * @return Flash status.
  */
flash_status_t flash_option_bytes_user_config(uint8_t fwdt_value, uint8_t dsm_value, uint8_t pwd_value);

/**
  * @fn flash_status_t flash_option_bytes_boot_config(uint8_t set_value)
  * @brief  Sets or resets the BOOT1 option bit.
  * @param  set_value: it can be:  
  *          @arg FLASH_OB_BOOT1_RESET(BOOT1 option bit reset)
  *          @arg FLASH_OB_BOOT1_SET(BOOT1 option bit set)
  * @return flash status.
  */
flash_status_t flash_option_bytes_boot_config(uint8_t set_value);

/**
  * @fn flash_status_t flash_option_bytes_vdda_config(uint8_t set_value)
  * @brief  Sets or resets the analogue detection on VDDA Power source.
  * @param  set_value: it can be:
  *          @arg FLASH_OB_VDDA_ON(VDDA Power source ON)
  *          @arg FLASH_OB_VDDA_OFF(VDDA Power source OFF)
  * @return flash status.
  */
flash_status_t flash_option_bytes_vdda_config(uint8_t set_value);

/**
  * @fn flash_status_t flash_option_bytes_sram_parity_config(uint8_t set_value)
  * @brief  Sets or resets the SRAM parity.
  * @param  set_value: Set or Reset the SRAM parity enable bit.
  *         It can be one of the following values:
  *          @arg FLASH_OB_SRAM_PARITY_SET(Set SRAM parity)
  *          @arg FLASH_OB_SRAM_PARITY_RESET(Reset SRAM parity)
  * @return flash status.
  */
flash_status_t flash_option_bytes_sram_parity_config(uint8_t set_value);

/**
  * @fn flash_status_t flash_option_bytes_user_write(uint8_t set_value)
  * @brief  Programs the FLASH User Option Byte, It can be FWDT_SW, RST_STOP, RST_STDBY,
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
flash_status_t flash_option_bytes_user_write(uint8_t set_value);

/**
  * @fn flash_status_t flash_option_bytes_data_program(uint32_t addr, uint8_t set_value) 
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   The flash_option_bytes_unlock() function must be called before runing this function.
  * @note   Call the flash_option_bytes_lock() to disable the flash register access and the option bytes.   
  * @param  addr: specifies the address to be programmed can be 0x1FFFF804 or 0x1FFFF806.
  * @param  set_value: specifies the data to be programmed.
  * @return flash status.
  */
flash_status_t flash_option_bytes_data_program(uint32_t addr, uint8_t set_value);

/**
  * @fn uint8_t flash_option_bytes_user_get(void)
  * @brief  Get the FLASH User Option Bytes values.
  * @param  None
  * @return The flash User Option Bytes .
  */
uint8_t flash_option_bytes_user_get(void);

/**
  * @fn uint32_t flash_option_bytes_write_protect_get(void)
  * @brief  Get the flash Write Protection Option Bytes value.
  * @param  None
  * @return Flash Write Protection Option Bytes register value(WPSTS).
  */
uint32_t flash_option_bytes_write_protect_get(void);

/**
  * @fn  bit_status_t flash_option_bytes_read_protect_get(void)
  * @brief  Checks whether the FLASH Read out Protection Status is set or not.
  * @param  None
  * @return status(SET or RESET)
  */
bit_status_t flash_option_bytes_read_protect_get(void);

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
void flash_interrupt_config(uint32_t set_value, enable_state_t enable_flag);

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
bit_status_t flash_flag_status_get(uint32_t status_flag);

/**
  * @fn void flash_flag_clear(uint32_t status_flag)
  * @brief  Clears the FLASH's flags.
  * @param  status_flag: specifies the FLASH flags to clear.It can be 
  *          @arg FLASH_FLAG_PGERR(FLASH Programming error flag flag)
  *          @arg FLASH_FLAG_WPERR(FLASH Write protected error flag)
  *          @arg FLASH_FLAG_ENDF(FLASH End of Programming flag)
  * @return None
  */
void flash_flag_clear(uint32_t status_flag);

/**
  * @fn flash_status_t flash_status_get(void)
  * @brief  Returns the flash Status.
  * @param  None
  * @return flash status.
  */
flash_status_t flash_status_get(void);

/**
  * @fn flash_status_t flash_operation_wait(uint32_t counter)
  * @brief  Waits for a FLASH operation to complete or  delay timeout.
  * @param  counter: FLASH programming Timeout
  * @return flash status.
  */
flash_status_t flash_operation_wait(uint32_t counter);
 
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

