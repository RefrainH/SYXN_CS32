#ifndef __GPIO_H__
#define __GPIO_H__


/* �����λ��GPIO */
#define BUTTON_0						GPIO_PIN_0
#define BUTTON_1						GPIO_PIN_1
#define BUTTON_2						GPIO_PIN_2
#define BUTTON_3						GPIO_PIN_3
#define BUTTON_4						GPIO_PIN_4
#define BUTTON_5						GPIO_PIN_5

/* ����LED��GPIO */
#define LED_RED_Error					GPIO_PIN_3				/* ����״ָ̬ʾ��-�� */
#define LED_YELLOW_Alarm				GPIO_PIN_4				/* ����״ָ̬ʾ��-�� */
#define LED_GREEN_Work					GPIO_PIN_5				/* ����״ָ̬ʾ��-�� */

/* ����������GPIO */
#define CURR_DETECT_C					GPIO_PIN_7
#define CURR_DETECT_B					GPIO_PIN_0
#define CURR_DETECT_A					GPIO_PIN_1

/* �����������GPIO */
#define TK_CONTROL_PIN					GPIO_PIN_13	    		/* ��ͨ�ѿ����� */
#define POWER_CONTROL_PIN				GPIO_PIN_14				/* ������Դ�������� */

/* �����ⲿ���GPIO */
#define STATE_DETECT_PIN    			GPIO_PIN_15				/* �ⲿ������·������� */


void StartLedGpioInit(void);									/* ��ʼ��LED�Ƶ�GPIO,Ϩ������LED,�ú�����bsp_Init���� */
void StartStateGpioInit(void);									/* �����ⲿ������GPIO,�ú�����bsp_Init���� */
void StartControlGpioInit(void);								/* ���ö�����Ƶ�GPIO,�ú�����bsp_Init���� */
void StartAllGpioInit(void);									/* �������gpio��ʼ������,�ú�����bsp_Init���� */
#endif 
