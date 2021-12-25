/**
  * @file   USART_TXpoll_RXinterrupt/cs32f0xx_int.c 
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

/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup USART_TXpoll_RXinterrupt
  * @{
  */

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
    while(1)
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
* @fn void USARTx_IRQHandler(void)
* @brief  This function handles USART interrupt request.
* @param  None
* @return None
*/
void USART1_IRQHandler(void)
{
    if (usart_interrupt_status_get(USART1, USART_INT_RXNE) == SET)
    {
        if (rx_index == 0)
        {
            tim_enable_ctrl(TIM3, ENABLE);
        }
        else {
            tim_counter_set(TIM3, 0);
        }
        if(rx_index <= MAXRXLEN)
        {
            rx_buf[rx_index++] = (uint8_t)usart_data_recv(USART1);
        }
        else
        {
            rx_index = 0;
            rx_buf[rx_index++] = (uint8_t)usart_data_recv(USART1);
        }
    }
}

/**
  * @fn void TIM3_IRQHandler(void)
  * @brief  This function handles TIM3  interrupt request.
  * @param  None
  * @return None
  */
void TIM3_IRQHandler(void)
{
    if (tim_interrupt_status_get(TIM3, TIM_INTR_UPDATE) != RESET)
    {
        tim_interrupt_status_clear(TIM3, TIM_INTR_UPDATE);
        rx_flag = 1;
        rx_index = 0;
        tim_enable_ctrl(TIM3, DISABLE);
    }
}


/**
  * @}
  */

/**
  * @}
  */


