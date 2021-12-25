#include "bsp.h"

/*
*********************************************************************************************************
*
*	模块名称 : 单板GPIO驱动模块
*	文件名称 : button.c
*	版    本 : V1.0
*	说    明 : 初始化单板GPIO
*
*	修改记录 :
*		版本号  日期        作者     说明

*********************************************************************************************************
*/



/*
*********************************************************************************************************
*	函 数 名: StartLedGpioInit
*	功能说明: 初始化LED灯的GPIO，熄灭所有LED
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void StartLedGpioInit(void)
{
    gpio_config_t  gpio_config_struct;
	/* 打开GPIO时钟 */
    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB, ENABLE); 
	
	/* 配置相关引脚
     * PB3  -->  LED1(Error)
	 * PB4  -->  LED2(Alarm)
	 * PB5  -->  LED3(MCU)
	*/
    gpio_config_struct.gpio_pin = LED_RED_Error | LED_YELLOW_Alarm | LED_GREEN_Work; 
	/* 配置GPIO为推挽输出 */
    gpio_config_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_config_struct.gpio_speed = GPIO_SPEED_HIGH;  
    gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
	/* 配置GPIO无上下拉 */
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;	
	/* 初始化GPIO */
    gpio_init(GPIOB, &gpio_config_struct);
	/* 初始化完成后所有LED熄灭 */
    gpio_bits_reset(GPIOB,LED_RED_Error | LED_YELLOW_Alarm | LED_GREEN_Work);	
}

/*
*********************************************************************************************************
*	函 数 名: StartStateGpioInit
*	功能说明: 配置外部试跳口GPIO，该函数被bsp_Init调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartStateGpioInit(void)
{
	gpio_config_t  gpio_config_struct;
	
	/* 打开GPIOB的时钟 */
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);	

	/* PB15 --> STATE:检测磁通状态 */
	gpio_config_struct.gpio_mode = GPIO_MODE_INPUT;				/* 输入模式 */
	gpio_config_struct.gpio_pin = STATE_DETECT_PIN;
	gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;			/* 外部电路有上拉电阻，所以这里配置成无上下拉模式 */
	gpio_init(GPIOB,&gpio_config_struct);						/* 初始化GPIO配置 */
}

/*
*********************************************************************************************************
*	函 数 名: StartControlGpioInit
*	功能说明: 配置对外控制的GPIO，该函数被bsp_Init调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartControlGpioInit(void)
{
	gpio_config_t  gpio_config_struct;
	
	/* 打开GPIOB的时钟 */
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);	
	
	/*
	 *  PB13 --> BREAK: 磁通脱扣控制
	 *  PB14 --> 自生电源电路控制引脚
	 */
	gpio_config_struct.gpio_pin = TK_CONTROL_PIN | POWER_CONTROL_PIN;
	gpio_config_struct.gpio_mode = GPIO_MODE_OUTPUT;				/* 推挽输出 */
	gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
	gpio_config_struct.gpio_speed = GPIO_SPEED_HIGH;
	gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;				/* 无上下拉配置 */
	gpio_init(GPIOB,&gpio_config_struct);							/* 初始化GPIO配置 */	
    gpio_bits_reset(GPIOB,TK_CONTROL_PIN | POWER_CONTROL_PIN);		
}

/*
*********************************************************************************************************
*	函 数 名: StartAllGpioInit
*	功能说明: 完成所有gpio初始化功能
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartAllGpioInit(void)
{
	StartLedGpioInit();
	StartStateGpioInit();
	StartControlGpioInit();
}


