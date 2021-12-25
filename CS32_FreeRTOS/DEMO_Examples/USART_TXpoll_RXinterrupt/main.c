/**
  * @file    USART_TXpoll_RXinterrupt/main.c 
  * @brief   Main program body
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "main.h"

/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup USART_TXpoll_RXinterrupt
  * @{
  */
	
uint8_t rx_buf[MAXRXLEN];
__IO uint8_t rx_index = 0x00;
__IO uint8_t rx_flag = 0;


/**
  * @fn int main(void)
  * @brief  Main program.   MCU clock setting is  configured  through SystemInit() 
  *          in startup file (startup_cs32f0xx.s) before to enter to application main.       
  * @param  None
  * @return None
  */
int main(void)
{  
    cs_start_usart_config();
    cs_start_usart_nvic_config();
    cs_start_usart_timeout_config();
    printf("--------USART_TXpoll_RXinterrupt --------\n\r");
    while(1)
    {
        if(rx_flag == 1)
        {
            printf("%s\n\r", rx_buf);
            rx_flag = 0;
        }
    }	
}

/**
  * @}
  */

/**
  * @}
  */

