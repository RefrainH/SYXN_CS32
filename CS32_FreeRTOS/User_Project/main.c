#include "bsp.h"


/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
void MX_FREERTOS_Init(void);



/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/


int main(void)
{  
	
	/* ����Ӳ����ʼ�� */
	bsp_Init();

	/* ���߳������ʼ�� */
	MX_FREERTOS_Init();	

	/* ���������� */
	osKernelStart();
	
	
    while (1) 
    {

    }
}








