/* USER CODE BEGIN Header */
/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */
/* USER CODE END Header */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/* Section where include file can be added */
/* USER CODE END Includes */

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
  #include <stdint.h>
  extern uint32_t SystemCoreClock;
  void xPortSysTickHandler(void);
#endif
#define configUSE_PREEMPTION                     1					        	/* 使能抢占式调度器 */
#define configSUPPORT_STATIC_ALLOCATION          1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_IDLE_HOOK                      0					         	/* 使能空闲任务的钩子函数 */
#define configUSE_TICK_HOOK                      1					        	/* 使能滴答定时器中断里面执行的钩子函数 */
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )	/* 系统主频48MHz */
#define configTICK_RATE_HZ                       ((TickType_t)1000)		/* 系统时钟节拍1KHz，即1ms */
#define configMAX_PRIORITIES                     ( 7 )				      	/* 定义可供用户使用的最大优先级数，优先级号位0.1.2.3.4.5.6 */
#define configMINIMAL_STACK_SIZE                 ((uint16_t)128)	  	/* 用于定义空闲任务的栈空间的大小，单位字，即4字节，128*4=512B */
#define configTOTAL_HEAP_SIZE                    ((size_t)1536)		  	/* 定义堆大小，FreeRTOS内核，用户动态内存申请，任务栈等都需要用这个空间 */
#define configMAX_TASK_NAME_LEN                  ( 16 )				      	/* 定义任务名最大的字符数，末尾的结束符'\0'也要计算在内 */
#define configUSE_16_BIT_TICKS                   0						        /* 在未使能该宏定义的情况下，TickType_t定义的是32位无符号数 */
#define configUSE_MUTEXES                        1					        	/* 使能互斥信号量 */
#define configQUEUE_REGISTRY_SIZE                8					        	/* 通过此定义来设置可以注册的信号量和消息队列个数 */

/* HC补充 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0					        	/* 此配置用于优化优先级列表中要执行的最高优先级任务的算法，配置为1时编译会出现很多警告 */
#define configUSE_TIMERS                         1					        	/* 使能软件定时器 */
#define configTIMER_TASK_PRIORITY                ( 2 )			      		/* 配置软件定时器任务的优先级 */
#define configTIMER_QUEUE_LENGTH                 10						        /* 配置软件定时器命令队列的长度 */
#define configTIMER_TASK_STACK_DEPTH             256					        /* 配置软件定时器任务的栈空间的大小*/



/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0					         	/* 配置为1，使能合作式调度相关函数 */
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )					      /* 此参数用于定义可供用户使用的最大的合作式任务优先级数 */

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet            1							
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       0
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             0
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetSchedulerState      1

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
/* USER CODE BEGIN 1 */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}
/* USER CODE END 1 */

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
//#define xPortPendSVHandler PendSV_Handler

#define xPortPendSVHandler PendSV_Handler


/* IMPORTANT: This define is commented when used with STM32Cube firmware, when the timebase source is SysTick,
              to prevent overwriting SysTick_Handler defined within STM32Cube HAL */

/* #define xPortSysTickHandler SysTick_Handler */

/* USER CODE BEGIN Defines */
/* Section where parameter definitions can be added (for instance, to override default ones in FreeRTOS.h) */
/* USER CODE END Defines */

#endif /* FREERTOS_CONFIG_H */
