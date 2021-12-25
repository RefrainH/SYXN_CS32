/**
  * @file    cs32f0xx_rcu.h
  * @brief   This file contains all the functions prototypes for the RCU 
  *          firmware library.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_RCU_H__
#define __CS32F0XX_RCU_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup RCU
  * @{
  */


/** 
  * @struct rcu_clock_t
  * @brief RCU clock structures definition  
  */ 
typedef struct
{
    uint32_t system_clk_freq;  //!< System clock frequency
    uint32_t hclk_freq;        //!< HCLK frequency
    uint32_t pclk_freq;        //!< PCLK frequency
    uint32_t adc_clk_freq;     //!< Adc clock frequency
    uint32_t cec_clk_freq;     //!< Cec clock frequency
    uint32_t i2c1_clk_freq;    //!< I2C1 clock frequency
    uint32_t usart1_clk_freq;  //!< Usart1 clock frequency
    uint32_t usart2_clk_freq;  //!< Usart2 clock frequency
    uint32_t usart3_clk_freq;  //!< Usart3 clock frequency
    uint32_t usb_clk_freq;     //!< Usb clock frequency
}rcu_clock_t;



/** 
  * @brief HXT clock definition  
  */
#define RCU_HXT_OFF           ((uint8_t)0x00)  //!< RCU HXT clock source is OFF
#define RCU_HXT_ON            ((uint8_t)0x01)  //!< RCU HXT clock source is ON 
#define RCU_HXT_BYPASS        ((uint8_t)0x05)  //!< RCU HXT clock source is BYPASSED
#define RCU_HXT_CHECK(clk)    (((clk) == RCU_HXT_OFF) || \
                               ((clk) == RCU_HXT_ON)  || \
                               ((clk) == RCU_HXT_BYPASS))
 
/** 
  * @brief PLL clock definition  
  */
#define RCU_PLL_CFG_HRC_DIV2      RCU_CFG_PLLSEL_HRC_DIV2       //!< RCU PLL clock source selection is HRC_DIV2
#define RCU_PLL_CFG_HXT           RCU_CFG_PLLSEL_HXT_PREDIV     //!< RCU PLL clock source selection is HXT
#define RCU_PLL_CFG_HRC           RCU_CFG_PLLSEL_HRC_PREDIV     //!< RCU PLL clock source selection is HRC

#define RCU_PLL_CFG_CHECK(clk) (((clk) == RCU_PLL_CFG_HRC_DIV2) || \
                                ((clk) == RCU_PLL_CFG_HRC)      || \
                                ((clk) == RCU_PLL_CFG_HXT))
                                  
/**
  * @brief RCU PLL multiplication factor definition
  */
#define RCU_PLL_MULTI_2            RCU_CFG_PLLMUF2  //!< RCU PLL clock multiplication factor is 2
#define RCU_PLL_MULTI_3            RCU_CFG_PLLMUF3  //!< RCU PLL clock multiplication factor is 3
#define RCU_PLL_MULTI_4            RCU_CFG_PLLMUF4  //!< RCU PLL clock multiplication factor is 4
#define RCU_PLL_MULTI_5            RCU_CFG_PLLMUF5  //!< RCU PLL clock multiplication factor is 5
#define RCU_PLL_MULTI_6            RCU_CFG_PLLMUF6  //!< RCU PLL clock multiplication factor is 6
#define RCU_PLL_MULTI_7            RCU_CFG_PLLMUF7  //!< RCU PLL clock multiplication factor is 7
#define RCU_PLL_MULTI_8            RCU_CFG_PLLMUF8  //!< RCU PLL clock multiplication factor is 8
#define RCU_PLL_MULTI_9            RCU_CFG_PLLMUF9  //!< RCU PLL clock multiplication factor is 9
#define RCU_PLL_MULTI_10           RCU_CFG_PLLMUF10 //!< RCU PLL clock multiplication factor is 10
#define RCU_PLL_MULTI_11           RCU_CFG_PLLMUF11 //!< RCU PLL clock multiplication factor is 11
#define RCU_PLL_MULTI_12           RCU_CFG_PLLMUF12 //!< RCU PLL clock multiplication factor is 12
#define RCU_PLL_MULTI_13           RCU_CFG_PLLMUF13 //!< RCU PLL clock multiplication factor is 13
#define RCU_PLL_MULTI_14           RCU_CFG_PLLMUF14 //!< RCU PLL clock multiplication factor is 14
#define RCU_PLL_MULTI_15           RCU_CFG_PLLMUF15 //!< RCU PLL clock multiplication factor is 15
#define RCU_PLL_MULTI_16           RCU_CFG_PLLMUF16 //!< RCU PLL clock multiplication factor is 16

#define RCU_PLL_MULTI_CHECK(value) (((value) == RCU_PLL_MULTI_2)  || ((value) == RCU_PLL_MULTI_3)  || \
                                    ((value) == RCU_PLL_MULTI_4)  || ((value) == RCU_PLL_MULTI_5)  || \
                                    ((value) == RCU_PLL_MULTI_6)  || ((value) == RCU_PLL_MULTI_7)  || \
                                    ((value) == RCU_PLL_MULTI_8)  || ((value) == RCU_PLL_MULTI_9)  || \
                                    ((value) == RCU_PLL_MULTI_10) || ((value) == RCU_PLL_MULTI_11) || \
                                    ((value) == RCU_PLL_MULTI_12) || ((value) == RCU_PLL_MULTI_13) || \
                                    ((value) == RCU_PLL_MULTI_14) || ((value) == RCU_PLL_MULTI_15) || \
                                    ((value) == RCU_PLL_MULTI_16))


/** 
  * @brief RCU PLL pre-divider factor definition
  */
#define  RCU_PLL_PREDIV_1               RCU_CFG2_PLLPDIV1_DIV1  //!< RCU PLL pre-divider factor is 1
#define  RCU_PLL_PREDIV_2               RCU_CFG2_PLLPDIV1_DIV2  //!< RCU PLL pre-divider factor is 2
#define  RCU_PLL_PREDIV_3               RCU_CFG2_PLLPDIV1_DIV3  //!< RCU PLL pre-divider factor is 3
#define  RCU_PLL_PREDIV_4               RCU_CFG2_PLLPDIV1_DIV4  //!< RCU PLL pre-divider factor is 4
#define  RCU_PLL_PREDIV_5               RCU_CFG2_PLLPDIV1_DIV5  //!< RCU PLL pre-divider factor is 5
#define  RCU_PLL_PREDIV_6               RCU_CFG2_PLLPDIV1_DIV6  //!< RCU PLL pre-divider factor is 6
#define  RCU_PLL_PREDIV_7               RCU_CFG2_PLLPDIV1_DIV7  //!< RCU PLL pre-divider factor is 7
#define  RCU_PLL_PREDIV_8               RCU_CFG2_PLLPDIV1_DIV8  //!< RCU PLL pre-divider factor is 8
#define  RCU_PLL_PREDIV_9               RCU_CFG2_PLLPDIV1_DIV9  //!< RCU PLL pre-divider factor is 9
#define  RCU_PLL_PREDIV_10              RCU_CFG2_PLLPDIV1_DIV10 //!< RCU PLL pre-divider factor is 10
#define  RCU_PLL_PREDIV_11              RCU_CFG2_PLLPDIV1_DIV11 //!< RCU PLL pre-divider factor is 11
#define  RCU_PLL_PREDIV_12              RCU_CFG2_PLLPDIV1_DIV12 //!< RCU PLL pre-divider factor is 12
#define  RCU_PLL_PREDIV_13              RCU_CFG2_PLLPDIV1_DIV13 //!< RCU PLL pre-divider factor is 13
#define  RCU_PLL_PREDIV_14              RCU_CFG2_PLLPDIV1_DIV14 //!< RCU PLL pre-divider factor is 14
#define  RCU_PLL_PREDIV_15              RCU_CFG2_PLLPDIV1_DIV15 //!< RCU PLL pre-divider factor is 15
#define  RCU_PLL_PREDIV_16              RCU_CFG2_PLLPDIV1_DIV16 //!< RCU PLL pre-divider factor is 16

#define RCU_PLL_PREDIV_CHECK(value) (((value) == RCU_PLL_PREDIV_1)  || ((value) == RCU_PLL_PREDIV_2)  || \
                                     ((value) == RCU_PLL_PREDIV_3)  || ((value) == RCU_PLL_PREDIV_4)  || \
                                     ((value) == RCU_PLL_PREDIV_5)  || ((value) == RCU_PLL_PREDIV_6)  || \
                                     ((value) == RCU_PLL_PREDIV_7)  || ((value) == RCU_PLL_PREDIV_8)  || \
                                     ((value) == RCU_PLL_PREDIV_9)  || ((value) == RCU_PLL_PREDIV_10) || \
                                     ((value) == RCU_PLL_PREDIV_11) || ((value) == RCU_PLL_PREDIV_12) || \
                                     ((value) == RCU_PLL_PREDIV_13) || ((value) == RCU_PLL_PREDIV_14) || \
                                     ((value) == RCU_PLL_PREDIV_15) || ((value) == RCU_PLL_PREDIV_16))
 
/** 
  * @brief RCU SYSCLK source select definition
  */
#define RCU_SYSCLK_SEL_HRC             RCU_CFG_SYSSW_HRC   //!< RCU config system clock sources is HRC
#define RCU_SYSCLK_SEL_HXT             RCU_CFG_SYSSW_HXT   //!< RCU config system clock sources is HXT
#define RCU_SYSCLK_SEL_PLL             RCU_CFG_SYSSW_PLL   //!< RCU config system clock sources is PLL 

#define RCU_SYSCLK_SEL_CHECK(value) (((value) == RCU_SYSCLK_SEL_HRC)   || ((value) == RCU_SYSCLK_SEL_HXT) || \
                                     ((value) == RCU_SYSCLK_SEL_PLL)) 
                                      
/** 
  * @brief RCU HCLK source definition
  */
#define RCU_HCLK_CFG_SYSCLK_DIV1             RCU_CFG_HCLKPDIV_DIV1   //!< SYSCLK not divided 
#define RCU_HCLK_CFG_SYSCLK_DIV2             RCU_CFG_HCLKPDIV_DIV2   //!< SYSCLK divided by 2
#define RCU_HCLK_CFG_SYSCLK_DIV4             RCU_CFG_HCLKPDIV_DIV4   //!< SYSCLK divided by 4
#define RCU_HCLK_CFG_SYSCLK_DIV8             RCU_CFG_HCLKPDIV_DIV8   //!< SYSCLK divided by 8
#define RCU_HCLK_CFG_SYSCLK_DIV16            RCU_CFG_HCLKPDIV_DIV16  //!< SYSCLK divided by 16
#define RCU_HCLK_CFG_SYSCLK_DIV64            RCU_CFG_HCLKPDIV_DIV64  //!< SYSCLK divided by 64
#define RCU_HCLK_CFG_SYSCLK_DIV128           RCU_CFG_HCLKPDIV_DIV128 //!< SYSCLK divided by 128
#define RCU_HCLK_CFG_SYSCLK_DIV256           RCU_CFG_HCLKPDIV_DIV256 //!< SYSCLK divided by 256
#define RCU_HCLK_CFG_SYSCLK_DIV512           RCU_CFG_HCLKPDIV_DIV512 //!< SYSCLK divided by 512

#define RCU_HCLK_CFG_CHECK(value) (((value) == RCU_HCLK_CFG_SYSCLK_DIV1)   || ((value) == RCU_HCLK_CFG_SYSCLK_DIV2)   || \
                                   ((value) == RCU_HCLK_CFG_SYSCLK_DIV4)   || ((value) == RCU_HCLK_CFG_SYSCLK_DIV8)   || \
                                   ((value) == RCU_HCLK_CFG_SYSCLK_DIV16)  || ((value) == RCU_HCLK_CFG_SYSCLK_DIV64)  || \
                                   ((value) == RCU_HCLK_CFG_SYSCLK_DIV128) || ((value) == RCU_HCLK_CFG_SYSCLK_DIV256) || \
                                   ((value) == RCU_HCLK_CFG_SYSCLK_DIV512))

/** 
  * @brief RCU PCLK source definition
  */
#define RCU_PCLK_CFG_HCLK_DIV1                    RCU_CFG_PCLKPDIV_DIV1  //!< HCLK not divied 
#define RCU_PCLK_CFG_HCLK_DIV2                    RCU_CFG_PCLKPDIV_DIV2  //!< HCLK divided by 2
#define RCU_PCLK_CFG_HCLK_DIV4                    RCU_CFG_PCLKPDIV_DIV4  //!< HCLK divided by 4
#define RCU_PCLK_CFG_HCLK_DIV8                    RCU_CFG_PCLKPDIV_DIV8  //!< HCLK divided by 8
#define RCU_PCLK_CFG_HCLK_DIV16                   RCU_CFG_PCLKPDIV_DIV16 //!< HCLK divided by 16
#define RCU_PCLK_CFG_CHECK(pclk) (((pclk) == RCU_PCLK_CFG_HCLK_DIV1) || ((pclk) == RCU_PCLK_CFG_HCLK_DIV2) || \
                                  ((pclk) == RCU_PCLK_CFG_HCLK_DIV4) || ((pclk) == RCU_PCLK_CFG_HCLK_DIV8) || \
                                  ((pclk) == RCU_PCLK_CFG_HCLK_DIV16))
  
/** 
  * @brief RCU ADC clock source definition
  */
#define RCU_ADCCLK_CFG_HRC14                 ((uint32_t)0x00000000) //!< RCU ADC config HRC14 as clock source 
#define RCU_ADCCLK_CFG_PCLK_DIV2             ((uint32_t)0x01000000) //!< RCU ADC config PCLK divided by 2 as clock source 
#define RCU_ADCCLK_CFG_PCLK_DIV4             ((uint32_t)0x01004000) //!< RCU ADC config PCLK divided by 4 as clock source  

#define RCU_ADCCLK_CFG_CHECK(adcclk) (((adcclk) == RCU_ADCCLK_CFG_HRC14)     || \
                                      ((adcclk) == RCU_ADCCLK_CFG_PCLK_DIV2) || \
                                      ((adcclk) == RCU_ADCCLK_CFG_PCLK_DIV4))

/**
  * @brief RCU CEC clock source definition
  */
#define RCU_CECCLK_CFG_HRC_DIV244            ((uint32_t)0x00000000) //!< RCU CEC clock source is HRC divided by 244
#define RCU_CECCLK_CFG_LXT                   RCU_CFG3_CECSEL        //!< RCU CEC clock source is LXT 

#define RCU_CECCLK_CFG_CHECK(value) (((value) == RCU_CECCLK_CFG_HRC_DIV244) || ((value) == RCU_CECCLK_CFG_LXT))


/** 
  * @brief RCU I2C1 clock source definition
  */
#define RCU_I2C1CLK_CFG_HRC                   ((uint32_t)0x00000000) //!< RCU I2C1 clock source is HRC 
#define RCU_I2C1CLK_CFG_SYSCLK                RCU_CFG3_I2C1SEL       //!< RCU I2C1 clock source is SYSCLK 

#define RCU_I2C1CLK_CFG_CHECK(value) (((value) == RCU_I2C1CLK_CFG_HRC) || ((value) == RCU_I2C1CLK_CFG_SYSCLK))



/** 
  * @brief RCU USART clock source definition
  */
#define RCU_USART1CLK_CFG_PCLK                  ((uint32_t)0x10000000) //!< RCU USART1 select PCLK as clock source 
#define RCU_USART1CLK_CFG_SYSCLK                ((uint32_t)0x10000001) //!< RCU USART1 select SYSCLK as clock source
#define RCU_USART1CLK_CFG_LXT                   ((uint32_t)0x10000002) //!< RCU USART1 select LXT as clock source
#define RCU_USART1CLK_CFG_HRC                   ((uint32_t)0x10000003) //!< RCU USART1 select HRC as clock source

#define RCU_USART2CLK_CFG_PCLK                  ((uint32_t)0x20000000) //!< RCU USART1 select PCLK as clock source
#define RCU_USART2CLK_CFG_SYSCLK                ((uint32_t)0x20010000) //!< RCU USART1 select SYSCLK as clock source
#define RCU_USART2CLK_CFG_LXT                   ((uint32_t)0x20020000) //!< RCU USART1 select LXT as clock source
#define RCU_USART2CLK_CFG_HRC                   ((uint32_t)0x20030000) //!< RCU USART1 select HRC as clock source


#define RCU_USARTxCLK_CFG_CHECK(value) (((value) == RCU_USART1CLK_CFG_PCLK) || ((value) == RCU_USART1CLK_CFG_SYSCLK) || \
                                        ((value) == RCU_USART1CLK_CFG_LXT)  || ((value) == RCU_USART1CLK_CFG_HRC)    || \
                                        ((value) == RCU_USART2CLK_CFG_PCLK) || ((value) == RCU_USART2CLK_CFG_SYSCLK) || \
                                        ((value) == RCU_USART2CLK_CFG_LXT)  || ((value) == RCU_USART2CLK_CFG_HRC)) 

                                          
/** 
  * @brief RCU interrupt definition
  */
#define RCU_INT_LRC_STAB          ((uint8_t)0x01) //!< RCU LRC stabilization interrupt
#define RCU_INT_LXT_STAB          ((uint8_t)0x02) //!< RCU LXT stabilization interrupt
#define RCU_INT_HRC_STAB          ((uint8_t)0x04) //!< RCU HRC stabilization interrupt
#define RCU_INT_HXT_STAB          ((uint8_t)0x08) //!< RCU HXT stabilization interrupt
#define RCU_INT_PLL_STAB          ((uint8_t)0x10) //!< RCU PLL stabilization interrupt
#define RCU_INT_HRC14_STAB        ((uint8_t)0x20) //!< RCU HRC14 stabilization interrupt
#define RCU_INT_CKFAILI           ((uint8_t)0x80) //!< RCU HXT clock fail interrupt

#define RCU_INT_CHECK(flag) ((((flag) & (uint8_t)0x80) == 0x00) && ((flag) != 0x00))

#define RCU_GET_INT_CHECK(flag) (((flag) == RCU_INT_LRC_STAB) || ((flag) == RCU_INT_LXT_STAB)   || \
                                 ((flag) == RCU_INT_HRC_STAB) || ((flag) == RCU_INT_HXT_STAB)   || \
                                 ((flag) == RCU_INT_PLL_STAB) || ((flag) == RCU_INT_HRC14_STAB) || \
                                 ((flag) == RCU_INT_CKFAILI))

#define RCU_CLEAR_INT_CHECK(flag) ((flag) != 0x00)
  
/** 
  * @brief RCU LXT definition
  */
#define RCU_LXT_OFF                      ((uint32_t)0x00000000) //!< RCU LXT clock source is OFF          
#define RCU_LXT_ON                       RCU_VBDC_LXTEN         //!< RCU LXT clock source is ON 
#define RCU_LXT_BYPASS                   ((uint32_t)(RCU_VBDC_LXTEN | RCU_VBDC_LXTBYP)) //!< RCU LXT clock source is BYPASSED 
#define RCU_LXT_CHECK(value) (((value) == RCU_LXT_OFF) || \
                              ((value) == RCU_LXT_ON)  || \
                              ((value) == RCU_LXT_BYPASS))
/** 
  * @brief RCU RTC clock definition
  */
#define RCU_RTCCLK_SEL_LXT             RCU_VBDC_RTCSRC_LXT //!< RCU LXT oscillator clock used as RTC clock
#define RCU_RTCCLK_SEL_LRC             RCU_VBDC_RTCSRC_LRC //!< RCU LRT oscillator clock used as RTC clock
#define RCU_RTCCLK_SEL_HXT             RCU_VBDC_RTCSRC_HXT //!< RCU HXT oscillator clock used as RTC clock

#define RCU_RTCCLK_SEL_CHECK(value) (((value) == RCU_RTCCLK_SEL_LXT) || \
                                     ((value) == RCU_RTCCLK_SEL_LRC) || \
                                     ((value) == RCU_RTCCLK_SEL_HXT))

/** 
  * @brief RCU LXT drive definition
  */
#define RCU_LXT_DRIVE_LEVEL_0          ((uint32_t)0x00000000) //!< RCU LXT oscillator low drive capability
#define RCU_LXT_DRIVE_LEVEL_1          RCU_VBDC_LXTDRV_0      //!< RCU LXT oscillator medium low drive capability
#define RCU_LXT_DRIVE_LEVEL_2          RCU_VBDC_LXTDRV_1      //!< RCU LXT oscillator medium high drive capability
#define RCU_LXT_DRIVE_LEVEL_3          RCU_VBDC_LXTDRV        //!< RCU LXT oscillator high drive capability
#define RCU_LXT_DRIVE_CHECK(value) (((value) == RCU_LXT_DRIVE_LEVEL_0) || \
                                    ((value) == RCU_LXT_DRIVE_LEVEL_1) || \
                                    ((value) == RCU_LXT_DRIVE_LEVEL_2) || \
                                    ((value) == RCU_LXT_DRIVE_LEVEL_3))
  
/** 
  * @brief RCU AHB peripherals enable definition
  */
#define RCU_AHB_PERI_PORTA           RCU_AHBEN_PAEN   //!< RCU GPIOA clock enable 
#define RCU_AHB_PERI_PORTB           RCU_AHBEN_PBEN   //!< RCU GPIOB clock enable 
#define RCU_AHB_PERI_PORTC           RCU_AHBEN_PCEN   //!< RCU GPIOC clock enable 
#define RCU_AHB_PERI_PORTD           RCU_AHBEN_PDEN   //!< RCU GPIOD clock enable 
#define RCU_AHB_PERI_PORTE           RCU_AHBEN_PEEN   //!< RCU GPIOE clock enable
#define RCU_AHB_PERI_PORTF           RCU_AHBEN_PFEN   //!< RCU GPIOF clock enable 
#define RCU_AHB_PERI_CRC             RCU_AHBEN_CRCEN  //!< RCU CRC clock enable 
#define RCU_AHB_PERI_FMC             RCU_AHBEN_FMCEN  //!< RCU FMC clock enable 
#define RCU_AHB_PERI_SRAM            RCU_AHBEN_RMCEN  //!< RCU SRAM clock enable 
#define RCU_AHB_PERI_DMA1            RCU_AHBEN_DMAEN  //!< RCU DMA1 clock enable 

#define RCU_AHB_PERI_CHECK(peri) ((((peri) & 0xFE81FFA8) == 0x00) && ((peri) != 0x00))
#define RCU_AHB_RST_PERI_CHECK(peri) ((((peri) & 0xFE81FFA8) == 0x00) && ((peri) != 0x00))

/** 
  * @brief RCU APB2 peripherals enable definition
  */
#define RCU_APB2_PERI_SYSCFG         RCU_APB2EN_SYSCFGEN //!< RCU APB2 peripheral system clock enable
#define RCU_APB2_PERI_USART6         RCU_APB2EN_USART6EN //!< RCU APB2 peripheral USART6 clock enable
#define RCU_APB2_PERI_USART7         RCU_APB2EN_USART7EN //!< RCU APB2 peripheral USART7 clock enable
#define RCU_APB2_PERI_USART8         RCU_APB2EN_USART8EN //!< RCU APB2 peripheral USART8 clock enable
#define RCU_APB2_PERI_ADC            RCU_APB2EN_ADCEN    //!< RCU APB2 peripheral ADC clock enable
#define RCU_APB2_PERI_TIM1           RCU_APB2EN_TIM1EN   //!< RCU APB2 peripheral TIM1 clock enable
#define RCU_APB2_PERI_SPI1           RCU_APB2EN_SPI1EN   //!< RCU APB2 peripheral SPI1 clock enable
#define RCU_APB2_PERI_USART1         RCU_APB2EN_USART1EN //!< RCU APB2 peripheral USART1 clock enable
#define RCU_APB2_PERI_TIM15          RCU_APB2EN_TIM15EN  //!< RCU APB2 peripheral TIM15 clock enable
#define RCU_APB2_PERI_TIM16          RCU_APB2EN_TIM16EN  //!< RCU APB2 peripheral TIM16 clock enable
#define RCU_APB2_PERI_TIM17          RCU_APB2EN_TIM17EN  //!< RCU APB2 peripheral TIM17 clock enable
#define RCU_APB2_PERI_DBG            RCU_APB2EN_DBGEN    //!< RCU APB2 peripheral DBG clock enable

#define RCU_APB2_PERI_CHECK(peri) ((((peri) & 0xFFB8A51E) == 0x00) && ((peri) != 0x00))

/** 
  * @brief RCU APB1 peripherals enable definition
  */
#define RCU_APB1_PERI_TIM2              RCU_APB1EN_TIM2EN    //!< RCU APB1 peripheral TIM2 clock enable
#define RCU_APB1_PERI_TIM3              RCU_APB1EN_TIM3EN    //!< RCU APB1 peripheral TIM3 clock enable
#define RCU_APB1_PERI_TIM6              RCU_APB1EN_TIM6EN    //!< RCU APB1 peripheral TIM6 clock enable
#define RCU_APB1_PERI_TIM7              RCU_APB1EN_TIM7EN    //!< RCU APB1 peripheral TIM7 clock enable
#define RCU_APB1_PERI_TIM14             RCU_APB1EN_TIM14EN   //!< RCU APB1 peripheral TIM14 clock enable
#define RCU_APB1_PERI_WWDT              RCU_APB1EN_WWDTEN    //!< RCU APB1 peripheral WWDT clock enable
#define RCU_APB1_PERI_SPI2              RCU_APB1EN_SPI2EN    //!< RCU APB1 peripheral SPI2 clock enable
#define RCU_APB1_PERI_USART2            RCU_APB1EN_USART2EN  //!< RCU APB1 peripheral USART2 clock enable
#define RCU_APB1_PERI_USART3            RCU_APB1EN_USART3EN  //!< RCU APB1 peripheral USART3 clock enable
#define RCU_APB1_PERI_USART4            RCU_APB1EN_USART4EN  //!< RCU APB1 peripheral USART4 clock enable
#define RCU_APB1_PERI_USART5            RCU_APB1EN_USART5EN  //!< RCU APB1 peripheral USART2 clock enable
#define RCU_APB1_PERI_I2C1              RCU_APB1EN_I2C1EN    //!< RCU APB1 peripheral I2C1 clock enable
#define RCU_APB1_PERI_I2C2              RCU_APB1EN_I2C2EN    //!< RCU APB1 peripheral I2C2 clock enable
#define RCU_APB1_PERI_PMU               RCU_APB1EN_PMUEN     //!< RCU APB1 peripheral PMU clock enable

#define RCU_APB1_PERI_CHECK(peri) ((((peri) & 0x8581B6CC) == 0x00) && ((peri) != 0x00))

/** 
  * @brief RCU CLKOUT select definition
  */
#define RCU_CLKOUT_SEL_NO_CLK             ((uint8_t)0x00) //!< RCU CLKOUT select no clock source
#define RCU_CLKOUT_SEL_HRC14              ((uint8_t)0x01) //!< RCU CLKOUT select HRC14 as clock source
#define RCU_CLKOUT_SEL_LRC                ((uint8_t)0x02) //!< RCU CLKOUT select LRC as clock source
#define RCU_CLKOUT_SEL_LXT                ((uint8_t)0x03) //!< RCU CLKOUT select LXT as clock source
#define RCU_CLKOUT_SEL_SYSCLK             ((uint8_t)0x04) //!< RCU CLKOUT select SYSCLK as clock source
#define RCU_CLKOUT_SEL_HRC                ((uint8_t)0x05) //!< RCU CLKOUT select HRC as clock source
#define RCU_CLKOUT_SEL_HXT                ((uint8_t)0x06) //!< RCU CLKOUT select HXT as clock source
#define RCU_CLKOUT_SEL_PLL_DIV2           ((uint8_t)0x07) //!< RCU CLKOUT select PLL divided by 2 as clock source
#define RCU_CLKOUT_SEL_PLL                ((uint8_t)0x87) //!< RCU CLKOUT select PLL as clock source

#define RCU_CLKOUT_SEL_CHECK(value) (((value) == RCU_CLKOUT_SEL_NO_CLK)  || ((value) == RCU_CLKOUT_SEL_HRC14)    || \
                                     ((value) == RCU_CLKOUT_SEL_SYSCLK)  || ((value) == RCU_CLKOUT_SEL_HRC)      || \
                                     ((value) == RCU_CLKOUT_SEL_HXT)     || ((value) == RCU_CLKOUT_SEL_PLL_DIV2) || \
                                     ((value) == RCU_CLKOUT_SEL_LRC)     || ((value) == RCU_CLKOUT_SEL_PLL)|| \
                                     ((value) == RCU_CLKOUT_SEL_LXT))
/** 
  * @brief RCU CLKOUT prescaler definition
  */
#if !defined (CS32F051)
#define RCU_CLKOUT_PDIV_1            RCU_CFG_CKOPDIV_1   //!< RCU CLKOUT is divided by 1
#define RCU_CLKOUT_PDIV_2            RCU_CFG_CKOPDIV_2   //!< RCU CLKOUT is divided by 2
#define RCU_CLKOUT_PDIV_4            RCU_CFG_CKOPDIV_4   //!< RCU CLKOUT is divided by 4
#define RCU_CLKOUT_PDIV_8            RCU_CFG_CKOPDIV_8   //!< RCU CLKOUT is divided by 8
#define RCU_CLKOUT_PDIV_16           RCU_CFG_CKOPDIV_16  //!< RCU CLKOUT is divided by 16
#define RCU_CLKOUT_PDIV_32           RCU_CFG_CKOPDIV_32  //!< RCU CLKOUT is divided by 32
#define RCU_CLKOUT_PDIV_64           RCU_CFG_CKOPDIV_64  //!< RCU CLKOUT is divided by 64
#define RCU_CLKOUT_PDIV_128          RCU_CFG_CKOPDIV_128 //!< RCU CLKOUT is divided by 128

#define RCU_CLKOUT_PDIV_CHECK(value) (((value) == RCU_CLKOUT_PDIV_1) ||((value) == RCU_CLKOUT_PDIV_2)     || \
                                     ((value) == RCU_CLKOUT_PDIV_4)  || ((value) == RCU_CLKOUT_PDIV_8)    || \
                                     ((value) == RCU_CLKOUT_PDIV_16) || ((value) == RCU_CLKOUT_PDIV_32)   || \
                                     ((value) == RCU_CLKOUT_PDIV_64) ||  ((value) == RCU_CLKOUT_PDIV_128)) 
#endif                                          


/** 
  * @brief RCU flag definition
  */
#define RCU_FLAG_HRC_STAB                  ((uint8_t)0x01) //!< RCU HRC stabilization flag 
#define RCU_FLAG_HXT_STAB                  ((uint8_t)0x11) //!< RCU HXT stabilization flag 
#define RCU_FLAG_PLL_STAB                  ((uint8_t)0x19) //!< RCU PLL stabilization flag 
#define RCU_FLAG_LXT_STAB                  ((uint8_t)0x21) //!< RCU LXT stabilization flag 
#define RCU_FLAG_LRC_STAB                  ((uint8_t)0x41) //!< RCU LRC stabilization flag 
#define RCU_FLAG_V15RSTF                   ((uint8_t)0x57) //!< RCU V15RSTF stabilization flag 
#define RCU_FLAG_OBURSTF                   ((uint8_t)0x59) //!< RCU OBURSTF stabilization flag 
#define RCU_FLAG_NRST                      ((uint8_t)0x5A) //!< RCU NRST stabilization flag 
#define RCU_FLAG_PORRST                    ((uint8_t)0x5B) //!< RCU PORRST stabilization flag 
#define RCU_FLAG_SWRST                     ((uint8_t)0x5C) //!< RCU SWRST stabilization flag 
#define RCU_FLAG_FWDTRST                   ((uint8_t)0x5D) //!< RCU FWDTRST stabilization flag 
#define RCU_FLAG_WWDTRST                   ((uint8_t)0x5E) //!< RCU WWDTRST stabilization flag 
#define RCU_FLAG_LPERST                    ((uint8_t)0x5F) //!< RCU LPERST stabilization flag 
#define RCU_FLAG_HRC14_STAB                ((uint8_t)0x61) //!< RCU HRC14 stabilization flag 

#define RCU_FLAG_CHECK(flag) (((flag) == RCU_FLAG_HRC_STAB)   || ((flag) == RCU_FLAG_HXT_STAB)   || \
                              ((flag) == RCU_FLAG_PLL_STAB)   || ((flag) == RCU_FLAG_LXT_STAB)   || \
                              ((flag) == RCU_FLAG_LRC_STAB)   || ((flag) == RCU_FLAG_OBURSTF)    || \
                              ((flag) == RCU_FLAG_NRST)       || ((flag) == RCU_FLAG_PORRST)     || \
                              ((flag) == RCU_FLAG_SWRST)      || ((flag) == RCU_FLAG_FWDTRST)    || \
                              ((flag) == RCU_FLAG_WWDTRST)    || ((flag) == RCU_FLAG_LPERST)     || \
                              ((flag) == RCU_FLAG_HRC14_STAB) || ((flag) == RCU_FLAG_V15RSTF))
                              

#define RCU_HRC_CALI_VALUE_CHECK(value) ((value) <= 0x1F)
#define RCU_HRC14_CALI_VALUE_CHECK(value) ((value) <= 0x1F)

/**
  * @}
  */ 

/**
  * @fn void rcu_def_init(void)
  * @brief  Resets the RCU clock configuration to the default reset state.
  * @note   The default reset state including HRC ON and used as system clock source 
  * @note   HRC14, HXT and PLL OFF AHB, APB prescaler set to 1, CSS and CLKOUT OFF
  * @note   All interrupts disabled, but it doesn't modify the configuration of the Peripheral 
  * @note   clocks LRC,LXT and RTC clocks
  * @param  None
  * @return None
  */
void rcu_def_init(void);

/**
  * @fn void rcu_hxt_config(uint8_t value)
  * @brief  Configures the High speed Extern crystal (HXT).
  * @param  value: specifies the new state of the HXT, it can be one of the following values:
  *          @arg RCU_HXT_OFF: turn off the HXT oscillator                             
  *          @arg RCU_HXT_ON: turn on the HXT oscillator
  *          @arg RCU_HXT_BYPASS: HXT oscillator bypassed with external clock
  * @return None
  */
void rcu_hxt_config(uint8_t value);

/**
  * @fn error_status_t rcu_hxt_stabilization_wait(void)
  * @brief  Waits for HXT stabilization. It waits on HSERDY flag to
  *         be set and return SUCCESS. The HXT is stopped by hardware 
  *         when entering STOP and STANDBY modes.
  * @param  None
  * @return SUCCESS(oscillator is stable) or ERROR(oscillator not yet stable)
  */
error_status_t rcu_hxt_stabilization_wait(void);

/**
  * @fn void rcu_hrc_calibration_adjust(uint8_t value)
  * @brief  Adjusts the Internal High speed RC (HRC) calibration value.
  * @param  value: specifies the HRC calibration trimming value (between 0 and 0x1F).
  * @return None
  */
void rcu_hrc_calibration_adjust(uint8_t value);

/**
  * @fn void rcu_hrc_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal High Speed RC (HRC).
  * @param  enable_flag: ENABLE or DISABLE the HRC.
  * @return None
  */
void rcu_hrc_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_hrc14_calibration_adjust(uint8_t value)
  * @brief  Adjusts the Internal High Speed RC14 for ADC (HRC14) 
  *         calibration value.
  * @param  value: specifies the HRC14 calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  * @return None
  */
void rcu_hrc14_calibration_adjust(uint8_t value);

/**
  * @fn void rcu_hrc14_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal High Speed RC14 for ADC (HRC14).
  * @param  enable_flag: ENABLE or DISABLE HRC14.
  * @return None
  */
void rcu_hrc14_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_hrc14_opening_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables ADC control HRC14 opening.
  * @param  enable_flag: ENABLE or DISABLE (HRC14 ADC opening).
  * @return None
  */
void rcu_hrc14_opening_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_lxt_config(uint32_t value)
  * @brief  Configures the External Low Speed crystal oscillator (LXT).
  * @param  value: specifies the new state of the LXT.
  *          It can be one of the following values:
  *           @arg RCU_LXT_OFF(turn OFF the LXT oscillator) 
  *           @arg RCU_LXT_ON(turn ON the LXT oscillator) 
  *           @arg RCU_LXT_BYPASS(LXT oscillator bypassed with external clock)
  * @return None
  */
void rcu_lxt_config(uint32_t value);

/**
  * @fn void rcu_lxt_drive_config(uint32_t value)
  * @brief  Configures the External Low Speed crystal oscillator (LXT) drive capability.
  * @param  value: specifies the new state of the LXT drive capability.
  *         It can be one of the following values:
  *          @arg RCU_LXT_DRIVE_LEVEL_0(LXT oscillator low drive capability)
  *          @arg RCU_LXT_DRIVE_LEVEL_1(LXT oscillator medium low drive capability)
  *          @arg RCU_LXT_DRIVE_LEVEL_2(LXT oscillator medium high drive capability)
  *          @arg RCU_LXT_DRIVE_LEVEL_3(LXT oscillator high drive capability)
  * @return None
  */
void rcu_lxt_drive_config(uint32_t value);

/**
  * @fn void rcu_lrc_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal Low Speed oscillator (LRC).
  * @note   After enabling the LRC, the application software should wait on 
  * @note   LRCSTAB flag to be set indicating that LRC clock is stable and can
  * @note   be used to clock the FWDT and/or the RTC.
  * @param  enable_flag: ENABLE or DISABLE LRC.
  * @return None
  */
void rcu_lrc_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_hxt_monitor_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Clock Security Monitor.
  * @param  enable_flag: ENABLE or DISABL HXT Monitor.
  * @return None
  */
void rcu_hxt_monitor_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_pll_config(uint32_t sel_value, uint32_t multiple)
  * @brief  Configures the PLL clock source and multiplication factor.
  *         It must be used only when the PLL is disabled.
  * @param  sel_value: specifies the PLL selected clock source.
  *          It can be one of the following values:
  *           @arg RCU_PLL_CFG_HRC_DIV2(HRC oscillator clock selected as PLL clock source)
  *           @arg RCU_PLL_CFG_HXT(PLLPDIV clock selected as PLL clock entry)
  * @param  multiple: specifies the PLL multiplication factor (between RCU_PLL_MULTI_2 ~ RCU_PLL_MULTI_16)  
  * @return None
  */
void rcu_pll_config(uint32_t sel_value, uint32_t multiple);

/**
  * @fn void rcu_pll_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the PLL. After enabling the PLL, the application 
  *         software should wait on  PLLSTAB flag to be set indicating that PLL clock 
  *         is stable and can be used as system clock source.
  * @param  enable_flag: ENABLE or DISABLE PLL.
  * @return None
  */
void rcu_pll_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_pll_divider_config(uint32_t value)
  * @brief  Configures the PLLPDIV division factor, used only when the PLL is disabled.    
  * @param  value: specifies the PLLPDIV clock division factor. It can be any
  *         of value between RCU_PLL_PREDIV_1 to RCU_PLL_PREDIV_16.
  * @return None
  */
void rcu_pll_divider_config(uint32_t value);

/**
  * @fn void rcu_clkout_config(uint8_t sel_value, uint32_t prescaler)
  * @brief  Selects the clock source to output on CLKOUT pin (PA8). It shout be configured
  *         in  multi-function mode.
  * @param  sel_value: specifies the clock source to output,it can be one of the following values:
  *          @arg RCU_CLKOUT_SEL_NO_CLK(No clock selected)
  *          @arg RCU_CLKOUT_SEL_HRC14(HRC14 oscillator clock selected)
  *          @arg RCU_CLKOUT_SEL_LRC(LRC oscillator clock selected)
  *          @arg RCU_CLKOUT_SEL_LXT( LXT oscillator clock selected)
  *          @arg RCU_CLKOUT_SEL_SYSCLK(System clock selected)
  *          @arg RCU_CLKOUT_SEL_HRC(HRC oscillator clock selected)
  *          @arg RCU_CLKOUT_SEL_HXT(HXT oscillator clock selected)
  *          @arg RCU_CLKOUT_SEL_PLL_DIV2(PLL clock divided by 2 selected)
  * @param  prescaler: specifies the prescaler on CLKOUT pin, it can be one of the following values:
  *          @arg RCU_CLKOUT_PDIV_1(CLKOUT clock is divided by 1)
  *          @arg RCU_CLKOUT_PDIV_2(CLKOUT clock is divided by 2)
  *          @arg RCU_CLKOUT_PDIV_4(CLKOUT clock is divided by 4)
  *          @arg RCU_CLKOUT_PDIV_8(CLKOUT clock is divided by 8)
  *          @arg RCU_CLKOUT_PDIV_16(CLKOUT clock is divided by 16)
  *          @arg RCU_CLKOUT_PDIV_32(CLKOUT clock is divided by 32)
  *          @arg RCU_CLKOUT_PDIV_64(CLKOUT clock is divided by 64)
  *          @arg RCU_CLKOUT_PDIV_128(CLKOUT clock is divided by 128)   
  * @return None
  */
void rcu_clkout_config(uint8_t sel_value, uint32_t prescaler);



/**
  * @fn void rcu_sysclk_config(uint32_t value)
  * @brief Configures the system clock (SYSCLK).  
  * @param  value: specifies the clock source used as system clock.  
  *         It can be one of the following values:
  *          @arg RCU_SYSCLK_SEL_HRC (HRC selected as system clock)
  *          @arg RCU_SYSCLK_SEL_HXT (HXT selected as system clock)
  *          @arg RCU_SYSCLK_SEL_PLL (PLL selected as system clock)
  * @return None
  */
void rcu_sysclk_config(uint32_t value);

/**
  * @fn uint8_t rcu_sysclk_switch_get(void)
  * @brief  Returns the clock source by switched.
  * @param  None
  * @return The clock source used as system clock. It an be one of the following values:
  *           @arg HRC used as system clock(0x00)
  *           @arg HXT used as system clock(0x04)  
  *           @arg PLL used as system clock(0x08)
  */
uint8_t rcu_sysclk_switch_get(void);

/**
  * @fn void rcu_hclk_config(uint32_t value)
  * @brief  Configures the AHB clock (HCLK).
  * @param  value: defines HCLK divider. It is derived from SYSCLK (the system clock).
  *          It can be one of the following values:
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV1 (HCLK = SYSCLK)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV2 (HCLK = SYSCLK/2)   
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV4 (HCLK = SYSCLK/4)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV8 (HCLK = SYSCLK/8)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV16 (HCLK = SYSCLK/16)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV64 (HCLK = SYSCLK/64)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV128 (HCLK = SYSCLK/128)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV256 (HCLK = SYSCLK/256)
  *           @arg RCU_HCLK_CFG_SYSCLK_DIV512 (HCLK = SYSCLK/512)
  * @return None
  */
void rcu_hclk_config(uint32_t value);

/**
  * @fn void rcu_pclk_config(uint32_t value)
  * @brief  Configures the APB clock (PCLK).
  * @param  value: defines the APB clock divider which is derived from HCLK (the AHB clock).
  *         It can be one of the following values:
  *          @arg RCU_PCLK_CFG_HCLK_DIV1 (APB clock = HCLK) 
  *          @arg RCU_PCLK_CFG_HCLK_DIV2 (APB clock = HCLK/2)
  *          @arg RCU_PCLK_CFG_HCLK_DIV4 (APB clock = HCLK/4)
  *          @arg RCU_PCLK_CFG_HCLK_DIV8 (APB clock = HCLK/8)
  *          @arg RCU_PCLK_CFG_HCLK_DIV16 (APB clock = HCLK/16)
  * @return None
  */
void rcu_pclk_config(uint32_t value);

/**
  * @fn void rcu_adcclk_config(uint32_t value)
  * @brief  Configures the ADC clock (ADCCLK).
  * @param  value: defines the ADC clock source which is derived from 
  *         PCLK (the HRC14 or APB clock). It can be one of the following values:
  *          @arg RCU_ADCCLK_CFG_HRC14 (ADC clock = HRC14 (14MHz))
  *          @arg RCU_ADCCLK_CFG_PCLK_DIV2 (ADC clock = PCLK/2)
  *          @arg RCU_ADCCLK_CFG_PCLK_DIV4 (ADC clock = PCLK/4)  
  * @return None
  */
void rcu_adcclk_config(uint32_t value); 


/**
  * @fn void rcu_i2cclk_config(uint32_t value)
  * @brief  Configures the I2C1 clock (I2C1CLK).
  * @param  value: defines the I2C1 clock source which is derived from 
  *         the HRC or System clock. It can be one of the following values:
  *          @arg RCU_I2C1CLK_CFG_HRC (I2C1 clock = HRC)
  *          @arg RCU_I2C1CLK_CFG_SYSCLK (I2C1 clock = System Clock)
  * @return None
  */
void rcu_i2cclk_config(uint32_t value);

/**
  * @fn void rcu_usartclk_config(uint32_t value)
  * @brief  Configures the USART1 clock (USART1CLK).
  * @param  value: defines the USART clock source which is derived from the HRC or System clock.
  *          It can be one of the following values:
  *           @arg RCU_USART1CLK_CFG_PCLK (USART1 clock =PCLK)
  *           @arg RCU_USART1CLK_CFG_SYSCLK (USART1 clock = System Clock)
  *           @arg RCU_USART1CLK_CFG_LXT (USART1 clock = LXT Clock)
  *           @arg RCU_USART1CLK_CFG_HRC (USART1 clock = HRC Clock) 
  * @return None
  */
void rcu_usartclk_config(uint32_t value);

/**
  * @fn void rcu_clk_freq_get(rcu_clock_t* ptr_clock)
  * @brief  Returns the frequencies of the System, AHB and APB busses clocks.             
  * @param  ptr_clock: pointer to a rcu_clock_t structure which will hold the clocks frequencies.  
  * @return None
  */
void rcu_clk_freq_get(rcu_clock_t* ptr_clock);

/**
  * @fn void rcu_rtcclk_config(uint32_t value)
  * @brief  Configures the RTC clock (RTCCLK).           
  * @param  value: specifies the RTC clock source,it can be one of the following values:
  *          @arg RCU_RTCCLK_SEL_LXT(LXT selected as RTC clock)
  *          @arg RCU_RTCCLK_SEL_LRC(LRC selected as RTC clock)
  *          @arg RCU_RTCCLK_SEL_HXT(HXT divided by 32 selected as RTC clock)                                  
  * @return None
  */
void rcu_rtcclk_config(uint32_t value);

/**
  * @fn void rcu_rtcclk_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the RTC clock.It must be used when
  *         the RTC clock source was selected using the rcu_rtcclk_config.
  * @param  enable_flag: ENABLE or DISABLE the RTC clock.
  * @return None
  */
void rcu_rtcclk_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_vbat_reset_enable_ctrl(enable_state_t enable_flag)
  * @brief  ENABLE or DISABLE VBAT domain software reset.
  * @param  enable_flag: ENABLE or DISABLE VBAT domain reset.
  * @return None
  */
void rcu_vbat_reset_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rcu_ahb_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @brief  Enables or disables the AHB peripheral clock.   
  * @param  value: specifies the AHB peripheral to gates its clock.
  *          It can be any combination of the following values:
  *           @arg RCU_AHB_PERI_PORTA (GPIOA clock)
  *           @arg RCU_AHB_PERI_PORTB (GPIOB clock)
  *           @arg RCU_AHB_PERI_PORTC (GPIOC clock)
  *           @arg RCU_AHB_PERI_PORTD (GPIOD clock)
  *           @arg RCU_AHB_PERI_PORTF (GPIOF clock)
  *           @arg RCU_AHB_PERI_CRC (CRC clock)
  *           @arg RCU_AHB_PERI_FMC (has effect only when the Flash memory is in power down mode)  
  *           @arg RCU_AHB_PERI_SRAM (SRAM clock)
  *           @arg RCU_AHB_PERI_DMA1 (DMA1 clock)
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral clock.
  * @return None
  */
void rcu_ahb_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_apb2_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
  * @param  value: specifies the APB2 peripheral clock, it can be any combination of the following values:
  *          @arg RCU_APB2_PERI_SYSCFG (SYSCFG clock)
  *          @arg RCU_APB2_PERI_USART6 (USART6 clock)
  *          @arg RCU_APB2_PERI_USART7 (USART7 clock)
  *          @arg RCU_APB2_PERI_USART8 (USART8 clock)
  *          @arg RCU_APB2_PERI_ADC (ADC1 clock)
  *          @arg RCU_APB2_PERI_TIM1 (TIM1 clock)
  *          @arg RCU_APB2_PERI_SPI1 (SPI1 clock)
  *          @arg RCU_APB2_PERI_USART1 (USART1 clock)  
  *          @arg RCU_APB2_PERI_TIM15 (TIM15 clock)
  *          @arg RCU_APB2_PERI_TIM16 (TIM16 clock)
  *          @arg RCU_APB2_PERI_TIM17 (TIM17 clock)
  *          @arg RCU_APB2_PERI_DBG (DBGMCU clock)
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral clock.
  * @return None
  */
void rcu_apb2_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_apb1_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @brief  Enables or disables the Low Speed  APB1 peripheral clock.
  * @param  value: specifies the APB1 peripheral to gates its clock, it can be any combination
  *         of the following values:
  *          @arg RCU_APB1_PERI_TIM2 (TIM2 clock)
  *          @arg RCU_APB1_PERI_TIM3 (TIM3 clock)
  *          @arg RCU_APB1_PERI_TIM6 (TIM6 clock)
  *          @arg RCU_APB1_PERI_TIM14 (TIM14 clock)
  *          @arg RCU_APB1_PERI_WWDT (WWDT clock)
  *          @arg RCU_APB1_PERI_SPI2 (SPI2 clock)
  *          @arg RCU_APB1_PERI_USART2 (USART2 clock)     
  *          @arg RCU_APB1_PERI_I2C1 (I2C1 clock)
  *          @arg RCU_APB1_PERI_I2C2 (I2C2 clock)  
  *          @arg RCU_APB1_PERI_PMU (PMU clock)                           
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral clock.
  * @return None
  */
void rcu_apb1_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_ahb_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @param  value: specifies the AHB peripheral to reset, it can be any combination of the following values:
  *          @arg RCU_AHB_PERI_PORTA (GPIOA clock)
  *          @arg RCU_AHB_PERI_PORTB (GPIOB clock)
  *          @arg RCU_AHB_PERI_PORTC (GPIOC clock)
  *          @arg RCU_AHB_PERI_PORTD (GPIOD clock)
  *          @arg RCU_AHB_PERI_PORTF (GPIOF clock)
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral reset.
  * @return None
  */
void rcu_ahb_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_apb2_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @brief ENABLE or DISABLE High Speed APB (APB2) peripheral reset.
  * @param  value: specifies the APB2 peripheral to reset,it can be any combination of the following values:
  *          @arg RCU_APB2_PERI_SYSCFG (SYSCFG clock)
  *          @arg RCU_APB2_PERI_USART6 (USART6 clock)
  *          @arg RCU_APB2_PERI_USART7 (USART7 clock)
  *          @arg RCU_APB2_PERI_USART8 (USART8 clock)
  *          @arg RCU_APB2_PERI_ADC (ADC clock)
  *          @arg RCU_APB2_PERI_TIM1 (TIM1 clock)
  *          @arg RCU_APB2_PERI_SPI1 (SPI1 clock)
  *          @arg RCU_APB2_PERI_USART1 (USART1 clock)
  *          @arg RCU_APB2_PERI_TIM15 (TIM15 clock)
  *          @arg RCU_APB2_PERI_TIM16 (TIM16 clock)
  *          @arg RCU_APB2_PERI_TIM17 (TIM17 clock)
  *          @arg RCU_APB2_PERI_DBG (DBG clock)
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral reset.
  * @return None
  */
void rcu_apb2_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_apb1_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
  * @brief ENABLE or DISABLE Low Speed APB (APB1) peripheral reset.
  * @param  value: specifies the APB1 peripheral to reset,it can be any combination of the following values:
  *          @arg RCU_APB1_PERI_TIM3 (TIM3 clock)
  *          @arg RCU_APB1_PERI_TIM6 (TIM6 clock)
  *          @arg RCU_APB1_PERI_TIM14 (TIM14 clock)
  *          @arg RCU_APB1_PERI_WWDT (WWDT clock)
  *          @arg RCU_APB1_PERI_SPI2 (SPI2 clock)
  *          @arg RCU_APB1_PERI_USART2 (USART2 clock)      
  *          @arg RCU_APB1_PERI_I2C1 (I2C1 clock)
  *          @arg RCU_APB1_PERI_I2C2 (I2C2 clock)
  *          @arg RCU_APB1_PERI_PMU (PMU clock)
  * @param  enable_flag: ENABLE or DISABLE the specified peripheral clock.
  * @return None
  */
void rcu_apb1_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag);

/**
  * @fn void rcu_interrupt_config(uint8_t value, enable_state_t enable_flag)
  * @brief Enables or disables the specified RCU interrupts.
  * @param  value: specifies the RCU interrupt sources to be enabled or disabled.
  *         It can be any combination of the following values:
  *          @arg RCU_INT_LRC_STAB (LRC stabilization interrupt)
  *          @arg RCU_INT_LXT_STAB (LXT stabilization interrupt)
  *          @arg RCU_INT_HRC_STAB (HRC stabilization interrupt)
  *          @arg RCU_INT_HXT_STAB (HXT stabilization interrupt)
  *          @arg RCU_INT_PLL_STAB (PLL stabilization interrupt)
  *          @arg RCU_INT_HRC14_STAB (HRC14 stabilization interrupt)
  * @param  enable_flag: ENABLE or DISABLE the specified RCU interrupts.
  * @return None
  */
void rcu_interrupt_config(uint8_t value, enable_state_t enable_flag);

/**
  * @fn bit_status_t rcu_flag_status_get(uint8_t flag)
  * @brief  Checks whether the specified RCU flag is set or not.
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *          @arg RCU_FLAG_HRC_STAB (HRC oscillator clock stabilization)
  *          @arg RCU_FLAG_HXT_STAB (HXT oscillator clock stabilization)
  *          @arg RCU_FLAG_PLL_STAB (PLL clock stabilization)
  *          @arg RCU_FLAG_LXT_STAB (LXT oscillator clock stabilization)
  *          @arg RCU_FLAG_LRC_STAB (LRC oscillator clock stabilization)
  *          @arg RCU_FLAG_OBURSTF (Option Byte Update reset )
  *          @arg RCU_FLAG_NRST (nRST Pin reset)
  *          @arg RCU_FLAG_V15RSTF (the 1.5V domain reset)
  *          @arg RCU_FLAG_PORRST (POR/PDR reset)
  *          @arg RCU_FLAG_SWRST (Software reset)
  *          @arg RCU_FLAG_FWDTRST (FWDT reset)
  *          @arg RCU_FLAG_WWDTRST (WWDT reset)
  *          @arg RCU_FLAG_LPERST (Low-power mode enter-reset)
  *          @arg RCU_FLAG_HRC14_STAB (HRC14 oscillator clock stabilization)
  * @return The new state of flag (SET or RESET).
  */
bit_status_t rcu_flag_status_get(uint8_t flag);

/**
  * @fn void rcu_flag_clear(void)
  * @brief  Clears the RCU reset flags.
  *         The reset flags are: RCU_FLAG_OBURSTF, RCU_FLAG_NRST, RCU_FLAG_V15RSTF,
  *         RCU_FLAG_PORRST, RCU_FLAG_SWRST, RCU_FLAG_FWDTRST, RCU_FLAG_WWDTRST,
  *         RCU_FLAG_LPERST.
  * @param  None
  * @return None
  */
void rcu_flag_clear(void);

/**
  * @fn bit_status_t rcu_interrupt_status_get(uint8_t flag)
  * @brief  Checks whether the specified RCU interrupt has occurred or not.
  * @param  flag: specifies the RCU interrupt source to check.
  *         It can be one of the following values:
  *          @arg RCU_INT_HXT_STAB (HXT oscillator clock interrupt)
  *          @arg RCU_INT_HRC_STAB (HRC oscillator clock interrupt)
  *          @arg RCU_INT_PLL_STAB (PLL clock interrupt)
  *          @arg RCU_INT_LXT_STAB (LXT oscillator clock interrupt)  
  *          @arg RCU_INT_LRC_STAB (LRC oscillator clock interrupt)
  *          @arg RCU_INT_HRC14_STAB (HRC14 stable interrupt)
  *          @arg RCU_INT_CKFAILI (HXT Clock Fail Interrupt)
  * @return The new state of flag (SET or RESET).
  */
bit_status_t rcu_interrupt_status_get(uint8_t flag);

/**
  * @fn void rcu_interrupt_clear(uint8_t flag)
  * @brief  Clears the RCU's interrupt pending bits.
  * @param  flag: specifies the interrupt pending bit to clear.
  *         It can be any combination of the following values:
  *          @arg RCU_INT_HXT_STAB (HXT oscillator clock interrupt)
  *          @arg RCU_INT_HRC_STAB (HRC oscillator clock interrupt)
  *          @arg RCU_INT_PLL_STAB (PLL clock interrupt)
  *          @arg RCU_INT_LXT_STAB(LXT oscillator clock interrupt)  
  *          @arg RCU_INT_LRC_STAB (LRC oscillator clock interrupt)
  *          @arg RCU_INT_HRC14_STAB (HRC14 stable interrupt)
  *          @arg RCU_INT_CKFAILI (HXT Clock Fail Interrupt)
  * @return None
  */
void rcu_interrupt_clear(uint8_t flag);

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

