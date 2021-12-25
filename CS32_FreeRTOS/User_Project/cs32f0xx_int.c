/**
  * @file   LED/cs32f0xx_int.c 
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
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"

extern osSemaphoreId BinarySemAdcConvCpltHandle;

/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup LED
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
//void SVC_Handler(void)
//{
//}

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
//void PendSV_Handler(void)
//{
//}

/**
  * @fn void SysTick_Handler(void)
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @return None
  */


/**
  * @fn void DMA1_Channel1_IRQHandler(void)
  * @brief  This function handles DMA1 Channel 1 interrupt request.
  * @param  None
  * @return None
  */
void DMA1_Channel1_IRQHandler(void)
{

    /* Test on DMA1 Channel1 Transfer Complete interrupt */
    if(dma_interrupt_status_get(DMA1_INT_CMP1))
    {
        /* Clear DMA1 Channel1 Half Transfer, Transfer Complete and  interrupt  bits */
        dma_interrupt_flag_clear(DMA1_INT_G1);
        osSemaphoreRelease(BinarySemAdcConvCpltHandle);

    }
}


//uint16_t  TestTime = 0 ;
void SysTick_Handler(void)
{
//	TestTime++;
//	TestTime =  TestTime ;
	
	timing_delay_decrement();
	//------------------------------------------
	
				#if (INCLUDE_xTaskGetSchedulerState == 1 )
				if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
				{
			#endif /* INCLUDE_xTaskGetSchedulerState */
				xPortSysTickHandler();
			#if (INCLUDE_xTaskGetSchedulerState == 1 )
				}
			#endif /* INCLUDE_xTaskGetSchedulerState */
	
	
}


/**
  * @}
  */

/**
  * @}
  */


