#ifndef __MAIN_H__
#define __MAIN_H__

/**
  * @file    USART_TXpoll_RXinterrupt/main.h 
  * @brief   Header for main.c module
  * @author  ChipSea MCU Group
  * @version V1.0.0
  * @date   2018.11.01         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2018 ChipSea</center></h2>
  *
  */

#include "cs32f0xx.h"
#include "usart.h"
#include "tim.h"

#define MAXRXLEN 160

extern uint8_t rx_buf[];
extern __IO uint8_t rx_index;
extern __IO uint8_t rx_flag;

#endif 


