#ifndef BREAKER_ADC_H
#define BREAKER_ADC_H

typedef enum
{
	BUTTON_0_IDX,						/* 电位器0 ID：			0 */
	BUTTON_1_IDX,						/* 电位器1 ID：			1 */
	BUTTON_2_IDX,						/* 电位器2 ID：			2 */
	BUTTON_3_IDX,						/* 电位器3 ID：			3 */
	BUTTON_4_IDX,						/* 电位器4 ID：			4 */
	BUTTON_5_IDX,						/* 电位器5 ID：		    5 */
	POWER_IDX,							/* 单板电源检测 ID：    6 */
	IC_IDX,								/* C相电流检测ID：		7 */
	IB_IDX,								/* B相电流检测ID：		8 */	
	IA_IDX,								/* A相电流检测ID：		9 */
	
	ADC_CHANLS_NUM,						/* ADC检测通道数-10 */
	ADC_CHANLS_MAX = (ADC_CHANLS_NUM-1)	
}AdcChannelIdxDef;



    /*----------------------------------电位器档位设置-------------------------------------*/	
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
	float acAn;					/* 存储电流计算后的均方根值 */
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
