/**
  * @file   TIM_BASE/cs32f0xx_int.c 
  * @brief  Main Interrupt Service Routines. It provides template for
  *            all exceptions handler and  peripherals interrupt service routine.
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01        
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */
  
#include "cs32f0xx_int.h"
#include "tim.h"
/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup TIM_BASE
  * @{
  */

extern uint16_t ch1_pulse;
extern uint16_t ch2_pulse;
extern uint16_t ch3_pulse;
extern uint16_t ch4_pulse;

static uint16_t capture_value = 0;

/**
  * @fn void NMI_Handler(void)
  * @brief   This function handles NMI exception.
  * @param  None
  * @return None
  */
void NMI_Handler(void)
{
}

/**
  * @fn void HardFault_Handler(void)
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @return None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @fn void MemManage_Handler(void)
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @return None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @fn void BusFault_Handler(void)
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @return None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @fn void UsageFault_Handler(void)
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @return None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @fn void SVC_Handler(void)
  * @brief  This function handles SVCall exception.
  * @param  None
  * @return None
  */
void SVC_Handler(void)
{
}

/**
  * @fn void DebugMon_Handler(void)
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @return None
  */
void DebugMon_Handler(void)
{
}

/**
  * @fn void PendSV_Handler(void)
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @return None
  */
void PendSV_Handler(void)
{
}

/**
  * @fn void SysTick_Handler(void)
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @return None
  */
void SysTick_Handler(void)
{
}

/**
  * @fn void TIM3_IRQHandler(void)
  * @brief  This function handles TIM3  interrupt request.
  * @param  None
  * @return None
  */
void TIM3_IRQHandler(void)
{
    if (tim_interrupt_status_get(TIM3, TIM_INTR_CH1) != RESET)
    {
        tim_interrupt_status_clear(TIM3, TIM_INTR_CH1);
    
        cs_start_gpio_toggle(GPIOA,0);
        capture_value = (uint16_t)tim_ch1cc_value_get(TIM3);
        tim_ch1cc_value_set(TIM3, capture_value + ch1_pulse);
    }
    else if (tim_interrupt_status_get(TIM3, TIM_INTR_CH2) != RESET)
    {
        tim_interrupt_status_clear(TIM3, TIM_INTR_CH2);
    
        cs_start_gpio_toggle(GPIOA,1);
        capture_value = (uint16_t)tim_ch2cc_value_get(TIM3);
        tim_ch2cc_value_set(TIM3, capture_value + ch2_pulse);
    }
    else if (tim_interrupt_status_get(TIM3, TIM_INTR_CH3) != RESET)
    {
        tim_interrupt_status_clear(TIM3, TIM_INTR_CH3);

        cs_start_gpio_toggle(GPIOA,2);
        capture_value = (uint16_t)tim_ch3cc_value_get(TIM3);
        tim_ch3cc_value_set(TIM3, capture_value + ch3_pulse);
    }
    else
    {
        tim_interrupt_status_clear(TIM3, TIM_INTR_CH4);
        cs_start_gpio_toggle(GPIOA,3);
        capture_value = (uint16_t)tim_ch4cc_value_get(TIM3);
        tim_ch4cc_value_set(TIM3, capture_value + ch4_pulse);
    }
}

/**
  * @}
  */

/**
  * @}
  */


