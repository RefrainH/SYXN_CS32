#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: StartUsartInit
*	功能说明: 调试串口初始化
*	形    参: 无
*	返 回 值: 无
*	PA9    ----		TX
*   PA10   ----     RX
*   波特率 ----     115200
*********************************************************************************************************
*/

void StartUsartInit(void)
{ 
    usart_config_t usart_config_struct;
    gpio_config_t  gpio_config_struct; 
    
    // Clock Config	
    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTA, ENABLE);
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_USART1, ENABLE); 
    
    // GPIO MF Config
    gpio_mf_config(GPIOA, GPIO_PIN_NUM9, GPIO_MF_SEL1);   
    gpio_mf_config(GPIOA, GPIO_PIN_NUM10, GPIO_MF_SEL1);
        
    gpio_config_struct.gpio_pin = GPIO_PIN_9 | GPIO_PIN_10;	 //PA9,PA10 USART	
    gpio_config_struct.gpio_mode = GPIO_MODE_MF;
    gpio_config_struct.gpio_speed = GPIO_SPEED_MEDIUM;
    gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;
    gpio_init(GPIOA, &gpio_config_struct);	
	
    // USART Config	
    usart_def_init(USART1);
    usart_config_struct.usart_rate = 115200;
    usart_config_struct.data_width = USART_DATA_WIDTH_8;
    usart_config_struct.stop_bits = USART_STOP_BIT_1;
    usart_config_struct.usart_parity = USART_PARITY_NO;
    usart_config_struct.flow_control = USART_FLOW_CONTROL_NONE;
    usart_config_struct.usart_mode = USART_MODE_RX | USART_MODE_TX;
    usart_init(USART1, &usart_config_struct);

    usart_enable_ctrl(USART1,ENABLE);	
}

/**
 * @fn void cs_start_usart_nvic_config(void)
 * @brief  Configuration usart interrupt . 		
 * @param  None
 * @return None
 */
void cs_start_usart_nvic_config(void)
{
    nvic_config_t  nvic_config_struct;	
	
    usart_enable_ctrl(USART1, DISABLE); //Enable USART 
    /* Enable the USART Interrupt */
    nvic_config_struct.nvic_IRQ_channel = IRQn_USART1;
    nvic_config_struct.nvic_channel_priority = 0;
    nvic_config_struct.nvic_enable_flag = ENABLE;
    nvic_init(&nvic_config_struct);

    usart_enable_ctrl(USART1, ENABLE); //Enable USART 
    usart_interrupt_config(USART1, USART_INT_RXNE, ENABLE); // Enable the USART receive interrupt
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @return None
  */
int fputc(int ch, FILE *f)
{
    UNUSED(f);
    usart_data_send(USART1, (uint8_t) ch);
    while(RESET == usart_flag_status_get(USART1, USART_FLAG_TCF));
    return ch;
}

    
