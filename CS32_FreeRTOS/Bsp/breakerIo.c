#include "bsp.h"



/*
*********************************************************************************************************
*	�� �� ��: TkOn
*	����˵��: PB13-BREAK��������ߵ�ƽ����ѿ۶���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TkOn(void)
{
    gpio_bits_set(GPIOB,TK_CONTROL_PIN);	
}

/*
*********************************************************************************************************
*	�� �� ��: TkOff
*	����˵��: PB13-BREAK��������͵�ƽ����ͨ���ѿ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TkOff(void)
{
    gpio_bits_reset(GPIOB,TK_CONTROL_PIN);	
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_power_on
*	����˵��: PB14��������͵�ƽ��������Դ��·��Ӳ��������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void cs_start_power_on(void)
{
    gpio_bits_reset(GPIOB,POWER_CONTROL_PIN);	
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_tk_off
*	����˵��: PB14��������ߵ�ƽ������������Դ��·
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void cs_start_power_off(void) 
{
    gpio_bits_set(GPIOB,POWER_CONTROL_PIN);	
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_gpio_read_state
*	����˵��: ��ȡstate���ŵ�ƽ 
*	��    ��: ��
*	�� �� ֵ: 0  ----  �ⲿ�����ź���Ч
*             1  ----  �ⲿ�����ź���Ч
*********************************************************************************************************
*/
uint8_t GetSwitchIoState(void)
{
	#if(DEV_TYPE <= DEV_TYPE_250A)
	return gpio_input_data_bit_read(GPIOB,STATE_DETECT_PIN);
	#else
	return !gpio_input_data_bit_read(GPIOB,STATE_DETECT_PIN);
	#endif 
}



/*
*********************************************************************************************************
*	�� �� ��: LedRedOn
*	����˵��: ����-����״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedRedOn(void)
{
    gpio_bits_set(GPIOB,LED_RED_Error);
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_led_alarm_on
*	����˵��: ����-�澯״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedYellowOn(void)
{
    gpio_bits_set(GPIOB,LED_YELLOW_Alarm);
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_mcu_alarm_on
*	����˵��: ����-mcu����״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedGreenOn(void)
{
    gpio_bits_set(GPIOB,LED_GREEN_Work);
}





/*
*********************************************************************************************************
*	�� �� ��: cs_start_led_error_on
*	����˵��: Ϩ��-����״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedRedOff(void)
{
    gpio_bits_reset(GPIOB,LED_RED_Error);
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_led_alarm_on
*	����˵��: Ϩ��-�澯״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedYellowOff(void)
{
    gpio_bits_reset(GPIOB,LED_YELLOW_Alarm);
}

/*
*********************************************************************************************************
*	�� �� ��: cs_start_mcu_alarm_on
*	����˵��: Ϩ��-mcu����״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedGreenOff(void)
{
    gpio_bits_reset(GPIOB,LED_GREEN_Work);
}


/*
*********************************************************************************************************
*	�� �� ��: LedYellowToggle
*	����˵��: ��˸-�澯״ָ̬ʾ��-��ɫ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LedYellowToggle(void)
{
    GPIOB->DO ^= LED_YELLOW_Alarm;	
}
