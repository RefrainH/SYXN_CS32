#ifndef BREAKER_ADC_H
#define BREAKER_ADC_H

typedef enum
{
	BUTTON_0_IDX,						/* ��λ��0 ID��			0 */
	BUTTON_1_IDX,						/* ��λ��1 ID��			1 */
	BUTTON_2_IDX,						/* ��λ��2 ID��			2 */
	BUTTON_3_IDX,						/* ��λ��3 ID��			3 */
	BUTTON_4_IDX,						/* ��λ��4 ID��			4 */
	BUTTON_5_IDX,						/* ��λ��5 ID��		    5 */
	POWER_IDX,							/* �����Դ��� ID��    6 */
	IC_IDX,								/* C��������ID��		7 */
	IB_IDX,								/* B��������ID��		8 */	
	IA_IDX,								/* A��������ID��		9 */
	
	ADC_CHANLS_NUM,						/* ADC���ͨ����-10 */
	ADC_CHANLS_MAX = (ADC_CHANLS_NUM-1)	
}AdcChannelIdxDef;



    /*----------------------------------��λ����λ����-------------------------------------*/	
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



#define BREAKER_ADC_LOG						0


#define PHASE_FREQ							50
#define PHASE_PERIOD_WINDOW_DIV				1//4
#define AN_COUNT_FREQ						(PHASE_FREQ*PHASE_PERIOD_WINDOW_DIV)

#define AN_AVER_COUNT						1//(AN_COUNT_FREQ/4)


#define PHASE_A_BITMASK		((uint8_t)(1<<0))
#define PHASE_B_BITMASK		((uint8_t)(1<<1))
#define PHASE_C_BITMASK		((uint8_t)(1<<2))
#define PHASE_N_BITMASK 	((uint8_t)(1<<3))


/* curr mode */
#define CURR_MODE_BIG		0
#define CURR_MODE_SMALL		1









typedef struct
{
	float dcAn;
	float acAn;					/* �洢���������ľ�����ֵ */
	uint32_t fn;
	uint16_t angle; 
}FFTParasDef;

typedef struct
{
	FFTParasDef fftPara;
	float anSum;			
	float anAver;
}PhaseInfoDef;

typedef struct
{
	PhaseInfoDef ia;
	PhaseInfoDef ib;
	PhaseInfoDef ic; 
}BreakerFftDef;

typedef struct
{
	float an;
	float anAver;
	float anSum;
}breakerParaDef;

typedef struct
{
	breakerParaDef ia;
	breakerParaDef ib;
	breakerParaDef ic;

	uint32_t 	periodIdx;
}BreakerParaInfoDef;




void AdcPrint(void);



void StartAdcInit(void);
void StartAdcDmaInit(void);

void StartAdcConvert(void);
void StopAdcConvert(void);

float GetIaA(void);
float GetIbA(void);
float GetIcA(void);

float GetAnRawIaAver(void);
float GetAnRawIbAver(void);
float GetAnRawIcAver(void);

float GetIaAver(void);
float GetIbAver(void);
float GetIcAver(void);

uint8_t ButtonGearConvert(uint32_t Value);

void BreakerAdcInit(void);
void BreakerAdcProc(void);
void PrintfAdc(void);



#endif
