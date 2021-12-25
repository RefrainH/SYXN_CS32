#ifndef __BREAKERIO_H__
#define __BREAKERIO_H__




void TkOn(void);												/* PB13-BREAK��������ߵ�ƽ����ѿ۶��� */
void TkOff(void);												/* PB13-BREAK��������͵�ƽ����ͨ���ѿ� */

void cs_start_power_on(void);									/* PB14��������͵�ƽ��������Դ��·��Ӳ�������������� */
void cs_start_power_off(void);									/* PB14��������ߵ�ƽ������������Դ��· */
	
void cs_start_gpio_state_init(void);							/* �����ⲿ������GPIO */

uint8_t GetSwitchIoState(void);									/* ��ȡstate���ŵ�ƽ */

void LedRedOn(void);											/* ����-����״ָ̬ʾ�� */
void LedYellowOn(void);											/* ����-�澯״ָ̬ʾ�� */
void LedGreenOn(void);											/* ����-mcu����״ָ̬ʾ�� */
void LedRedOff(void);											/* Ϩ��-����״ָ̬ʾ�� */
void LedYellowOff(void);										/* Ϩ��-�澯״ָ̬ʾ�� */
void LedGreenOff(void);											/* Ϩ��-mcu����״ָ̬ʾ�� */

void LedYellowToggle(void);										/* ��˸-��˸-�澯״ָ̬ʾ��*/

#endif 
