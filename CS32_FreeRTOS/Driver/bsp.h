#ifndef __BSP_H__
#define __BSP_H__

/*
*********************************************************************************************************
*                                         ��׼��
*********************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*********************************************************************************************************
*                                         CS32��
*********************************************************************************************************
*/
#include "cs32f0xx.h"
#include "cs32f0xx_conf.h"
#include "cmsis_os.h"
/*
*********************************************************************************************************
*                                           OS
*********************************************************************************************************
*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"


/*
*********************************************************************************************************
*                                           BSP������
*********************************************************************************************************
*/
#include "about.h"
#include "systick.h"
#include "usart.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"
#include "iwdg.h"

/* Bsp */
#include "breakerIo.h"
#include "breakerAdc.h"
#include "calibMeterMem.h"
#include "iwdg.h"


/* App */
#include "currProtector.h"
#include "log.h"
#include "usrLib.h"
#include "breaker.h"
#include "watchdogMonitor.h"
#include "memMgr.h"












#define DEBUG_PRINT 					1				/* ���Խ׶δ�ӡ��Ϣ */

#define ADC1_OUTDAT_REG_ADDRESS			0x40012440		/* ADC�����ַ����DMA��ʼ���������� */



#define RTC_PER_MIN_NTF  		   (1 << 1)
#define PWR_DROP_DOWN_NTF		   (1 << 2)
#define PWR_OFF_NTF				   (1 << 3)				
#define PWR_ON_NTF				   (1 << 4)		
#define RS485_COMM_END			   (1 << 5)	
#define SWITCH_CHG_ON2OFF		   (1 << 6)
#define SWITCH_CHG_OFF2ON		   (1 << 7)
#define RTC_PER_SEC_NTF  		   (1 << 8)



/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);									/* ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ����� */
void StartSysInit(void);								/* �ϵ������ϵͳ��������ʱ�� */
void StartClockShow(void);								/* ͨ�����ڴ�ӡ��ǰϵͳʱ������ */




#endif 


