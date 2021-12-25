#ifndef __GPIO_H__
#define __GPIO_H__


/* 定义电位器GPIO */
#define BUTTON_0						GPIO_PIN_0
#define BUTTON_1						GPIO_PIN_1
#define BUTTON_2						GPIO_PIN_2
#define BUTTON_3						GPIO_PIN_3
#define BUTTON_4						GPIO_PIN_4
#define BUTTON_5						GPIO_PIN_5

/* 定义LED用GPIO */
#define LED_RED_Error					GPIO_PIN_3				/* 错误状态指示灯-红 */
#define LED_YELLOW_Alarm				GPIO_PIN_4				/* 警告状态指示灯-黄 */
#define LED_GREEN_Work					GPIO_PIN_5				/* 运行状态指示灯-绿 */

/* 定义电流检测GPIO */
#define CURR_DETECT_C					GPIO_PIN_7
#define CURR_DETECT_B					GPIO_PIN_0
#define CURR_DETECT_A					GPIO_PIN_1

/* 定义输出控制GPIO */
#define TK_CONTROL_PIN					GPIO_PIN_13	    		/* 磁通脱扣引脚 */
#define POWER_CONTROL_PIN				GPIO_PIN_14				/* 自生电源控制引脚 */

/* 定义外部检测GPIO */
#define STATE_DETECT_PIN    			GPIO_PIN_15				/* 外部试跳电路检测引脚 */


void StartLedGpioInit(void);									/* 初始化LED灯的GPIO,熄灭所有LED,该函数被bsp_Init调用 */
void StartStateGpioInit(void);									/* 配置外部试跳口GPIO,该函数被bsp_Init调用 */
void StartControlGpioInit(void);								/* 配置对外控制的GPIO,该函数被bsp_Init调用 */
void StartAllGpioInit(void);									/* 完成所有gpio初始化功能,该函数被bsp_Init调用 */
#endif 
