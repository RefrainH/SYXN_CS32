#ifndef __BREAKERIO_H__
#define __BREAKERIO_H__




void TkOn(void);												/* PB13-BREAK引脚输出高电平完成脱扣动作 */
void TkOff(void);												/* PB13-BREAK引脚输出低电平，磁通不脱扣 */

void cs_start_power_on(void);									/* PB14引脚输出低电平，自生电源电路由硬件控制正常工作 */
void cs_start_power_off(void);									/* PB14引脚输出高电平，屏蔽自生电源电路 */
	
void cs_start_gpio_state_init(void);							/* 配置外部试跳口GPIO */

uint8_t GetSwitchIoState(void);									/* 读取state引脚电平 */

void LedRedOn(void);											/* 点亮-错误状态指示灯 */
void LedYellowOn(void);											/* 点亮-告警状态指示灯 */
void LedGreenOn(void);											/* 点亮-mcu运行状态指示灯 */
void LedRedOff(void);											/* 熄灭-错误状态指示灯 */
void LedYellowOff(void);										/* 熄灭-告警状态指示灯 */
void LedGreenOff(void);											/* 熄灭-mcu运行状态指示灯 */

void LedYellowToggle(void);										/* 闪烁-闪烁-告警状态指示灯*/

#endif 
