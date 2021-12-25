#include "bsp.h"



/*
*********************************************************************************************************
*	函 数 名: StartIwdgInit
*	功能说明: 初始化独立看门狗
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartIwdgInit(void)
{
	/* 使能对寄存器FWDT_PDIV的写操作 */
    fwdt_write_access_enable_ctrl(FWDT_WRITE_ACCESS_ENABLE);  
	
	/* 设置FWDT预分频值：LRC/64 */
    fwdt_prescaler_set(FWDT_PRESCALER_64);   
    /*
    Counter Reload Value = 250ms/IWDG counter clock period
                         = 250ms / (LRC/32)
                         = 0.25s / (LRC_Freq/32)
                         = LRC_Freq/(32 * 4)
                         = LRC_Freq/128
    */
	/* 设置FWDT重加载值，范围是0 ~ 0xFFF */
    fwdt_reload_set(4095);
	
	/* 按照FWDT重装载寄存器的值重装载FWDT计数器 */
    fwdt_reload_counter();
	
	/* 使能FWDT */
    fwdt_enable();	
}



/*
*********************************************************************************************************
*	函 数 名: IwdgFeed
*	功能说明: 喂狗
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void IwdgFeed(void)
{
	fwdt_reload_counter();
}

