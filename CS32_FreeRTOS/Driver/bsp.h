#ifndef __BSP_H__
#define __BSP_H__

/*
*********************************************************************************************************
*                                         标准库
*********************************************************************************************************
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
*********************************************************************************************************
*                                         CS32库
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
*                                           BSP驱动库
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












#define DEBUG_PRINT 					1				/* 调试阶段打印信息 */

#define ADC1_OUTDAT_REG_ADDRESS			0x40012440		/* ADC外设地址，被DMA初始化函数调用 */



#define RTC_PER_MIN_NTF  		   (1 << 1)
#define PWR_DROP_DOWN_NTF		   (1 << 2)
#define PWR_OFF_NTF				   (1 << 3)				
#define PWR_ON_NTF				   (1 << 4)		
#define RS485_COMM_END			   (1 << 5)	
#define SWITCH_CHG_ON2OFF		   (1 << 6)
#define SWITCH_CHG_OFF2ON		   (1 << 7)
#define RTC_PER_SEC_NTF  		   (1 << 8)



/* 提供给其他C文件调用的函数 */
void bsp_Init(void);									/* 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。 */
void StartSysInit(void);								/* 上电后配置系统及各外设时钟 */
void StartClockShow(void);								/* 通过串口打印当前系统时钟配置 */




#endif 


