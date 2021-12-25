/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : tim.c
*	��    �� : V1.0
*	˵    �� : ����TIM3��ΪADC�Ĵ���Դ
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	�� �� ��: StartAdcTimInit
*	����˵��: ADC_TIM�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

    /*-----------------------------------------------------------------------
		ʱ�ӵ���������: 

        System Clock source       = PLL (HSE)
        SYSCLK(Hz)                = 48000000 (CPU Clock)
        HCLK(Hz)                  = 48000000 (AHB Clock)
        PCLK(Hz)                  = 48000000 (AHB Clock) 

        APB ��ʱ����  TIM1, TIM2, TIM3 , TIM6, TIM14, TIM15, TIM16��TIM17
		TimeOut = ((Prescaler + 1) * (Period + 1)) / TimeClockFren
		
		TimeOut:��ʱ�����ʱ��(��λΪus)�����ٴ���(����)һ��TIM�ж�
		Prescaler:��ƵTIMʱ�ӵ�Ԥ��Ƶ��ֵ 
		Period:��������ֵ��TIM�����������ֵ�����¼���
		TimeClockFren:��ʱ��������ʱ��Ƶ��(��λMHz)
	----------------------------------------------------------------------- */

void StartAdcTimInit(void)
{
    timer_config_t  timer_config_struct;
    timer_compare_t timer_compare_struct; 

    /* ����TIM3ʱ�� */
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_TIM1, ENABLE);

	/* ȱʡTIM1�Ĵ�����ֵ */
    tim_def_init(TIM1);  
    tim_config_struct_init(&timer_config_struct);  
    tim_compare_struct_init(&timer_compare_struct);

	/* TimeOut = ((Prescaler + 1) * (Period + 1)) / TimeClockFren = (15000 * 1) / 48000000 = 0.3125ms */
    timer_config_struct.time_period = 30000-1;				//15000-1 -> 64��������    60000-1 -> 16��������
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
*	�� �� ��: StartAdcTrigTimer
*	����˵��: ����TIM1���蹤��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartAdcTrigTimer(void)
{
	tim_enable_ctrl(TIM1,ENABLE);
}



/*
*********************************************************************************************************
*	�� �� ��: StopAdcTrigTimer
*	����˵��: ֹͣTIM1���蹤��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StopAdcTrigTimer(void)
{
	tim_enable_ctrl(TIM1,DISABLE);
}


