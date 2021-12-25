/**
  * @file    cs32f0xx_tim.c
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the TIM peripheral(TimeBase management;
  *             Output Compare management; Input Capture management;
  *             Interrupts, DMA and flags management; Clocks management;
  *             Synchronization management; Specific interface management;
  *             Specific remapping management).
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
    1. TIM TimeBase management includes all needed functions to configure the TM Timebase unit:        
          Set/Get Prescaler; Set/Get Update; Counter modes configuration; Set Clock division;
          Single pulse enable; Update Request selection; Update Disable Configuration;
          UVAL shadow enable; nable/Disable the counter.
         
    2. TIM Output Compare management includes all needed functions to configure the Capture/Compare: 
          Configure each channel, independently; Select the output compare modes;
          Select the Polarities of each channel; Set/Get the Capture/Compare register values;
          Select the Output Compare Fast mode; Select the Output Compare Forced mode;
          Output Compare-Preload Configuration; Clear Output Compare Reference;
          Select the OCREF Clear selection; Enable/Disable the Capture/Compare Channels.
 
    3. TIM Input Capture management includes all needed functions to configure the Capture/Compare:         
          Configure each channel in input capture mode; Configure Channel1/2 in PWM Input mode;
          Set the Input Capture Prescaler; Get the Capture/Compare values.
         
   4. Advanced-control timers (TIM1) specific features:
         Configures the Break input, dead time, Lock level, the OSSI, the OSSR State and the 
         AOE(automatic output enable); Enable/Disable the TIM peripheral Main Outputs; 
         Select the Commutation event; Set/Reset the Capture Compare Preload Control bit.

    5. TIM interrupts, DMA and flags management.
         Enable/Disable interrupt sources; Get flags status; Clear flags/ Pending bits;
         Enable/Disable DMA requests; Configure DMA burst mode; Select CaptureCompare DMA request

    6. TIM clocks management: this group includes all needed functions to configure
        the clock controller unit:
         Select Internal/External clock; Select the external clock mode: ETR(Mode1/Mode2), TIx or ITRx.        

    7. TIM synchronization management: this group includes all needed functions to configure
        the Synchronization unit:
         Select Input Trigger; Select Output Trigger;  
         Select Master Slave Mode; ETR Configuration when used as external trigger. 

    8. TIM specific interface management, this group includes all needed functions to 
       use the specific TIM interface:
         Encoder Interface Configuration; Select Hall Sensor.   

    9.TIM specific remapping management includes the Remapping configuration of specific timers.

    @endverbatim
  *
  */  

#include "cs32f0xx_tim.h"
#include "cs32f0xx_rcu.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @defgroup TIM 
  * @brief TIM driver modules
  * @{
  */

#define TIM_SMCFG_ET_MASK             ((uint16_t)0x00FF) 
#define TIM_CHxxCFGR_OFFSET           ((uint16_t)0x0018)
#define TIM_CCCTR_CHxCCEN_SET         ((uint16_t)0x0001)
#define TIM_CCCTR_CHxNCCEN_SET        ((uint16_t)0x0004) 
  

static void tim_ch1ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter); 
static void tim_ch2ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter);
static void tim_ch3ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter);
static void tim_ch4ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter);  


/**
  * @fn void tim_def_init(tim_reg_t* ptr_timer)
  * @brief  Deinitializes the TIMx peripheral registers to their default reset values.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return None
  */
void tim_def_init(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer)); 

    if (ptr_timer == TIM1)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM1, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM1, DISABLE);  
    }     
    else if (ptr_timer == TIM2)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM2, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM2, DISABLE);
    }
    else if (ptr_timer == TIM3)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM3, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM3, DISABLE);
    }
    else if (ptr_timer == TIM6)
    {
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM6, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM6, DISABLE);
    } 
    else if (ptr_timer == TIM14) 
    {       
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM14, ENABLE);
        rcu_apb1_periph_reset_enable_ctrl(RCU_APB1_PERI_TIM14, DISABLE);  
    }        
    else if (ptr_timer == TIM15)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM15, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM15, DISABLE);
    } 
    else if (ptr_timer == TIM16)
    {
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM16, ENABLE);
        rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM16, DISABLE);
    } 
    else
    {
        if (ptr_timer == TIM17)
        {
            rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM17, ENABLE);
            rcu_apb2_periph_reset_enable_ctrl(RCU_APB2_PERI_TIM17, DISABLE);
        }  
    }
     
}

/**
  * @fn void tim_timer_config(tim_reg_t* ptr_timer, timer_config_t* ptr_config)
  * @brief  Initializes the TIMx Time Base Unit peripheral according to 
  *         the specified parameters in the TIM_TimeBaseInitStruct.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices. 
  * @param  ptr_config: pointer to a timer_config_t structure that contains the configuration 
  *         information for the specified TIM peripheral.
  * @return None
  */
void tim_timer_config(tim_reg_t* ptr_timer, timer_config_t* ptr_config)
{
    uint16_t temp = 0;
    
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_COUNT_PATTERN_CHECK(ptr_config->count_mode));
    ASSERT(TIM_CKDIV_CHECK(ptr_config->clock_divide));

    temp = ptr_timer->CTR1;  

    if ((ptr_timer == TIM1) || (ptr_timer == TIM2) || (ptr_timer == TIM3))
    {
        /* Select the Counter  */
        temp &= (uint16_t)(~((uint16_t)(TIM_CTR1_DIR | TIM_CTR1_CPS)));
        temp |= (uint32_t)ptr_config->count_mode;
    }
 
    if (ptr_timer != TIM6)
    {
        /* Set the clock division */
        temp &= (uint16_t)(~((uint16_t)TIM_CTR1_CKDIV));
        temp |= (uint32_t)ptr_config->clock_divide;
    }

    ptr_timer->CTR1 = temp;

    /* Set the Autoreload value */
    ptr_timer->UVAL = ptr_config->time_period ; 
    ptr_timer->PDIV = ptr_config->time_divide;
    
    if ((ptr_timer == TIM1) || (ptr_timer == TIM15)|| (ptr_timer == TIM16) || (ptr_timer == TIM17))  
    {
        if (ptr_config->count_mode == TIM_COUNT_PATTERN_DOWN)
        {
            /* Set the Repetition Counter value */
            ptr_timer->UVALREP = ptr_config->repeate_count + 1;
        }
        else {
            /* Set the Repetition Counter value */
            ptr_timer->UVALREP = ptr_config->repeate_count;
        }
    }

    /*  update event to reload the Prescaler and the Repetition counter values  */    
    ptr_timer->SWEGR = TIM_PDIV_MODE_IMMEDIATE;           
}

/**
  * @fn void tim_config_struct_init(timer_config_t* ptr_config)
  * @brief  Fills each TIM_TimeBaseInitStruct member with its default value.
  * @param  ptr_config: pointer to a timer_config_t structure which will be initialized.
  * @return None
  */
void tim_config_struct_init(timer_config_t* ptr_config)
{
    ptr_config->time_period = 0xFFFFFFFF;
    ptr_config->time_divide = 0x0000;
    ptr_config->clock_divide = TIM_CKDIV_DIV1;
    ptr_config->count_mode = TIM_COUNT_PATTERN_UP;
    ptr_config->repeate_count = 0x0000;
}

/**
  * @fn void tim_pdiv_register_config(tim_reg_t* ptr_timer, uint16_t value, uint16_t flag)
  * @brief  Configures the TIMx PDIV (pre-divider).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Prescaler Register value
  * @param  flag: specifies the TIM Prescaler Reload mode, it be one of the following values:
  *          @arg TIM_PDIV_MODE_UPDATE (The Prescaler is loaded at the update event)
  *          @arg TIM_PDIV_MODE_IMMEDIATE (The Prescaler is loaded immediatly)
  * @return None
  */
void tim_pdiv_register_config(tim_reg_t* ptr_timer, uint16_t value, uint16_t flag)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_PDIV_MODE_CHECK(flag));
  
    /* Set the pre-divider value */
    ptr_timer->PDIV = value;
    /* Set or reset the UEG Bit */
    ptr_timer->SWEGR = flag;
}

/**
  * @fn void tim_count_pattern_config(tim_reg_t* ptr_timer, uint16_t value)
  * @brief Specifies the TIMx Counter Mode to be used.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, or 3 to select the TIM peripheral
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Counter Mode to be used ,it can be one of the following values:
  *          @arg TIM_COUNT_PATTERN_UP (TIM Up Counting)
  *          @arg TIM_COUNT_PATTERN_DOWN (TIM Down Counting)
  *          @arg TIM_COUNT_PATTERN_CENTER_ALIGNED1 (TIM Center Aligned 1)
  *          @arg TIM_COUNT_PATTERN_CENTER_ALIGNED2 (TIM Center Aligned 2)
  *          @arg TIM_COUNT_PATTERN_CENTER_ALIGNED3 (TIM Center Aligned 3)
  * @return None
  */
void tim_count_pattern_config(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t temp = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_COUNT_PATTERN_CHECK(value));
  
    temp = ptr_timer->CTR1; 
    temp &= (uint16_t)(~((uint16_t)(TIM_CTR1_DIR | TIM_CTR1_CPS)));  /* clear the CPS and DIR Bits */
  
    /* Set the count pattern selection */
    temp |= value;  
    ptr_timer->CTR1 = temp;
}

/**
  * @fn void tim_counter_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Sets the TIMx Counter Register value
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Counter register new value.
  * @return None
  */
void tim_counter_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
   
    /* Set the Counter Register value */
    
    ptr_timer->CNT = value;
}

/**
  * @fn void tim_counter_update_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Sets the counter update register value,
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the counter update register value.
  * @return None
  */
void tim_counter_update_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
  
    /* Set counter update register */
    ptr_timer->UVAL = value;
}   

/**
  * @fn uint32_t tim_counter_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx Counter value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Counter Register value.
  */
uint32_t tim_counter_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
  
    /* Get the Counter Register value */
    return ptr_timer->CNT;
}

/**
  * @fn uint16_t tim_pdiv_register_value_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx Prescaler value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Prescaler Register value.
  */
uint16_t tim_pdiv_register_value_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
  
    /* Get the Prescaler Register value */
    return ptr_timer->PDIV;
}

/**
  * @fn void tim_update_disable_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief Enables or Disables the TIMx Update event.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE the CTR1_UPD bit
  * @return None
  */
void tim_update_disable_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the Update Disable Bit */
        ptr_timer->CTR1 |= TIM_CTR1_UPD;
    }
    else
    {
        /* Reset the Update Disable Bit */
        ptr_timer->CTR1 &= (uint16_t)~((uint16_t)TIM_CTR1_UPD);
    }
}

/**
  * @fn void tim_update_request_select(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  select the TIMx Update Request Interrupt source.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Update source, it can be one of the following values:
  *          @arg TIM_UPDATE_SEL_REGULAR (Source of update is the counter overrun/underrun or
  *               the setting of UG bit, or an update generation through the slave mode controller)
  *          @arg TIM_UPDATE_SEL_GLOBAL (Source of update is counter overflow/underflow)
  * @return None
  */
void tim_update_request_select(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_UPDATE_SEL_CHECK(value));
  
    if (value != TIM_UPDATE_SEL_GLOBAL)
    {
        /* Set the URSEL Bit */
        ptr_timer->CTR1 |= TIM_CTR1_URSEL;
    }
    else
    {
        /* Reset the URSEL Bit */
        ptr_timer->CTR1 &= (uint16_t)~((uint16_t)TIM_CTR1_URSEL);
    }
}

/**
  * @fn void tim_uval_shadow_config(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief Enables or disables TIMxUVAL shadow register on UVALSEN.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE the TIMx UVAL shadow register.
  * @return None
  */
void tim_uval_shadow_config(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the UVAL shadow Bit */
        ptr_timer->CTR1 |= TIM_CTR1_UVALSEN;
    }
    else
    {
        /* clear the UVAL shadow Bit */
        ptr_timer->CTR1 &= (uint16_t)~((uint16_t)TIM_CTR1_UVALSEN);
    }
}

/**
  * @fn void tim_clock_division_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets the TIMx Clock Division value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3,  14, 15, 16 and 17,
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the clock division value, it can be one of the following value:
  *          @arg TIM_CKDIV_DIV1
  *          @arg TIM_CKDIV_DIV2
  *          @arg TIM_CKDIV_DIV4
  * @return None
  */
void tim_clock_division_set(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CKDIV_CHECK(value));
  
    /* clear the CKDIV Bits */
    ptr_timer->CTR1 &= (uint16_t)~((uint16_t)TIM_CTR1_CKDIV);
    /* Set the CKDIV value */
    ptr_timer->CTR1 |= value;
}

/**
  * @fn void tim_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief Enables or disables the specified TIM peripheral.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE the TIMx peripheral.
  * @return None
  */
void tim_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer)); 
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the TIM Counter */
        ptr_timer->CTR1 |= TIM_CTR1_CEN;
    }
    else
    {
        /* Disable the TIM Counter */
        ptr_timer->CTR1 &= (uint16_t)(~((uint16_t)TIM_CTR1_CEN));
    }
}
  
/**
  * @fn void tim_chopr_register_config(tim_reg_t* ptr_timer, timer_protect_t *ptr_protect)
  * @brief  Configures the: Break feature, dead time, Lock level, OSSI/OSSR State
  *         and the AOE(automatic output enable).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 15, 16 or 17. 
  * @param  ptr_protect: pointer to a timer_protect_t structure that contains the CHOPR 
  *         Register configuration for the TIM peripheral.
  * @return None
  */
void tim_chopr_register_config(tim_reg_t* ptr_timer, timer_protect_t *ptr_protect)
{
    ASSERT(TIM_LIST2_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_RUNOS_CHECK(ptr_protect->run_offstate));
    ASSERT(TIM_IDLEOS_CHECK(ptr_protect->idle_offstate));
    ASSERT(TIM_LOCK_LEVEL_CHECK(ptr_protect->lock_level));
    ASSERT(TIM_BREAK_CHECK(ptr_protect->break_flag));
    ASSERT(TIM_BREAK_POLARITY_CHECK(ptr_protect->break_polarity));
    ASSERT(TIM_CHOPAEN_CHECK(ptr_protect->auto_output));
  
    ptr_timer->CHOPR = ptr_protect->run_offstate | ptr_protect->idle_offstate  |
                                   ptr_protect->lock_level  | ptr_protect->dead_time      |
                                   ptr_protect->break_flag  | ptr_protect->break_polarity |
                                   ptr_protect->auto_output;
}

/**
  * @fn void tim_protect_struct_init(timer_protect_t* ptr_protect)
  * @brief  Fills each ptr_protect member with its default value.
  *         CHOPR(TIMx channel output protect register)
  * @param  ptr_protect: pointer to a timer_protect_t structure be initialized.
  * @return None
  */
void tim_protect_struct_init(timer_protect_t* ptr_protect)
{
    ptr_protect->run_offstate = TIM_RUNOS_DISABLE;
    ptr_protect->idle_offstate = TIM_IDLEOS_DISABLE;
    ptr_protect->lock_level = TIM_LOCK_LEVEL_OFF;
    ptr_protect->dead_time = 0x00;
    ptr_protect->break_flag = TIM_BREAK_DISABLE;
    ptr_protect->break_polarity = TIM_BREAK_POLARITY_LOW;
    ptr_protect->auto_output = TIM_CHOPAEN_DISABLE;
}

/**
  * @fn void tim_pwm_output_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief  Enables or disables the TIMx Channel output pad.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 15, 16 or 17.
  * @param  enable_flag: ENABLE or DISABLE the TIM peripheral Main Outputs.
  * @return None
  */
void tim_pwm_output_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_LIST2_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Channel output  enable */
        ptr_timer->CHOPR |= TIM_CHOPR_CHOPEN;
    }
    else
    {
        /*Channel output  disable */
        ptr_timer->CHOPR &= (uint16_t)(~((uint16_t)TIM_CHOPR_CHOPEN));
    }  
}

/**
  * @fn void tim_ch1oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
  * @brief  Initializes the TIMx channel1 output compare(CH1OC).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 and 17
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_compare: pointer to a timer_compare_t structure  that contains 
  *         the configuration for the specified TIM peripheral.
  * @return None
  */
void tim_ch1oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
{
    uint16_t reg_channel12 = 0;
    uint16_t reg_ccctr = 0;
    uint16_t reg_ctr2 = 0;
   
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOCMSEL_CHECK(ptr_compare->time_mode));
    ASSERT(TIM_CHx_OUTPUT_CHECK(ptr_compare->output_state));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(ptr_compare->output_polarity));   
    if ((ptr_compare->time_mode == TIM_CHxOCMSEL_PWM1) && ((ptr_timer->CTR1 & 0x0010) == 0x10) && (tim_pdiv_register_value_get(ptr_timer) != 0))
    {
		
    }
    else
    {
        /* Disable the Channel 1: Reset the CH1CCEN Bit */
        ptr_timer->CCCTR &= (uint16_t)(~(uint16_t)TIM_CCCTR_CH1CCEN);
      
        reg_ccctr = ptr_timer->CCCTR;  
        reg_ctr2 =  ptr_timer->CTR2;
        reg_channel12 = ptr_timer->CH12CFGR;
        
        /* Clear the Output Compare Mode Bits */
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH12CFGR_CH1OCMSEL));
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH12CFGR_CH1FS));

        /* Select the Output Compare Mode */
        reg_channel12 |= ptr_compare->time_mode;
      
        /* clear the Output Polarity level */
        reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH1CCP));
      
        /* Set the Output Compare Polarity */
        reg_ccctr |= ptr_compare->output_polarity;
      
        /* Set the Output State */
        reg_ccctr |= ptr_compare->output_state;
        
        if((ptr_timer == TIM1) || (ptr_timer == TIM15) || (ptr_timer == TIM16) || (ptr_timer == TIM17))
        {
            ASSERT(TIM_CHxNCCEN_CHECK(ptr_compare->output_state_n));
            ASSERT(TIM_CHxNCCP_POLARITY_CHECK(ptr_compare->output_polarity_n));
            ASSERT(TIM_IVOxN_CHECK(ptr_compare->idle_state_n));
            ASSERT(TIM_IVOx_CHECK(ptr_compare->idle_state));
        
            /* clear the Output N Polarity level */
            reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH1NCCP));
            /* Set the Output N Polarity */
            reg_ccctr |= ptr_compare->output_polarity_n;
        
            /* clear the Output N State */
            reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH1NCCEN));    
            /* Set the Output N State */
            reg_ccctr |= ptr_compare->output_state_n;
        
            /* clear the Ouput Compare and Output Compare N IDLE State */
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO1));
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO1N));
        
            /* Set the Output Idle state */
            reg_ctr2 |= ptr_compare->idle_state;
            /* Set the Output N Idle state */
            reg_ctr2 |= ptr_compare->idle_state_n;
        }
      
        ptr_timer->CTR2 = reg_ctr2;  
        ptr_timer->CH12CFGR = reg_channel12;
      
        ptr_timer->CH1CCVAL = ptr_compare->timer_pulse;    
        ptr_timer->CCCTR = reg_ccctr;
    }
}

/**
  * @fn void tim_ch2oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
  * @brief Initializes the TIMx channel2 output compare(CH2OC).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_compare: pointer to a timer_compare_t structure that contains the 
  *         configuration for the specified TIM peripheral.
  * @return None
  */
void tim_ch2oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
{
    uint16_t reg_channel12 = 0;
    uint16_t reg_ccctr = 0;
    uint16_t reg_ctr2 = 0;
   
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_CHxOCMSEL_CHECK(ptr_compare->time_mode));
    ASSERT(TIM_CHx_OUTPUT_CHECK(ptr_compare->output_state));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(ptr_compare->output_polarity));  
  
    if ((ptr_compare->time_mode == TIM_CHxOCMSEL_PWM1) && ((ptr_timer->CTR1 & 0x0010) == 0x10) && (tim_pdiv_register_value_get(ptr_timer) != 0))
    {

    }
    else
    {
        /* Disable the Channel 2: clear the CH2CCEN bit */
        ptr_timer->CCCTR &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH2CCEN));
        
        reg_ccctr = ptr_timer->CCCTR;
        reg_ctr2 =  ptr_timer->CTR2;  
        reg_channel12 = ptr_timer->CH12CFGR;
            
        /* Clear the Output Compare mode and Capture/Compare selection bits */
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH12CFGR_CH2OCMSEL));
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH12CFGR_CH2FS));
        
        /* Select the Output Compare Mode */
        reg_channel12 |= (uint16_t)(ptr_compare->time_mode << 8);
      
        /* clear the Output Polarity level */
        reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH2CCP));
        /* Set the Output Compare Polarity */
        reg_ccctr |= (uint16_t)(ptr_compare->output_polarity << 4);
        
        /* Set the Output State */
        reg_ccctr |= (uint16_t)(ptr_compare->output_state << 4);
        
        if((ptr_timer == TIM1) || (ptr_timer == TIM15))
        {
            ASSERT(TIM_IVOx_CHECK(ptr_compare->idle_state));
            
            /* clear the Ouput Compare State */
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO2));
        
            /* Set the Output Idle state */
            reg_ctr2 |= (uint16_t)(ptr_compare->idle_state << 2);
        
            if (ptr_timer == TIM1)
            {    
                ASSERT(TIM_CHxNCCEN_CHECK(ptr_compare->output_state_n));
                ASSERT(TIM_CHxNCCP_POLARITY_CHECK(ptr_compare->output_polarity_n));
                ASSERT(TIM_IVOxN_CHECK(ptr_compare->idle_state_n));
          
                /* clear the Output N Polarity level */
                reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH2NCCP));
                /* Set the Output N Polarity */
                reg_ccctr |= (uint16_t)(ptr_compare->output_polarity_n << 4);
          
                /* Reset the Output N State */
                reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH2NCCEN));    
                /* Set the Output N State */
                reg_ccctr |= (uint16_t)(ptr_compare->output_state_n << 4);
          
                /* clear the Output Compare N IDLE State */
                reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO2N));
          
                /* Set the Output N Idle state */
                reg_ctr2 |= (uint16_t)(ptr_compare->idle_state_n << 2);
            }
        }
      
        ptr_timer->CTR2 = reg_ctr2;  
        ptr_timer->CH12CFGR = reg_channel12;
      
        ptr_timer->CH2CCVAL = ptr_compare->timer_pulse;  
        ptr_timer->CCCTR = reg_ccctr;
    }
}

/**
  * @fn void tim_ch3oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
  * @brief Initializes the TIMx channel3 output compare(CH3OC).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_compare: pointer to a timer_compare_t structure that contains the 
  *         configuration for the specified TIM peripheral.
  * @return None
  */
void tim_ch3oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
{
    uint16_t reg_channel12 = 0;
    uint16_t reg_ccctr = 0;
    uint16_t reg_ctr2 = 0;
   
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_CHxOCMSEL_CHECK(ptr_compare->time_mode));
    ASSERT(TIM_CHx_OUTPUT_CHECK(ptr_compare->output_state));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(ptr_compare->output_polarity));   
  
    if ((ptr_compare->time_mode == TIM_CHxOCMSEL_PWM1) && ((ptr_timer->CTR1 & 0x0010) == 0x10) && (tim_pdiv_register_value_get(ptr_timer) != 0))
    {
 
    }
    else
    {
        /* Disable the Channel 3: clear the CH3CCEN Bit */
        ptr_timer->CCCTR &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH3CCEN));
        /* Get the CCTR register value*/
        reg_ccctr = ptr_timer->CCCTR;
        /* Get the CTR2 register value*/  
        reg_ctr2 =  ptr_timer->CTR2;
        /* Get the CH34CFGR register value*/  
        reg_channel12 = ptr_timer->CH34CFGR;
        
        /* clear the Output Compare mode and Capture/Compare selection bits */
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH34CFGR_CH3OCMSEL));
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH34CFGR_CH3FS));  
        /* Select the Output Compare Mode */
        reg_channel12 |= ptr_compare->time_mode;
      
        /* Clear the Output Polarity level */
        reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH3CCP));
        /* Set the Output Compare Polarity */
        reg_ccctr |= (uint16_t)(ptr_compare->output_polarity << 8);
      
        /* Set the Output State */
        reg_ccctr |= (uint16_t)(ptr_compare->output_state << 8);
        
        if(ptr_timer == TIM1)
        {
            ASSERT(TIM_CHxNCCEN_CHECK(ptr_compare->output_state_n));
            ASSERT(TIM_CHxNCCP_POLARITY_CHECK(ptr_compare->output_polarity_n));
            ASSERT(TIM_IVOxN_CHECK(ptr_compare->idle_state_n));
            ASSERT(TIM_IVOx_CHECK(ptr_compare->idle_state));
        
            /* Clear the Output N Polarity level */
            reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH3NCCP));
            /* Set the Output N Polarity */
            reg_ccctr |= (uint16_t)(ptr_compare->output_polarity_n << 8);

            /* Clear the Output N State */
            reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH3NCCEN));    
            /* Set the Output N State */
            reg_ccctr |= (uint16_t)(ptr_compare->output_state_n << 8);

            /* Clear the Ouput Compare and Output Compare N IDLE State */
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO3));
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO3N));

            /* Set the Output Idle state */
            reg_ctr2 |= (uint16_t)(ptr_compare->idle_state << 4);
            /* Set the Output N Idle state */
            reg_ctr2 |= (uint16_t)(ptr_compare->idle_state_n << 4);
        }
      
        ptr_timer->CTR2 = reg_ctr2;  
        ptr_timer->CH34CFGR = reg_channel12;

        /* Set the Capture Compare Register value */
        ptr_timer->CH3CCVAL = ptr_compare->timer_pulse;  
        ptr_timer->CCCTR = reg_ccctr;
    }
}

/**
  * @fn void tim_ch4oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
  * @brief  Initializes the TIMx channel4 output compare(CH4OC).
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_compare: pointer to a timer_compare_t structure that contains the 
  *         configuration for the specified TIM peripheral.
  * @return None
  */
void tim_ch4oc_init(tim_reg_t* ptr_timer, timer_compare_t* ptr_compare)
{
    uint16_t reg_channel12 = 0;
    uint16_t reg_ccctr = 0;
    uint16_t reg_ctr2 = 0;
    
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_CHxOCMSEL_CHECK(ptr_compare->time_mode));
    ASSERT(TIM_CHx_OUTPUT_CHECK(ptr_compare->output_state));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(ptr_compare->output_polarity));   
  
    if ((ptr_compare->time_mode == TIM_CHxOCMSEL_PWM1) && ((ptr_timer->CTR1 & 0x0010) == 0x10) && (tim_pdiv_register_value_get(ptr_timer) != 0))
    {

    }
    else
    {
        /* Disable the Channel 4: clear the CH4CCEN Bit */
        ptr_timer->CCCTR &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH4CCEN));  
      
        reg_ccctr = ptr_timer->CCCTR;   
        reg_ctr2 =  ptr_timer->CTR2;  
        reg_channel12 = ptr_timer->CH34CFGR;
            
        /* Clear the Output Compare mode and Capture/Compare selection Bits */
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH34CFGR_CH4OCMSEL));
        reg_channel12 &= (uint16_t)(~((uint16_t)TIM_CH34CFGR_CH4FS));
      
        /* Select the Output Compare Mode */
        reg_channel12 |= (uint16_t)(ptr_compare->time_mode << 8);
      
        /* Clear the Output Polarity level */
        reg_ccctr &= (uint16_t)(~((uint16_t)TIM_CCCTR_CH4CCP));
        /* Set the Output Compare Polarity */
        reg_ccctr |= (uint16_t)(ptr_compare->output_polarity << 12);
      
        /* Set the Output State */
        reg_ccctr |= (uint16_t)(ptr_compare->output_state << 12);
        
        if(ptr_timer == TIM1)
        {
            ASSERT(TIM_IVOx_CHECK(ptr_compare->idle_state));
            /* Clear the Ouput Compare IDLE State */
            reg_ctr2 &= (uint16_t)(~((uint16_t)TIM_CTR2_IVO4));
            /* Set the Output Idle state */
            reg_ctr2 |= (uint16_t)(ptr_compare->idle_state << 6);
        }
      
        ptr_timer->CTR2 = reg_ctr2; 
        ptr_timer->CH34CFGR = reg_channel12;

        ptr_timer->CH4CCVAL = ptr_compare->timer_pulse;
        ptr_timer->CCCTR = reg_ccctr;
    }
}

/**
  * @fn tim_compare_struct_init(timer_compare_t* ptr_compare)
  * @brief  Fills each ptr_compare member with its default value.
  * @param  ptr_compare: pointer to a timer_compare_t structure being initialized.    
  * @return None
  */
void tim_compare_struct_init(timer_compare_t* ptr_compare)
{
    ptr_compare->time_mode = TIM_CHxOCMSEL_TIMING;
    ptr_compare->output_state = TIM_CHx_OUTPUT_DISABLE;
    ptr_compare->output_state_n = TIM_CHxNCCEN_DISABLE;
    ptr_compare->timer_pulse = 0;
    ptr_compare->output_polarity = TIM_CHxCCP_POLARITY_HIGH;
    ptr_compare->output_polarity_n = TIM_CHxCCP_POLARITY_HIGH;
    ptr_compare->idle_state = TIM_IVOx_RESET;
    ptr_compare->idle_state_n = TIM_IVOxN_RESET;
}   

/**
  * @fn void tim_choc_mode_select(tim_reg_t* ptr_timer, uint16_t channel_num, uint16_t value)
  * @brief  Set the TIMx CHxOC mode selection.It must be disable the selected channel
  *         before changing the Output Compare Mode.     
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  channel_num: specifies the TIM Channel,it can be one of the following values:
  *          @arg TIM_CHANNEL_NUM_1 (TIM Channel 1)
  *          @arg TIM_CHANNEL_NUM_2 (TIM Channel 2)
  *          @arg TIM_CHANNEL_NUM_3 (TIM Channel 3)
  *          @arg TIM_CHANNEL_NUM_4 (TIM Channel 4)
  * @param  mode: specifies the TIM Output Compare Mode, it can be one of the following values:
  *          @arg TIM_CHxOCMSEL_TIMING
  *          @arg TIM_CHxOCMSEL_ACTIVE
  *          @arg TIM_CHxOCMSEL_TOGGLE
  *          @arg TIM_CHxOCMSEL_PWM1
  *          @arg TIM_CHxOCMSEL_PWM2
  *          @arg TIM_CHxOCMSEL_FORCED_ACTIVE
  *          @arg TIM_CHxOCMSEL_FORCED_INACTIVE
  * @return None
  */
void tim_choc_mode_select(tim_reg_t* ptr_timer, uint16_t channel_num, uint16_t mode)
{
    uint32_t temp = 0;
    uint16_t value = 0;

    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));  
    ASSERT(TIM_ALL_CHxOCMSEL_CHECK(mode));
  
    temp = (uint32_t) ptr_timer;
    temp += TIM_CHxxCFGR_OFFSET;
    
    value =  (uint16_t)(TIM_CCCTR_CHxCCEN_SET << (uint16_t)channel_num);

    /* Disable the Channel: Reset the CH1OCMSEL Bit */
    ptr_timer->CCCTR &= (uint16_t) ~value;

    if((channel_num == TIM_CHANNEL_NUM_1) ||(channel_num == TIM_CHANNEL_NUM_3))
    {
        temp += (channel_num>>1);

        /* Reset the CH1OCMSEL bits */
        *(__IO uint32_t *) temp &= (uint32_t)~((uint32_t)TIM_CH12CFGR_CH1OCMSEL);
   
        /* Configure the CH1OCMSEL bits */
        *(__IO uint32_t *) temp |= mode;
    }
    else
    {
        temp += (uint16_t)(channel_num - (uint16_t)4)>> (uint16_t)1;

        /* Reset the CH2OCMSEL bits */
        *(__IO uint32_t *) temp &= (uint32_t)~((uint32_t)TIM_CH12CFGR_CH2OCMSEL);
    
        /* Configure the CH2OCMSEL bits*/
        *(__IO uint32_t *) temp |= (uint16_t)(mode << 8);
    }
}

/**
  * @fn void tim_ch1cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Set the TIMx CH1CC (channel 1 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies value.
  * @return None
  */
void tim_ch1cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
  
    ptr_timer->CH1CCVAL = value;
}

/**
  * @fn void tim_ch2cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Set the TIMx CH2CC(channel 2 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies value.
  * @return None
  */
void tim_ch2cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
  
    ptr_timer->CH2CCVAL = value;
}

/**
  * @fn void tim_ch3cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Set the TIMx CH3CC(channel 3 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies value.
  * @return None
  */
void tim_ch3cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
  
    ptr_timer->CH3CCVAL = value;
}

/**
  * @fn void tim_ch4cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
  * @brief  Set the TIMx CH4CC(channel 4 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be  1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies value.
  * @return None
  */
void tim_ch4cc_value_set(tim_reg_t* ptr_timer, uint32_t value)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
  
    ptr_timer->CH4CCVAL = value;
}

/**
  * @fn void tim_ch1oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Forces the TIMx channel 1 waveform to active or inactive level.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the forced Action to be set to the output waveform.
  *          It can be one of the following values:
  *           @arg TIM_CHxOCMSEL_FORCED_ACTIVE
  *           @arg TIM_CHxOCMSEL_FORCED_INACTIVE
  * @return None
  */
void tim_ch1oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOCMSEL_FORCED_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH1OCMSEL Bits */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH1OCMSEL);
    /* Configure The Forced output Mode */
    reg_value |= value;
  
    ptr_timer->CH12CFGR = reg_value;
}
 
/**
  * @fn void tim_ch2oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Forces the TIMx channel 1 waveform to active or inactive level.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the forced Action to be set to the output waveform.
  *          It can be one of the following values:
  *           @arg TIM_CHxOCMSEL_FORCED_ACTIVE
  *           @arg TIM_CHxOCMSEL_FORCED_INACTIVE
  * @return None
  */
void tim_ch2oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOCMSEL_FORCED_CHECK(value));
    
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH2OCMSEL Bits */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH2OCMSEL);
    /* Configure The Forced output Mode */
    reg_value |= (uint16_t)(value << 8);
  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch3oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Forces the TIMx channel 3 waveform to active or inactive level.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the forced Action to be set to the output waveform.
  *          It can be one of the following values:
  *           @arg TIM_CHxOCMSEL_FORCED_ACTIVE
  *           @arg TIM_CHxOCMSEL_FORCED_INACTIVE
  * @return None
  */
void tim_ch3oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOCMSEL_FORCED_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH3OCMSEL Bits */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH3OCMSEL);
    /* Configure The Forced output Mode */
    reg_value |= value;
  
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch4oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Forces the TIMx channel 4 waveform to active or inactive level.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the forced Action to be set to the output waveform.
  *          It can be one of the following values:
  *           @arg TIM_CHxOCMSEL_FORCED_ACTIVE
  *           @arg TIM_CHxOCMSEL_FORCED_INACTIVE
  * @return None
  */
void tim_ch4oc_mode_force_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOCMSEL_FORCED_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH4OCMSEL Bits */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH4OCMSEL);
    /* Configure The Forced output Mode */
    reg_value |= (uint16_t)(value << 8);
  
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch_preload_shadow_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief  config CHPSEN (Preload shadow enable of channel ) bit, to enable or disable
  *         preload shadow value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 ,3 or 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE Preload shadow.
  * @return None
  */
void tim_ch_preload_shadow_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{ 
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the CHPSEN Bit */
        ptr_timer->CTR2 |= TIM_CTR2_CHPSEN;
    }
    else
    {
        /* Reset the CHPSEN Bit */
        ptr_timer->CTR2 &= (uint16_t)~((uint16_t)TIM_CTR2_CHPSEN);
    }
}


/**
  * @fn void tim_ch1oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Enables or disables the TIMx CH1OC(channel 1 output compare) Preload value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 and 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the TIMx peripheral Preload register
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_PRELOAD_ENABLE
  *           @arg TIM_CHxOC_PRELOAD_DISABLE
  * @return None
  */
void tim_ch1oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_PRELOAD_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Reset the CH1OCVPEN Bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH1OCVPEN);
    /* Enable or disable channel 1 output compare preload value */
    reg_value |= value;  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch2oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Enables or disables the TIMx CH2OC(channel 2 output compare) Preload value.
  * @param  ptr_timer: pointer to TIMx where x can be1, 2, 3 and 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the TIMx peripheral Preload register
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_PRELOAD_ENABLE
  *           @arg TIM_CHxOC_PRELOAD_DISABLE
  * @return None
  */
void tim_ch2oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_PRELOAD_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* clear the CH2OCVPEN Bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH2OCVPEN);
    /* Enable or disable channel 2 output compare preload value */
    reg_value |= (uint16_t)(value << 8);
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch3oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Enables or disables the TIMx CH3OC(channel 3 output compare) Preload value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3 .
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the TIMx peripheral Preload register
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_PRELOAD_ENABLE
  *           @arg TIM_CHxOC_PRELOAD_DISABLE
  * @return None
  */
void tim_ch3oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_PRELOAD_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH3OCVPEN Bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH3OCVPEN);
    /* Enable or disable channel 3 output compare preload value */
    reg_value |= value;
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch4oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Enables or disables the TIMx CH4OC(channel 4 output compare) Preload value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3 .
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the TIMx peripheral Preload register
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_PRELOAD_ENABLE
  *           @arg TIM_CHxOC_PRELOAD_DISABLE
  * @return None
  */
void tim_ch4oc_preload_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_PRELOAD_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH4OCVPEN Bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH4OCVPEN);
    /* Enable or disable channel 4 output compare preload value */
    reg_value |= (uint16_t)(value << 8);
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch1oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Set the TIMx CH1OC(channel 1 Output Compare) Fast feature.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Fast Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_FAST_ENABLE (output compare fast enable)
  *           @arg TIM_CHxOC_FAST_DISABLE (output compare fast disable)
  * @return None
  */
void tim_ch1oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_FAST_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH1OCFEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH1OCFEN);
    /* Enable or disable channel 1 output compare fast feature */
    /* Config the CH1OCFEN bit */
    reg_value |= value;  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch2oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Set the TIMx CH2OC(channel 2 Output Compare) Fast feature.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Fast Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_FAST_ENABLE (output compare fast enable)
  *           @arg TIM_CHxOC_FAST_DISABLE (output compare fast disable)
  * @return None
  */
void tim_ch2oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_FAST_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH2OCFEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH2OCFEN);
    /* Enable or disable channel 2 output compare fast feature */
    /* Config the CH2OCFEN bit */
    reg_value |= (uint16_t)(value << 8);  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch3oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Set the TIMx CH3OC(channel 3 Output Compare) Fast feature.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Fast Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_FAST_ENABLE (output compare fast enable)
  *           @arg TIM_CHxOC_FAST_DISABLE (output compare fast disable)
  * @return None
  */
void tim_ch3oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_FAST_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH3OCFEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH3OCFEN);
    /* Enable or disable channel 3 output compare fast feature */
    /* config the CH3OCFEN bit */
    reg_value |= value;  
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch4oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Set the TIMx CH4OC(channel 4 Output Compare) Fast feature.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Fast Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_FAST_ENABLE (output compare fast enable)
  *           @arg TIM_CHxOC_FAST_DISABLE (output compare fast disable)
  * @return None
  */
void tim_ch4oc_fast_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
    
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_FAST_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* clear the CH4OCFEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH4OCFEN);
    /* Enable or disable channel 4 output compare fast feature */
    /* config the CH4OCFEN bit */
    reg_value |= (uint16_t)(value << 8);  
    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch1oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Clears or safeguards the TIMx CH1OC(channel 1 Output Compare) signal on an external event.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Clear Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_CLEAR_ENABLE (output compare clear enable)
  *           @arg TIM_CHxOC_CLEAR_DISABLE (output compare clear disable)
  * @return None
  */
void tim_ch1oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_CLEAR_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH1OCCEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH1OCCEN);
    /* Config CH1OCCEN bit */
    reg_value |= value;
  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch2oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Clears or safeguards the TIMx CH2OC(channel 2 Output Compare) signal on an external event.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15 .
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Clear Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_CLEAR_ENABLE (output compare clear enable)
  *           @arg TIM_CHxOC_CLEAR_DISABLE (output compare clear disable)
  * @return None
  */
void tim_ch2oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_CLEAR_CHECK(value));
  
    reg_value = ptr_timer->CH12CFGR;
    /* Clear the CH2OCCEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH2OCCEN);
    /* Config CH2OCCEN bit */
    reg_value |= (uint16_t)(value << 8);
  
    ptr_timer->CH12CFGR = reg_value;
}

/**
  * @fn void tim_ch3oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Clears or safeguards the TIMx CH3OC(channel 3 Output Compare) signal on an external event.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Clear Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_CLEAR_ENABLE (output compare clear enable)
  *           @arg TIM_CHxOC_CLEAR_DISABLE (output compare clear disable)
  * @return None
  */
void tim_ch3oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_CLEAR_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* Clear the CH3OCCEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH3OCCEN);
    /* Config CH3OCCEN bit */
    reg_value |= value; 

    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch4oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Clears or safeguards the TIMx CH4OC(channel 4 Output Compare) signal on an external event.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: new state of the Output Compare Clear Enable Bit.
  *          It can be one of the following values:
  *           @arg TIM_CHxOC_CLEAR_ENABLE (output compare clear enable)
  *           @arg TIM_CHxOC_CLEAR_DISABLE (output compare clear disable)
  * @return None
  */
void tim_ch4oc_clear_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxOC_CLEAR_CHECK(value));
  
    reg_value = ptr_timer->CH34CFGR;
    /* Clear the CH4OCCEN bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH4OCCEN);  
    /* Config CH4OCCEN bit */
    reg_value |= (uint16_t)(value << 8);  

    ptr_timer->CH34CFGR = reg_value;
}

/**
  * @fn void tim_ch1cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH1CC (channel 1 capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the CH1CC Polarity, It can be one of the following values:
  *          @arg TIM_CHxCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch1cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(value));
  
    reg_value = ptr_timer->CCCTR;
    /* Clear CH1CCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH1CCP);
  
    /* Config CH1CCP bit */
    reg_value |= value;
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch1ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH1NCC (channel 1 complementary capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 15, 16 or 17.
  * @param  value: specifies the Polarity, It can be one of the following values:
  *          @arg TIM_CHxNCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxNCCP_POLARITY_LOW (Compare active low)   
  * @return None
  */
void tim_ch1ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST2_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxNCCP_POLARITY_CHECK(value));
   
    reg_value = ptr_timer->CCCTR;  
    /*Clear CH1NCCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH1NCCP);  
  
    /*Config CH1NCCP bit */
    reg_value |= value;  
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch2cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH2CC (channel 2 capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies Polarity, It can be one of the following values:
  *          @arg TIM_CHxCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch2cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(value));
  
    reg_value = ptr_timer->CCCTR;
    /* Clear CH2CCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH2CCP);
  
    /* Config CH2CCP bit */
    reg_value |= (uint16_t)(value << 4);
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch2ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH2NCC (channel 2 complementary capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1.
  * @param  value: specifies the Polarity, It can be one of the following values:
  *          @arg TIM_CHxNCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxNCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch2ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST1_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxNCCP_POLARITY_CHECK(value));
  
    reg_value = ptr_timer->CCCTR;
    /* Clear CH2NCCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH2NCCP);
  
    /*Config CH2NCCP bit */
    reg_value |= (uint16_t)(value << 4);
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch3cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH3CC (channel 3 capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies Polarity, It can be one of the following values:
  *          @arg TIM_CHxCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch3cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
    
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(value));
  
    reg_value = ptr_timer->CCCTR;
    /* clear CH3CCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH3CCP);
  
    /* Config CH3CCP bit */
    reg_value |= (uint16_t)(value << 8);  
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch3ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH3NCC (channel 3 complementary capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1.
  * @param  value: specifies the Polarity, It can be one of the following values:
  *          @arg TIM_CHxNCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxNCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch3ncc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
 
    ASSERT(TIM_LIST1_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxNCCP_POLARITY_CHECK(value));
    
    reg_value = ptr_timer->CCCTR;
    /* Clear CH3NCCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH3NCCP);

    /* Config CH3NCCP bBit */
    reg_value |= (uint16_t)(value << 8);  
    ptr_timer->CCCTR = reg_value;
}

/**
  * @fn void tim_ch4cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx CH4CC (channel 4 capture compare) polarity.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies Polarity, It can be one of the following values:
  *          @arg TIM_CHxCCP_POLARITY_HIGH (Compare active high)
  *          @arg TIM_CHxCCP_POLARITY_LOW (Compare active low)    
  * @return None
  */
void tim_ch4cc_polarity_set(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxCCP_POLARITY_CHECK(value));
  
    reg_value = ptr_timer->CCCTR;
    /* Clear CH4CCP bit */
    reg_value &= (uint16_t)~((uint16_t)TIM_CCCTR_CH4CCP);
    
    /* Config CH4CCP bit */
    reg_value |= (uint16_t)(value << 12);  
    ptr_timer->CCCTR = reg_value;
}   

/**
  * @fn void tim_ocref_clear_select(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Selects the OCREF Clear source.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Clear source,it can be one of the following values:
  *          @arg TIM_OCREF_CLEAR_ETRF (The internal OCREF clear input is connected to ETRF)
  *          @arg TIM_OCREF_CLEAR_CLR (The internal OCREF clear input is connected to OCREF_CLR input)  
  * @return None
  */
void tim_ocref_clear_select(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_OCREF_CLEAR_CHECK(value));

    /* Set the OCREF source */
    ptr_timer->SMCFG &=  (uint16_t)~((uint16_t)TIM_SMCFG_OCREFCS);
    ptr_timer->SMCFG |=  value;
}

/**
  * @fn void tim_chcc_enable_ctrl(tim_reg_t* ptr_timer, uint16_t channel, uint16_t value)
  * @brief  Enables or disables the TIM channelx Capture Compare .
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  channel: specifies the TIM Channel,it can be one of the following values:
  *          @arg TIM_CHANNEL_NUM_1 (Channel 1)
  *          @arg IIM_CHANNEL_NUM_2 (Channel 2)
  *          @arg TIM_CHANNEL_NUM_3 (Channel 3)
  *          @arg TIM_CHANNEL_NUM_4 (Channel 4)
  * @param  value: Enable or disable specifies the TIM Channel.
  *         It can be: TIM_CHxCC_ENABLE or TIM_CHxCC_DISABLE. 
  * @return None
  */
void tim_chcc_enable_ctrl(tim_reg_t* ptr_timer, uint16_t channel, uint16_t value)
{
    uint16_t temp = 0;

    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_CHxCC_STATE_CHECK(value));

    temp =  (uint16_t)(TIM_CCCTR_CHxCCEN_SET << channel);

    /* clear the CHxCCEN bit */
    ptr_timer->CCCTR &= (uint16_t)~ temp;

    /* config CHxCCEN bit */ 
    ptr_timer->CCCTR |=  (uint16_t)(value << channel);
}

/**
  * @fn void tim_chncc_enable_ctrl(tim_reg_t* ptr_timer, uint16_t channel, uint16_t value)
  * @brief  Enables or disables the TIM channel complementary Capture Compare .
  * @param  ptr_timer: pointer to TIMx where x can be 1, 15, 16 or 17.  
  * @param  channel: specifies the TIM Channel,it can be one of the following values:
  *          @arg TIM_CHANNEL_NUM_1 (Channel 1)
  *          @arg TIM_CHANNEL_NUM_2 (Channel 2)
  *          @arg TIM_CHANNEL_NUM_3 (Channel 3)
  * @param  value: Enable or disable specifies the TIM Channel.
  *         It can be: TIM_CHxNCC_ENABLE or TIM_CHxNCC_DISABLE. 
  * @return None
  */
void tim_chncc_enable_ctrl(tim_reg_t* ptr_timer, uint16_t channel, uint16_t value)
{
    uint16_t temp = 0;

    ASSERT(TIM_LIST2_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_COMPLEMENTARY_CHANNEL_CHECK(channel));
    ASSERT(TIM_CHxNCC_STATE_CHECK(value));

    temp = (uint16_t)(TIM_CCCTR_CHxNCCEN_SET << channel);

    /* clear the CHxNCCEN bit */
    ptr_timer->CCCTR &= (uint16_t) ~temp;

    /* Set or reset the CHxNCCEN bit */ 
    ptr_timer->CCCTR |=  (uint16_t)(value << channel);
}

/**
  * @fn void tim_ch_preload_update_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief  Enable or disable CHPUS (channel Preload update source).
  * @param  ptr_timer: pointer to TIMx where x can be  1, 15, 16 or 17.
  * @param  enable_flag: It can be: ENABLE or DISABLE
  * @return None
  */
void tim_ch_preload_update_enable_ctrl(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_LIST2_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the CHPUS bit */
        ptr_timer->CTR2 |= TIM_CTR2_CHPUS;
    }
    else
    {
        /* Reset the CHPUS bit */
        ptr_timer->CTR2 &= (uint16_t)~((uint16_t)TIM_CTR2_CHPUS);
    }
}

/**
  * @fn void tim_chic_init(tim_reg_t* ptr_timer, timer_capture_t* ptr_capture)
  * @brief  Initializes TIM CHxIC(channel input capture) according to the specified
  *         parameters in the ptr_capture.
  * @param  ptr_timer: pointer to TIMx where x can be  1, 2, 3, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_capture: pointer to a timer_capture_t structure that contains 
  *         the configuration information for the specified TIM peripheral.
  * @return None
  */
void tim_chic_init(tim_reg_t* ptr_timer, timer_capture_t* ptr_capture)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_ALL_CHANNE_CHECK(ptr_capture->time_channel));  
    ASSERT(TIM_CHIC_SEL_CHECK(ptr_capture->input_select));
    ASSERT(TIM_CHIC_PREDIVIDE_CHECK(ptr_capture->input_predivide));
    ASSERT(TIM_CHxIC_FILTER_CHECK(ptr_capture->input_filter));
    ASSERT(TIM_CHxIC_POLARITY_CHECK(ptr_capture->input_polarity));
    
    if (ptr_capture->time_channel == TIM_CHANNEL_NUM_1)
    {
        ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
        /* TI1 Configuration */
        tim_ch1ic_config(ptr_timer, ptr_capture->input_polarity,
               ptr_capture->input_select,ptr_capture->input_filter);
               
        /* Set the Input Capture Prescaler value */
        tim_ch1ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
    else if (ptr_capture->time_channel == TIM_CHANNEL_NUM_2)
    {
        ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
        /* TI2 Configuration */
        tim_ch2ic_config(ptr_timer, ptr_capture->input_polarity,
               ptr_capture->input_select,ptr_capture->input_filter);
               
        /* Set the Input Capture Prescaler value */
        tim_ch2ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
    else if (ptr_capture->time_channel == TIM_CHANNEL_NUM_3)
    {
        ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
        /* TI3 Configuration */
        tim_ch3ic_config(ptr_timer,  ptr_capture->input_polarity,
               ptr_capture->input_select,ptr_capture->input_filter);
               
        /* Set the Input Capture Prescaler value */
        tim_ch3ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
    else
    {
        ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
        /* TI4 Configuration */
        tim_ch4ic_config(ptr_timer, ptr_capture->input_polarity,
               ptr_capture->input_select,ptr_capture->input_filter);
               
        /* Set the Input Capture Prescaler value */
        tim_ch4ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
}

/**
  * @fn void tim_capture_struct_init(timer_capture_t* ptr_capture)
  * @brief  Fills each ptr_capture member with its default value.
  * @param  ptr_capture: pointer to a timer_capture_t structure which will be initialized.        
  * @return None
  */
void tim_capture_struct_init(timer_capture_t* ptr_capture)
{
    ptr_capture->time_channel = TIM_CHANNEL_NUM_1;
    ptr_capture->input_polarity = TIM_CHxIC_POLARITY_RISING;
    ptr_capture->input_select = TIM_CHIC_SEL_DIRECT_INTR;
    ptr_capture->input_predivide = TIM_CHIC_PREDIVIDE_DIV1;
    ptr_capture->input_filter = 0x00;
}

/**
  * @fn void tim_pwm_config(tim_reg_t* ptr_timer, timer_capture_t* ptr_capture)
  * @brief  Configures the TIM peripheral according to the specified
  *         parameters in the ptr_capture to measure an external PWM signal.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  ptr_capture: pointer to a timer_capture_t structurethat contains 
  *         the configuration for the specified TIM peripheral.
  * @return None
  */
void tim_pwm_config(tim_reg_t* ptr_timer, timer_capture_t* ptr_capture)
{
    uint16_t polarity = TIM_CHxIC_POLARITY_RISING;
    uint16_t select = TIM_CHIC_SEL_DIRECT_INTR;
  
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
  
    /* Select the Opposite Input Polarity */
    if (ptr_capture->input_polarity == TIM_CHxIC_POLARITY_RISING)
    {
        polarity = TIM_CHxIC_POLARITY_FALLING;
    }
    else
    {
        polarity = TIM_CHxIC_POLARITY_RISING;
    }
    /* Select the Opposite Input */
    if (ptr_capture->input_select == TIM_CHIC_SEL_DIRECT_INTR)
    {
        select = TIM_CHIC_SEL_INDIRECT_INTR;
    }
    else
    {
        select = TIM_CHIC_SEL_DIRECT_INTR;
    }
    if (ptr_capture->time_channel == TIM_CHANNEL_NUM_1)
    {
        /* TI1 Configuration */
        tim_ch1ic_config(ptr_timer, ptr_capture->input_polarity, ptr_capture->input_select,
                ptr_capture->input_filter);
    
        /* Set the Input Capture Prescaler value */
        tim_ch1ic_predivide_set(ptr_timer, ptr_capture->input_predivide);

        /* TI2 Configuration */
        tim_ch2ic_config(ptr_timer, polarity, select, ptr_capture->input_filter);
        /* Set the Input Capture Prescaler value */
        tim_ch2ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
    else
    { 
        /* TI2 Configuration */
        tim_ch2ic_config(ptr_timer, ptr_capture->input_polarity, ptr_capture->input_select,
               ptr_capture->input_filter);

        /* Set the Input Capture Prescaler value */
        tim_ch2ic_predivide_set(ptr_timer, ptr_capture->input_predivide);

        /* TI1 Configuration */
        tim_ch1ic_config(ptr_timer, polarity, select, ptr_capture->input_filter);
        /* Set the Input Capture Prescaler value */
        tim_ch1ic_predivide_set(ptr_timer, ptr_capture->input_predivide);
    }
}

/**
  * @fn uint32_t tim_ch1cc_value_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx channel 1 capture compare value.
  * @param ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Capture Compare value.
  */
uint32_t tim_ch1cc_value_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
  
    return ptr_timer->CH1CCVAL;
}

/**
  * @fn uint32_t tim_ch2cc_value_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx CH2CC(channel 2 capture compare) value.
  * @param ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Capture Compare value.
  */
uint32_t tim_ch2cc_value_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
  
    return ptr_timer->CH2CCVAL;
}

/**
  * @fn uint32_t tim_ch3cc_value_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx CH3CC(channel 3 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Capture Compare value.
  */
uint32_t tim_ch3cc_value_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer)); 
  
    return ptr_timer->CH3CCVAL;
}

/**
  * @fn uint32_t tim_ch4cc_value_get(tim_reg_t* ptr_timer)
  * @brief  Gets the TIMx CH4CC(channel 4 capture compare) value.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return Capture Compare value.
  */
uint32_t tim_ch4cc_value_get(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
  
    return ptr_timer->CH4CCVAL;
}

/**
  * @fn void tim_ch1ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets the TIMx  CH1IC (channel 1 Input Capture) pre-divide.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.  
  * @param  value: specifies pre-divide value,it can be one of the following values:
  *          @arg TIM_CHIC_PREDIVIDE_DIV1 (no prescaler)
  *          @arg TIM_CHIC_PREDIVIDE_DIV2 (capture is done once every 2 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV4 (capture is done once every 4 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV8 (capture is done once every 8 events)
  * @return None
  */
void tim_ch1ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHIC_PREDIVIDE_CHECK(value));
    
    /* Clear the CH1ICPDIV bits */
    ptr_timer->CH12CFGR &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH1ICPDIV);
    /* Set the CH1ICPDIV bits */
    ptr_timer->CH12CFGR |= value;
}

/**
  * @fn void tim_ch2ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets the TIMx CH2IC (channel 2 Input Capture) pre-divide.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.  
  * @param  value: specifies pre-divide value,it can be one of the following values:
  *          @arg TIM_CHIC_PREDIVIDE_DIV1 (no prescaler)
  *          @arg TIM_CHIC_PREDIVIDE_DIV2(capture is done once every 2 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV4(capture is done once every 4 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV8(capture is done once every 8 events)
  * @return None
  */
void tim_ch2ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHIC_PREDIVIDE_CHECK(value));
  
    /* Clear the CH2ICPDIV  bits */
    ptr_timer->CH12CFGR &= (uint16_t)~((uint16_t)TIM_CH12CFGR_CH2ICPDIV);
    /* Set the CH2ICPDIV  bits */
    ptr_timer->CH12CFGR |= (uint16_t)(value << 8);
}

/**
  * @fn void tim_ch3ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets the TIMx CH3IC (channel 3 Input Capture) pre-divide.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies pre-divide value,it can be one of the following values:
  *          @arg TIM_CHIC_PREDIVIDE_DIV1 (no prescaler)
  *          @arg TIM_CHIC_PREDIVIDE_DIV2 (capture is done once every 2 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV4 (capture is done once every 4 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV8 (capture is done once every 8 events)
  * @return None
  */
void tim_ch3ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHIC_PREDIVIDE_CHECK(value));
  
    /* Clear the CH3ICPDIV  bits */
    ptr_timer->CH34CFGR &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH3ICPDIV);
    /* Set the CH3ICPDIV  bits */
    ptr_timer->CH34CFGR |= value;
}

/**
  * @fn void tim_ch4ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets the TIMx CH4IC (channel 4 Input Capture) pre-divide.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3 .  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies pre-divide value,it can be one of the following values:
  *          @arg TIM_CHIC_PREDIVIDE_DIV1 (no prescaler)
  *          @arg TIM_CHIC_PREDIVIDE_DIV2 (capture is done once every 2 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV4 (capture is done once every 4 events)
  *          @arg TIM_CHIC_PREDIVIDE_DIV8 (capture is done once every 8 events)
  * @return None
  */
void tim_ch4ic_predivide_set(tim_reg_t* ptr_timer, uint16_t value)
{  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHIC_PREDIVIDE_CHECK(value));
  
    /* Clear the CH4ICPDIV  bits */
    ptr_timer->CH34CFGR &= (uint16_t)~((uint16_t)TIM_CH34CFGR_CH4ICPDIV);
    /* Set the CH4ICPDIV  bits */
    ptr_timer->CH34CFGR |= (uint16_t)(value << 8);
}

/**
  * @fn void tim_interrupt_config(tim_reg_t* ptr_timer, uint16_t value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified TIM interrupts.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17. 
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: enabled or disabled specifies the TIM interrupts sources.
  *          It can be any combination of the following values:
  *           @arg TIM_INTR_UPDATE(TIM update Interrupt source);
  *           @arg TIM_INTR_CH1 (TIMx channel 1 Capture Compare Interrupt source)
  *           @arg TIM_INTR_CH2 (TIMx channel 2 Capture Compare Interrupt source)
  *           @arg TIM_INTR_CH3 (TIMx channel 3 Capture Compare Interrupt source)
  *           @arg TIM_INTR_CH4 (TIMx channel 4 Capture Compare Interrupt source)
  *           @arg TIM_INTR_COM (TIMx Commutation Interrupt source)
  *           @arg TIM_INTR_TRIGGER (TIM Trigger Interrupt source)
  *           @arg TIM_INTR_BREAK (TIM Break Interrupt source)
  * @note   TIM6 and TIM7 can only generate an update interrupt;
  * @note   TIM15 can have only TIM_INTR_UPDATE, TIM_INTR_CH1, TIM_INTR_CH2 or TIM_INTR_TRIGGER. 
  * @note   TIM14, TIM16 and TIM17 can have TIM_INTR_UPDATE or TIM_INTR_CH1.   
  * @note   TIM_INTR_BREAK is used only with TIM1 and TIM15. 
  * @note   TIM_INTR_COM is used only with TIM1, TIM15, TIM16 and TIM17.   
  * @param  enable_flag: ENABLE or DISABLE the TIM interrupts.
  * @return None
  */
void tim_interrupt_config(tim_reg_t* ptr_timer, uint16_t value, enable_state_t enable_flag)
{  
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_INTR_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the Interrupt sources */
        ptr_timer->DIEN |= value;
    }
    else
    {
        /* Disable the Interrupt sources */
        ptr_timer->DIEN &= (uint16_t)~value;
    }
}

/**
  * @fn void tim_event_source_set(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIMx event to be generate by software.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the event source, it can be one or more of the following values:  
  *          @arg TIM_EVENT_SOURCE_UPDATE(TIM update Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_CH1 (TIMx channel 1 Capture Compare Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_CH2 (TIMx channel 2 Capture Compare Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_CH3 (TIMx channel 3 Capture Compare Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_CH4 (TIMx channel 4 Capture Compare Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_COM (TIMx Commutation Interrupt Event source)
  *          @arg TIM_EVENT_SOURCE_TRIGGER (TIMx Trigger Interrupt Event source) 
  *          @arg TIM_EVENT_SOURCE_BREAK (TIMx Break Interrupt Event source)
  * @note   TIM6 and TIM7 can only generate an update event.  
  * @note   TIM_EVENT_SOURCE_COM and TIM_EVENT_SOURCE_BREAK are used only with TIM1.          
  * @return None
  */
void tim_event_source_set(tim_reg_t* ptr_timer, uint16_t value)
{ 
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_EVENT_SOURCE_CHECK(value)); 
  
    /* Set the event sources */
    ptr_timer->SWEGR = value;
}

/**
  * @fn bit_status_t tim_flag_status_get(tim_reg_t* ptr_timer, uint16_t flag)
  * @brief  Checks whether the specified TIM flag is set or not.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17. 
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices. 
  * @param  flag: specifies the flag to check, it can be one of the following values:
  *          @arg TIM_FLAG_UPDATE (TIMx update Flag)
  *          @arg TIM_FLAG_CH1CC (TIMx chnnel 1Capture Compare Flag)
  *          @arg TIM_FLAG_CH2CC (TIMx chnnel 2Capture Compare Flag)
  *          @arg TIM_FLAG_CH3CC (TIMx chnnel 3Capture Compare Flag)
  *          @arg TIM_FLAG_CH4CC (TIMx chnnel 4Capture Compare Flag)
  *          @arg TIM_FLAG_CHCOM (TIMx Commutation Flag)
  *          @arg TIM_FLAG_TRIGGER (TIMx Trigger Flag)
  *          @arg TIM_FLAG_BREAK (TIMx Break Flag)
  *          @arg TIM_FLAG_CH1ICOF (TIMx chnnel 1 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH2ICOF (TIMx chnnel 2 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH3ICOF (TIMx chnnel 3 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH4ICOF (TIMx chnnel 4 Capture Compare overflow Flag)
  * @note   TIM6 and TIM7 can have only one update flag. 
  * @note   TIM15 can have only TIM_FLAG_UPDATE, TIM_FLAG_CH1CC, TIM_FLAG_CH2CC or TIM_FLAG_TRIGGER.
  * @note   TIM14, TIM16 and TIM17 can have TIM_FLAG_UPDATE or TIM_FLAG_CH1CC.   
  * @note   TIM_FLAG_BREAK is used only with TIM1 and TIM15. 
  * @note   TIM_FLAG_CHCOM is used only with TIM1 TIM15, TIM16 and TIM17.
  * @return SET or RESET.
  */
bit_status_t tim_flag_status_get(tim_reg_t* ptr_timer, uint16_t flag)
{ 
    bit_status_t status = RESET; 
   
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_FLAG_CHECK(flag));
  
    if ((ptr_timer->STS & flag) != (uint16_t)RESET)
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
  * @fn void tim_flag_status_clear(tim_reg_t* ptr_timer, uint16_t flag)
  * @brief  Clears the TIMx's flags.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17.
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices. 
  * @param  flag: specifies the flag to check,it can be one of the following values:
  *          @arg TIM_FLAG_UPDATE (TIMx update Flag)
  *          @arg TIM_FLAG_CH1CC (TIMx chnnel 1Capture Compare Flag)
  *          @arg TIM_FLAG_CH2CC (TIMx chnnel 2Capture Compare Flag)
  *          @arg TIM_FLAG_CH3CC (TIMx chnnel 3Capture Compare Flag)
  *          @arg TIM_FLAG_CH4CC (TIMx chnnel 4Capture Compare Flag)
  *          @arg TIM_FLAG_CHCOM (TIMx Commutation Flag)
  *          @arg TIM_FLAG_TRIGGER (TIMx Trigger Flag)
  *          @arg TIM_FLAG_BREAK (TIM Break Flag)
  *          @arg TIM_FLAG_CH1ICOF (TIMx chnnel 1 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH2ICOF (TIMx chnnel 2 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH3ICOF (TIMx chnnel 3 Capture Compare overflow Flag)
  *          @arg TIM_FLAG_CH4ICOF (TIMx chnnel 4 Capture Compare overflow Flag)
  * @note   TIM6 and TIM7 can have only one update flag. 
  * @note   TIM15 can have only TIM_FLAG_UPDATE, TIM_FLAG_CH1CC, TIM_FLAG_CH2CC or TIM_FLAG_TRIGGER.
  * @note   TIM14, TIM16 and TIM17 can have TIM_FLAG_UPDATE or TIM_FLAG_CH1CC.   
  * @note   TIM_FLAG_BREAK is used only with TIM1 and TIM15. 
  * @note   TIM_FLAG_CHCOM is used only with TIM1 TIM15, TIM16 and TIM17.
  * @return None
  */
void tim_flag_status_clear(tim_reg_t* ptr_timer, uint16_t flag)
{  
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CLEAR_FLAG_CHECK(flag));
   
    /* Clear the flags */
    ptr_timer->STS = (uint16_t)~flag;
}

/**
  * @fn bit_status_t tim_interrupt_status_get(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Checks whether the TIM interrupt has occurred or not.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.  
  * @param  value: specifies the TIM interrupt source ,it can be one of the following values:
  *          @arg TIM_INTR_UPDATE (TIMx update Interrupt source)
  *          @arg TIM_INTR_CH1 (TIMx channel 1 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH2 (TIMx channel 2 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH3 (TIMx channel 3 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH4 (TIMx channel 4 Capture Compare Interrupt source)
  *          @arg TIM_INTR_COM (TIMx Commutation Interrupt source)
  *          @arg TIM_INTR_TRIGGER (TIMx Trigger Interrupt source) 
  *          @arg TIM_INTR_BREAK (TIMx Break Interrupt source)
  * @note   TIM6 and TIM7 can only generate an update interrupt;
  * @note   TIM15 can have only TIM_INTR_UPDATE, TIM_INTR_CH1,TIM_INTR_CH2 or TIM_INTR_TRIGGER. 
  * @note   TIM14, TIM16 and TIM17 can have TIM_INTR_UPDATE or TIM_INTR_CH1.   
  * @note   TIM_INTR_BREAK is used only with TIM1 and TIM15. 
  * @note   TIM_INTR_COM is used only with TIM1, TIM15, TIM16 and TIM17.  
  * @return SET or RESET.
  */
bit_status_t tim_interrupt_status_get(tim_reg_t* ptr_timer, uint16_t value)
{   
    bit_status_t bit_status = RESET;  
    uint16_t status = 0x0;
    uint16_t temp = 0x0;
  
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_GET_INTR_CHECK(value));
   
    status = ptr_timer->STS & value;  
    temp = ptr_timer->DIEN & value;
  
    if ((status != (uint16_t)RESET) && (temp != (uint16_t)RESET))
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
  * @fn bit_status_t tim_interrupt_status_clear(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Clears the TIMx's interrupt  bits.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, 14, 15, 16 or 17.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the TIM interrupt source, it can be one of the following values:
  *          @arg TIM_INTR_UPDATE (TIMx update Interrupt source)
  *          @arg TIM_INTR_CH1 (TIMx channel 1 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH2 (TIMx channel 2 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH3 (TIMx channel 3 Capture Compare Interrupt source)
  *          @arg TIM_INTR_CH4 (TIMx channel 4 Capture Compare Interrupt source)
  *          @arg TIM_INTR_COM (TIMx Commutation Interrupt source)
  *          @arg TIM_INTR_TRIGGER (TIMx Trigger Interrupt source) 
  *          @arg TIM_INTR_BREAK (TIMx Break Interrupt source)
  * @note   TIM6 and TIM7 can only generate an update interrupt;
  * @note   TIM15 can have only TIM_INTR_UPDATE, TIM_INTR_CH1,TIM_INTR_CH2 or TIM_INTR_TRIGGER. 
  * @note   TIM14, TIM16 and TIM17 can have TIM_INTR_UPDATE or TIM_INTR_CH1.   
  * @note   TIM_INTR_BREAK is used only with TIM1 and TIM15. 
  * @note   TIM_INTR_COM is used only with TIM1, TIM15, TIM16 and TIM17.  
  * @return None.
  */
void tim_interrupt_status_clear(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_ALL_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_INTR_CHECK(value));
   
    /* Clear the INTR bit */
    ptr_timer->STS = (uint16_t)~value;
}

/**
  * @fn void tim_dma_access_config(tim_reg_t* ptr_timer, uint16_t addr, uint16_t len)
  * @brief  Configures the TIMx's DMA interface.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 15, 16 or 17, 
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  addr: DMA Base address, it can be one of the following values:         
  *          @arg TIM_DMA_BASE_CTR1
  *          @arg TIM_DMA_BASE_CTR2
  *          @arg TIM_DMA_BASE_SMCFG
  *          @arg TIM_DMA_BASE_DIEN
  *          @arg TIM_DMA_BASE_STS
  *          @arg TIM_DMA_BASE_SWEGR 
  *          @arg TIM_DMA_BASE_CH12CFGR
  *          @arg TIM_DMA_BASE_CH34CFGR
  *          @arg TIM_DMA_BASE_CCCTR
  *          @arg TIM_DMA_BASE_CNT
  *          @arg TIM_DMA_BASE_PDIV
  *          @arg TIM_DMA_BASE_UVAL
  *          @arg TIM_DMA_BASE_CH1CCVAL
  *          @arg TIM_DMA_BASE_CH2CCVAL
  *          @arg TIM_DMA_BASE_CH3CCVAL
  *          @arg TIM_DMA_BASE_CH4CCVAL
  *          @arg TIM_DMA_BASE_DMAACR
  *          @arg TIM_DMA_BASE_REMAP
  * @param  len: DMA Burst length. This parameter can be one value
  *         between: TIM_DMA_TRANSFER_LEN_1BYTE and TIM_DMA_TRANSFER_LEN_18BYTES.
  * @return None
  */
void tim_dma_access_config(tim_reg_t* ptr_timer, uint16_t addr, uint16_t len)
{
    ASSERT(TIM_LIST4_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_DMA_BASE_CHECK(addr)); 
    ASSERT(TIM_DMA_TRANSFER_LEN_CHECK(len));
    
    /* Set the DMA Base and the DMA Burst Length */
    ptr_timer->DMAACR = addr | len;
}

/**
  * @fn void tim_dma_enable_ctrl(tim_reg_t* ptr_timer, uint16_t value, enable_state_t enable_flag)
  * @brief Enables or disables the TIMx's DMA Requests.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 15, 16 or 17. 
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the DMA Request sources, it can be any combination of the following values:
  *          @arg TIM_DMA_UPDATE (TIMx update Interrupt source)
  *          @arg TIM_DMA_CH1 (TIMx channel 1 Capture Compare DMA source)
  *          @arg TIM_DMA_CH2 (TIMx channel 2 Capture Compare DMA source)
  *          @arg TIM_DMA_CH3 (TIMx channel 3 Capture Compare DMA source)
  *          @arg TIM_DMA_CH4 (TIMx channel 4 Capture Compare DMA source)
  *          @arg TIM_DMA_COM (TIMx Commutation DMA source)
  *          @arg TIM_DMA_TRIGGER (TIMx Trigger DMA source)
  * @param  enable_flag: ENABLE or DISABLE the DMA Request sources.
  * @return None
  */
void tim_dma_enable_ctrl(tim_reg_t* ptr_timer, uint16_t value, enable_state_t enable_flag)
{ 
    ASSERT(TIM_LIST10_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_DMA_SOURCE_CHECK(value));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Enable the DMA sources */
        ptr_timer->DIEN |= value; 
    }
    else
    {
        /* Disable the DMA sources */
        ptr_timer->DIEN &= (uint16_t)~value;
    }
}

/**
  * @fn void tim_dma_request_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief  enable or disable channel DMA request source.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 15, 16 or 17, 
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE Capture Compare DMA source
  * @return None
  */
void tim_dma_request_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_LIST5_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the CHDMARS bit */
        ptr_timer->CTR2 |= TIM_CTR2_CHDMARS;
    }
    else
    {
        /* Reset the CHDMARS bit */
        ptr_timer->CTR2 &= (uint16_t)~((uint16_t)TIM_CTR2_CHDMARS);
    }
}

/**
  * @fn void tim_internal_clock_config(tim_reg_t* ptr_timer)
  * @brief  Configures the TIMx internal Clock
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @return None
  */
void tim_internal_clock_config(tim_reg_t* ptr_timer)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
  
    /* Disable slave mode to clock the prescaler directly with the internal clock */
    ptr_timer->SMCFG &=  (uint16_t)(~((uint16_t)TIM_SMCFG_SMCFG));
}

/**
  * @fn void tim_internal_trigger_external_clock_config(tim_reg_t* ptr_timer, uint16_t value) 
  * @brief  Config the TIMx Internal Trigger as EXTCLK (External Clock)
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: Trigger source, it can be one of the following values:
  *          @arg TIM_TRIGGER_SEL_ITRO (Internal Trigger 0)
  *          @arg TIM_TRIGGER_SEL_ITR1 (Internal Trigger 1)
  *          @arg TIM_TRIGGER_SEL_ITR2 (Internal Trigger 2)
  *          @arg TIM_TRIGGER_SEL_ITR3 (Internal Trigger 3)
  * @return None
  */
void tim_internal_trigger_external_clock_config(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_INTERNAL_TRIGGER_SEL_CHECK(value));
  
    /* Select the Internal Trigger */
    tim_input_trigger_select(ptr_timer, value);
    /* Select the External clock mode1 */
    ptr_timer->SMCFG |= TIM_SLAVE_MODE_EXTERNAL1;
}

/**
  * @fn void tim_ti_external_clock_config(tim_reg_t* ptr_timer, uint16_t value,uint16_t polarity, uint16_t filter) 
  * @brief  Configures the TIMx Trigger as EXTCLK(External Clock)
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: Trigger source,it can be one of the following values:
  *          @arg TIM_TIMx_EXTlCLK1_SOURCE_TI1ED (TI1 Edge Detector)
  *          @arg TIM_TIMx_EXTlCLK1_SOURCE_TI1 (Filtered Timer Input 1)
  *          @arg TIM_TIMx_EXTlCLK1_SOURCE_TI2 (Filtered Timer Input 2)
  * @param  polarity: specifies the TIx Polarity,it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_FALLING
  * @param  filter: specifies the filter value (between 0x0 and 0xF).
  * @return None
  */
void tim_ti_external_clock_config(tim_reg_t* ptr_timer, uint16_t value,uint16_t polarity, uint16_t filter)                                
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_CHxIC_POLARITY_CHECK(polarity));
    ASSERT(TIM_CHxIC_FILTER_CHECK(filter));
  
    /* Configure the Timer Input Clock Source */
    if (value == TIM_TIMx_EXTlCLK1_SOURCE_TI2)
    {
        tim_ch2ic_config(ptr_timer, polarity, TIM_CHIC_SEL_DIRECT_INTR, filter);
    }
    else
    {
        tim_ch1ic_config(ptr_timer, polarity, TIM_CHIC_SEL_DIRECT_INTR, filter);
    }
  
    /* Select the Trigger source */
    tim_input_trigger_select(ptr_timer, value); 
    /* Set the External Clock mode1 */ 
    ptr_timer->SMCFG |= TIM_SLAVE_MODE_EXTERNAL1;
}

/**
  * @fn void tim_external_clock_mode1_config(tim_reg_t* ptr_timer, uint16_t divide, uint16_t polarity,uint16_t filter)
  * @brief  Configures the EXTCLK(External clock) Mode1
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  divide: The external Trigger Pre-divide, it can be one of the following values:
  *          @arg TIM_EXT_TRG_PDIV_OFF (Prescaler OFF)
  *          @arg TIM_EXT_TRG_PDIV2 (divided by 2)
  *          @arg TIM_EXT_TRG_PDIV4 (divided by 4)
  *          @arg TIM_EXT_TRG_PDIV8 (divided by 8)
  * @param  polarity: The external Trigger Polarity, it can be one of the following values:
  *          @arg TIM_EXT_TRG_POLARITY_INVERTED (active low or falling edge active)
  *          @arg TIM_EXT_TRG_POLARITY_NON_INVERTED (active high or rising edge active)
  * @param  filter: External Trigger Filter. It is a value between 0x00 and 0x0F.
  * @return None
  */
void tim_external_clock_mode1_config(tim_reg_t* ptr_timer, uint16_t divide, uint16_t polarity,uint16_t filter)                             
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_EXT_TRG_PDIV_CHECK(divide));
    ASSERT(TIM_EXT_TRG_POLARITY_CHECK(polarity));
    ASSERT(TIM_EXT_FILTER_CHECK(filter));
  
    /* Configure the ETR Clock source */
    tim_external_trigger_config(ptr_timer, divide, polarity, filter);
  
    reg_value = ptr_timer->SMCFG;
    /* clear the SMCFG bits */
    reg_value &= (uint16_t)(~((uint16_t)TIM_SMCFG_SMCFG));
  
    /* Select the External clock mode1 */
    reg_value |= TIM_SLAVE_MODE_EXTERNAL1;
  
    reg_value &= (uint16_t)(~((uint16_t)TIM_SMCFG_TRIGS));
    reg_value |= TIM_TRIGGER_SEL_EXTERNAL;
  
    ptr_timer->SMCFG = reg_value;
}

/**
  * @fn void tim_external_clock_mode2_config(tim_reg_t* ptr_timer, uint16_t divide, uint16_t polarity,uint16_t filter)
  * @brief  Configures the EXTCLK(External clock) Mode 2.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  divide: The external Trigger Pre-divide, it can be one of the following values:
  *          @arg TIM_EXT_TRG_PDIV_OFF (Prescaler OFF)
  *          @arg TIM_EXT_TRG_PDIV2 (divided by 2)
  *          @arg TIM_EXT_TRG_PDIV4 (divided by 4)
  *          @arg TIM_EXT_TRG_PDIV8 (divided by 8)
  * @param  polarity: The external Trigger Polarity, it can be one of the following values:
  *          @arg TIM_EXT_TRG_POLARITY_INVERTED (active low or falling edge active)
  *          @arg TIM_EXT_TRG_POLARITY_NON_INVERTED (active high or rising edge active)
  * @param  filter: External Trigger Filter.It is a value between 0x00 and 0x0F.
  * @return None
  */
void tim_external_clock_mode2_config(tim_reg_t* ptr_timer, uint16_t divide, uint16_t polarity, uint16_t filter)                            
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_EXT_TRG_PDIV_CHECK(divide));
    ASSERT(TIM_EXT_TRG_POLARITY_CHECK(polarity));
    ASSERT(TIM_EXT_FILTER_CHECK(filter));
  
    /* Configure the ETR Clock source */
    tim_external_trigger_config(ptr_timer, divide, polarity, filter);
    /* Enable the External clock mode2 */
    ptr_timer->SMCFG |= TIM_SMCFG_ECMODE2;
}

/**
  * @fn void tim_input_trigger_select(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Selects the Input Trigger source
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: The Input Trigger source, it can be one of the following values:
  *          @arg TIM_TRIGGER_SEL_ITRO (Internal Trigger 0)
  *          @arg TIM_TRIGGER_SEL_ITR1 (Internal Trigger 1)
  *          @arg TIM_TRIGGER_SEL_ITR2 (Internal Trigger 2)
  *          @arg TIM_TRIGGER_SEL_ITR3 (Internal Trigger 3)
  *          @arg TIM_TRIGGER_SEL_EDGE (TI1 Edge Detector)
  *          @arg TIM_TRIGGER_SEL_FILTER1 (Filtered Timer Input 1) 
  *          @arg TIM_TRIGGER_SEL_FILTER2 (Filtered Timer Input 2) 
  *          @arg TIM_TRIGGER_SEL_EXTERNAL (External Trigger input)
  * @return None
  */
void tim_input_trigger_select(tim_reg_t* ptr_timer, uint16_t value)
{
    uint16_t reg_value = 0;

    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_TRIGGER_SEL_CHECK(value));

    reg_value = ptr_timer->SMCFG;
    /* clear the TRIGS Bits */
    reg_value &= (uint16_t)(~((uint16_t)TIM_SMCFG_TRIGS));
  
    /* Set the Input Trigger source */
    reg_value |= value;  
    /* Write to SMCFG */
    ptr_timer->SMCFG = reg_value;
}

/**
  * @fn void tim_trigger_output_select(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Selects the TIMx Trigger Output Mode.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 6, 7, or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Trigger Output source, it can be one of the following values:
  *         for all TIMx: 
  *          @arg TIM_TRG_OUTPUT_SEL_RESET
  *          @arg TIM_TRG_OUTPUT_SEL_ENABLE
  *          @arg TIM_TRG_OUTPUT_SEL_UPDATE
  *         for all TIMx except TIM6 and TIM7: 
  *          @arg TIM_TRG_OUTPUT_SEL_CH1OC
  *          @arg TIM_TRG_OUTPUT_SEL_CH1OC_REF
  *          @arg TIM_TRG_OUTPUT_SEL_CH2OC_REF
  *          @arg TIM_TRG_OUTPUT_SEL_CH3OC_REF
  *          @arg TIM_TRG_OUTPUT_SEL_CH4OC_REF 
  * @return None
  */
void tim_trigger_output_select(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST9_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_TRG_OUTPUT_SEL_CHECK(value));

    /* Reset the MMTOC bits */
    ptr_timer->CTR2 &= (uint16_t)~((uint16_t)TIM_CTR2_MMTOC);
    ptr_timer->CTR2 |=  value;
}

/**
  * @fn void tim_slave_mode_select(tim_reg_t* ptr_timer, uint16_t value) 
  * @brief  Selects the TIMx Slave Mode.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Timer Slave Mode.
  *         It can be one of the following values:
  *          @arg TIM_SLAVE_MODE_RESET
  *          @arg TIM_SLAVE_MODE_TRIGGER 
  *          @arg TIM_SLAVE_MODE_EXTERNAL1
  * @return None
  */
void tim_slave_mode_select(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer)); 
    ASSERT(TIM_SLAVE_MODE_CHECK(value));
  
    /* Clear the SMCFG bits */
    ptr_timer->SMCFG &= (uint16_t)~((uint16_t)TIM_SMCFG_SMCFG);
    /* Select the slave mode */
    ptr_timer->SMCFG |= value;
}

/**
  * @fn void tim_master_slave_control(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Sets or Resets the TIMx Master/Slave Mode.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3 or 15.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  value: specifies the Timer Master Slave Mode.
  *         It can be one of the following values:
  *          @arg TIM_MASTER_SLAVE_MODE_ENABLE
  *          @arg TIM_MASTER_SLAVE_MODE_DISABLE
  * @return None
  */
void tim_master_slave_control(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST6_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_MASTER_SLAVE_MODE_CHECK(value));
  
    /* clear the SMCFG_MSM bit */
    ptr_timer->SMCFG &= (uint16_t)~((uint16_t)TIM_SMCFG_MSM);
    
    /* config the SMCFG_MSM bit */
    ptr_timer->SMCFG |= value;
}

/**
  * @fn void tim_external_trigger_config(tim_reg_t* ptr_timer, uint16_t divide, 
                                                          uint16_t polarity, uint16_t filter) 
  * @brief  Configures the TIMx External Trigger .
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  divide: The external Trigger Pre-divide, it can be one of the following values:
  *          @arg TIM_EXT_TRG_PDIV_OFF (Prescaler OFF)
  *          @arg TIM_EXT_TRG_PDIV2 (divided by 2)
  *          @arg TIM_EXT_TRG_PDIV4 (divided by 4)
  *          @arg TIM_EXT_TRG_PDIV8 (divided by 8)
  * @param  polarity: The external Trigger Polarity, it can be one of the following values:
  *          @arg TIM_EXT_TRG_POLARITY_INVERTED (active low or falling edge active)
  *          @arg TIM_EXT_TRG_POLARITY_NON_INVERTED (active high or rising edge active)
  * @param  filter: External Trigger Filter. It is a value between 0x00 and 0x0F.
  * @return None
  */
void tim_external_trigger_config(tim_reg_t* ptr_timer, uint16_t divide,uint16_t polarity, uint16_t filter)                                                                             
{
    uint16_t reg_value = 0;
  
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_EXT_TRG_PDIV_CHECK(divide));
    ASSERT(TIM_EXT_TRG_POLARITY_CHECK(polarity));
    ASSERT(TIM_EXT_FILTER_CHECK(filter));
    
    reg_value = ptr_timer->SMCFG;
    /* Reset the ET Bits */
    reg_value &= TIM_SMCFG_ET_MASK;
  
    /* Set the Prescaler, the Filter value and the Polarity */
    reg_value |= (uint16_t)(divide | (uint16_t)(polarity | (uint16_t)(filter << (uint16_t)8)));
    /* write to SMCFG */
    ptr_timer->SMCFG = reg_value;
}

/**
  * @fn void tim_encoder_interface_set(tim_reg_t* ptr_timer, uint16_t mode, uint16_t polarity, uint16_t value) 
  * @brief  Configures the TIMx Encoder Interface.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  mode: specifies the TIMx Encoder Mode, it can be one of the following values:
  *          @arg TIM_ENCODER_MODE_TI1 (Counter counts on TI1FP1 edge depending on TI2FP2 level)
  *          @arg TIM_ENCODER_MODE_TI2 (Counter counts on TI2FP2 edge depending on TI1FP1 level)
  *          @arg TIM_ENCODER_MODE_TI12 (Counter counts on both TI1FP1 and TI2FP2 edges depending
  *               on the level of the other input)
  * @param  polarity: specifies the CH1IC Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_FALLING
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_BOTH_EDGE
  * @param  value: specifies the CH2IC Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_FALLING
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_BOTH_EDGE
  * @return None
  */
void tim_encoder_interface_set(tim_reg_t* ptr_timer, uint16_t mode, uint16_t polarity, uint16_t value)                               
{
    uint16_t temp1 = 0;
    uint16_t temp2 = 0;
    uint16_t temp3 = 0;
    
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(TIM_ENCODER_MODE_CHECK(mode));
    ASSERT(TIM_CHxIC_POLARITY_CHECK(polarity));
    ASSERT(TIM_CHxIC_POLARITY_CHECK(value));
  
    temp1 = ptr_timer->SMCFG;  
    temp2 = ptr_timer->CH12CFGR;  
    temp3 = ptr_timer->CCCTR;
  
    /* Set the encoder Mode */
    temp1 &= (uint16_t)(~((uint16_t)TIM_SMCFG_SMCFG));
    temp1 |= mode;
  
    temp2 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CH12CFGR_CH1FS)) & (uint16_t)(~((uint16_t)TIM_CH12CFGR_CH2FS)));
    temp2 |= TIM_CH12CFGR_CH1FS_0 | TIM_CH12CFGR_CH2FS_0;
  
    /* Set the TI1 and the TI2 Polarities */
    temp3 &= (uint16_t)~((uint16_t)(TIM_CCCTR_CH1CCP | TIM_CCCTR_CH1NCCP)) & (uint16_t)~((uint16_t)(TIM_CCCTR_CH2CCP | TIM_CCCTR_CH2NCCP));
    temp3 |= (uint16_t)(polarity | (uint16_t)(value << (uint16_t)4));
    
    ptr_timer->SMCFG = temp1;  
    ptr_timer->CH12CFGR = temp2;  
    ptr_timer->CCCTR = temp3;
}

/**
  * @fn void tim_xor_input_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
  * @brief  Enables or disables XOR input for TI1.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.
  * @param  enable_flag: ENABLE or DISABLE XOR input.
  * @return None
  */
void tim_xor_input_set(tim_reg_t* ptr_timer, enable_state_t enable_flag)
{
    ASSERT(TIM_LIST3_TIMx_CHECK(ptr_timer));
    ASSERT(FUNCTIONAL_STATE_CHECK(enable_flag));
  
    if (enable_flag != DISABLE)
    {
        /* Set the TI1XOR bit */
        ptr_timer->CTR2 |= TIM_CTR2_TI1XOR;
    }
    else
    {
        /* clear the TI1XOR bit */
        ptr_timer->CTR2 &= (uint16_t)~((uint16_t)TIM_CTR2_TI1XOR);
    }
}

/**
  * @fn void tim_remap_config(tim_reg_t* ptr_timer, uint16_t value)
  * @brief  Configures the TIM14 Remapping input Capabilities.
  * @param  ptr_timer: pointer to TIMx where x can be 14.  
  * @param  value: specifies the TIM input reampping source, it can be one of the following values:
  *          @arg TIM14_REMAP_PORT (TIM14 Channel 1 is connected to PORT)
  *          @arg TIM14_REMAP_RTC_CLK (TIM14 Channel 1 is connected to RTC input clock)
  *          @arg TIM14_REMAP_HXT_DIV32 (TIM14 Channel 1 is connected to HXT/32 clock)  
  *          @arg TIM14_REMAP_MCO (TIM14 Channel 1 is connected to CLKOUT clock) 
  * @return None
  */
void tim_remap_config(tim_reg_t* ptr_timer, uint16_t value)
{
    ASSERT(TIM_LIST11_TIMx_CHECK(ptr_timer));
    ASSERT(TIM14_REMAP_CHECK(value));

    /* Set the Timer remapping configuration */
    ptr_timer->REMAP =  value;
}

/**
  * @fn static void tim_ch1ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select,uint16_t filter) 
  * @brief  Configure the Channel1 as Input.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, 14, 15, 16 or 17 .  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.   
  * @param  polarity: The Input Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_FALLING
  * @param  select: specifies the input to be used, it can be one of the following values:
  *          @arg TIM_CHIC_SEL_DIRECT_INTR
  *          @arg TIM_CHIC_SEL_INDIRECT_INTR
  *          @arg TIM_CHIC_SEL_TRC
  * @param  filter: Specifies the Input Capture Filter, the value between 0x00 and 0x0F
  * @return None
  */
static void tim_ch1ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select,uint16_t filter)                       
{
    uint16_t reg1 = 0;
    uint16_t reg2 = 0;
  
    /* Disable the Channel 1: clear the CH1CCEN Bit */
    ptr_timer->CCCTR &= (uint16_t)~((uint16_t)TIM_CCCTR_CH1CCEN);
    reg1 = ptr_timer->CH12CFGR;
    reg2 = ptr_timer->CCCTR;
    
    /* Select the Input and set the filter */
    reg1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CH12CFGR_CH1FS)) & ((uint16_t)~((uint16_t)TIM_CH12CFGR_CH1ICFLT)));
    reg1 |= (uint16_t)(select | (uint16_t)(filter << (uint16_t)4));
 
    /* Select the Polarity and set the CH1CCEN bit */
    reg2 &= (uint16_t)~((uint16_t)(TIM_CCCTR_CH1CCP | TIM_CCCTR_CH1NCCP));
    reg2 |= (uint16_t)(polarity | (uint16_t)TIM_CCCTR_CH1CCEN);
  
    ptr_timer->CH12CFGR = reg1;
    ptr_timer->CCCTR = reg2;
}

/**
  * @fn static void tim_ch2ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select,uint16_t filter) 
  * @brief  Configure the Channel2 as Input.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2, 3, or 15 .  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices. 
  * @param  polarity: The Input Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_FALLING
  * @param  select: specifies the input to be used, it can be one of the following values:
  *          @arg TIM_CHIC_SEL_DIRECT_INTR
  *          @arg TIM_CHIC_SEL_INDIRECT_INTR
  *          @arg TIM_CHIC_SEL_TRC
  * @param  filter: Specifies the Input Capture Filter, the value between 0x00 and 0x0F
  * @return None
  */
static void tim_ch2ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select,uint16_t filter)                       
{
    uint16_t reg1 = 0;
    uint16_t reg2 = 0;
    uint16_t temp = 0;
  
    /* Disable the Channel 2: clear the CH2CCEN Bit */
    ptr_timer->CCCTR &= (uint16_t)~((uint16_t)TIM_CCCTR_CH2CCEN);
    reg1 = ptr_timer->CH12CFGR;
    reg2 = ptr_timer->CCCTR;
    temp = (uint16_t)(polarity << 4);
  
    /* Select the Input and set the filter */
    reg1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CH12CFGR_CH2FS)) & ((uint16_t)~((uint16_t)TIM_CH12CFGR_CH2ICFLT)));
    reg1 |= (uint16_t)(filter << 12);
    reg1 |= (uint16_t)(select << 8); 
  
    /* Select the Polarity and set the CH2CCEN bit */
    reg2 &= (uint16_t)~((uint16_t)(TIM_CCCTR_CH2CCP | TIM_CCCTR_CH2NCCP));
    reg2 |= (uint16_t)(temp | (uint16_t)TIM_CCCTR_CH2CCEN);  
  
    ptr_timer->CH12CFGR = reg1 ;
    ptr_timer->CCCTR = reg2;
}


/**
  * @fn static void tim_ch3ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter)
  * @brief  Configure the Channel3 as Input.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices. 
  * @param  polarity: The Input Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @argTIM_CHxIC_POLARITY_FALLING
  * @param  select: specifies the input to be used, It can be one of the following values:
  *          @arg TIM_CHIC_SEL_DIRECT_INTR;TIM_CHIC_SEL_INDIRECT_INTR
  *          @arg TIM_CHIC_SEL_TRC
  * @param  filter: Specifies the Input Capture Filter, the value between 0x00 and 0x0F
  * @return None
  */
static void tim_ch3ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter)                      
{
    uint16_t reg1 = 0;
    uint16_t reg2 = 0;
    uint16_t temp = 0;
  
    /* Disable the Channel 3: clear the CH3CCEN bit */
    ptr_timer->CCCTR &= (uint16_t)~((uint16_t)TIM_CCCTR_CH3CCEN);
    reg1 = ptr_timer->CH34CFGR;
    reg2 = ptr_timer->CCCTR;
    temp = (uint16_t)(polarity << 8);
  
    /* Select the Input and set the filter */
    reg1 &= (uint16_t)(((uint16_t)~((uint16_t)TIM_CH34CFGR_CH3FS)) & ((uint16_t)~((uint16_t)TIM_CH34CFGR_CH3ICFLT)));
    reg1 |= (uint16_t)(select | (uint16_t)(filter << (uint16_t)4));
  
    /* Select the Polarity and set the CH3CCEN bit */
    reg2 &= (uint16_t)~((uint16_t)(TIM_CCCTR_CH3CCP | TIM_CCCTR_CH3NCCP));
    reg2 |= (uint16_t)(temp | (uint16_t)TIM_CCCTR_CH3CCEN);  
  
    ptr_timer->CH34CFGR = reg1;
    ptr_timer->CCCTR = reg2;
}

/**
  * @fn static void tim_ch4ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter)
  * @brief  Configure the Channel4 as Input.
  * @param  ptr_timer: pointer to TIMx where x can be 1, 2 or 3.  
  * @note   TIM2 is not applicable for CS32F030, CS32F036 devices.   
  * @param  polarity: The Input Polarity, it can be one of the following values:
  *          @arg TIM_CHxIC_POLARITY_RISING
  *          @arg TIM_CHxIC_POLARITY_FALLING
  * @param  select: specifies the input to be used, it can be one of the following values:
  *          @arg TIM_CHIC_SEL_DIRECT_INTR
  *          @arg TIM_CHIC_SEL_INDIRECT_INTR
  *          @arg TIM_CHIC_SEL_TRC
  * @param  filter: Specifies the Input Capture Filter, the value between 0x00 and 0x0F
  * @return None
  */
static void tim_ch4ic_config(tim_reg_t* ptr_timer, uint16_t polarity, uint16_t select, uint16_t filter)                      
{
    uint16_t reg1 = 0;
    uint16_t reg2 = 0;
    uint16_t temp = 0;

    /* Disable the Channel 4: clear the CH4CCEN Bit */
    ptr_timer->CCCTR &= (uint16_t)~((uint16_t)TIM_CCCTR_CH4CCEN);
    reg1 = ptr_timer->CH34CFGR;
    reg2 = ptr_timer->CCCTR;
    temp = (uint16_t)(polarity << 12);
  
    /* Select the Input and set the filter */
    reg1 &= (uint16_t)((uint16_t)(~(uint16_t)TIM_CH34CFGR_CH4FS) & ((uint16_t)~((uint16_t)TIM_CH34CFGR_CH4ICFLT)));
    reg1 |= (uint16_t)(select << 8);
    reg1 |= (uint16_t)(filter << 12);  
  
    /* Select the Polarity and set the CH4CCEN Bit */
    reg2 &= (uint16_t)~((uint16_t)(TIM_CCCTR_CH4CCP | TIM_CCCTR_CH4NCCP));
    reg2 |= (uint16_t)(temp | (uint16_t)TIM_CCCTR_CH4CCEN);
    
    ptr_timer->CH34CFGR = reg1;
    ptr_timer->CCCTR = reg2;
}

/**
  * @}
  */
 
/**
  * @}
  */

