#include "bsp.h"
#include "cs32f0xx_int.h"

/*
*********************************************************************************************************
*	                                   宏定义声明
*********************************************************************************************************
*/

#define NPT								32		//64					/* 定义每20ms内，ADC各通道的采样点数 */
#define DC_VOL							100
#define A   							330
#define FS								(PHASE_FREQ*NPT)	
#define ADC_SAMPLE_POINTS				(NPT/PHASE_PERIOD_WINDOW_DIV)

#define CURR_MODE_SMALL_THRESHOLD		95
#define CURR_MODE_BIG_THRESHOLD			105
#define CURR_MODE_FORCE_BIG_VOL			130 //<130 force big

#define IN_MODE_THRESHOLD				150

/*
*********************************************************************************************************
*	                                   变量声明
*********************************************************************************************************
*/
__IO int16_t adcVals[ADC_SAMPLE_POINTS][ADC_CHANLS_NUM] = {0};

uint32_t adcValsFftIn[ADC_CHANLS_NUM][NPT] = {0};						/* 转换过后的实际存放ADC原始值的数组 */

uint32_t ButtonAdcValue0;												/* 电位器ADC原始值 */
uint32_t ButtonAdcValue1;
uint32_t ButtonAdcValue2;
uint32_t ButtonAdcValue3;
uint32_t ButtonAdcValue4;
uint32_t ButtonAdcValue5;	

uint8_t S1_VAL;															/* 电位器实际键值 */
uint8_t S2_VAL;
uint8_t S3_VAL;
uint8_t S4_VAL;
uint8_t S5_VAL;
uint8_t S6_VAL;


BreakerFftDef	BreakerFft;
BreakerParaInfoDef	breakerParaInfo;

volatile uint8_t ADC_DMA_TRANSFER = 0;									/* DMA1中断标志位 */

/*
*********************************************************************************************************
*	                                   函数声明
*********************************************************************************************************
*/
//void cr4_fft_64_stm32(void *pssOUT, void *pssIN, uint16_t Nbin);


/*
*********************************************************************************************************
*	                                   RTOS相关声明
*********************************************************************************************************
*/
osSemaphoreId BinarySemAdcConvCpltHandle;						/* 创建一个信号量 */



/*
*********************************************************************************************************
*	函 数 名: StartAdcInit
*	功能说明: ADC外设初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartAdcInit(void)
{
	uint32_t ADC_CALB = 0;
    adc_config_t   adc_config_struct;
    gpio_config_t  gpio_config_struct;
	
	/* 	CHANNEL | GPIO引脚	|	  ADC Channel	  | 		功能			|	ADC通道数据转换时间：tCONV = 采样时间 + ( 12.5 * ADC时钟周期 )   当前ADC时钟为14MHz	
	 *		0		PA0				ADC_IN0			 电位器采样通道0			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	1		PA1				ADC_IN1			 电位器采样通道1			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	2		PA2  			ADC_IN2			 电位器采样通道2			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	3		PA3				ADC_IN3			 电位器采样通道3			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	4		PA4				ADC_IN4			 电位器采样通道4			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	5		PA5				ADC_IN5			 电位器采样通道5 			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	6		PA6				ADC_IN6			 自生电源监控采样			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	7		PA7				ADC_IN7			 C相电流采样				(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	8		PB0				ADC_IN8			 B相电流采样				(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	9		PB1             ADC_IN9          A相电流采样				(28.5 + 12.5) * 1/14M = 2.9uS
	 */
	
	/* 打开ADC和GPIO时钟 */
    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTA, ENABLE); /* PA0~PA7 */   
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);  /* PB0,PB1 */
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_ADC, ENABLE); 

    /* 电位器采样GPIO初始化 */
    gpio_config_struct.gpio_pin = BUTTON_0 | BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4 | BUTTON_5 |CURR_DETECT_C  ;
    gpio_config_struct.gpio_mode = GPIO_MODE_AN;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL ;
    gpio_init(GPIOA, &gpio_config_struct);	
	
	/* 电流采样GPIO初始化 */
    gpio_config_struct.gpio_pin = CURR_DETECT_A | CURR_DETECT_B;
    gpio_config_struct.gpio_mode = GPIO_MODE_AN;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL ;
    gpio_init(GPIOB, &gpio_config_struct);
 
	/* 将外设ADC1的全部寄存器重设为缺省值 */
    adc_def_init(ADC1);   
    adc_config_struct_init(&adc_config_struct);  

    /* 配置ADC1外设的工作模式 */ 
    adc_config_struct.adc_resolution = ADC_CONV_RES_12BITS;				  		/* 转换数据分辨率为12位 */
    adc_config_struct.conversion_mode = DISABLE; 										    /* 使能ADC转换 */
    adc_config_struct.trigger_mode = ADC_TRIG_MODE_SEL_RISING;					/* 硬件上升沿触发检测 */
	adc_config_struct.hardware_trigger =  ADC_HW_TRIG_SEL_T1_CH4CC;  			/* 使用TIM1定时触发 */ 	
    adc_config_struct.data_align = ADC_DATA_ALIGN_RIGHT;							  /* 转换数据右对齐 */
    adc_config_struct.scan_direction = ADC_CONV_SEQ_DIR_UPWARD;					/* 向前扫描:正向扫描通道0到通道18 */
    adc_init(ADC1, &adc_config_struct); 															  /* 初始化ADC1 */

    /* 配置ADC1各通道的采样时间 */ 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_0 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_1 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_2 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_3 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_4 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_5 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_6 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_7 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_8 , ADC_SAMPLE_TIMES_28_5); 
    adc_channel_config(ADC1, ADC_CONV_CHANNEL_9 , ADC_SAMPLE_TIMES_28_5); 	

	/* ADC自校准 */
    ADC_CALB = adc_calibration_value_get(ADC1); 								/* 在ADC进行校准时，应用程序不得使用ADC，必须等待校准完成 */
																				/* 每次ADC掉电后，校准系数都会丢失 */
	if(ADC_CALB == 0)
	{
	 	/* 打印出错的源代码文件名、函数名称 */
		printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		printf("ADC calibration wrong!\r\n");	
	}
	else
	{
		/* 打印自校准系数   */
		printf("ADC calibration value = 0x%x \r\n",ADC_CALB);
	}
	
	/* 使能ADC的DMA请求，设置DMA为循环模式 */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	adc_dma_mode_set(ADC1, ADC_DMA_MODE_CIRCULAR); 

	/* 使能ADC外设 */ 
	adc_dma_enable_ctrl(ADC1, ENABLE); 	
    adc_enable_ctrl(ADC1, ENABLE);     

	/* ADC_FLAG_EOI：ADC初始化结束标志 */
    while(!adc_flag_status_get(ADC1, ADC_FLAG_EOI));
	
	/* 初始化DMA1 */
	StartAdcDmaInit();	
	
}

/*
*********************************************************************************************************
*	函 数 名: StartAdcDmaInit
*	功能说明: ADC_DMA外设初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartAdcDmaInit(void)
{
	dma_config_t  dma_configStruct;
	nvic_config_t nvic_config_struct;
	/* 开启DMA时钟 */ 
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_DMA1, ENABLE);
	
	/* 配置DMA工作模式 */
	dma_def_init(DMA1_CHANNEL1);												/* 将外设DMA的全部寄存器重设为缺省值 */	
	
	
	dma_configStruct.peri_base_addr = (uint32_t)ADC1_OUTDAT_REG_ADDRESS;		/* 配置外设寄存器地址 */
	dma_configStruct.mem_base_addr = (uint32_t)&adcVals[0][0];					/* 配置存储器地址--(uint32_t)&adcVals[0][0];	 */
	dma_configStruct.transfer_direct = DMA_TRANS_DIR_FROM_PERI;					/* 配置数据传输方向：从外设到内存 */			
	dma_configStruct.buf_size = ADC_CHANLS_NUM*ADC_SAMPLE_POINTS;				/* 配置每次传输的数据大小 */
	dma_configStruct.peri_inc_flag = DMA_PERI_INC_DISABLE;						/* 外设地址不自增 */
	dma_configStruct.mem_inc_flag = DMA_MEM_INC_ENABLE;							/* 内设地址自增 */
	dma_configStruct.peri_data_width = DMA_PERI_DATA_WIDTH_HALFWORD;			/* 配置数据宽度 */
	dma_configStruct.mem_data_width = DMA_MEM_DATA_WIDTH_HALFWORD;				/* 配置数据宽度 */
	dma_configStruct.operate_mode = DMA_OPERATE_MODE_CIRCULAR;					/* 配置DMA为循环传输模式 */
	dma_configStruct.priority_level = DMA_CHANNEL_PRIORITY_HIGH;				/* 配置该DMA通道的优先级：高 */
	dma_configStruct.m2m_flag = DMA_M2M_MODE_DISABLE;							/* 禁止DMA使用内存-内存的工作模式 */							
	dma_init(DMA1_CHANNEL1, &dma_configStruct);									/* 将配置写入DMA控制结构体 */

	
	dma_interrupt_set(DMA1_CHANNEL1,DMA_INT_CONFIG_CMP,ENABLE);					/* 使能DMA中断， 传输完成中断屏蔽*/
	
	nvic_config_struct.nvic_IRQ_channel = IRQn_DMA1_CHANNEL1;					/* 使能DMA1 channel1 IRQ Channel */
	nvic_config_struct.nvic_channel_priority = 0;
	nvic_config_struct.nvic_enable_flag = ENABLE;
	nvic_init(&nvic_config_struct);
	
	dma_enable_ctrl(DMA1_CHANNEL1, ENABLE); 									/* 使能DMA1 */
	

}


/*
*********************************************************************************************************
*	函 数 名: StartAdcConvert
*	功能说明: 启动ADC转换
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StartAdcConvert(void)
{
	/* 开启ADC转换 */
    adc_conversion_start(ADC1);
	
	/* 开启TIM1定时触发 */
	StartAdcTrigTimer();
}

/*
*********************************************************************************************************
*	函 数 名: StopAdcConvert
*	功能说明: 停止ADC转换
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void StopAdcConvert(void)
{
  /* 停止ADC转换 */
  adc_conversion_stop(ADC1);
	
  /* 停止TIM3定时触发 */	
  StopAdcTrigTimer();
}



void CountFFTParas(uint32_t *fftIn, FFTParasDef *para)
{
	para->acAn = sqrSumAverSqrt(fftIn, NPT);	
}

void CountFFTParas_I(uint32_t *fftIn, FFTParasDef *para)
{
	/* 均方根值计算，fftIn：数据地址 NPT：数据个数 */
	para->acAn = sqrSumAverSqrt(fftIn, NPT);
}

/* 该函数在源代码中未使用 */
#if 0
void CountFFTParas_Is(uint32_t *fftIn, FFTParasDef *para)
{
	uint32_t max = 0;
	uint32_t min = 0;
	uint32_t mid = 0;
	uint16_t i = 0;

	CountMaxMin32(fftIn, NPT, &max, &min);
	mid = (max+min)/2;
	for(i=0; i<NPT; i++)
	{
		fftIn[i] = abs(fftIn[i]-mid);
	}

	CountFFTParas(fftIn, para);
}
#endif

float Linearfitting(float fftAn, CalibInfoDef *para)
{
	float an = 0;

	if(para->second.fftAn == para->frist.fftAn)
	{
		an = 0;

	}
	else
	{
		an = (para->second.An) - ((para->second.fftAn-fftAn))*((para->second.An-para->frist.An))/((para->second.fftAn-para->frist.fftAn));

	}
  
	return an;	
}



/*
*********************************************************************************************************
*	函 数 名: countbreakerParaAn
*	功能说明: 将ADC采集转换后的均方根值换算成真实的电流值
*	形    参:  uint8_t idx         ： 
*			   float fftAn         ：
*			   CalibInfoDef *para  ： 
*	返 回 值: 无
*********************************************************************************************************
*/
float countbreakerParaAn(uint8_t idx, float fftAn, CalibInfoDef *para)
{
	float an = fftAn;
    float tmp = 0;
    CalibInfoDef line;

	#if (DEV_TYPE_250A == DEV_TYPE)
    if(fftAn > 1265)
    {
        line.frist.An = 3500;
        line.frist.fftAn = 1265.59f;
        line.second.An = 4000;
        line.second.fftAn = 1276.989f;
        an = Linearfitting(fftAn, &line);
    }
    else if(fftAn > 1220)
    {
        line.frist.An = 3000;
        line.frist.fftAn = 1220.622f;
        line.second.An = 3500;
        line.second.fftAn = 1265.59f;
        an = Linearfitting(fftAn, &line);			
    }
    else if(fftAn > 619)
    {
        an = 0.0000044649f*fftAn*fftAn*fftAn - 0.0083945596f*fftAn*fftAn + 7.0288443712f*fftAn - 1194.8188753816f;
    }
    else
    {
        an = Linearfitting(fftAn, para);
    }
    
	#elif (DEV_TYPE_400A == DEV_TYPE)
    if(fftAn > 380)
    {
        an = 0.0000128810f*fftAn*fftAn*fftAn - 0.0225955140f*fftAn*fftAn + 15.66945201f*fftAn - 2431.206437f;
    }
    else
    {
        an = Linearfitting(fftAn, para);
    } 

	#elif (DEV_TYPE_630A == DEV_TYPE)
    if(fftAn > 230)
    {
        an = 0.0000200147f*fftAn*fftAn*fftAn - 0.017477739f*fftAn*fftAn + 10.18431671f*fftAn - 711.9305831f;   
    }
    else
    {
        an = Linearfitting(fftAn, para);
    } 
	#else		
		
	#endif
   
	if(an < 0)
	{
		an = 0;
	}

	return an;
}

static void IabcAnCount( void )
{
	/* A相：计算A相ADC通道采集值的均方根值，均方根值存放在 BreakerFft.ia.fftPara.acAn 中*/
	CountFFTParas_I(&adcValsFftIn[IA_IDX][0], &BreakerFft.ia.fftPara);
	/* 将均方根值做累加计算 */
	BreakerFft.ia.anSum += BreakerFft.ia.fftPara.acAn;
	/* breakerParaInfo.ia.an 此参数即为经过计算过后的当前电流值，更换CS32后可能部分参数需要修改 */
	breakerParaInfo.ia.an = countbreakerParaAn(IA_IDX, BreakerFft.ia.fftPara.acAn, &(calibMeterEx.ia));
	/* 将计算值做累加计算 */
	breakerParaInfo.ia.anSum += breakerParaInfo.ia.an;
	
	/* IbL */
	CountFFTParas_I(&adcValsFftIn[IB_IDX][0], &BreakerFft.ib.fftPara);
	BreakerFft.ib.anSum += BreakerFft.ib.fftPara.acAn;
	breakerParaInfo.ib.an = countbreakerParaAn(IB_IDX, BreakerFft.ib.fftPara.acAn, &(calibMeterEx.ib));
	breakerParaInfo.ib.anSum += breakerParaInfo.ib.an;

	/* IcL */
	CountFFTParas_I(&adcValsFftIn[IC_IDX][0], &BreakerFft.ic.fftPara);
	BreakerFft.ic.anSum += BreakerFft.ic.fftPara.acAn;
	breakerParaInfo.ic.an = countbreakerParaAn(IC_IDX, BreakerFft.ic.fftPara.acAn, &(calibMeterEx.ic));
	breakerParaInfo.ic.anSum += breakerParaInfo.ic.an;	
}


static void AnAverCount( void )
{
	/* periodIdx自增 */
	breakerParaInfo.periodIdx++;
	
	if(breakerParaInfo.periodIdx>=AN_AVER_COUNT) //PHASE_FREQ
	{
	    portENTER_CRITICAL();
		/* 做各项电流参数的原始值平均值计算？ */
		BreakerFft.ia.anAver = BreakerFft.ia.anSum/AN_AVER_COUNT;
		BreakerFft.ib.anAver = BreakerFft.ib.anSum/AN_AVER_COUNT;
		BreakerFft.ic.anAver = BreakerFft.ic.anSum/AN_AVER_COUNT;
		/* 做各项电流参数的计算值平均值计算？ */
		breakerParaInfo.ia.anAver = breakerParaInfo.ia.anSum/AN_AVER_COUNT;
		breakerParaInfo.ib.anAver = breakerParaInfo.ib.anSum/AN_AVER_COUNT;
		breakerParaInfo.ic.anAver = breakerParaInfo.ic.anSum/AN_AVER_COUNT;
        portEXIT_CRITICAL();
		/* 全部清零，等待下一次计算 */
		BreakerFft.ia.anSum = 0;
		BreakerFft.ib.anSum = 0;
		BreakerFft.ic.anSum = 0;

		breakerParaInfo.ia.anSum = 0;
		breakerParaInfo.ib.anSum = 0;
		breakerParaInfo.ic.anSum = 0;
		
		breakerParaInfo.periodIdx = 0;
	}
}

static void BreakerAdcHandler(void)
{
	uint16_t channel = 0;								/* ADC通道数目 */
	uint16_t points = 0;								/* 每周期采样点数 */
	uint16_t offset = 0;
				
	
#if BREAKER_ADC_LOG
	static uint32_t sTick = 0;
    sTick = xTaskGetTickCount();
#endif
	/* adcValsFftIn[channel][points]为转换后的adc数据的存储地址，大小为adcValsFftIn[9][64] */
	for(channel = 0; channel < ADC_CHANLS_NUM; channel++)
	{
		#if (4==PHASE_PERIOD_WINDOW_DIV)
			offset = NPT-ADC_SAMPLE_POINTS;
			memmove(&adcValsFftIn[channel][0], &adcValsFftIn[channel][ADC_SAMPLE_POINTS], offset*sizeof(uint32_t) );
		#else
			offset = 0;
		#endif
		/* ADC原始值数据转换，存储方式由数组[采样个数][采样通道] -> 数组[采样通道][采样个数] */
		for(points = 0; points < ADC_SAMPLE_POINTS; points++)
		{
			adcValsFftIn[channel][points+offset] = adcVals[points][channel];
		}
	}

	IabcAnCount();
	AnAverCount();
		
	/* 存储电位器ADC值 */
	ButtonAdcValue0 = adcValsFftIn[BUTTON_0_IDX][7];
	ButtonAdcValue1 = adcValsFftIn[BUTTON_1_IDX][7];
	ButtonAdcValue2 = adcValsFftIn[BUTTON_2_IDX][7];
	ButtonAdcValue3 = adcValsFftIn[BUTTON_3_IDX][7];
	ButtonAdcValue4 = adcValsFftIn[BUTTON_4_IDX][7];
	ButtonAdcValue5 = adcValsFftIn[BUTTON_5_IDX][7];					
	/* 电位器键值档位值载入 */
	S1_VAL = ButtonGearConvert(ButtonAdcValue0);
	S2_VAL = ButtonGearConvert(ButtonAdcValue1);
	S3_VAL = ButtonGearConvert(ButtonAdcValue2);
	S4_VAL = ButtonGearConvert(ButtonAdcValue3);
	S5_VAL = ButtonGearConvert(ButtonAdcValue4);
	S6_VAL = ButtonGearConvert(ButtonAdcValue5);	
	
	#if BREAKER_ADC_LOG
	sTick = xTaskGetTickCount() - sTick;
	log_t("adc cplt : %lu ms, tick - %lu\r\n", sTick, xTaskGetTickCount());
	#endif	

	BreakerHandler(&breakerParaInfo);				/* BreakerHandler函数在breaker.c中处理 */
}




/*
*********************************************************************************************************
*	函 数 名: HAL_ADC_ConvCpltCallback
*	功能说明: ADC中断回调函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
#if 0
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	osSemaphoreRelease(BinarySemAdcConvCpltHandle);
}
#endif

/*
*********************************************************************************************************
*	函 数 名: BreakerAdcInit
*	功能说明: 启动ADC,该函数被操作系统直接调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BreakerAdcInit(void)
{
	/* 初始化 BinarySemAdcConvCplt 信号量结构体*/
	osSemaphoreDef(BinarySemAdcConvCplt);
	
	/* 创建 BinarySemAdcConvCplt信号量，信号量个数为1个 */
	BinarySemAdcConvCpltHandle = osSemaphoreCreate(osSemaphore(BinarySemAdcConvCplt), 1);
	
	printf("BinarySemAdcConvCplt has created\r\n");	

	/* 启动ADC转换:开启ADC转换+开启定时器 */
	StartAdcConvert();
	
}

/*
*********************************************************************************************************
*	函 数 名: BreakerAdcProc
*	功能说明: ADC处理函数,该函数被操作系统直接调用
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void BreakerAdcProc(void)
{

	/* 等待信号量释放，时间为2000 */
	osSemaphoreWait(BinarySemAdcConvCpltHandle, 2000);

	/* ADC采样数据处理 */
	BreakerAdcHandler();

}


float GetIaA(void)
{
	return breakerParaInfo.ia.an;
}

float GetIbA(void)
{
	return breakerParaInfo.ib.an;
}

float GetIcA(void)
{
	return breakerParaInfo.ic.an;
}


float GetIaAver(void)
{

    portENTER_CRITICAL();

	float an = breakerParaInfo.ia.anAver;

    portEXIT_CRITICAL();

    return an;
}

float GetIbAver(void)
{
	
    portENTER_CRITICAL();
	
	float an = breakerParaInfo.ib.anAver;
	
    portEXIT_CRITICAL();
	
    return an;
}

float GetIcAver(void)
{
	
    portENTER_CRITICAL();
	
	float an = breakerParaInfo.ic.anAver;
   
	portEXIT_CRITICAL();
	
    return an;
}

float GetAnRawIaAver(void)
{
	
    portENTER_CRITICAL();
	
	float an = BreakerFft.ia.anAver;
   
	portEXIT_CRITICAL();
	
    return an;
}

float GetAnRawIbAver(void)
{
	
    portENTER_CRITICAL();
	
	float an = BreakerFft.ib.anAver;
    
	portEXIT_CRITICAL();
	
    return an;

}

float GetAnRawIcAver(void)
{
	
    portENTER_CRITICAL();
	
	float an = BreakerFft.ic.anAver;
    
	portEXIT_CRITICAL();
	
    return an;
}







/* 测试使用 */
void AdcPrint(void)
{
	#if 1
	uint8_t points = 0;
	uint8_t channel = 0;
	
	while(1)
	{
		delay(1000);
		if(ADC_DMA_TRANSFER == SET)
		{
			for(channel=0; channel<ADC_CHANLS_NUM; channel++)
			{
				for(points=0; points<ADC_SAMPLE_POINTS; points++)
				{
					adcValsFftIn[channel][points] = adcVals[points][channel];
				}				
			}		
			
			/* 存储电位器ADC值 */
			ButtonAdcValue0 = adcValsFftIn[0][7];
			ButtonAdcValue1 = adcValsFftIn[1][7];
			ButtonAdcValue2 = adcValsFftIn[2][7];
			ButtonAdcValue3 = adcValsFftIn[3][7];
			ButtonAdcValue4 = adcValsFftIn[4][7];
			ButtonAdcValue5 = adcValsFftIn[5][7];		
			/* 电位器键值档位值载入 */
			S1_VAL = ButtonGearConvert(ButtonAdcValue0);
			S2_VAL = ButtonGearConvert(ButtonAdcValue1);
			S3_VAL = ButtonGearConvert(ButtonAdcValue2);
			S4_VAL = ButtonGearConvert(ButtonAdcValue3);
			S5_VAL = ButtonGearConvert(ButtonAdcValue4);
			S6_VAL = ButtonGearConvert(ButtonAdcValue5);				

			
			printf("BUTTON_VALUE_1: %d   Gear-> '%d'\r\n",ButtonAdcValue0,S1_VAL);
			printf("BUTTON_VALUE_2: %d   Gear-> '%d'\r\n",ButtonAdcValue1,S2_VAL);			
			printf("BUTTON_VALUE_3: %d   Gear-> '%d'\r\n",ButtonAdcValue2,S3_VAL);			
			printf("BUTTON_VALUE_4: %d   Gear-> '%d'\r\n",ButtonAdcValue3,S4_VAL);			
			printf("BUTTON_VALUE_5: %d   Gear-> '%d'\r\n",ButtonAdcValue4,S5_VAL);		
			printf("BUTTON_VALUE_6: %d   Gear-> '%d'\r\n",ButtonAdcValue5,S6_VAL);
				
			ADC_DMA_TRANSFER = RESET;
		}
	}	
	#endif
	
	#if 0
	uint8_t i = 0;
	uint32_t value[10] = {0};
	while(1)
	{

		
		delay(1000);
		while((dma_flag_status_get(DMA1_FLAG_CMP1)) == RESET ); //wait DMA1 CMP1 flag.
		dma_flag_clear(DMA1_FLAG_CMP1); //Clear DMA CMP1 flag.
			
				
				for(i = 0;i < 10;i++)   
				{
					value[i] = ((adc_conv[i]* 1) / 1);
				} 					
				printf("\r\n ch0 = %d \r\n",adc_conv[0]);		
				printf("\r\n ch1 = %d \r\n",adc_conv[1]);		
				printf("\r\n ch2 = %d \r\n",adc_conv[2]);
				printf("\r\n ch3 = %d \r\n",adc_conv[3]);
				printf("\r\n ch4 = %d \r\n",adc_conv[4]);
				printf("\r\n ch5 = %d \r\n",adc_conv[5]);
				printf("\r\n ch6 = %d \r\n",adc_conv[6]);
				printf("\r\n ch7 = %d \r\n",adc_conv[7]);
				printf("\r\n ch8 = %d \r\n",adc_conv[8]);
				printf("\r\n ch9 = %d \r\n",adc_conv[9]);	
				
				for(i = 0;i < 10;i++)  
				{
					value[i] = 0;
				}		
				ADC_DMA_TRANSFER = 0;
			
	}
	#endif
}



/*
*********************************************************************************************************
*	函 数 名: ButtonGearConvert
*	功能说明: 将电位器的ADC值转换为当前定义的档位值
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
	/* ADC精度为12位，即电位器旋转到限位值为4096(3.3V)
	 * 电位器共需分为10个档位，各档位ADC值分配如下
	 * 
	 *   档位					ADC采样原始值						    实际电压值
	 *   OFF                      0 ~ 100							  0V ~ 0.08258V
	 *    1						101 ~ 590                          0.081385V ~ 0.475422V
	 *    2                     591 ~ 1080						   0.476227V ~ 0.870264V
	 *	  3						1081 ~ 1570						   0.871069V ~ 1.265106V
     *	  4                     1571 ~ 2060						   1.265911V ~ 1.659948V
     *    5                     2061 ~ 2550						   1.660753V ~ 2.054790V
     *    6                     2551 ~ 3040						   2.053555V ~ 2.449632V
     *	  7						3041 ~ 3530						   2.450437V ~ 2.844474V
     *    8 					3531 ~ 4020						   2.845279V ~ 3.239316V
     *	  9						4021 ~ 4096						   3.240121V ~ 3.3V
	*/
uint8_t ButtonGearConvert(uint32_t Value)
{
	uint32_t ButtonAdcValue = Value;
	uint8_t  S_VAL = 0;
	if((ButtonAdcValue >= 0) && (ButtonAdcValue <=100))
	{
		S_VAL = 0;									/* OFF档 */
	}
	else if((ButtonAdcValue >= 101) && (ButtonAdcValue <= 590))
	{
		S_VAL = 1;									/* 1档 */
	}
	else if((ButtonAdcValue >= 591) && (ButtonAdcValue <= 1080))
	{
		S_VAL = 2;									/* 2档 */
	}	
	else if((ButtonAdcValue >= 1081) && (ButtonAdcValue <= 1570))
	{
		S_VAL = 3;									/* 3档 */
	}		
	else if((ButtonAdcValue >= 1571) && (ButtonAdcValue <= 2060))
	{
		S_VAL = 4;									/* 4档 */
	}
	else if((ButtonAdcValue >= 2061) && (ButtonAdcValue <= 2550))
	{
		S_VAL = 5;									/* 5档 */
	}	
	else if((ButtonAdcValue >= 2551) && (ButtonAdcValue <= 3040))
	{
		S_VAL = 6;									/* 6档 */
	}	
	else if((ButtonAdcValue >= 3041) && (ButtonAdcValue <= 3530))
	{
		S_VAL = 7;									/* 7档 */
	}	
	else if((ButtonAdcValue >= 3531) && (ButtonAdcValue <= 4020))
	{
		S_VAL = 8;									/* 8档 */
	}	
	else if((ButtonAdcValue >= 4021) && (ButtonAdcValue <= 4096))
	{
		S_VAL = 9;									/* 9档 */
	}	
	return S_VAL;
}




