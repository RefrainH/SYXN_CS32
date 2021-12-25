#include "bsp.h"
#include "cs32f0xx_int.h"

/*
*********************************************************************************************************
*	                                   �궨������
*********************************************************************************************************
*/

#define NPT								32		//64					/* ����ÿ20ms�ڣ�ADC��ͨ���Ĳ������� */
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
*	                                   ��������
*********************************************************************************************************
*/
__IO int16_t adcVals[ADC_SAMPLE_POINTS][ADC_CHANLS_NUM] = {0};

uint32_t adcValsFftIn[ADC_CHANLS_NUM][NPT] = {0};						/* ת�������ʵ�ʴ��ADCԭʼֵ������ */

uint32_t ButtonAdcValue0;												/* ��λ��ADCԭʼֵ */
uint32_t ButtonAdcValue1;
uint32_t ButtonAdcValue2;
uint32_t ButtonAdcValue3;
uint32_t ButtonAdcValue4;
uint32_t ButtonAdcValue5;	

uint8_t S1_VAL;															/* ��λ��ʵ�ʼ�ֵ */
uint8_t S2_VAL;
uint8_t S3_VAL;
uint8_t S4_VAL;
uint8_t S5_VAL;
uint8_t S6_VAL;


BreakerFftDef	BreakerFft;
BreakerParaInfoDef	breakerParaInfo;

volatile uint8_t ADC_DMA_TRANSFER = 0;									/* DMA1�жϱ�־λ */

/*
*********************************************************************************************************
*	                                   ��������
*********************************************************************************************************
*/
//void cr4_fft_64_stm32(void *pssOUT, void *pssIN, uint16_t Nbin);


/*
*********************************************************************************************************
*	                                   RTOS�������
*********************************************************************************************************
*/
osSemaphoreId BinarySemAdcConvCpltHandle;						/* ����һ���ź��� */



/*
*********************************************************************************************************
*	�� �� ��: StartAdcInit
*	����˵��: ADC�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartAdcInit(void)
{
	uint32_t ADC_CALB = 0;
    adc_config_t   adc_config_struct;
    gpio_config_t  gpio_config_struct;
	
	/* 	CHANNEL | GPIO����	|	  ADC Channel	  | 		����			|	ADCͨ������ת��ʱ�䣺tCONV = ����ʱ�� + ( 12.5 * ADCʱ������ )   ��ǰADCʱ��Ϊ14MHz	
	 *		0		PA0				ADC_IN0			 ��λ������ͨ��0			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	1		PA1				ADC_IN1			 ��λ������ͨ��1			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	2		PA2  			ADC_IN2			 ��λ������ͨ��2			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	3		PA3				ADC_IN3			 ��λ������ͨ��3			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	4		PA4				ADC_IN4			 ��λ������ͨ��4			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	5		PA5				ADC_IN5			 ��λ������ͨ��5 			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	6		PA6				ADC_IN6			 ������Դ��ز���			(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	7		PA7				ADC_IN7			 C���������				(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	8		PB0				ADC_IN8			 B���������				(28.5 + 12.5) * 1/14M = 2.9uS
	 *  	9		PB1             ADC_IN9          A���������				(28.5 + 12.5) * 1/14M = 2.9uS
	 */
	
	/* ��ADC��GPIOʱ�� */
    rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTA, ENABLE); /* PA0~PA7 */   
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_PORTB,ENABLE);  /* PB0,PB1 */
    rcu_apb2_periph_clock_enable_ctrl(RCU_APB2_PERI_ADC, ENABLE); 

    /* ��λ������GPIO��ʼ�� */
    gpio_config_struct.gpio_pin = BUTTON_0 | BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4 | BUTTON_5 |CURR_DETECT_C  ;
    gpio_config_struct.gpio_mode = GPIO_MODE_AN;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL ;
    gpio_init(GPIOA, &gpio_config_struct);	
	
	/* ��������GPIO��ʼ�� */
    gpio_config_struct.gpio_pin = CURR_DETECT_A | CURR_DETECT_B;
    gpio_config_struct.gpio_mode = GPIO_MODE_AN;
    gpio_config_struct.gpio_pull = GPIO_PULL_NO_PULL ;
    gpio_init(GPIOB, &gpio_config_struct);
 
	/* ������ADC1��ȫ���Ĵ�������Ϊȱʡֵ */
    adc_def_init(ADC1);   
    adc_config_struct_init(&adc_config_struct);  

    /* ����ADC1����Ĺ���ģʽ */ 
    adc_config_struct.adc_resolution = ADC_CONV_RES_12BITS;				  		/* ת�����ݷֱ���Ϊ12λ */
    adc_config_struct.conversion_mode = DISABLE; 										    /* ʹ��ADCת�� */
    adc_config_struct.trigger_mode = ADC_TRIG_MODE_SEL_RISING;					/* Ӳ�������ش������ */
	adc_config_struct.hardware_trigger =  ADC_HW_TRIG_SEL_T1_CH4CC;  			/* ʹ��TIM1��ʱ���� */ 	
    adc_config_struct.data_align = ADC_DATA_ALIGN_RIGHT;							  /* ת�������Ҷ��� */
    adc_config_struct.scan_direction = ADC_CONV_SEQ_DIR_UPWARD;					/* ��ǰɨ��:����ɨ��ͨ��0��ͨ��18 */
    adc_init(ADC1, &adc_config_struct); 															  /* ��ʼ��ADC1 */

    /* ����ADC1��ͨ���Ĳ���ʱ�� */ 
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

	/* ADC��У׼ */
    ADC_CALB = adc_calibration_value_get(ADC1); 								/* ��ADC����У׼ʱ��Ӧ�ó��򲻵�ʹ��ADC������ȴ�У׼��� */
																				/* ÿ��ADC�����У׼ϵ�����ᶪʧ */
	if(ADC_CALB == 0)
	{
	 	/* ��ӡ�����Դ�����ļ������������� */
		printf("Error: file %s, function %s()\r\n", __FILE__, __FUNCTION__);
		printf("ADC calibration wrong!\r\n");	
	}
	else
	{
		/* ��ӡ��У׼ϵ��   */
		printf("ADC calibration value = 0x%x \r\n",ADC_CALB);
	}
	
	/* ʹ��ADC��DMA��������DMAΪѭ��ģʽ */                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	adc_dma_mode_set(ADC1, ADC_DMA_MODE_CIRCULAR); 

	/* ʹ��ADC���� */ 
	adc_dma_enable_ctrl(ADC1, ENABLE); 	
    adc_enable_ctrl(ADC1, ENABLE);     

	/* ADC_FLAG_EOI��ADC��ʼ��������־ */
    while(!adc_flag_status_get(ADC1, ADC_FLAG_EOI));
	
	/* ��ʼ��DMA1 */
	StartAdcDmaInit();	
	
}

/*
*********************************************************************************************************
*	�� �� ��: StartAdcDmaInit
*	����˵��: ADC_DMA�����ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartAdcDmaInit(void)
{
	dma_config_t  dma_configStruct;
	nvic_config_t nvic_config_struct;
	/* ����DMAʱ�� */ 
	rcu_ahb_periph_clock_enable_ctrl(RCU_AHB_PERI_DMA1, ENABLE);
	
	/* ����DMA����ģʽ */
	dma_def_init(DMA1_CHANNEL1);												/* ������DMA��ȫ���Ĵ�������Ϊȱʡֵ */	
	
	
	dma_configStruct.peri_base_addr = (uint32_t)ADC1_OUTDAT_REG_ADDRESS;		/* ��������Ĵ�����ַ */
	dma_configStruct.mem_base_addr = (uint32_t)&adcVals[0][0];					/* ���ô洢����ַ--(uint32_t)&adcVals[0][0];	 */
	dma_configStruct.transfer_direct = DMA_TRANS_DIR_FROM_PERI;					/* �������ݴ��䷽�򣺴����赽�ڴ� */			
	dma_configStruct.buf_size = ADC_CHANLS_NUM*ADC_SAMPLE_POINTS;				/* ����ÿ�δ�������ݴ�С */
	dma_configStruct.peri_inc_flag = DMA_PERI_INC_DISABLE;						/* �����ַ������ */
	dma_configStruct.mem_inc_flag = DMA_MEM_INC_ENABLE;							/* �����ַ���� */
	dma_configStruct.peri_data_width = DMA_PERI_DATA_WIDTH_HALFWORD;			/* �������ݿ�� */
	dma_configStruct.mem_data_width = DMA_MEM_DATA_WIDTH_HALFWORD;				/* �������ݿ�� */
	dma_configStruct.operate_mode = DMA_OPERATE_MODE_CIRCULAR;					/* ����DMAΪѭ������ģʽ */
	dma_configStruct.priority_level = DMA_CHANNEL_PRIORITY_HIGH;				/* ���ø�DMAͨ�������ȼ����� */
	dma_configStruct.m2m_flag = DMA_M2M_MODE_DISABLE;							/* ��ֹDMAʹ���ڴ�-�ڴ�Ĺ���ģʽ */							
	dma_init(DMA1_CHANNEL1, &dma_configStruct);									/* ������д��DMA���ƽṹ�� */

	
	dma_interrupt_set(DMA1_CHANNEL1,DMA_INT_CONFIG_CMP,ENABLE);					/* ʹ��DMA�жϣ� ��������ж�����*/
	
	nvic_config_struct.nvic_IRQ_channel = IRQn_DMA1_CHANNEL1;					/* ʹ��DMA1 channel1 IRQ Channel */
	nvic_config_struct.nvic_channel_priority = 0;
	nvic_config_struct.nvic_enable_flag = ENABLE;
	nvic_init(&nvic_config_struct);
	
	dma_enable_ctrl(DMA1_CHANNEL1, ENABLE); 									/* ʹ��DMA1 */
	

}


/*
*********************************************************************************************************
*	�� �� ��: StartAdcConvert
*	����˵��: ����ADCת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StartAdcConvert(void)
{
	/* ����ADCת�� */
    adc_conversion_start(ADC1);
	
	/* ����TIM1��ʱ���� */
	StartAdcTrigTimer();
}

/*
*********************************************************************************************************
*	�� �� ��: StopAdcConvert
*	����˵��: ֹͣADCת��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void StopAdcConvert(void)
{
  /* ֹͣADCת�� */
  adc_conversion_stop(ADC1);
	
  /* ֹͣTIM3��ʱ���� */	
  StopAdcTrigTimer();
}



void CountFFTParas(uint32_t *fftIn, FFTParasDef *para)
{
	para->acAn = sqrSumAverSqrt(fftIn, NPT);	
}

void CountFFTParas_I(uint32_t *fftIn, FFTParasDef *para)
{
	/* ������ֵ���㣬fftIn�����ݵ�ַ NPT�����ݸ��� */
	para->acAn = sqrSumAverSqrt(fftIn, NPT);
}

/* �ú�����Դ������δʹ�� */
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
*	�� �� ��: countbreakerParaAn
*	����˵��: ��ADC�ɼ�ת����ľ�����ֵ�������ʵ�ĵ���ֵ
*	��    ��:  uint8_t idx         �� 
*			   float fftAn         ��
*			   CalibInfoDef *para  �� 
*	�� �� ֵ: ��
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
	/* A�ࣺ����A��ADCͨ���ɼ�ֵ�ľ�����ֵ��������ֵ����� BreakerFft.ia.fftPara.acAn ��*/
	CountFFTParas_I(&adcValsFftIn[IA_IDX][0], &BreakerFft.ia.fftPara);
	/* ��������ֵ���ۼӼ��� */
	BreakerFft.ia.anSum += BreakerFft.ia.fftPara.acAn;
	/* breakerParaInfo.ia.an �˲�����Ϊ�����������ĵ�ǰ����ֵ������CS32����ܲ��ֲ�����Ҫ�޸� */
	breakerParaInfo.ia.an = countbreakerParaAn(IA_IDX, BreakerFft.ia.fftPara.acAn, &(calibMeterEx.ia));
	/* ������ֵ���ۼӼ��� */
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
	/* periodIdx���� */
	breakerParaInfo.periodIdx++;
	
	if(breakerParaInfo.periodIdx>=AN_AVER_COUNT) //PHASE_FREQ
	{
	    portENTER_CRITICAL();
		/* ���������������ԭʼֵƽ��ֵ���㣿 */
		BreakerFft.ia.anAver = BreakerFft.ia.anSum/AN_AVER_COUNT;
		BreakerFft.ib.anAver = BreakerFft.ib.anSum/AN_AVER_COUNT;
		BreakerFft.ic.anAver = BreakerFft.ic.anSum/AN_AVER_COUNT;
		/* ��������������ļ���ֵƽ��ֵ���㣿 */
		breakerParaInfo.ia.anAver = breakerParaInfo.ia.anSum/AN_AVER_COUNT;
		breakerParaInfo.ib.anAver = breakerParaInfo.ib.anSum/AN_AVER_COUNT;
		breakerParaInfo.ic.anAver = breakerParaInfo.ic.anSum/AN_AVER_COUNT;
        portEXIT_CRITICAL();
		/* ȫ�����㣬�ȴ���һ�μ��� */
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
	uint16_t channel = 0;								/* ADCͨ����Ŀ */
	uint16_t points = 0;								/* ÿ���ڲ������� */
	uint16_t offset = 0;
				
	
#if BREAKER_ADC_LOG
	static uint32_t sTick = 0;
    sTick = xTaskGetTickCount();
#endif
	/* adcValsFftIn[channel][points]Ϊת�����adc���ݵĴ洢��ַ����СΪadcValsFftIn[9][64] */
	for(channel = 0; channel < ADC_CHANLS_NUM; channel++)
	{
		#if (4==PHASE_PERIOD_WINDOW_DIV)
			offset = NPT-ADC_SAMPLE_POINTS;
			memmove(&adcValsFftIn[channel][0], &adcValsFftIn[channel][ADC_SAMPLE_POINTS], offset*sizeof(uint32_t) );
		#else
			offset = 0;
		#endif
		/* ADCԭʼֵ����ת�����洢��ʽ������[��������][����ͨ��] -> ����[����ͨ��][��������] */
		for(points = 0; points < ADC_SAMPLE_POINTS; points++)
		{
			adcValsFftIn[channel][points+offset] = adcVals[points][channel];
		}
	}

	IabcAnCount();
	AnAverCount();
		
	/* �洢��λ��ADCֵ */
	ButtonAdcValue0 = adcValsFftIn[BUTTON_0_IDX][7];
	ButtonAdcValue1 = adcValsFftIn[BUTTON_1_IDX][7];
	ButtonAdcValue2 = adcValsFftIn[BUTTON_2_IDX][7];
	ButtonAdcValue3 = adcValsFftIn[BUTTON_3_IDX][7];
	ButtonAdcValue4 = adcValsFftIn[BUTTON_4_IDX][7];
	ButtonAdcValue5 = adcValsFftIn[BUTTON_5_IDX][7];					
	/* ��λ����ֵ��λֵ���� */
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

	BreakerHandler(&breakerParaInfo);				/* BreakerHandler������breaker.c�д��� */
}




/*
*********************************************************************************************************
*	�� �� ��: HAL_ADC_ConvCpltCallback
*	����˵��: ADC�жϻص�����
*	��    ��: ��
*	�� �� ֵ: ��
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
*	�� �� ��: BreakerAdcInit
*	����˵��: ����ADC,�ú���������ϵͳֱ�ӵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BreakerAdcInit(void)
{
	/* ��ʼ�� BinarySemAdcConvCplt �ź����ṹ��*/
	osSemaphoreDef(BinarySemAdcConvCplt);
	
	/* ���� BinarySemAdcConvCplt�ź������ź�������Ϊ1�� */
	BinarySemAdcConvCpltHandle = osSemaphoreCreate(osSemaphore(BinarySemAdcConvCplt), 1);
	
	printf("BinarySemAdcConvCplt has created\r\n");	

	/* ����ADCת��:����ADCת��+������ʱ�� */
	StartAdcConvert();
	
}

/*
*********************************************************************************************************
*	�� �� ��: BreakerAdcProc
*	����˵��: ADC������,�ú���������ϵͳֱ�ӵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BreakerAdcProc(void)
{

	/* �ȴ��ź����ͷţ�ʱ��Ϊ2000 */
	osSemaphoreWait(BinarySemAdcConvCpltHandle, 2000);

	/* ADC�������ݴ��� */
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







/* ����ʹ�� */
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
			
			/* �洢��λ��ADCֵ */
			ButtonAdcValue0 = adcValsFftIn[0][7];
			ButtonAdcValue1 = adcValsFftIn[1][7];
			ButtonAdcValue2 = adcValsFftIn[2][7];
			ButtonAdcValue3 = adcValsFftIn[3][7];
			ButtonAdcValue4 = adcValsFftIn[4][7];
			ButtonAdcValue5 = adcValsFftIn[5][7];		
			/* ��λ����ֵ��λֵ���� */
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
*	�� �� ��: ButtonGearConvert
*	����˵��: ����λ����ADCֵת��Ϊ��ǰ����ĵ�λֵ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
	/* ADC����Ϊ12λ������λ����ת����λֵΪ4096(3.3V)
	 * ��λ�������Ϊ10����λ������λADCֵ��������
	 * 
	 *   ��λ					ADC����ԭʼֵ						    ʵ�ʵ�ѹֵ
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
		S_VAL = 0;									/* OFF�� */
	}
	else if((ButtonAdcValue >= 101) && (ButtonAdcValue <= 590))
	{
		S_VAL = 1;									/* 1�� */
	}
	else if((ButtonAdcValue >= 591) && (ButtonAdcValue <= 1080))
	{
		S_VAL = 2;									/* 2�� */
	}	
	else if((ButtonAdcValue >= 1081) && (ButtonAdcValue <= 1570))
	{
		S_VAL = 3;									/* 3�� */
	}		
	else if((ButtonAdcValue >= 1571) && (ButtonAdcValue <= 2060))
	{
		S_VAL = 4;									/* 4�� */
	}
	else if((ButtonAdcValue >= 2061) && (ButtonAdcValue <= 2550))
	{
		S_VAL = 5;									/* 5�� */
	}	
	else if((ButtonAdcValue >= 2551) && (ButtonAdcValue <= 3040))
	{
		S_VAL = 6;									/* 6�� */
	}	
	else if((ButtonAdcValue >= 3041) && (ButtonAdcValue <= 3530))
	{
		S_VAL = 7;									/* 7�� */
	}	
	else if((ButtonAdcValue >= 3531) && (ButtonAdcValue <= 4020))
	{
		S_VAL = 8;									/* 8�� */
	}	
	else if((ButtonAdcValue >= 4021) && (ButtonAdcValue <= 4096))
	{
		S_VAL = 9;									/* 9�� */
	}	
	return S_VAL;
}




