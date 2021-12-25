#include "main.h"

/**
  * @file    I2C_EEPROM/main.c 
  * @brief   Main program body
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */


/** @addtogroup CS32F0xx_DEMO_Examples
  * @{
  */
  
/** @addtogroup I2C_EEPROM
  * @{
  */
static uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len);

/**
  * @fn int main(void)
  * @brief  Main program.   MCU clock setting is  configured  through SystemInit() 
  *          in startup file (startup_cs32f0xx.s) before to enter to application main.       
  * @param  None
  * @return None
  */
int main(void)
{  
    uint8_t i2c_wbuf[] = "I2C EEPROM test!";
    uint8_t i2c_rbuf[32];
    uint16_t len = 0;
		
    memset(i2c_rbuf,0,32); 
    len = sizeof(i2c_wbuf);
	
    cs_start_usart_config();
    cs_start_e2prom_init();
    printf("--------I2C EEPROM DEMO--------\n\r");
	
    cs_start_e2prom_buffer_write(i2c_wbuf, 0x10, len); // Write EEPROM
    cs_start_e2prom_buff_read(i2c_rbuf, 0x10, (uint16_t *)(&len)); // Read  EEPROM
	
    if(buf_compare(i2c_wbuf, i2c_rbuf,len) == 1)
    {
        printf("I2C EEPROM 24C02 operation Success!\n\r");
    }
    else
    {
        printf("I2C EEPROM 24C02 operation Failed!\n\r");
    }
    while(1)
    {
		
    }	
}

/**
  * @fn static uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len)
  * @brief  Compares two buffers.
  * @param  src, dst: buffers to be compared.
  * @param  len: buffer's length
  * @return uint8_t flag: 1---src identical to dst
  *                       0---src differs from dst
  */
static uint8_t buf_compare(uint8_t* src, uint8_t* dst, uint16_t len)
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

