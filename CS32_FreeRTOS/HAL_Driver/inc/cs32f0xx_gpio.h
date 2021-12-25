/**
  * @file    cs32f0xx_gpio.h
  * @brief   This file contains all the functions prototypes for the GPIO firmware library.          
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_GPIO_H__
#define __CS32F0XX_GPIO_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup GPIO
  * @{
  */

                                  
/** 
  * @enum gpio_mode_t
  * @brief GPIO input/output mode enumeration
  */ 
typedef enum
{
    GPIO_MODE_INPUT   = 0x00,  //!< GPIO Input Mode            
    GPIO_MODE_OUTPUT  = 0x01,  //!< GPIO Output Mode             
    GPIO_MODE_MF      = 0x02,  //!< GPIO multi-function Mode 
    GPIO_MODE_AN      = 0x03   //!< GPIO Analog In/Out Mode      
}gpio_mode_t;

#define GPIO_MODE_CHECK(flag) (((flag) == GPIO_MODE_INPUT)  || \
                               ((flag) == GPIO_MODE_OUTPUT) || \
                               ((flag) == GPIO_MODE_MF)     || \
                               ((flag) == GPIO_MODE_AN))
                              
/** 
  * @enum gpio_output_type_t
  * @brief  GPIO output type enumeration
  */
typedef enum
{
    GPIO_OUTPUT_PP = 0x00,  //!< GPIO output push-pull (reset state)
    GPIO_OUTPUT_OD = 0x01   //!< GPIO output open-drain
}gpio_output_type_t;

#define GPIO_OUTPUT_TYPE_CHECK(flag) (((flag) == GPIO_OUTPUT_PP) || \
                                      ((flag) == GPIO_OUTPUT_OD))

/** 
  * @enum gpio_speed_t 
  * @brief GPIO Output Maximum frequency enumeration    
  */
typedef enum
{
    GPIO_SPEED_LOW      = 0x00,  //!< I/O output speed: Low 2 MHz 
    GPIO_SPEED_MEDIUM   = 0x01,  //!< I/O output speed: Medium 10 MHz 
    GPIO_SPEED_HIGH     = 0x03   //!< I/O output speed: High 50 MHz 
}gpio_speed_t;

#define GPIO_SPEED_CHECK(flag) (((flag) == GPIO_SPEED_LOW)    || \
                                ((flag) == GPIO_SPEED_MEDIUM) || \
                                ((flag) == GPIO_SPEED_HIGH))
                                   
/** 
  * @enum gpio_pull_t
  * @brief GPIO Config PullUp/PullDown enumeration   
  */
typedef enum
{
    GPIO_PULL_NO_PULL = 0x00, //!< No PullUp/PullDown config
    GPIO_PULL_UP      = 0x01, //!< GPIO PullUp config
    GPIO_PULL_DOWN    = 0x02  //!< GPIO PullDown config
}gpio_pull_t;

#define GPIO_PULL_CHECK(flag) (((flag) == GPIO_PULL_NO_PULL) || \
                               ((flag) == GPIO_PULL_UP)      || \
                               ((flag) == GPIO_PULL_DOWN))

/** 
  * @enum gpio_bit_t
  * @brief GPIO Bit_SET_and_Bit_RESET  enumeration   
  */
typedef enum
{ 
    BIT_RESET = 0,  //!< GPIO bit reset
    BIT_SET         //!< GPIO bit set
}gpio_bit_t;

#define GPIO_BIT_STATE_CHECK(flag) (((flag) == BIT_RESET) || \
                                    ((flag) == BIT_SET))

/**
  * @struct gpio_config_t
  * @brief GPIO Init structure definition  
  */
typedef struct
{
    uint32_t           gpio_pin;        //!< Specifies the GPIO pins to be configured                                       
    gpio_mode_t        gpio_mode;       //!< Specifies the operating mode for the selected pins
    gpio_speed_t       gpio_speed;      //!< Specifies the speed for the selected pins
    gpio_output_type_t gpio_out_type;   //!< Specifies the operating output type for the selected pins
    gpio_pull_t        gpio_pull;       //!< Specifies the operating Pull-up/Pull down for the selected pins
}gpio_config_t;

/** 
  * @brief Check GPIO port definition  
  */ 
#define GPIO_PORT_LIST1_CHECK(port) (((port) == GPIOA) || \
                                     ((port) == GPIOB)) 

#define GPIO_ALL_PORT_CHECK(port) (((port) == GPIOA) || \
                                   ((port) == GPIOB) || \
                                   ((port) == GPIOC) || \
                                   ((port) == GPIOD) || \
                                   ((port) == GPIOE) || \
                                   ((port) == GPIOF))

/** 
  * @brief  GPIO pins definition
  */
#define GPIO_PIN_0                 ((uint16_t)0x0001)  //!< select Pin 0 
#define GPIO_PIN_1                 ((uint16_t)0x0002)  //!< select Pin 1 
#define GPIO_PIN_2                 ((uint16_t)0x0004)  //!< select Pin 2 
#define GPIO_PIN_3                 ((uint16_t)0x0008)  //!< select Pin 3 
#define GPIO_PIN_4                 ((uint16_t)0x0010)  //!< select Pin 4 
#define GPIO_PIN_5                 ((uint16_t)0x0020)  //!< select Pin 5 
#define GPIO_PIN_6                 ((uint16_t)0x0040)  //!< select Pin 6 
#define GPIO_PIN_7                 ((uint16_t)0x0080)  //!< select Pin 7 
#define GPIO_PIN_8                 ((uint16_t)0x0100)  //!< select Pin 8 
#define GPIO_PIN_9                 ((uint16_t)0x0200)  //!< select Pin 9 
#define GPIO_PIN_10                ((uint16_t)0x0400)  //!< select Pin 10 
#define GPIO_PIN_11                ((uint16_t)0x0800)  //!< select Pin 11
#define GPIO_PIN_12                ((uint16_t)0x1000)  //!< select Pin 12
#define GPIO_PIN_13                ((uint16_t)0x2000)  //!< select Pin 13
#define GPIO_PIN_14                ((uint16_t)0x4000)  //!< select Pin 14
#define GPIO_PIN_15                ((uint16_t)0x8000)  //!< select Pin 15
#define GPIO_PIN_ALL               ((uint16_t)0xFFFF)  //!< All pins selected 

#define GPIO_PIN_CHECK(pin) ((pin) != (uint16_t)0x00)

#define GPIO_PIN_LIST_CHECK(pin) (((pin) == GPIO_PIN_0) || ((pin) == GPIO_PIN_1) || \
                                  ((pin) == GPIO_PIN_2) || ((pin) == GPIO_PIN_3) || \
                                  ((pin) == GPIO_PIN_4) || ((pin) == GPIO_PIN_5) || \
                                  ((pin) == GPIO_PIN_6) || ((pin) == GPIO_PIN_7) || \
                                  ((pin) == GPIO_PIN_8) || ((pin) == GPIO_PIN_9) || \
                                  ((pin) == GPIO_PIN_10) || ((pin) == GPIO_PIN_11) || \
                                  ((pin) == GPIO_PIN_12) || ((pin) == GPIO_PIN_13) || \
                                  ((pin) == GPIO_PIN_14) || ((pin) == GPIO_PIN_15))
/** 
  * @brief  GPIO pin number definition
  */
#define GPIO_PIN_NUM0            ((uint8_t)0x00)
#define GPIO_PIN_NUM1            ((uint8_t)0x01)
#define GPIO_PIN_NUM2            ((uint8_t)0x02)
#define GPIO_PIN_NUM3            ((uint8_t)0x03)
#define GPIO_PIN_NUM4            ((uint8_t)0x04)
#define GPIO_PIN_NUM5            ((uint8_t)0x05)
#define GPIO_PIN_NUM6            ((uint8_t)0x06)
#define GPIO_PIN_NUM7            ((uint8_t)0x07)
#define GPIO_PIN_NUM8            ((uint8_t)0x08)
#define GPIO_PIN_NUM9            ((uint8_t)0x09)
#define GPIO_PIN_NUM10           ((uint8_t)0x0A)
#define GPIO_PIN_NUM11           ((uint8_t)0x0B)
#define GPIO_PIN_NUM12           ((uint8_t)0x0C)
#define GPIO_PIN_NUM13           ((uint8_t)0x0D)
#define GPIO_PIN_NUM14           ((uint8_t)0x0E)
#define GPIO_PIN_NUM15           ((uint8_t)0x0F)

#define GPIO_PIN_NUM_CHECK(number) (((number) == GPIO_PIN_NUM0) || ((number) == GPIO_PIN_NUM1) || \
                                    ((number) == GPIO_PIN_NUM2) || ((number) == GPIO_PIN_NUM3) || \
                                    ((number) == GPIO_PIN_NUM4) || ((number) == GPIO_PIN_NUM5) || \
                                    ((number) == GPIO_PIN_NUM6) || ((number) == GPIO_PIN_NUM7) || \
                                    ((number) == GPIO_PIN_NUM8) || ((number) == GPIO_PIN_NUM9) || \
                                    ((number) == GPIO_PIN_NUM10) || ((number) == GPIO_PIN_NUM11) || \
                                    ((number) == GPIO_PIN_NUM12) || ((number) == GPIO_PIN_NUM13) || \
                                    ((number) == GPIO_PIN_NUM14) || ((number) == GPIO_PIN_NUM15))
                                       
/** 
  * @brief GPIO MF 0 (multi-function 0 selection) definition 
  */
#define GPIO_MF_SEL0      ((uint8_t)0x00) //!< WKUP, TIM15, SPI1, TIM17, CLKOUT, SWDAT, SWCLK, TIM14, BOOT,USART1,                                                
                                          //!< SPI2, TS, TIM3, USART4,  TIM3, USART2, USART3,  TIM16, TIM1. 
                                                                                                
/** 
  * @brief GPIO MF1(multi-function 1 selection) definition 
  */
#define GPIO_MF_SEL1      ((uint8_t)0x01) //!< USART2,, TIM3, USART1, IR, I2C1, I2C2, TIM15, SPI2,  SPI1.
                                                                                                
/** 
  * @brief GPIO MF2(multi-function 2 selection) definition 
  */
#define GPIO_MF_SEL2     ((uint8_t)0x02) //!< TIM2, TIM1.
                                                
/** 
  * @brief GPIO MF3(multi-function 3 selection) definition 
  */
#define GPIO_MF_SEL3      ((uint8_t)0x03) //!< TS, I2C1, TIM15, EVENTOUT. 

/** 
  * @brief GPIO MF4(multi-function 4 selection) definition 
  */
#define GPIO_MF_SEL4        ((uint8_t)0x04) //!< TIM14, USART4, USART3,  I2C1. 
                                                
/** 
  * @brief GPIO MF5(multi-function 5 selection) definition 
  */
#define GPIO_MF_SEL5        ((uint8_t)0x05) //!< TIM16, TIM17, TIM15, SPI2, I2C2, CLKOUT, I2C1
                                                
/** 
  * @brief GPIO MF6(multi-function 6 selection) definition 
  */
#define GPIO_MF_SEL6        ((uint8_t)0x06) //!< EVENTOUT.

/** 
  * @brief GPIO MF7(multi-function 7 selection) definition 
  */
#define GPIO_MF_SEL7       ((uint8_t)0x07) //!< COMP1 OUT and COMP2 OUT. 

#define GPIO_MF_SEL_CHECK(flag)   (((flag) == GPIO_MF_SEL0) || ((flag) == GPIO_MF_SEL1) || \
                                   ((flag) == GPIO_MF_SEL2) || ((flag) == GPIO_MF_SEL3) || \
                                   ((flag) == GPIO_MF_SEL4) || ((flag) == GPIO_MF_SEL5) || \
                                   ((flag) == GPIO_MF_SEL6) || ((flag) == GPIO_MF_SEL7))

/**
  * @}
  */
 


/**
  * @fn void gpio_def_init(gpio_reg_t* ptr_gpio)
  * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return None
  */
void gpio_def_init(gpio_reg_t* ptr_gpio);

/**
  * @fn void gpio_init(gpio_reg_t* ptr_gpio, gpio_config_t* ptr_config)
  * @brief  Initializes the GPIOx peripheral according to parameters in the ptr_config.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  ptr_config: pointer to a gpio_config_t structure that contains
  *         the configuration for the specified GPIO peripheral.
  * @return None
  */
void gpio_init(gpio_reg_t* ptr_gpio, gpio_config_t* ptr_config);

/**
  * @fn void gpio_struct_init(gpio_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its default value.
  * @param  ptr_config: pointer to a gpio_config_t structure which will be initialized.
  * @return None
  */
void gpio_struct_init(gpio_config_t* ptr_config);

/**
  * @fn void gpio_pin_lock_config(gpio_reg_t* ptr_gpio, uint16_t gpio_pin)
  * @brief  Locks GPIO Pins configuration registers, The locked registers are GPIOx_PFR,
  *         GPIOx_PODENR, GPIOx_POSR, GPIOx_PUPDR, GPIOx_MFSELL, GPIOx_MFSELH.     
  * @note   The configuration of the locked GPIO pins can no be modified until the device reset.
  * @param  ptr_gpio: pointer to GPIOx where x can be (A or B).
  * @param  pin: specifies the port pin to be written (GPIO_PIN_0 ~ GPIO_PIN_15).
  * @return None
  */
void gpio_pin_lock_config(gpio_reg_t* ptr_gpio, uint16_t pin);

/**
  * @fn uint8_t gpio_input_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t gpio_pin)
  * @brief  Reads the specified input port pin.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port pin to be written.
  * @return The input port pin value.
  */
uint8_t gpio_input_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin);

/**
  * @fn uint16_t gpio_input_data_read(gpio_reg_t* ptr_gpio)
  * @brief  Reads the specified input port pin.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return The input port pin value.
  */
uint16_t gpio_input_data_read(gpio_reg_t* ptr_gpio);

/**
  * @fn uint8_t gpio_output_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Reads the specified output data port bit.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: Specifies the port bit to read.
  * @return The output port pin value.
  */
uint8_t gpio_output_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin);

/**
  * @fn uint16_t gpio_output_data_read(gpio_reg_t* ptr_gpio) 
  * @brief  Reads the specified GPIO output data port.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return GPIO output data port value.
  */
uint16_t gpio_output_data_read(gpio_reg_t* ptr_gpio);

/**
  * @fn  void gpio_bits_set(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Sets the selected data port bits.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port bits to be written.
  * @return None
  */
void gpio_bits_set(gpio_reg_t* ptr_gpio, uint16_t pin);

/**
  * @fn void gpio_bits_reset(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Clears the selected data port bits.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port bits to be written.
  * @return None
  */
void gpio_bits_reset(gpio_reg_t* ptr_gpio, uint16_t pin);

/**
  * @fn void gpio_bit_write(gpio_reg_t* ptr_gpio, uint16_t pin, gpio_bit_t flag)
  * @brief Sets or clears the selected data port bit.
  * @param ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param pin: specifies the port bit to be written.
  * @param flag: specifies the value to be written to the selected bit.
  *        It can be BIT_RESET( clear the port pin), BIT_SET (set the port pin).
  * @return None
  */
void gpio_bit_write(gpio_reg_t* ptr_gpio, uint16_t pin, gpio_bit_t flag);

/**
  * @fn void gpio_write(gpio_reg_t* ptr_gpio, uint16_t value)
  * @brief  Writes data to the specified GPIO data port.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  value: specifies the value to be written to the port output data register.
  * @return None
  */
void gpio_write(gpio_reg_t* ptr_gpio, uint16_t value);

/**
  * @fn void gpio_mf_config(gpio_reg_t* ptr_gpio, uint16_t pin_num, uint8_t select)
  * @brief  Config the specified GPIO data port multi-function selection .
  * @param  ptr_gpio: where x can be (A, B, C, D, E or F) to select the GPIO peripheral.
  * @param  pin_num: specifies the pin for multi-function selection function.    
  * @param  select: selects the pin to used as multi-function selection.
  *          It can be one of the following value:
  *            @arg GPIO_MF_SEL0(WKUP, EVENTOUT, TIM15, SPI1, TIM17, CLKOUT, SWDAT, 
  *                        SWCLK, TIM14, BOOT, USART1, IR_OUT, SPI2, TIM3,
  *                         USART2, TIM16, TIM1, USART8)
  *            @arg GPIO_MF_SEL1(USART2,  TIM3, USART1, USART2,  I2C1,
  *                        I2C2, TIM15, SPI2, USART3, TS, SPI1)
  *            @arg GPIO_MF_SEL2(TIM2, TIM1, TIM16, TIM17)          
  * @return None
  */
void gpio_mf_config(gpio_reg_t* ptr_gpio, uint16_t pin_num, uint8_t select);

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

