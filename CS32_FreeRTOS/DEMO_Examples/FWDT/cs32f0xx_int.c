/**
  * @file   FWDT/cs32f0xx_int.c 
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
#include "fwdt.h"

/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup FWDT
  * @{
  */


static volatile uint16_t capture_value1 = 0;
static volatile uint16_t capture_value2 = 0;
static volatile uint32_t capture = 0;


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
    timing_delay_decrement();
}

#ifdef LRC_TIM_MEASURE

/**
  * @fn void TIM14_IRQHandler(void)
  * @brief  This function handles TIM14  interrupt request.
  * @param  None
  * @return None
  */
void TIM14_IRQHandler(void)
{
    if (tim_interrupt_status_get(TIM14, TIM_INTR_CH1) != RESET)
    {
        if(capture_count == 0)
        {
            /* Get the Input capture value */
            capture_value1 = (uint16_t)tim_ch1cc_value_get(TIM14);
        }
        else if(capture_count == 1)
        {
            /* Get the Input capture value */
            capture_value2 = (uint16_t)tim_ch1cc_value_get(TIM14);

            /* capture computation */
            if (capture_value2 > capture_value1)
            {
                capture = (capture_value2 - capture_value1);
            }
            else
            {
                capture = ((0xFFFF - capture_value1) + capture_value2);
            }
            /* Frequency computation */
            freq_lrc = (uint32_t) SystemCoreClock / capture;
            freq_lrc *= 8;
        }

        capture_count++;

        /* Clear TIM14 capture compare interrupt  bit */
        tim_interrupt_status_clear(TIM14, TIM_INTR_CH1);
    }
}
#endif

/**
  * @fn void EXTI4_15_IRQHandler(void)
  * @brief  This function handles External line 4 to 15 interrupt request.
  * @param  None
  * @return None
  */
void EXTI4_15_IRQHandler(void)
{
    if (exti_interrupt_status_get(EXTI_LINE_11) != RESET)
    {
        exti_interrupt_status_clear(EXTI_LINE_11);

        /*  when the FWDT counter reaches 0the FWDT reset occurs */           
        *(__IO uint32_t *) 0x00040001 = 0xFF;
    }
}

/**
  * @}
  */

/**
  * @}
  */


