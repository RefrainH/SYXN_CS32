/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : tim.c
*	版    本 : V1.0
*	说    明 : 配置TIM3作为ADC的触发源
*
*	修改记录 :
*		版本号  日期        作者     说明
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	函 数 名: StartAdcTimInit
*	功能说明: ADC_TIM外设初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

    /*-----------------------------------------------------------------------
		时钟的配置如下: 

        System Clock source       = PLL (HSE)
        SYSCLK(Hz)                = 48000000 (CPU Clock)
        HCLK(Hz)                  = 48000000 (AHB Clock)
        PCLK(Hz)                  = 48000000 (AHB Clock) 

        APB 定时器有  TIM1, TIM2, TIM3 , TIM6, TIM14, TIM15, TIM16，TIM17
		TimeOut = ((Prescaler + 1) * (Period + 1)) / TimeClockFren
		
		TimeOut:定时器溢出时间(单位为us)，多少触发(进入)一次TIM中断
		Prescaler:分频TIM时钟的预分频器值 
		Period:计数重载值，TIM计数超过这个值则重新计数
		TimeClockFren:定时器的输入时钟频率(单位MHz)
	----------------------------------------------------------------------- */

void StartAdcTimInit(void)
{
    timer_config_t  timer_config_struct;
    timer_compare_t timer_compare_struct; 

    /* 开启TIM3时钟 */
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_TIM1, ENABLE);

	/* 缺省TIM1寄存器的值 */
    tim_def_init(TIM1);  
    tim_config_struct_init(&timer_config_struct);  
    tim_compare_struct_init(&timer_compare_struct);

	/* TimeOut = ((Prescaler + 1) * (Period + 1)) / TimeClockFren = (15000 * 1) / 48000000 = 0.3125ms */
    timer_config_struct.time_period = 30000-1;				//15000-1 -> 64个采样点    60000-1 -> 16个采样点
    timer_config_struct.time_divide = 0x0;
    timer_config_struct.clock_divide = 0x0;
    timer_config_struct.count_mode = TIM_COUNT_PATTERN_UP;  
    tim_timer_config(TIM1, &timer_config_struct);

    timer_compare_struct.time_mode = TIM_CHxOCMSEL_PWM1;
    timer_compare_struct.output_state = TIM_CHx_OUTPUT_ENABLE;
    timer_compare_struct.timer_pulse = 0x01;
    tim_ch4oc_init(TIM1,&timer_compare_struct);

    tim_enable_ctrl(TIM1,ENABLE);
    tim_pwm_output_set(TIM1,ENABLE);
//  tim_trigger_output_select(TIM1, TIM_TRG_OUTPUT_SEL_UPDATE);  
 
}
    

/*
*********************************************************************************************************
*	函 数 名: StartAdcTrigTimer
*	功能说明: 启动TIM1外设工作
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartAdcTrigTimer(void)
{
	tim_enable_ctrl(TIM1,ENABLE);
}



/*
*********************************************************************************************************
*	函 数 名: StopAdcTrigTimer
*	功能说明: 停止TIM1外设工作
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StopAdcTrigTimer(void)
{
	tim_enable_ctrl(TIM1,DISABLE);
}


