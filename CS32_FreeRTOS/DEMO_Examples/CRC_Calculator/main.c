/**
  * @file    CRC_Calculator/main.c 
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
  
/** @addtogroup CRC_Calculator 
  * @{
  */
	
#define COUNT_OF_VALUE(a)      (uint8_t)(sizeof(a) / sizeof(*(a)))

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
    cs_start_crc_config();
    printf("--------CRC Calculator--------\n\r");
    uint8_t crc_buffer[] = "CS32F036 START CRC DEMO";
    uint32_t len = COUNT_OF_VALUE(crc_buffer) - 1;
    uint32_t crc_compute = cs_start_crc_compute(crc_buffer, len);
    printf("crc_compute=0x%x\n\r", crc_compute);
    while(1)
    {
		
    }	
}

/**
  * @}
  */

/**
  * @}
  */

