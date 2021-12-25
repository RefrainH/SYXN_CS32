/*
*********************************************************************************************************
*
*	模块名称 : BSP模块(For CS32F030C8T6)
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : 这是硬件底层驱动程序的主文件。每个c文件可以 #include "bsp.h" 来包含所有的外设驱动模块。
*			   bsp = Borad surport packet 板级支持包
*	修改记录 :
*		版本号  日期         作者       说明  
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	                                   宏定义声明
*********************************************************************************************************
*/

/* 该宏定义用于选择是使用外部晶振还是内部晶振作为MCU时钟 */
//#define HSE_CLOCK_8M

/*  ADC时钟源选择
 *  ADC_CLOCK_PCLK_4 -- ADC的时钟源配置为PCLK/4
 *  ADC_CLOCK_PCLK_2 -- ADC的时钟源配置为PCLK/2
 *  ADC_CLOCK_HRC14  -- ADC的时钟源为内部专用14MHz时钟
 *  亦可注释该宏定义，在ADC外设参数配置中单独设置时钟
 */
//#define ADC_CLOCK_PCLK_2				/* ADC的时钟设置暂未搞定，单独配置ADC时钟来源为PCLK后，似乎并未起效 */









/*
*********************************************************************************************************
*	                                   函数声明
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{

	#ifdef HSE_CLOCK_8M
	/* 单板时钟配置 */
	StartSysInit();						/* 疑似配置该时钟函数后，系统在运行一段时间之后跑飞 */	
	#endif

	/* 单板GPIO初始化配置 */
	StartAllGpioInit();

	/* 滴答定时器初始化 */
	cs_start_systick_config();	

	/* 单板调试USART初始化配置 */
	StartUsartInit();

	/* 打印当前主系统及各外设时钟 */
//	StartClockShow();
	
	/* 单板ADC触发用定时器初始化配置 */
	StartAdcTimInit();
	
    /* 单板ADC初始化 */
	StartAdcInit();	
	
	/* 单板看门狗初始化 */
	StartIwdgInit();

	/* 电流互感器计算线性修正值初始化 */
	MemMgrInit();
}



/*
*********************************************************************************************************
*	函 数 名: cs_start_set_sys_init
*	功能说明: 上电后配置系统及各外设时钟
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartSysInit(void)
{
    /*-----------------------------------------------------------------------
		时钟的配置如下: 

        System Clock source       = PLL (HSE)
        SYSCLK(Hz)                = 48000000 (CPU Clock)
        HCLK(Hz)                  = 48000000 (AHB Clock)
        PCLK(Hz)                  = 48000000 (AHB Clock) 

        APB 定时器有  TIM1, TIM2, TIM3 , TIM6, TIM14, TIM15, TIM16，TIM17


	----------------------------------------------------------------------- */
	rcu_def_init();										/* 将外设RCU寄存器重置为缺省值 */
	
	__IO uint32_t status = 0;
	
	rcu_hxt_config(RCU_HXT_ON);							/* 使能外部晶振HXT */
	status = rcu_hxt_stabilization_wait();				/* 等待HXT起振 */
	
	if(status == SUCCESS)								/* 如果HXT起振成功 */
	{
		rcu_hclk_config(RCU_HCLK_CFG_SYSCLK_DIV1);		/* 设置AHB时钟 HCLK = SYSCLK */
		rcu_pclk_config(RCU_PCLK_CFG_HCLK_DIV1);        /* 设置APB时钟 PCLK = HCLK */
		rcu_pll_config(RCU_PLL_CFG_HXT,RCU_CFG_PLLMUF6);/* 配置PLL时钟源及倍频因子:PLL = HXT * 6 = 48MHz */
        rcu_pll_enable_ctrl(ENABLE);					/* 使能PLL */

		/* 检查PLL是否准备就绪 */
		while(rcu_flag_status_get(RCU_FLAG_PLL_STAB) == 0)
		{
		}
		
		rcu_sysclk_config(RCU_SYSCLK_SEL_PLL);			/* 选择PLL作为系统时钟源 */
		
		/* 等待PLL设置为系统时钟源操作成功 */
		while((RCU->CFG & (uint32_t)RCU_CFG_SYSSS) != (uint32_t)RCU_CFG_SYSSS_PLL)
		{
		}
	}
	
    rcu_hxt_monitor_enable_ctrl(ENABLE);				/* 使能HXT监测功能 */
	
	
    /*----------------------------------ADC时钟配置-------------------------------------*/	
	/* ADC具有双时钟域结构，CS32可以向ADC提供独立于PCLK的异步工作时钟
	 * 1. ADC 时钟可以是 HRC14(ADC 异步时钟)，是独立于 PCLK 的异步时钟。要选择此方案，必须重置 ADC_CLK 寄存器的 CKSRC[1:0]位
	 * 2. ADC 时钟可由 ADC 总线接口的 APB 时钟生成，根据 CKSRC[1:0]位选择分频因子（2 或 4）。要选择此方案，ADC_CLK 寄存器的 CKSRC[1:0]位必须与“00”不同。
	 * 时钟源 1 的优势在于：无论选择什么 APB 时钟方案，都可以达到最大 ADC 时钟频率 。
	 * 时钟源 2 具有绕过时钟域同步的优点。当 ADC 由 TIM 触发且应用要求精确触发 ADC 的情况下，则需要选择时钟源 2（否则，两个时钟域之间的同步会增加触发瞬间的不确定性）。
	 *
	 * |    ADC时钟源    |    CKSRC[1:0]    |           触发事件与开始转换之间的延迟                |
	 *        HRC                 00                    延迟不确定(抖动)
	 *     PCLK二分频             01                    延迟确定(没有抖动)且等于2.75倍ADC时钟周期 
	 *     PCLK四分频             10                    延迟确定(没有抖动)且等于2.625倍ADC时钟周期
	*/

#ifdef 	ADC_CLOCK_HRC14				/* 选择内部14MHz振荡器作为ADC时钟源 */
	adc_reg_t adc_clock_t;			
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_ASYN);
	/* 若ADC使用内部HRC14作为时钟源，则以下函数可能需要调试校准 */
    rcu_hrc14_calibration_adjust(0x00);					/* 调整HRC14校准补偿值(范围是0到0x1F)，用来适应调整调整电压和温度对HRC14频率的影响，当前为默认值设置 */
    rcu_hrc14_enable_ctrl(ENABLE);						/* 使能HRC14(HRC14是内部14MHz RC振荡器 向ADC提供高频的工作时钟) */	
#endif	

#ifdef ADC_CLOCK_PCLK_4				/* 选择PCLK/4作为ADC时钟源 */
	adc_reg_t adc_clock_t;		
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_SYN_DIV4);	
#endif

#ifdef ADC_CLOCK_PCLK_2				/* 选择PCLK/2作为ADC时钟源 */
	adc_reg_t adc_clock_t;		
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_SYN_DIV2);	
#endif


    /*----------------------------------SYSTICK时钟配置-------------------------------------*/	
    SysTick_Config(SystemCoreClock / 1000);		/* Setup SysTick Timer for 1 msec interrupts */

} 








/*
*********************************************************************************************************
*	函 数 名: cs_start_clock_show
*	功能说明: 通过串口打印当前系统时钟配置
*	形    参：无
*	返 回 值: 无
*   uint32_t system_clk_freq;  //!< System clock frequency
*   uint32_t hclk_freq;        //!< HCLK frequency
*   uint32_t pclk_freq;        //!< PCLK frequency
*   uint32_t adc_clk_freq;     //!< Adc clock frequency
*   uint32_t cec_clk_freq;     //!< Cec clock frequency
*   uint32_t i2c1_clk_freq;    //!< I2C1 clock frequency
*   uint32_t usart1_clk_freq;  //!< Usart1 clock frequency
*   uint32_t usart2_clk_freq;  //!< Usart2 clock frequency
*   uint32_t usart3_clk_freq;  //!< Usart3 clock frequency
*   uint32_t usb_clk_freq;     //!< Usb clock frequency
*********************************************************************************************************
*/
void StartClockShow(void)
{
	 uint32_t clock_source_now;
	
	 rcu_clock_t rcu_clock_struct;
	 /* 返回片上主系统及各外设的时钟频率 */
	 rcu_clk_freq_get(&rcu_clock_struct);				
	 /* 打印当前各时钟频率 */
	 printf("System clock = %d\r\n",rcu_clock_struct.system_clk_freq);
	 printf("Hclk_freq = %d\r\n",rcu_clock_struct.hclk_freq);
	 printf("Pclk_freq = %d\r\n",rcu_clock_struct.pclk_freq);
	 printf("Adc_clk_freq = %d\r\n",rcu_clock_struct.adc_clk_freq);	
	 printf("Cec_clk_freq = %d\r\n",rcu_clock_struct.cec_clk_freq);		
	 printf("I2c1_clk_freq = %d\r\n",rcu_clock_struct.i2c1_clk_freq);	
	 printf("Usart1_clk_freq = %d\r\n",rcu_clock_struct.usart1_clk_freq);
	 printf("Usart2_clk_freq = %d\r\n",rcu_clock_struct.usart2_clk_freq);
	 printf("Usart3_clk_freq = %d\r\n",rcu_clock_struct.usart3_clk_freq);
	 printf("Usb_clk_freq = %d\r\n",rcu_clock_struct.usb_clk_freq);	
	
	 clock_source_now = rcu_sysclk_switch_get();	
	 switch(clock_source_now)
	 {
	 case 0x08:
	 printf("sysclock source = PLL\r\n"); break;
	 case 0x00:
	 printf("sysclock source = HRC\r\n"); break;	
	 case 0x04:
	 printf("sysclock source = HXT\r\n"); break;	 
	 }
}






