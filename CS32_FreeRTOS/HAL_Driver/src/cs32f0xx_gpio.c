/**
  * @file    cs32f0xx_gpio.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the GPIO peripheral:
  *           + Initialization and Configuration functions
  *           + GPIO Read and Write functions
  *           + GPIO multi-function configuration functions
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
      1. Enable the GPIO AHB clock using rcu_ahb_periph_clock_enable_ctrl()
      2. Configure the GPIO pin(s) using gpio_init(),there is four possible configuration for each pin:          
         a.Input: Floating, Pull-up, Pull-down.
         b.Output: Push-Pull (Pull-up, Pull-down or no Pull). Open Drain (Pull-up, Pull-down or no Pull).
                   In output mode, the speed is configurable: Low, Medium, Fast or High.              
         c. Multi-function:Push-Pull /Open Drain (Pull-up, Pull-down or no Pull).                                  
         d. Analog: required mode when a pin is to be used as ADC channel,DAC output or comparator input.              
      3. Peripherals  multi-function:
         a. For ADC, DAC and comparators, configure the desired pin in analog 
              mode using gpio_config_t.gpio_mode = GPIO_MODE_AN
         b. For other peripherals (TIM, USART...):
              (1). Connect the pin to the desired peripherals' MFSEL (multi-function selection)
              using gpio_mf_config() function. For PortC, 
                    PortD and PortF, no configuration is needed.
              (2). Configure the desired pin in  multi-function mode using
                    gpio_config_t->gpio_mode = GPIO_MODE_MF
              (3). Select the type, pull-up/pull-down and output speed via 
                    gpio_pull, gpio_out_type and gpio_speed members
              (4). Call gpio_init() function
      4. To get the level of a pin configured in input mode use gpio_input_data_bit_read()
      5. To set/reset the level of a pin configured in output mode use
          gpio_bits_set()/gpio_bits_reset()
      6. During and just after reset, the  multi-functions are not active and 
          the GPIO pins are configured in input floating mode (except JTAG pins).
      7. The LXT oscillator pins OSC32_IN and OSC32_OUT can be used as 
          general-purpose (PC14 and PC15, respectively) when the LXT oscillator 
          is off. The LXT has priority over the GPIO function.
      8. The HXT oscillator pins OSC_IN/OSC_OUT can be used as general-purpose 
          PD0 and PD1, respectively, when the HXT oscillator is off. The HXT has 
          priority over the GPIO function.
    @endverbatim
  *
  */

#include "cs32f0xx_gpio.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup GPIO
  * @{
  */


/**
  * @fn void gpio_def_init(gpio_reg_t* ptr_gpio)
  * @brief  Deinitializes the GPIOx peripheral registers to their default reset values.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return None
  */
void gpio_def_init(gpio_reg_t* ptr_gpio)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));

    if(ptr_gpio == GPIOA)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTA, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTA, DISABLE);
    }
    else if(ptr_gpio == GPIOB)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTB, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTB, DISABLE);
    }
    else if(ptr_gpio == GPIOC)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTC, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTC, DISABLE);
    }
    else if(ptr_gpio == GPIOD)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTD, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTD, DISABLE);
    }
    else if(ptr_gpio == GPIOE)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTE, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTE, DISABLE);
    }
    else if(ptr_gpio == GPIOF)
    {
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTF, ENABLE);
        rcu_ahb_periph_reset_enable_ctrl(RCU_AHB_PERI_PORTF, DISABLE);
    }
}

/**
  * @fn void gpio_init(gpio_reg_t* ptr_gpio, gpio_config_t* ptr_config)
  * @brief  Initializes the GPIOx peripheral according to parameters in the ptr_config.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  ptr_config: pointer to a gpio_config_t structure that contains
  *         the configuration for the specified GPIO peripheral.
  * @return None
  */
void gpio_init(gpio_reg_t* ptr_gpio, gpio_config_t* ptr_config)
{
    uint32_t pin = 0x00;
    uint32_t pos = 0x00;
    uint32_t current_pin = 0x00;

    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_CHECK(ptr_config->gpio_pin));
    ASSERT(GPIO_MODE_CHECK(ptr_config->gpio_mode));
    ASSERT(GPIO_PULL_CHECK(ptr_config->gpio_pull));

    for (pin = 0x00; pin < 0x10; pin++)
    {
        pos = ((uint32_t)0x01) << pin;

        /* Get the port pins position */
        current_pin = (ptr_config->gpio_pin) & pos;

        if (current_pin == pos)
        {
            if ((ptr_config->gpio_mode == GPIO_MODE_OUTPUT) || (ptr_config->gpio_mode == GPIO_MODE_MF))
            {
                ASSERT(GPIO_SPEED_CHECK(ptr_config->gpio_speed));

                /* Speed mode configuration */
                ptr_gpio->POSR &= ~(GPIO_POSR_OS0 << (pin * 2));
                ptr_gpio->POSR |= ((uint32_t)(ptr_config->gpio_speed) << (pin * 2));

                ASSERT(GPIO_OUTPUT_TYPE_CHECK(ptr_config->gpio_out_type));

                /* Output mode configuration */
                ptr_gpio->PODENR &= ~((GPIO_PODENR_ODEN_0) << ((uint16_t)pin));
                ptr_gpio->PODENR |= (uint16_t)(((uint16_t)ptr_config->gpio_out_type) << ((uint16_t)pin));
            }
			
            ptr_gpio->PFR  &= ~(GPIO_PFR_FUNC0 << (pin * 2));
            ptr_gpio->PFR |= (((uint32_t)ptr_config->gpio_mode) << (pin * 2));

            /* Pull-up Pull down configuration */
            ptr_gpio->PUPDR &= ~(GPIO_PUPDR_PUPD0 << ((uint16_t)pin * 2));
            ptr_gpio->PUPDR |= (((uint32_t)ptr_config->gpio_pull) << (pin * 2));
        }
    }
}

/**
  * @fn void gpio_struct_init(gpio_config_t* ptr_config)
  * @brief  Fills each ptr_config member with its default value.
  * @param  ptr_config: pointer to a gpio_config_t structure which will be initialized.
  * @return None
  */
void gpio_struct_init(gpio_config_t* ptr_config)
{
    /* Set gpio_pin with its default value */
    ptr_config->gpio_pin  = GPIO_PIN_ALL;
    /* Set gpio_mode with its default value */
    ptr_config->gpio_mode = GPIO_MODE_INPUT;
    /* Set gpio_speed with its default value */
    ptr_config->gpio_speed = GPIO_SPEED_MEDIUM;
    /* Set gpio_out_type with its default value */
    ptr_config->gpio_out_type = GPIO_OUTPUT_PP;
    /* Set gpio_pull with its default value */
    ptr_config->gpio_pull = GPIO_PULL_NO_PULL;
}

/**
  * @fn void gpio_pin_lock_config(gpio_reg_t* ptr_gpio, uint16_t gpio_pin)
  * @brief  Locks GPIO Pins configuration registers, The locked registers are GPIOx_PFR,
  *         GPIOx_PODENR, GPIOx_POSR, GPIOx_PUPDR, GPIOx_MFSELL, GPIOx_MFSELH.     
  * @note   The configuration of the locked GPIO pins can no be modified until the device reset.
  * @param  ptr_gpio: pointer to GPIOx where x can be (A or B).
  * @param  pin: specifies the port pin to be written (GPIO_PIN_0 ~ GPIO_PIN_15).
  * @return None
  */
void gpio_pin_lock_config(gpio_reg_t* ptr_gpio, uint16_t pin)
{
    __IO uint32_t temp = 0x00010000;

    ASSERT(GPIO_PORT_LIST1_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_CHECK(pin));

    temp |= pin;
    /* Set LOCK */  
    ptr_gpio->LOCK = temp;  
    /* Reset LOCK */ 
    ptr_gpio->LOCK =  pin;
    /* Set LOCK */ 
    ptr_gpio->LOCK = temp; 
    /* Get LOCK */  
    temp = ptr_gpio->LOCK;
    /* Get LOCK */   
    temp = ptr_gpio->LOCK;
}

/**
  * @fn uint8_t gpio_input_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t gpio_pin)
  * @brief  Reads the specified input port pin.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port pin to be written.
  * @return The input port pin value.
  */
uint8_t gpio_input_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin)
{
    uint8_t status = 0x00;

    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_LIST_CHECK(pin));

    if ((ptr_gpio->DI & pin) != (uint32_t)BIT_RESET)
    {
        status = (uint8_t)BIT_SET;
    }
    else
    {
        status = (uint8_t)BIT_RESET;
    }
    return status;
}

/**
  * @fn uint16_t gpio_input_data_read(gpio_reg_t* ptr_gpio)
  * @brief  Reads the specified input port pin.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return The input port pin value.
  */
uint16_t gpio_input_data_read(gpio_reg_t* ptr_gpio)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));

    return ((uint16_t)ptr_gpio->DI);
}

/**
  * @fn uint8_t gpio_output_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Reads the specified output data port bit.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: Specifies the port bit to read.
  * @return The output port pin value.
  */
uint8_t gpio_output_data_bit_read(gpio_reg_t* ptr_gpio, uint16_t pin)
{
    uint8_t status = 0x00;

    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_LIST_CHECK(pin));

    if ((ptr_gpio->DO & pin) != (uint32_t)BIT_RESET)
    {
        status = (uint8_t)BIT_SET;
    }
    else
    {
        status = (uint8_t)BIT_RESET;
    }
    return status;
}

/**
  * @fn uint16_t gpio_output_data_read(gpio_reg_t* ptr_gpio) 
  * @brief  Reads the specified GPIO output data port.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @return GPIO output data port value.
  */
uint16_t gpio_output_data_read(gpio_reg_t* ptr_gpio)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));

    return ((uint16_t)ptr_gpio->DO);
}

/**
  * @fn  void gpio_bits_set(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Sets the selected data port bits.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port bits to be written.
  * @return None
  */
void gpio_bits_set(gpio_reg_t* ptr_gpio, uint16_t pin)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_CHECK(pin));

    ptr_gpio->SCR = pin;
}

/**
  * @fn void gpio_bits_reset(gpio_reg_t* ptr_gpio, uint16_t pin)
  * @brief  Clears the selected data port bits.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  pin: specifies the port bits to be written.
  * @return None
  */
void gpio_bits_reset(gpio_reg_t* ptr_gpio, uint16_t pin)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_CHECK(pin));

    ptr_gpio->CLRR = pin;
}

/**
  * @fn void gpio_bit_write(gpio_reg_t* ptr_gpio, uint16_t pin, gpio_bit_t flag)
  * @brief Sets or clears the selected data port bit.
  * @param ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param pin: specifies the port bit to be written.
  * @param flag: specifies the value to be written to the selected bit.
  *        It can be BIT_RESET( clear the port pin), BIT_SET (set the port pin).
  * @return None
  */
void gpio_bit_write(gpio_reg_t* ptr_gpio, uint16_t pin, gpio_bit_t flag)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_LIST_CHECK(pin));
    ASSERT(GPIO_BIT_STATE_CHECK(flag));

    if (flag != BIT_RESET)
    {
        ptr_gpio->SCR = pin;
    }
    else
    {
        ptr_gpio->CLRR = pin;
    }
}

/**
  * @fn void gpio_write(gpio_reg_t* ptr_gpio, uint16_t value)
  * @brief  Writes data to the specified GPIO data port.
  * @param  ptr_gpio: pointer to GPIOx where x can be A, B, C, D, E or F.
  * @param  value: specifies the value to be written to the port output data register.
  * @return None
  */
void gpio_write(gpio_reg_t* ptr_gpio, uint16_t value)
{
    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));

    ptr_gpio->DO = value;
}

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
void gpio_mf_config(gpio_reg_t* ptr_gpio, uint16_t pin_num, uint8_t select)
{
    uint32_t temp = 0x00;
    uint32_t temp1 = 0x00;

    ASSERT(GPIO_ALL_PORT_CHECK(ptr_gpio));
    ASSERT(GPIO_PIN_NUM_CHECK(pin_num));
    ASSERT(GPIO_MF_SEL_CHECK(select));

    temp = ((uint32_t)(select) << ((uint32_t)((uint32_t)pin_num & (uint32_t)0x07) * 4));
    ptr_gpio->MFSEL[pin_num >> 0x03] &= ~((uint32_t)0xF << ((uint32_t)((uint32_t)pin_num & (uint32_t)0x07) * 4));
    temp1 = ptr_gpio->MFSEL[pin_num >> 0x03] | temp;
    ptr_gpio->MFSEL[pin_num >> 0x03] = temp1;
}

/**
  * @}
  */

/**
  * @}
  */

