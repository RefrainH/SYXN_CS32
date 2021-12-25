#include "bsp.h"



/*
*********************************************************************************************************
*	�� �� ��: StartIwdgInit
*	����˵��: ��ʼ���������Ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartIwdgInit(void)
{
	/* ʹ�ܶԼĴ���FWDT_PDIV��д���� */
    fwdt_write_access_enable_ctrl(FWDT_WRITE_ACCESS_ENABLE);  
	
	/* ����FWDTԤ��Ƶֵ��LRC/64 */
    fwdt_prescaler_set(FWDT_PRESCALER_64);   
    /*
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LRC/32)
                         = 0.25s / (LRC_Freq/32)
                         = LRC_Freq/(32 * 4)
                         = LRC_Freq/128
    */
	/* ����FWDT�ؼ���ֵ����Χ��0 ~ 0xFFF */
    fwdt_reload_set(4095);
	
	/* ����FWDT��װ�ؼĴ�����ֵ��װ��FWDT������ */
    fwdt_reload_counter();
	
	/* ʹ��FWDT */
    fwdt_enable();	
}



/*
*********************************************************************************************************
*	�� �� ��: IwdgFeed
*	����˵��: ι��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void IwdgFeed(void)
{
	fwdt_reload_counter();
}
