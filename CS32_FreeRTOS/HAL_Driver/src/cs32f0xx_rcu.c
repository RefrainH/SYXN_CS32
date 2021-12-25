/**
  * @file    cs32f0xx_rcu.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Reset and clock unit (RCU) peripheral:
  *           + Internal/external clocks, PLL, CSS and CLKOUT configuration
  *           + System, AHB and APB busses clocks configuration
  *           + Peripheral clocks configuration
  *           + Interrupts and flags management
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
    1. After reset the device is running from HRC (8 MHz) ,all peripherals  are off except
          internal SRAM, Flash and SWD.
         a. There is no prescaler on High speed (AHB) and Low speed (APB) busses;
             all peripherals mapped on these busses are running at HRC speed.
         b. The clock for all peripherals is switched off, except the SRAM and FLASH.
         c. All GPIOs are in input floating state, except the SWD pins which are assigned
              to be used for debug purpose.
    2. Once the device started from reset, the user application has to:
         a. Configure the clock source to be used to drive the System clock
             (if the application needs higher frequency/performance)
         b. Configure the System clock frequency and Flash settings
         c. Configure the AHB and APB busses prescalers
         d. Enable the clock for the peripheral(s) to be used
         e. Configure the clock source(s) for peripherals which clocks are not
             derived from the System clock (ADC, CEC, I2C, USART, RTC and FWDT)

 @endverbatim
  *
  */
  
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup RCU
  * @brief RCU driver modules
  * @{
  */ 


/** 
  * @brief RCU Flag Mask
  */ 
#define RCU_FLAG_MASK                 ((uint8_t)0x1F)

/** 
  * @brief CTR register byte 2 (Bits[23:16]) base address
  */ 
#define RCU_CTR_BYTE2_ADDRESS          ((uint32_t)0x40021002)

/** 
  * @brief CFG register byte 3 (Bits[31:23]) base address 
  */ 
#define RCU_CFG_BYTE3_ADDRESS        ((uint32_t)0x40021007)

/** 
  * @brief  INTR register byte 1 (Bits[15:8]) base address
  */ 
#define RCU_INTR_BYTE1_ADDRESS         ((uint32_t)0x40021009)

/** 
  * @brief INTR register byte 2 (Bits[23:16]) base address 
  */ 
#define RCU_INTR_BYTE2_ADDRESS         ((uint32_t)0x4002100A)

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};



/**
  * @fn void rcu_def_init(void)
  * @brief  Resets the RCU clock configuration to the default reset state.
  * @note   The default reset state including HRC ON and used as system clock source 
  * @note   HRC14, HXT and PLL OFF AHB, APB prescaler set to 1, CSS and CLKOUT OFF
  * @note   All interrupts disabled, but it doesn't modify the configuration of the Peripheral 
  * @note   clocks LRC, LXT and RTC clocks
  * @param  None
  * @return None
  */
void rcu_def_init(void)
{
    /* Set HRCEN bit */
    RCU->CTR |= (uint32_t)0x00000001;

#if defined (CS32F051)
    /* Reset SYSSW[1:0], HCLKPDIV[3:0], PCLKPDIV[2:0] and CKOSEL[2:0] bits */
    RCU->CFG &= (uint32_t)0xF8FFB80C;
#else
    /* Reset SYSSW[1:0], HCLKPDIV[3:0], PCLKPDIV[2:0], ADCPDIV, CKOSEL[2:0], CKOPDIV[2:0] and CKOPLLND bits */
    RCU->CFG &= (uint32_t)0x08FFB80C;
#endif 
  
    /* Reset HXTON, HXTME and PLLEN bits */
    RCU->CTR &= (uint32_t)0xFEF6FFFF;

    /* Reset HXTBPS bit */
    RCU->CTR &= (uint32_t)0xFFFBFFFF;

    /* Reset PLLSEL, PLLHXTPDIV and PLLMUF[3:0] bits */
    RCU->CFG &= (uint32_t)0xFFC0FFFF;

    /* Reset PLLPDIV[3:0] bits */
    RCU->CFG2 &= (uint32_t)0xFFFFFFF0;

    /* Reset USARTSEL[1:0], I2CSEL, CECSEL and ADCSEL bits */
    RCU->CFG3 &= (uint32_t)0xFFF0FEAC;
  
    /* Reset HRC14 bit */
    RCU->CTR2 &= (uint32_t)0xFFFFFFFE;

    /* Disable all interrupts */
    RCU->INTR = 0x00000000;
}

/**
  * @fn void rcu_hxt_config(uint8_t value)
  * @brief  Configures the High speed Extern crystal (HXT).
  * @param  value: specifies the new state of the HXT, it can be one of the following values:
  *          @arg RCU_HXT_OFF: turn off the HXT oscillator                             
  *          @arg RCU_HXT_ON: turn on the HXT oscillator
  *          @arg RCU_HXT_BYPASS: HXT oscillator bypassed with external clock
  * @return None
  */
void rcu_hxt_config(uint8_t value)
{
    ASSERT(RCU_HXT_CHECK(value));

    /* Reset HXTEN and HXTBPS bits */
    *(__IO uint8_t *) RCU_CTR_BYTE2_ADDRESS = RCU_HXT_OFF;

    /* Set the new HXT configuration*/
    *(__IO uint8_t *) RCU_CTR_BYTE2_ADDRESS = value;

}

/**
  * @fn error_status_t rcu_hxt_stabilization_wait(void)
  * @brief  Waits for HXT stabilization. It waits on HSERDY flag to
  *         be set and return SUCCESS. The HXT is stopped by hardware 
  *         when entering STOP and STANDBY modes.
  * @param  None
  * @return SUCCESS(oscillator is stable) or ERROR(oscillator not yet stable)
  */
error_status_t rcu_hxt_stabilization_wait(void)
{
    __IO uint32_t counter = 0;
    error_status_t flag = ERROR;
    bit_status_t status = RESET;
  
    /* Wait till HXT is stable and if timeout is reached exit */
    do
    {
        status = rcu_flag_status_get(RCU_FLAG_HXT_STAB);
        counter++;  
    } while ((counter != HXT_STARTUP_TIMEOUT) && (status == RESET));
  
    if (rcu_flag_status_get(RCU_FLAG_HXT_STAB) != RESET)
    {
        flag = SUCCESS;
    }
    else
    {
        flag = ERROR;
    }  
    return (flag);
}

/**
  * @fn void rcu_hrc_calibration_adjust(uint8_t value)
  * @brief  Adjusts the Internal High speed RC (HRC) calibration value.
  * @param  value: specifies the HRC calibration trimming value (between 0 and 0x1F).
  * @return None
  */
void rcu_hrc_calibration_adjust(uint8_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_HRC_CALI_VALUE_CHECK(value));
  
    reg = RCU->CTR;
  
    /* Clear HRCTRIM[4:0] bits */
    reg &= ~RCU_CTR_HRCTRIM;
  
    /* Set the HRCTRIM[4:0] bits */
    reg |= (uint32_t)value << 3;

    RCU->CTR = reg;
}

/**
  * @fn void rcu_hrc_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal High Speed RC (HRC).
  * @param  enable_flag: ENABLE or DISABLE the HRC.
  * @return None
  */
void rcu_hrc_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    { 
        /* Enable HRC */
        RCU->CTR |= RCU_CTR_HRCEN;
    }
    else
    {
        /* Disable HRC*/
        RCU->CTR &= ~RCU_CTR_HRCEN;
    }
}

/**
  * @fn void rcu_hrc14_calibration_adjust(uint8_t value)
  * @brief  Adjusts the Internal High Speed RC14 for ADC (HRC14) 
  *         calibration value.
  * @param  value: specifies the HRC14 calibration trimming value.
  *         This parameter must be a number between 0 and 0x1F.
  * @return None
  */
void rcu_hrc14_calibration_adjust(uint8_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_HRC14_CALI_VALUE_CHECK(value));
    
    reg = RCU->CTR2;
    
    /* Clear HRC14TRIM[4:0] bits */
    reg &= ~RCU_CTR2_HRC14TRIM;
    
    /* Set the HRCTRIM14[4:0] bits */
    reg |= (uint32_t)value << 3;
    
    /* Store the new value */
    RCU->CTR2 = reg;
}

/**
  * @fn void rcu_hrc14_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal High Speed RC14 for ADC (HRC14).
  * @param  enable_flag: ENABLE or DISABLE HRC14.
  * @return None
  */
void rcu_hrc14_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
    
    if (enable_flag != DISABLE)
    {
        /* Enable HRC14 */
        RCU->CTR2 |= RCU_CTR2_HRC14EN;
    } 
    else
    {
        /* Disable HRC14 */
        RCU->CTR2 &= ~RCU_CTR2_HRC14EN;
    }
}

/**
  * @fn void rcu_hrc14_opening_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables ADC control HRC14 opening.
  * @param  enable_flag: ENABLE or DISABLE (HRC14 ADC opening).
  * @return None
  */
void rcu_hrc14_opening_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable ADC control HRC14 opening*/
        RCU->CTR2 &= ~RCU_CTR2_ADCDISHRC14;
    }
    else
    {
        /* Disable ADC control HRC14 opening*/
        RCU->CTR2 |= RCU_CTR2_ADCDISHRC14;
    }
}

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
void rcu_lxt_config(uint32_t value)
{
    ASSERT(RCU_LXT_CHECK(value));

    /* Reset LXTEN bit */
    RCU->VBDC &= ~(RCU_VBDC_LXTEN);

    /* Reset LXTBYP bit */
    RCU->VBDC &= ~(RCU_VBDC_LXTBYP);

    /* Configure LXT */
    RCU->VBDC |= value;
}

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
void rcu_lxt_drive_config(uint32_t value)
{
    ASSERT(RCU_LXT_DRIVE_CHECK(value));
  
    /* Clear LXTDRV[5:3] bits */
    RCU->VBDC &= ~(RCU_VBDC_LXTDRV);

    /* Set the LXT Drive */
    RCU->VBDC |= value;
}

/**
  * @fn void rcu_lrc_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Internal Low Speed oscillator (LRC).
  * @note   After enabling the LRC, the application software should wait on 
  * @note   LRCSTAB flag to be set indicating that LRC clock is stable and can
  * @note   be used to clock the FWDT and/or the RTC.
  * @param  enable_flag: ENABLE or DISABLE LRC.
  * @return None
  */
void rcu_lrc_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable LRC */
        RCU->STS |= RCU_STS_LRCEN;
    }
    else
    {
        /* Disable LRC */
        RCU->STS &= ~RCU_STS_LRCEN;
    }
}

/**
  * @fn void rcu_hxt_monitor_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the Clock Security Monitor.
  * @param  enable_flag: ENABLE or DISABL HXT Monitor.
  * @return None
  */
void rcu_hxt_monitor_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable HXT Monitor*/
        RCU->CTR |= RCU_CTR_HXTME;
    }
    else
    {
        /* Disable HXT Monitor*/
        RCU->CTR &= ~RCU_CTR_HXTME;
    }
}


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
void rcu_pll_config(uint32_t sel_value, uint32_t multiple)
{
    ASSERT(RCU_PLL_CFG_CHECK(sel_value));
    ASSERT(RCU_PLL_MULTI_CHECK(multiple));

    /* Clear PLLSEL [16] and PLLMUF [21:18] bits */
    RCU->CFG &= ~(RCU_CFG_PLLMUF | RCU_CFG_PLLSEL);
    /* Set the PLL source and multiplier */
    RCU->CFG |= (uint32_t)(sel_value | multiple);
}

/**
  * @fn void rcu_pll_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the PLL. After enabling the PLL, the application 
  *         software should wait on  PLLSTAB flag to be set indicating that PLL clock 
  *         is stable and can be used as system clock source.
  * @param  enable_flag: ENABLE or DISABLE PLL.
  * @return None
  */
void rcu_pll_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable PLL clock */
        RCU->CTR |= RCU_CTR_PLLEN;
    }
    else
    {
        /* Disable PLL clock */
        RCU->CTR &= ~RCU_CTR_PLLEN;
    }
}


/**
  * @fn void rcu_pll_divider_config(uint32_t value)
  * @brief  Configures the PLLPDIV division factor, used only when the PLL is disabled.    
  * @param  value: specifies the PLLPDIV clock division factor. It can be any
  *         of value between RCU_PLL_PREDIV_1 to RCU_PLL_PREDIV_16.
  * @return None
  */
void rcu_pll_divider_config(uint32_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_PLL_PREDIV_CHECK(value));

    reg = RCU->CFG2;
    /* Clear PLLPDIV[3:0] bits */
    reg &= ~(RCU_CFG2_PLLPDIV1);
    /* Set the PLLPDIV division factor */
    reg |= value;
    RCU->CFG2 = reg;
}


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
void rcu_clkout_config(uint8_t sel_value, uint32_t prescaler)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_CLKOUT_SEL_CHECK(sel_value));
    ASSERT(RCU_CLKOUT_PDIV_CHECK(prescaler));
    
    reg = RCU->CFG;
    /* Clear CFG_CKOPDIV[2:0] bits */
    reg &= ~(RCU_CFG_CKOPDIV | RCU_CFG_CKOSEL | RCU_CFG_CKOPLLND);
    /* Set the CFG_CKOPDIV and CFG_CKOSEL */
    reg |= (prescaler | ((uint32_t)sel_value<<24));
    RCU->CFG = reg;
}


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
void rcu_sysclk_config(uint32_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_SYSCLK_SEL_CHECK(value));
  
    reg = RCU->CFG;
  
    /* Clear SYSSW[1:0] bits */
    reg &= ~RCU_CFG_SYSSW;
    
    /* Set SYSSW[1:0] bits */
    reg |= value;
  
    RCU->CFG = reg;
}

/**
  * @fn uint8_t rcu_sysclk_switch_get(void)
  * @brief  Returns the clock source by switched.
  * @param  None
  * @return The clock source used as system clock. It an be one of the following values:
  *           @arg HRC used as system clock(0x00)
  *           @arg HXT used as system clock(0x04)  
  *           @arg PLL used as system clock(0x08)
  */
uint8_t rcu_sysclk_switch_get(void)
{
    return ((uint8_t)(RCU->CFG & RCU_CFG_SYSSS));
}

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
void rcu_hclk_config(uint32_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_HCLK_CFG_CHECK(value));
  
    reg = RCU->CFG;
  
    /* Clear HPRE[3:0] bits */
    reg &= ~RCU_CFG_HCLKPDIV;
  
    /* Set HPRE[3:0] bits */
    reg |= value; 
    
    RCU->CFG = reg;
}

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
void rcu_pclk_config(uint32_t value)
{
    uint32_t reg = 0;
  
    ASSERT(RCU_PCLK_CFG_CHECK(value));
  
    reg = RCU->CFG;
  
    /* Clear PCLKPDIV[2:0] bits */
    reg &= ~RCU_CFG_PCLKPDIV;
  
    /* Set PCLKPDIV[2:0] bits*/
    reg |= value;  

    RCU->CFG = reg;
}

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
void rcu_adcclk_config(uint32_t value)
{ 
    ASSERT(RCU_ADCCLK_CFG_CHECK(value));

    /* Clear ADCPDIV bit */
    RCU->CFG &= ~RCU_CFG_ADCPDIV;
    /* Set ADCPDIV bits  */
    RCU->CFG |= value & 0xFFFF;

    /* Clear ADCSEL bit */
    RCU->CFG3 &= ~RCU_CFG3_ADCSEL; 
    /* Set ADCSEL bits*/
    RCU->CFG3 |= value >> 16;  
}


/**
  * @fn void rcu_i2cclk_config(uint32_t value)
  * @brief  Configures the I2C1 clock (I2C1CLK).
  * @param  value: defines the I2C1 clock source which is derived from 
  *         the HRC or System clock. It can be one of the following values:
  *          @arg RCU_I2C1CLK_CFG_HRC (I2C1 clock = HRC)
  *          @arg RCU_I2C1CLK_CFG_SYSCLK (I2C1 clock = System Clock)
  * @return None
  */
void rcu_i2cclk_config(uint32_t value)
{ 

    ASSERT(RCU_I2C1CLK_CFG_CHECK(value));
	
    /* Clear I2CSEL bit */
    RCU->CFG3 &= ~RCU_CFG3_I2C1SEL;
    /* Set I2CSEL bits */
    RCU->CFG3 |= value;
}

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
void rcu_usartclk_config(uint32_t value)
{ 
    uint32_t temp = 0;
  
    ASSERT(RCU_USARTxCLK_CFG_CHECK(value));
    
    /* Get USART index */
    temp = (value >> 28);

    /* Clear USART1SEL[1:0] bits */
    if (temp == (uint32_t)0x00000001)
    {
        /* Clear USART1SEL[1:0] bits */  
        RCU->CFG3 &= ~RCU_CFG3_USART1SEL;
    }
    else if (temp == (uint32_t)0x00000002)
    {
        /* Clear USART2SEL[1:0] bits */
        RCU->CFG3 &= ~RCU_CFG3_USART2SEL;
    }
    else 
    {
        /* Clear USART3SEL[1:0] bits */
        RCU->CFG3 &= ~RCU_CFG3_USART3SEL;
    }

    /* Set USARTxSEL bits*/
    RCU->CFG3 |= value;
}   


/**
  * @fn void rcu_clk_freq_get(rcu_clock_t* ptr_clock)
  * @brief  Returns the frequencies of the System, AHB and APB busses clocks.             
  * @param  ptr_clock: pointer to a rcu_clock_t structure which will hold the clocks frequencies.  
  * @return None
  */
void rcu_clk_freq_get(rcu_clock_t* ptr_clock)
{
    uint32_t temp = 0;
    uint32_t divide = 0;  
    uint32_t presc = 0;
    uint32_t pll_mull = 0;
    uint32_t pll_sel = 0;
    uint32_t pll_clk = 0;

    temp = RCU->CFG & RCU_CFG_SYSSS;
  
    switch (temp)
    {
        case 0x00:  /* HRC used as system clock */
            ptr_clock->system_clk_freq = HRC_VALUE;
            break;

        case 0x04:  /* HXT used as system clock */
            ptr_clock->system_clk_freq = HXT_VALUE;
            break;

        case 0x08:  /* PLL used as system clock */
            pll_mull = RCU->CFG & RCU_CFG_PLLMUF;
            pll_sel = RCU->CFG & RCU_CFG_PLLSEL;
            pll_mull = ( pll_mull >> 18) + 2;
      
            if (pll_sel == 0x00)
            {
                /* HRC oscillator clock divided by 2 selected as PLL clock entry */
                pll_clk = (HRC_VALUE >> 1) * pll_mull;
            }
            else
            {
                divide = (RCU->CFG2 & RCU_CFG2_PLLPDIV1) + 1;
                /* HXT oscillator clock selected as PLLPDIV clock entry */
                pll_clk = (HXT_VALUE / divide) * pll_mull; 
            }
            ptr_clock->system_clk_freq = pll_clk;      
            break;

        default: /* HRC used as system clock */
            ptr_clock->system_clk_freq = HRC_VALUE;
            break;
    }
  
    /* Get HCLK prescaler */
    temp = RCU->CFG & RCU_CFG_HCLKPDIV;
    temp = temp >> 4;
    presc = APBAHBPrescTable[temp]; 
    /* HCLK clock frequency */
    ptr_clock->hclk_freq = ptr_clock->system_clk_freq >> presc;

    /* Get PCLK prescaler */
    temp = RCU->CFG & RCU_CFG_PCLKPDIV;
    temp = temp >> 8;
    presc = APBAHBPrescTable[temp];
    /* PCLK clock frequency */
    ptr_clock->pclk_freq = ptr_clock->hclk_freq >> presc;

    /* ADCCLK clock frequency */
    if((RCU->CFG3 & RCU_CFG3_ADCSEL) != RCU_CFG3_ADCSEL)
    {
        /* ADC Clock is HRC14 Osc. */
        ptr_clock->adc_clk_freq = HRC14_VALUE;
    }
    else
    {
        if((RCU->CFG & RCU_CFG_ADCPDIV) != RCU_CFG_ADCPDIV)
        {
            /* ADC Clock is derived from PCLK/2 */
            ptr_clock->adc_clk_freq = ptr_clock->pclk_freq >> 1;
        }
        else
        {
            /* ADC Clock is derived from PCLK/4 */
            ptr_clock->adc_clk_freq = ptr_clock->pclk_freq >> 2;
        }
    
    }

    /* I2C1CLK clock frequency */
    if((RCU->CFG3 & RCU_CFG3_I2C1SEL) != RCU_CFG3_I2C1SEL)
    {
        /* I2C1 Clock is HRC Osc. */
        ptr_clock->i2c1_clk_freq = HRC_VALUE;
    }
    else
    {
        /* I2C1 Clock is System Clock */
        ptr_clock->i2c1_clk_freq = ptr_clock->system_clk_freq;
    }

    /* USART1CLK clock frequency */
    if((RCU->CFG3 & RCU_CFG3_USART1SEL) == 0x0)
    {
        /* USART1 Clock is PCLK */
        ptr_clock->usart1_clk_freq = ptr_clock->pclk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART1SEL) == RCU_CFG3_USART1SEL_0)
    {
        /* USART1 Clock is System Clock */
        ptr_clock->usart1_clk_freq = ptr_clock->system_clk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART1SEL) == RCU_CFG3_USART1SEL_1)
    {
        /* USART1 Clock is LXT Osc. */
        ptr_clock->usart1_clk_freq = LXT_VALUE;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART1SEL) == RCU_CFG3_USART1SEL)
    {
        /* USART1 Clock is HRC Osc. */
        ptr_clock->usart1_clk_freq = HRC_VALUE;
    }
  
    /* USART2CLK clock frequency */
    if((RCU->CFG3 & RCU_CFG3_USART2SEL) == 0x0)
    {
        /* USART Clock is PCLK */
        ptr_clock->usart2_clk_freq = ptr_clock->pclk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART2SEL) == RCU_CFG3_USART2SEL_0)
    {
        /* USART Clock is System Clock */
        ptr_clock->usart2_clk_freq = ptr_clock->system_clk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART2SEL) == RCU_CFG3_USART2SEL_1)
    {
        /* USART Clock is LXT Osc. */
    ptr_clock->usart2_clk_freq = LXT_VALUE;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART2SEL) == RCU_CFG3_USART2SEL)
    {
        /* USART Clock is HRC Osc. */
        ptr_clock->usart2_clk_freq = HRC_VALUE;
    }
  
    /* USART3CLK clock frequency */
    if((RCU->CFG3 & RCU_CFG3_USART3SEL) == 0x0)
    {
        /* USART Clock is PCLK */
        ptr_clock->usart3_clk_freq = ptr_clock->pclk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART3SEL) == RCU_CFG3_USART3SEL_0)
    {
        /* USART Clock is System Clock */
        ptr_clock->usart3_clk_freq = ptr_clock->system_clk_freq;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART3SEL) == RCU_CFG3_USART3SEL_1)
    {
        /* USART Clock is LXT Osc. */
        ptr_clock->usart3_clk_freq = LXT_VALUE;
    }
    else if((RCU->CFG3 & RCU_CFG3_USART3SEL) == RCU_CFG3_USART3SEL)
    {
        /* USART Clock is HRC Osc. */
        ptr_clock->usart3_clk_freq = HRC_VALUE;
    }
  
}

/**
  * @fn void rcu_rtcclk_config(uint32_t value)
  * @brief  Configures the RTC clock (RTCCLK).           
  * @param  value: specifies the RTC clock source,it can be one of the following values:
  *          @arg RCU_RTCCLK_SEL_LXT(LXT selected as RTC clock)
  *          @arg RCU_RTCCLK_SEL_LRC(LRC selected as RTC clock)
  *          @arg RCU_RTCCLK_SEL_HXT(HXT divided by 32 selected as RTC clock)                                  
  * @return None
  */
void rcu_rtcclk_config(uint32_t value)
{
    ASSERT(RCU_RTCCLK_SEL_CHECK(value));
  
    /* Select the RTC clock source */
    RCU->VBDC |= value;
}

/**
  * @fn void rcu_rtcclk_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables the RTC clock. It must be used when
  *         the RTC clock source was selected using the rcu_rtcclk_config.
  * @param  enable_flag: ENABLE or DISABLE the RTC clock.
  * @return None
  */
void rcu_rtcclk_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the RTC clock */
        RCU->VBDC |= RCU_VBDC_RTCCLKEN;
    }
    else
    {
        /* Disable the RTC clock */
        RCU->VBDC &= ~RCU_VBDC_RTCCLKEN;
    }
}

/**
  * @fn void rcu_vbat_reset_enable_ctrl(enable_state_t enable_flag)
  * @brief  ENABLE or DISABLE VBAT domain software reset.
  * @param  enable_flag: ENABLE or DISABLE VBAT domain reset.
  * @return None
  */
void rcu_vbat_reset_enable_ctrl(enable_state_t enable_flag)
{
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable VBAT domain software reset */
        RCU->VBDC |= RCU_VBDC_VBTRST;
    }
    else
    {
        /* Disable VBAT domain software reset */
        RCU->VBDC &= ~RCU_VBDC_VBTRST;
    }
}

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
void rcu_ahb_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_AHB_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the AHB peripheral clock */
        RCU->AHBEN |= value;
    }
    else
    {
        /* Disable the AHB peripheral clock */
        RCU->AHBEN &= ~value;
    }
}

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
void rcu_apb2_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_APB2_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable APB2 peripheral clock */
        RCU->APB2EN |= value;
    }
    else
    {
        /* Disable APB2 peripheral clock */
        RCU->APB2EN &= ~value;
    }
}

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
void rcu_apb1_periph_clock_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_APB1_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable APB1 peripheral clock */
        RCU->APB1EN |= value;
    }
    else
    {
        /* Disable APB1 peripheral clock */
        RCU->APB1EN &= ~value;
    }
}

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
void rcu_ahb_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_AHB_RST_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable AHB peripheral */
        RCU->AHBRST |= value;
    }
    else
    {
        /* Disable AHB peripheral */
        RCU->AHBRST &= ~value;
    }
}

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
void rcu_apb2_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_APB2_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable APB peripheral */
        RCU->APB2RST |= value;
    }
    else
    {
        /* Disable APB peripheral */
        RCU->APB2RST &= ~value;
    }
}

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
void rcu_apb1_periph_reset_enable_ctrl(uint32_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_APB1_PERI_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));

    if (enable_flag != DISABLE)
    {
        /* Enable APB1 peripheral */
        RCU->APB1RST |= value;
    }
    else
    {
        /* Disable APB1 peripheral */
        RCU->APB1RST &= ~value;
    }
}

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
void rcu_interrupt_config(uint8_t value, enable_state_t enable_flag)
{
    ASSERT(RCU_INT_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Perform Byte access to RCU_INTR[13:8] bits to enable the selected interrupts */
        *(__IO uint8_t *) RCU_INTR_BYTE1_ADDRESS |= value;
    }
    else
    {
        /* Perform Byte access to RCU_INTR[13:8] bits to disable the selected interrupts */
        *(__IO uint8_t *) RCU_INTR_BYTE1_ADDRESS &= (uint8_t)~value;
    }
}

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
bit_status_t rcu_flag_status_get(uint8_t flag)
{
    uint32_t temp = 0;
    uint32_t status_reg = 0;
    bit_status_t status = RESET;

    ASSERT(RCU_FLAG_CHECK(flag));
    
    /* Get the RCU register index */
    temp = flag >> 5;
    
    if (temp == 0)               /* The flag to check is in CTR register */
    {
        status_reg = RCU->CTR;
    }
    else if (temp == 1)          /* The flag to check is in VBDC register */
    {
        status_reg = RCU->VBDC;
    }
    else if (temp == 2)          /* The flag to check is in STS register */
    {
        status_reg = RCU->STS;
    }
    else                        /* The flag to check is in CTR2 register */
    {
        status_reg = RCU->CTR2;
    }    

    /* Get the flag position */
    temp = flag & RCU_FLAG_MASK;

    if ((status_reg & ((uint32_t)1 << temp)) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    return status;
}

/**
  * @fn void rcu_flag_clear(void)
  * @brief  Clears the RCU reset flags.
  *         The reset flags are: RCU_FLAG_OBURSTF, RCU_FLAG_NRST, RCU_FLAG_V15RSTF,
  *         RCU_FLAG_PORRST, RCU_FLAG_SWRST, RCU_FLAG_FWDTRST, RCU_FLAG_WWDTRST,
  *         RCU_FLAG_LPERST.
  * @param  None
  * @return None
  */
void rcu_flag_clear(void)
{
    /* Set CRSTF bit to clear the reset flags */
    RCU->STS |= RCU_STS_CRSTF;
}

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
bit_status_t rcu_interrupt_status_get(uint8_t flag)
{
    bit_status_t bitstatus = RESET;
  
    ASSERT(RCU_GET_INT_CHECK(flag));
  
    /* Check the status of the specified RCU interrupt */
    if ((RCU->INTR & flag) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return  bitstatus;
}

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
void rcu_interrupt_clear(uint8_t flag)
{
    ASSERT(RCU_CLEAR_INT_CHECK(flag));
  
    /* Perform Byte access to RCU_INTR[23:16] bits to clear the selected interrupt bits */     
    *(__IO uint8_t *) RCU_INTR_BYTE2_ADDRESS = flag;
}


/**
  * @}
  */

/**
  * @}
  */

