#include "bsp.h"

/*
*********************************************************************************************************
*
*	ģ������ : ����GPIO����ģ��
*	�ļ����� : button.c
*	��    �� : V1.0
*	˵    �� : ��ʼ������GPIO
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��

*********************************************************************************************************
*/



/*
*********************************************************************************************************
*	�� �� ��: StartLedGpioInit
*	����˵��: ��ʼ��LED�Ƶ�GPIO��Ϩ������LED
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/

void StartLedGpioInit(void)
{
    gpio_config_t  gpio_config_struct;
	/* ��GPIOʱ�� */
    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB, ENABLE); 
	
	/* �����������
     * PB3  -->  LED1(Error)
	 * PB4  -->  LED2(Alarm)
	 * PB5  -->  LED3(MCU)
	*/
    gpio_config_struct.gpio_pin = LED_RED_Error | LED_YELLOW_Alarm | LED_GREEN_Work; 
	/* ����GPIOΪ������� */
    gpio_config_struct.gpio_mode = GPIO_MODE_OUTPUT;
    gpio_config_struct.gpio_speed = GPIO_SPEED_HIGH;  
    gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
	/* ����GPIO�������� */
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;	
	/* ��ʼ��GPIO */
    gpio_init(GPIOB, &gpio_config_struct);
	/* ��ʼ����ɺ�����LEDϨ�� */
    gpio_bits_reset(GPIOB,LED_RED_Error | LED_YELLOW_Alarm | LED_GREEN_Work);	
}

/*
*********************************************************************************************************
*	�� �� ��: StartStateGpioInit
*	����˵��: �����ⲿ������GPIO���ú�����bsp_Init����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartStateGpioInit(void)
{
	gpio_config_t  gpio_config_struct;
	
	/* ��GPIOB��ʱ�� */
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);	

	/* PB15 --> STATE:����ͨ״̬ */
	gpio_config_struct.gpio_mode = GPIO_MODE_INPUT;				/* ����ģʽ */
	gpio_config_struct.gpio_pin = STATE_DETECT_PIN;
	gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;			/* �ⲿ��·���������裬�����������ó���������ģʽ */
	gpio_init(GPIOB,&gpio_config_struct);						/* ��ʼ��GPIO���� */
}

/*
*********************************************************************************************************
*	�� �� ��: StartControlGpioInit
*	����˵��: ���ö�����Ƶ�GPIO���ú�����bsp_Init����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartControlGpioInit(void)
{
	gpio_config_t  gpio_config_struct;
	
	/* ��GPIOB��ʱ�� */
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);	
	
	/*
	 *  PB13 --> BREAK: ��ͨ�ѿۿ���
	 *  PB14 --> ������Դ��·��������
	 */
	gpio_config_struct.gpio_pin = TK_CONTROL_PIN | POWER_CONTROL_PIN;
	gpio_config_struct.gpio_mode = GPIO_MODE_OUTPUT;				/* ������� */
	gpio_config_struct.gpio_out_type = GPIO_OUTPUT_PP;
	gpio_config_struct.gpio_speed = GPIO_SPEED_HIGH;
	gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL;				/* ������������ */
	gpio_init(GPIOB,&gpio_config_struct);							/* ��ʼ��GPIO���� */	
    gpio_bits_reset(GPIOB,TK_CONTROL_PIN | POWER_CONTROL_PIN);		
}

/*
*********************************************************************************************************
*	�� �� ��: StartAllGpioInit
*	����˵��: �������gpio��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartAllGpioInit(void)
{
	StartLedGpioInit();
	StartStateGpioInit();
	StartControlGpioInit();
}


