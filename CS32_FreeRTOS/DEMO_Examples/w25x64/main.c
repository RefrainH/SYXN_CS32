/**
  * @file    w25x64/main.c 
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
  
/** @addtogroup w25x64
  * @{
  */
uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len);

/**
  * @fn int main(void)
  * @brief  Main program.   MCU clock setting is  configured  through SystemInit() 
  *          in startup file (startup_cs32f0xx.s) before to enter to application main.       
  * @param  None
  * @return None
  */
int main(void)
{  
    uint8_t spi_wbuf[] = "w25x64 test!";
    uint8_t spi_rbuf[16];
    uint8_t len = 0;
    uint32_t device_id = 0; 
		
    memset(spi_rbuf,0,16); 
    len = sizeof(spi_wbuf);
	
    cs_start_usart_config();
    cs_start_w25x64_init();
    cs_start_systick_config();
    printf("--------W25X64 DEMO--------\n\r");
	
    device_id = cs_start_w25x64_get_id();
    printf("device_id=%x\n\r",device_id);
	
    delay(2);
    cs_start_w25x64_wait_sector_erase(0x00000);
    cs_start_w25x64_buffer_write(spi_wbuf,0x00000,len);
    cs_start_w25x64_buffer_read(spi_rbuf,0x00000,len);
    if(buf_compare(spi_wbuf, spi_rbuf,len) == 1)
    {
        printf("SPI FLASH W25X64 operation Success!\n\r");
    }
    else
    {
        printf("SPI FLASH W25X64 operation Failed!\n\r");
    }
    while(1)
    {
    
    }	
}

/**
  * @fn uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len)
  * @brief  Compares two buffers.
  * @param  src, dst: buffers to be compared.
  * @param  len: buffer's length
  * @return uint8_t flag: 1---src identical to dst
  *                       0---src differs from dst
  */
uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len)
{
    while(len--)
    {
        if(*src != *dst)
        {
            return 0;
        }
		
        src++;
        dst++;
    }
    return 1;
}

/**
  * @}
  */

/**
  * @}
  */

