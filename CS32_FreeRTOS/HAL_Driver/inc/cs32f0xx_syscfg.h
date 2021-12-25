/**
  * @file    cs32f0xx_syscfg.h
  * @brief   This file contains all the functions prototypes for the SYSCFG firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_SYSCFG_H__
#define __CS32F0XX_SYSCFG_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup SYSCFG
  * @{
  */

  
/** 
  * @brief SYSCFG EXTI Port Sources definition
  */ 
#define SYSCFG_EXTI_PORT_PA       ((uint8_t)0x00) //!< External interrupt port source from GPIO A 
#define SYSCFG_EXTI_PORT_PB       ((uint8_t)0x01) //!< External interrupt port source from GPIO B
#define SYSCFG_EXTI_PORT_PC       ((uint8_t)0x02) //!< External interrupt port source from GPIO C
#define SYSCFG_EXTI_PORT_PD       ((uint8_t)0x03) //!< External interrupt port source from GPIO D
#define SYSCFG_EXTI_PORT_PE       ((uint8_t)0x04) //!< External interrupt port source from GPIO E
#define SYSCFG_EXTI_PORT_PF       ((uint8_t)0x05) //!< External interrupt port source from GPIO F

#define SYSCFG_EXTI_PORT_CHECK(port) (((port) == SYSCFG_EXTI_PORT_PA) || ((port) == SYSCFG_EXTI_PORT_PB) || \
                                         ((port) == SYSCFG_EXTI_PORT_PC) || ((port) == SYSCFG_EXTI_PORT_PD) || \
                                         ((port) == SYSCFG_EXTI_PORT_PE) || ((port) == SYSCFG_EXTI_PORT_PF))

/**  
  * @brief SYSCFG EXTI pin Sources definition
  */ 
#define SYSCFG_EXTI_PIN_0            ((uint8_t)0x00) //!< External interrupt from pin 0
#define SYSCFG_EXTI_PIN_1            ((uint8_t)0x01) //!< External interrupt from pin 1
#define SYSCFG_EXTI_PIN_2            ((uint8_t)0x02) //!< External interrupt from pin 2
#define SYSCFG_EXTI_PIN_3            ((uint8_t)0x03) //!< External interrupt from pin 3
#define SYSCFG_EXTI_PIN_4            ((uint8_t)0x04) //!< External interrupt from pin 4
#define SYSCFG_EXTI_PIN_5            ((uint8_t)0x05) //!< External interrupt from pin 5
#define SYSCFG_EXTI_PIN_6            ((uint8_t)0x06) //!< External interrupt from pin 6
#define SYSCFG_EXTI_PIN_7            ((uint8_t)0x07) //!< External interrupt from pin 7
#define SYSCFG_EXTI_PIN_8            ((uint8_t)0x08) //!< External interrupt from pin 8
#define SYSCFG_EXTI_PIN_9            ((uint8_t)0x09) //!< External interrupt from pin 9
#define SYSCFG_EXTI_PIN_10           ((uint8_t)0x0A) //!< External interrupt from pin 10
#define SYSCFG_EXTI_PIN_11           ((uint8_t)0x0B) //!< External interrupt from pin 11
#define SYSCFG_EXTI_PIN_12           ((uint8_t)0x0C) //!< External interrupt from pin 12
#define SYSCFG_EXTI_PIN_13           ((uint8_t)0x0D) //!< External interrupt from pin 13
#define SYSCFG_EXTI_PIN_14           ((uint8_t)0x0E) //!< External interrupt from pin 14
#define SYSCFG_EXTI_PIN_15           ((uint8_t)0x0F) //!< External interrupt from pin 15

#define SYSCFG_EXTI_PIN_CHECK(pin) (((pin) == SYSCFG_EXTI_PIN_0) || ((pin) == SYSCFG_EXTI_PIN_1) || \
                                       ((pin) == SYSCFG_EXTI_PIN_2)  ||  ((pin) == SYSCFG_EXTI_PIN_3)  || \
                                       ((pin) == SYSCFG_EXTI_PIN_4)  ||  ((pin) == SYSCFG_EXTI_PIN_5)  || \
                                       ((pin) == SYSCFG_EXTI_PIN_6)  ||  ((pin) == SYSCFG_EXTI_PIN_7)  || \
                                       ((pin) == SYSCFG_EXTI_PIN_8)  ||  ((pin) == SYSCFG_EXTI_PIN_9)  || \
                                       ((pin) == SYSCFG_EXTI_PIN_10) ||  ((pin) == SYSCFG_EXTI_PIN_11) || \
                                       ((pin) == SYSCFG_EXTI_PIN_12) ||  ((pin) == SYSCFG_EXTI_PIN_13) || \
                                       ((pin) == SYSCFG_EXTI_PIN_14) ||  ((pin) == SYSCFG_EXTI_PIN_15))
                                       
/** 
  * @brief SYSCFG Memory Remap definition
  */ 
#define SYSCFG_MEM_REMAP_FLASH              ((uint8_t)0x00) //!< Remap main flash memory 
#define SYSCFG_MEM_REMAP_SYS_MEMORY         ((uint8_t)0x01) //!< Remap system flash memory 
#define SYSCFG_MEM_REMAP_SRAM               ((uint8_t)0x03) //!< Remap embedded SRAM 

#define SYSCFG_MEM_REMAP_CHECK(flag) (((flag) == SYSCFG_MEM_REMAP_FLASH) || ((flag) == SYSCFG_MEM_REMAP_SYS_MEMORY) || \
                                         ((flag) == SYSCFG_MEM_REMAP_SRAM))

/** 
  * @brief SYSCFG DMA remap config definition
  */ 
#define SYSCFG_DMA_REMAP_TIM3        SYSCFG_RMAPCFG_TIM3_DMA             //!< Remap TIM3 DMA requests from channel 4 to channel 6 
#define SYSCFG_DMA_REMAP_TIM2        SYSCFG_RMAPCFG_TIM2_DMA             //!< Remap TIM2 DMA requests from channel 3/4 to channel 7
#define SYSCFG_DMA_REMAP_TIM1        SYSCFG_RMAPCFG_TIM1_DMA             //!< Remap TIM1 DMA requests from channel 2/3/4 to channel 6 
#define SYSCFG_DMA_REMAP_I2C1        SYSCFG_RMAPCFG_I2C1_DMA             //!< Remap I2C1 DMA requests from channel 3/2 to channel 7/6
#define SYSCFG_DMA_REMAP_USART3      SYSCFG_RMAPCFG_USART3_DMA           //!< Remap USART3 DMA requests from channel 6/7 to channel 3/2
#define SYSCFG_DMA_REMAP_USART2      SYSCFG_RMAPCFG_USART2_DMA           //!< Remap USART2 DMA requests from channel 4/5 to channel 6/7
#define SYSCFG_DMA_REMAP_SPI2        SYSCFG_CFGR1_SPI2_DMA_RMP           //!< Remap SPI2 DMA requests from channel 4/5 to channel 6/7
#define SYSCFG_DMA_REMAP_TIM17_2     SYSCFG_RMAPCFG_TIM17_DMA_CHRMAP2    //!< Remap TIM17 DMA requests from channel 1/2 to channel 7
#define SYSCFG_DMA_REMAP_TIM16_2     SYSCFG_RMAPCFG_TIM16_DMA_CHRMAP2    //!< Remap TIM16 DMA requests from channel 3/4 to channel 6
#define SYSCFG_DMA_REMAP_TIM17       SYSCFG_RMAPCFG_TIM17_DMA_CHRMAP     //!< Remap TIM17 DMA requests from channel 1 to channel 2 
#define SYSCFG_DMA_REMAP_TIM16       SYSCFG_RMAPCFG_TIM16_DMA_CHRMAP     //!< Remap TIM16 DMA requests from channel 3 to channel 4
#define SYSCFG_DMA_REMAP_USART1RX    SYSCFG_RMAPCFG_USART1RX_DMA_CHRMAP  //!< Remap USART1 Rx DMA requests from channel 3 to channel 5
#define SYSCFG_DMA_REMAP_USART1TX    SYSCFG_RMAPCFG_USART1TX_DMA_CHRMAP  //!< Remap USART1 Tx DMA requests from channel 2 to channel 4
#define SYSCFG_DMA_REMAP_ADC1        SYSCFG_RMAPCFG_ADC_DMA_CHRMAP       //!< Remap ADC1 DMA requests from channel 1 to channel 2

#define SYSCFG_DMA_REMAP_CHECK(flag) (((flag) == SYSCFG_DMA_REMAP_TIM17) || ((flag) == SYSCFG_DMA_REMAP_TIM16) || \
                                        ((flag) == SYSCFG_DMA_REMAP_USART1RX)||  ((flag) == SYSCFG_DMA_REMAP_USART1TX) || \
                                        ((flag) == SYSCFG_RMAPCFG_TIM3_DMA)  ||  ((flag) == SYSCFG_RMAPCFG_TIM2_DMA)   || \
                                        ((flag) == SYSCFG_RMAPCFG_TIM1_DMA)  ||  ((flag) == SYSCFG_RMAPCFG_I2C1_DMA)   || \
                                        ((flag) == SYSCFG_RMAPCFG_USART3_DMA)||  ((flag) == SYSCFG_RMAPCFG_USART2_DMA) || \
                                        ((flag) == SYSCFG_CFGR1_SPI2_DMA_RMP)||  ((flag) == SYSCFG_RMAPCFG_TIM17_DMA_CHRMAP2) || \
                                        ((flag) == SYSCFG_DMA_REMAP_ADC1))   ||  ((flag) == SYSCFG_RMAPCFG_TIM16_DMA_CHRMAP2)                           

/** 
  * @brief SYSCFG Lock Config
  */ 
#define SYSCFG_TIM_BREAK_LVD            SYSCFG_ERRLOCK_LVD_TIMBRK_LOCK        //!< Connects the LVD event to TIM1 break enable, not available for CS32F030 devices 
#define SYSCFG_TIM_BREAK_SRAM_PARITY    SYSCFG_ERRLOCK_SRAM_PRTY_TIMBRK_LOCK  //!< Connects the SRAM_PARITY error signal to TIM1 break enable
#define SYSCFG_TIM_BREAK_LOCKUP         SYSCFG_ERRLOCK_LOCKUP_TIMBRK_LOCK     //!< Connects Lockup output of CortexM0 to TIM1 break enable 

#define SYSCFG_TIM_BREAK_CHECK(flag) (((flag) == SYSCFG_TIM_BREAK_LVD)|| ((flag) == SYSCFG_TIM_BREAK_SRAM_PARITY)|| \
                                          ((flag) == SYSCFG_TIM_BREAK_LOCKUP))

/**  
  * @brief SYSCFG SRAM PE(parity error) flag definition
  */
#define SYSCFG_FLAG_PE             SYSCFG_ERRLOCK_SRAM_PRTY_ERR  //!< SRAM parity error flag
#define SYSCFG_FLAG_CHECK(flag) (((flag) == SYSCFG_FLAG_PE))

/** 
  * @brief IRDA ENV SEL definition
  */
#define SYSCFG_IRDA_ENV_SEL_TIM16     (SYSCFG_RMAPCFG_IRDA_ENV_SEL_0 & SYSCFG_RMAPCFG_IRDA_ENV_SEL_1)    //!< Timer16 is selected as IRDA Modulation envelope source
#define SYSCFG_IRDA_ENV_SEL_USART1    (SYSCFG_RMAPCFG_IRDA_ENV_SEL_0)  //!< USART1 is selected as IRDA Modulation envelope source
#define SYSCFG_IRDA_ENV_SEL_USART4    (SYSCFG_RMAPCFG_IRDA_ENV_SEL_1)  //!< USART4 is selected as IRDA Modulation envelope source

#define SYSCFG_IRDA_ENV_SEL(flag) (((flag) == SYSCFG_IRDA_ENV_SEL_TIM16) || ((flag) == SYSCFG_IRDA_ENV_SEL_USART1) || \
                                    ((flag) == SYSCFG_IRDA_ENV_SEL_USART4))




/**
  * @fn void syscfg_def_init(void)
  * @brief  Deinitializes the SYSCFG registers to their default reset values.
  * @note   MEM_REMAP and ERRLOCK are not affected by APB reset.
  * @param  None
  * @return None
  */
void syscfg_def_init(void);

/**
  * @fn void syscfg_memory_remap_set(uint32_t value)
  * @brief  Set the memory mapping at address 0x00000000.
  * @param  value: selects the memory remapping, it can be:
  *          @arg SYSCFG_MEM_REMAP_FLASH(Main Flash memory mapped at 0x00000000)
  *          @arg SYSCFG_MEM_REMAP_SYS_MEMORY(System Flash memory mapped at 0x00000000)
  *          @arg SYSCFG_MEM_REMAP_SRAM(Embedded SRAM mapped at 0x00000000)
  * @return None
  */
void syscfg_memory_remap_set(uint32_t value);

/**
  * @fn void syscfg_dma_channel_remap_config(uint32_t value, enable_state_t enable_flag)
  * @brief  Configure the DMA channels remapping.
  * @param  value: selects the DMA channels remap, It can be
  *          @arg SYSCFG_DMA_REMAP_TIM17(Remap TIM17 DMA from channel 1 to channel 2)
  *          @arg SYSCFG_DMA_REMAP_TIM16(Remap TIM16 DMA from channel 3 to channel 4)
  *          @arg SYSCFG_DMA_REMAP_USART1RX(Remap USART1 Rx DMA from channel 3 to channel 5)
  *          @arg SYSCFG_DMA_REMAP_USART1TX(Remap USART1 Tx DMA from channel 2 to channel 4)
  *          @arg SYSCFG_DMA_REMAP_ADC1(Remap ADC1 DMA requests from channel 1 to channel 2)
  * @param  enable_flag: ENABLE or DISABLE the DMA channel remapping. 
  * @return None
  */
void syscfg_dma_channel_remap_config(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void syscfg_exti_line_config(uint8_t port, uint8_t pin)
  * @brief  Selects the GPIO pin used as EXTI Line.
  * @param  port: selects the GPIO port to be used as source for EXTI lines                    
  * @param  pin: specifies the EXTI line to be configured.
  * @return None
  */
void syscfg_exti_line_config(uint8_t port, uint8_t pin);

/**
  * @fn void syscfg_time_break_config(uint32_t value)
  * @brief  Connect the selected parameter to the TIM1 break.
  * @note   The selected configuration is locked and can be unlocked by system reset
  * @param  value: selects the configuration to be connected to TIM1 break. It can be 
  *          @arg SYSCFG_TIM_BREAK_LVD (Connects the LVD event to TIM1 break enable)
  *          @arg SYSCFG_TIM_BREAK_SRAM_PARITY(Connects the SRAM_PARITY error signal to TIM1 break enable)
  *          @arg SYSCFG_TIM_BREAK_LOCKUP(Connects Lockup output of CortexM0 to TIM1 break enable)
  * @return None
  */
void syscfg_time_break_config(uint32_t value);

/**
  * @fn bit_status_t syscfg_flag_status_get(uint32_t flag)
  * @brief   Checks whether the specified SYSCFG flag is set or not.
  * @param   flag: specifies the SYSCFG flag to check. It can be
  *          @arg SYSCFG_FLAG_PE(SRAM parity error flag)
  * @return  state SET or RESET.
  */
bit_status_t syscfg_flag_status_get(uint32_t flag);

/**
  * @fn void syscfg_flag_clear(uint32_t flag)
  * @brief  Clear the selected SYSCFG flag.
  * @param  flag: selects the flag to be cleared, it can be 
  *          @arg SYSCFG_FLAG_PE(SRAM parity error flag)
  * @return None
  */
void syscfg_flag_clear(uint32_t flag);
#define UNUSED(x) (void)x
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

