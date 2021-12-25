/*
*********************************************************************************************************
*
*	ģ������ : BSPģ��(For CS32F030C8T6)
*	�ļ����� : bsp.c
*	��    �� : V1.0
*	˵    �� : ����Ӳ���ײ�������������ļ���ÿ��c�ļ����� #include "bsp.h" ���������е���������ģ�顣
*			   bsp = Borad surport packet �弶֧�ְ�
*	�޸ļ�¼ :
*		�汾��  ����         ����       ˵��  
*********************************************************************************************************
*/
#include "bsp.h"


/*
*********************************************************************************************************
*	                                   �궨������
*********************************************************************************************************
*/

/* �ú궨������ѡ����ʹ���ⲿ�������ڲ�������ΪMCUʱ�� */
//#define HSE_CLOCK_8M

/*  ADCʱ��Դѡ��
 *  ADC_CLOCK_PCLK_4 -- ADC��ʱ��Դ����ΪPCLK/4
 *  ADC_CLOCK_PCLK_2 -- ADC��ʱ��Դ����ΪPCLK/2
 *  ADC_CLOCK_HRC14  -- ADC��ʱ��ԴΪ�ڲ�ר��14MHzʱ��
 *  ���ע�͸ú궨�壬��ADC������������е�������ʱ��
 */
//#define ADC_CLOCK_PCLK_2				/* ADC��ʱ��������δ�㶨����������ADCʱ����ԴΪPCLK���ƺ���δ��Ч */









/*
*********************************************************************************************************
*	                                   ��������
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*	�� �� ��: bsp_Init
*	����˵��: ��ʼ�����е�Ӳ���豸���ú�������CPU�Ĵ���������ļĴ�������ʼ��һЩȫ�ֱ�����ֻ��Ҫ����һ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init(void)
{

	#ifdef HSE_CLOCK_8M
	/* ����ʱ������ */
	StartSysInit();						/* �������ø�ʱ�Ӻ�����ϵͳ������һ��ʱ��֮���ܷ� */	
	#endif

	/* ����GPIO��ʼ������ */
	StartAllGpioInit();

	/* �δ�ʱ����ʼ�� */
	cs_start_systick_config();	

	/* �������USART��ʼ������ */
	StartUsartInit();

	/* ��ӡ��ǰ��ϵͳ��������ʱ�� */
//	StartClockShow();
	
	/* ����ADC�����ö�ʱ����ʼ������ */
	StartAdcTimInit();
	
    /* ����ADC��ʼ�� */
	StartAdcInit();	
	
	/* ���忴�Ź���ʼ�� */
	StartIwdgInit();

	/* ����������������������ֵ��ʼ�� */
	MemMgrInit();
}



/*
*********************************************************************************************************
*	�� �� ��: cs_start_set_sys_init
*	����˵��: �ϵ������ϵͳ��������ʱ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartSysInit(void)
{
    /*-----------------------------------------------------------------------
		ʱ�ӵ���������: 

        System Clock source       = PLL (HSE)
        SYSCLK(Hz)                = 48000000 (CPU Clock)
        HCLK(Hz)                  = 48000000 (AHB Clock)
        PCLK(Hz)                  = 48000000 (AHB Clock) 

        APB ��ʱ����  TIM1, TIM2, TIM3 , TIM6, TIM14, TIM15, TIM16��TIM17


	----------------------------------------------------------------------- */
	rcu_def_init();										/* ������RCU�Ĵ�������Ϊȱʡֵ */
	
	__IO uint32_t status = 0;
	
	rcu_hxt_config(RCU_HXT_ON);							/* ʹ���ⲿ����HXT */
	status = rcu_hxt_stabilization_wait();				/* �ȴ�HXT���� */
	
	if(status == SUCCESS)								/* ���HXT����ɹ� */
	{
		rcu_hclk_config(RCU_HCLK_CFG_SYSCLK_DIV1);		/* ����AHBʱ�� HCLK = SYSCLK */
		rcu_pclk_config(RCU_PCLK_CFG_HCLK_DIV1);        /* ����APBʱ�� PCLK = HCLK */
		rcu_pll_config(RCU_PLL_CFG_HXT,RCU_CFG_PLLMUF6);/* ����PLLʱ��Դ����Ƶ����:PLL = HXT * 6 = 48MHz */
        rcu_pll_enable_ctrl(ENABLE);					/* ʹ��PLL */

		/* ���PLL�Ƿ�׼������ */
		while(rcu_flag_status_get(RCU_FLAG_PLL_STAB) == 0)
		{
		}
		
		rcu_sysclk_config(RCU_SYSCLK_SEL_PLL);			/* ѡ��PLL��Ϊϵͳʱ��Դ */
		
		/* �ȴ�PLL����Ϊϵͳʱ��Դ�����ɹ� */
		while((RCU->CFG & (uint32_t)RCU_CFG_SYSSS) != (uint32_t)RCU_CFG_SYSSS_PLL)
		{
		}
	}
	
    rcu_hxt_monitor_enable_ctrl(ENABLE);				/* ʹ��HXT��⹦�� */
	
	
    /*----------------------------------ADCʱ������-------------------------------------*/	
	/* ADC����˫ʱ����ṹ��CS32������ADC�ṩ������PCLK���첽����ʱ��
	 * 1. ADC ʱ�ӿ����� HRC14(ADC �첽ʱ��)���Ƕ����� PCLK ���첽ʱ�ӡ�Ҫѡ��˷������������� ADC_CLK �Ĵ����� CKSRC[1:0]λ
	 * 2. ADC ʱ�ӿ��� ADC ���߽ӿڵ� APB ʱ�����ɣ����� CKSRC[1:0]λѡ���Ƶ���ӣ�2 �� 4����Ҫѡ��˷�����ADC_CLK �Ĵ����� CKSRC[1:0]λ�����롰00����ͬ��
	 * ʱ��Դ 1 ���������ڣ�����ѡ��ʲô APB ʱ�ӷ����������Դﵽ��� ADC ʱ��Ƶ�� ��
	 * ʱ��Դ 2 �����ƹ�ʱ����ͬ�����ŵ㡣�� ADC �� TIM ������Ӧ��Ҫ��ȷ���� ADC ������£�����Ҫѡ��ʱ��Դ 2����������ʱ����֮���ͬ�������Ӵ���˲��Ĳ�ȷ���ԣ���
	 *
	 * |    ADCʱ��Դ    |    CKSRC[1:0]    |           �����¼��뿪ʼת��֮����ӳ�                |
	 *        HRC                 00                    �ӳٲ�ȷ��(����)
	 *     PCLK����Ƶ             01                    �ӳ�ȷ��(û�ж���)�ҵ���2.75��ADCʱ������ 
	 *     PCLK�ķ�Ƶ             10                    �ӳ�ȷ��(û�ж���)�ҵ���2.625��ADCʱ������
	*/

#ifdef 	ADC_CLOCK_HRC14				/* ѡ���ڲ�14MHz������ΪADCʱ��Դ */
	adc_reg_t adc_clock_t;			
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_ASYN);
	/* ��ADCʹ���ڲ�HRC14��Ϊʱ��Դ�������º���������Ҫ����У׼ */
    rcu_hrc14_calibration_adjust(0x00);					/* ����HRC14У׼����ֵ(��Χ��0��0x1F)��������Ӧ����������ѹ���¶ȶ�HRC14Ƶ�ʵ�Ӱ�죬��ǰΪĬ��ֵ���� */
    rcu_hrc14_enable_ctrl(ENABLE);						/* ʹ��HRC14(HRC14���ڲ�14MHz RC���� ��ADC�ṩ��Ƶ�Ĺ���ʱ��) */	
#endif	

#ifdef ADC_CLOCK_PCLK_4				/* ѡ��PCLK/4��ΪADCʱ��Դ */
	adc_reg_t adc_clock_t;		
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_SYN_DIV4);	
#endif

#ifdef ADC_CLOCK_PCLK_2				/* ѡ��PCLK/2��ΪADCʱ��Դ */
	adc_reg_t adc_clock_t;		
	adc_clock_select(&adc_clock_t,ADC_CLOCK_SEL_SYN_DIV2);	
#endif


    /*----------------------------------SYSTICKʱ������-------------------------------------*/	
    SysTick_Config(SystemCoreClock / 1000);		/* Setup SysTick Timer for 1 msec interrupts */

} 








/*
*********************************************************************************************************
*	�� �� ��: cs_start_clock_show
*	����˵��: ͨ�����ڴ�ӡ��ǰϵͳʱ������
*	��    �Σ���
*	�� �� ֵ: ��
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
	 /* ����Ƭ����ϵͳ���������ʱ��Ƶ�� */
	 rcu_clk_freq_get(&rcu_clock_struct);				
	 /* ��ӡ��ǰ��ʱ��Ƶ�� */
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






