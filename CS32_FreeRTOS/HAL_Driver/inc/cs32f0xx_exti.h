/**
  * @file    cs32f0xx_exti.h
  * @brief   This file contains all the functions prototypes for the EXTI firmware library
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_EXTI_H__
#define __CS32F0XX_EXTI_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"


/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup EXTI
  * @{
  */


/** 
  * @brief EXTI mode enumeration  
  */
typedef enum
{
    EXTI_MODE_INTR = 0x00,     //<! EXTI interrupt mode
    EXTI_MODE_EVENT = 0x04     //<! EXTI event mode
}exti_mode_t;

#define EXTI_MODE_CHECK(mode) (((mode) == EXTI_MODE_INTR) || \
                               ((mode) == EXTI_MODE_EVENT))

/** 
  * @brief EXTI trigger enumeration  
  */
typedef enum
{
    EXTI_TRIGGER_RISING = 0x08,            //<! External interrupt trigger (rising signal)
    EXTI_TRIGGER_FALLING = 0x0C,           //<! External interrupt trigger (falling signal)
    EXTI_TRIGGER_RISING_FALLING = 0x10     //<! External interrupt trigger (rising_falling signal)
}exti_trigger_t;

#define EXTI_TRIGGER_EDGE_CHECK(flag) (((flag) == EXTI_TRIGGER_RISING) || \
                                       ((flag) == EXTI_TRIGGER_FALLING)|| \
                                       ((flag) == EXTI_TRIGGER_RISING_FALLING))

/**
  * @brief  EXTI Init Structure definition
  */
typedef struct
{
    uint32_t exti_line;               //<! Specifies the EXTI lines to be enabled or disabled
    exti_mode_t exti_mode;            //<! Specifies the mode for the EXTI lines
    exti_trigger_t exti_trigger;      //<! Specifies the trigger signal active edge for the EXTI lines
    enable_state_t exti_line_cmd;     //<! Specifies the new state of the selected EXTI lines
    uint8_t reserved0;
}exti_config_t;

/** @defgroup EXTI_Lines 
  * @{
  */
#define EXTI_LINE_0       ((uint32_t)0x00000001)  //<! External interrupt line 0  
#define EXTI_LINE_1       ((uint32_t)0x00000002)  //<! External interrupt line 1  
#define EXTI_LINE_2       ((uint32_t)0x00000004)  //<! External interrupt line 2  
#define EXTI_LINE_3       ((uint32_t)0x00000008)  //<! External interrupt line 3  
#define EXTI_LINE_4       ((uint32_t)0x00000010)  //<! External interrupt line 4 
#define EXTI_LINE_5       ((uint32_t)0x00000020)  //<! External interrupt line 5  
#define EXTI_LINE_6       ((uint32_t)0x00000040)  //<! External interrupt line 6 
#define EXTI_LINE_7       ((uint32_t)0x00000080)  //<! External interrupt line 7  
#define EXTI_LINE_8       ((uint32_t)0x00000100)  //<! External interrupt line 8  
#define EXTI_LINE_9       ((uint32_t)0x00000200)  //<! External interrupt line 9  
#define EXTI_LINE_10      ((uint32_t)0x00000400)  //<! External interrupt line 10
#define EXTI_LINE_11      ((uint32_t)0x00000800)  //<! External interrupt line 11 
#define EXTI_LINE_12      ((uint32_t)0x00001000)  //<! External interrupt line 12 
#define EXTI_LINE_13      ((uint32_t)0x00002000)  //<! External interrupt line 13 
#define EXTI_LINE_14      ((uint32_t)0x00004000)  //<! External interrupt line 14 
#define EXTI_LINE_15      ((uint32_t)0x00008000)  //<! External interrupt line 15
#define EXTI_LINE_16      ((uint32_t)0x00010000)  //<! External interrupt line 16 
                                                     
#define EXTI_LINE_17      ((uint32_t)0x00020000)  //<! Internal interrupt line 17 connected to the RTC Alarm event                                                                                                             
#define EXTI_LINE_19      ((uint32_t)0x00080000)  //<! Internal interrupt line 19                                                                                                          
#define EXTI_LINE_20      ((uint32_t)0x00100000)  //<! Internal interrupt line 20                                                                                                           
#define EXTI_LINE_21      ((uint32_t)0x00200000)  //<! Internal interrupt line 21                                                   
#define EXTI_LINE_22      ((uint32_t)0x00400000)  //<! Internal interrupt line 22                                                  
#define EXTI_LINE_23      ((uint32_t)0x00800000)  //<! Internal interrupt line 23                                                 
#define EXTI_LINE_25      ((uint32_t)0x02000000)  //<! Internal interrupt line 25                                                 
#define EXTI_LINE_26      ((uint32_t)0x04000000)  //<! Internal interrupt line 26                                             
#define EXTI_LINE_27      ((uint32_t)0x08000000)  //<! Internal interrupt line 27                                                     
#define EXTI_LINE_31      ((uint32_t)0x80000000)  //<! Internal interrupt line 31
                                                     
#define EXTI_LINE_CHECK(line) ((((line) & (uint32_t)0x71000000) == 0x00) && ((line) != (uint16_t)0x00))

#define EXTI_LINE_GET_CHECK(line) (((line) == EXTI_LINE_0)  || ((line) == EXTI_LINE_1)  || \
                                   ((line) == EXTI_LINE_2)  || ((line) == EXTI_LINE_3)  || \
                                   ((line) == EXTI_LINE_4)  || ((line) == EXTI_LINE_5)  || \
                                   ((line) == EXTI_LINE_6)  || ((line) == EXTI_LINE_7)  || \
                                   ((line) == EXTI_LINE_8)  || ((line) == EXTI_LINE_9)  || \
                                   ((line) == EXTI_LINE_10) || ((line) == EXTI_LINE_11) || \
                                   ((line) == EXTI_LINE_12) || ((line) == EXTI_LINE_13) || \
                                   ((line) == EXTI_LINE_14) || ((line) == EXTI_LINE_15) || \
                                   ((line) == EXTI_LINE_16) || ((line) == EXTI_LINE_17) || \
                                   ((line) == EXTI_LINE_19) || ((line) == EXTI_LINE_20) ||\
                                   ((line) == EXTI_LINE_22)|| ((line) == EXTI_LINE_21) || \
                                   ((line) == EXTI_LINE_23) || ((line) == EXTI_LINE_25) || \
                                   ((line) == EXTI_LINE_26) || ((line) == EXTI_LINE_27)|| \
                                   ((line) == EXTI_LINE_31))
/**
  * @}
  */



/**
  * @fn void exti_def_init(void)
  * @brief  Deinitializes the EXTI peripheral registers to their default reset values.
  * @param  None
  * @return None
  */
void exti_def_init(void);

/**
  * @fn void exti_init(exti_config_t* ptr_config)
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the ptr_config.
  * @param  ptr_config: pointer to a exti_config_t structure that contains the 
  *         configuration for the EXTI peripheral.
  * @return None
  */
void exti_init(exti_config_t* ptr_config);

/**
  * @fn void exti_struct_init(exti_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its reset value.
  * @param  ptr_config: pointer to a exti_config_t structure which will be initialized
  * @return None
  */
void exti_struct_init(exti_config_t* ptr_config);

/**
  * @fn void exti_sw_interrupt_trigger(uint32_t value)
  * @brief  Generates a Software interrupt on selected EXTI line.
  * @param  value: specifies the EXTI line on which the software interrupt will be generated.
  *         It can be any combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_sw_interrupt_trigger(uint32_t value);

/**
  * @fn bit_status_t exti_flag_status_get(uint32_t value)
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  value: specifies the EXTI line flag to check, it can be:
  *          EXTI_Linex where x can be (0~27).
  * @return SET or RESET
  */
bit_status_t exti_flag_status_get(uint32_t value);

/**
  * @fn void exti_flag_clear(uint32_t value)
  * @brief  Clears the EXTI's line flags.
  * @param  value: specifies the EXTI lines flags to clear, it can be any
  *         combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_flag_clear(uint32_t value);

/**
  * @fn bit_status_t exti_interrupt_status_get(uint32_t value)
  * @brief  Checks whether the specified EXTI line is set or not.
  * @param  value: specifies the EXTI line to check, it can be 
  *         EXTI_Linex where x can be (0~27).
  * @return SET or RESET.
  */
bit_status_t exti_interrupt_status_get(uint32_t value);

/**
  * @fn void exti_interrupt_status_clear(uint32_t value)
  * @brief  Clears the EXTI's line  bits.
  * @param  value: specifies the EXTI lines to clear, it can be any
  *         combination of EXTI_Linex where x can be (0~27).
  * @return None
  */
void exti_interrupt_status_clear(uint32_t value);

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

