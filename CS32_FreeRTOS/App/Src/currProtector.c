#include "currProtector.h"
#include <stdbool.h>
#include "breakerAdc.h"
//#include "cmsis_os.h"
#include "breaker.h"
#include "breakerAdc.h"
#include "currProtectorLongDelay.h"
#include "currProtectorShortDelay.h"
#include "currProtectorShortInstant.h"
#include "log.h"
#include "about.h"
#include "memMgr.h"
#include "breakerIo.h"

#include "bsp.h"

extern uint8_t S1_VAL;										/* ��λ����ֵ */
extern uint8_t S2_VAL;
extern uint8_t S3_VAL;
extern uint8_t S4_VAL;
extern uint8_t S5_VAL;
extern uint8_t S6_VAL;

CurrProtectorCfgDef currProtectorCfg;

				/* S1_VAL: 0   1    2    3    4    5    6    7    8    9 */
#if (DEV_TYPE_250A == DEV_TYPE)
static uint16_t ir1[10] = {0, 100, 125, 140, 150, 160, 180, 200, 225, 250};									/* {180, 200, 225, 250, 0, 100, 125, 140, 150, 160} */
#elif (DEV_TYPE_400A == DEV_TYPE)
static uint16_t ir1[10] = {0, 160, 180, 200, 225, 280, 300, 315, 360, 400};									/* {300, 340, 380, 400, 0, 160, 180, 200, 240, 280} */
#elif (DEV_TYPE_630A == DEV_TYPE)
static uint16_t ir1[10] = {0, 250, 300, 350, 400, 450, 500, 550, 600, 630};									/* {500, 560, 600, 630, 0, 250, 315, 360, 400, 460} */
#endif
				/* S2_VAL:    0     1     2     3     4      5      6      7      8      9 */
static uint16_t t1Ms[10] = {3000, 5000, 7000, 9000, 12000, 13000, 14000, 15000, 16000, 18000};				/* {13000, 14000, 15000, 16000, 18000, 3000, 5000, 7000, 9000, 12000} */
				/* S3_VAL:        0   1    2    3    4    5    6    7     8     9 */
static uint16_t ir2Percent[10] = {0, 200, 300, 400, 500, 600, 700, 800, 1000, 1200};						/* {700, 800, 1000, 1200, 0, 200, 300, 400, 500, 600} */
				/* S4_VAL:        0   1    2    3    4    5    6    7     8     9 */
static uint16_t t2Ms[10] = 		 {100, 200, 300, 400, 500, 500, 400, 300, 200, 100}; 					    /* {500, 400, 300, 200, 100, 100, 200, 300, 400, 500} ǰ5����ʱ�ޣ���5����ʱ��*/
				/* S5_VAL:        0   1    2    3    4    5    6    7     8     9 */
static uint16_t ir3Percent[10] = {0, 400, 600, 700, 800, 1000, 1100, 1200, 1300, 1400};						/* {1100, 1200, 1300, 1400, 0, 400, 600, 700, 800, 1000} */
				/* S6_VAL:        0   1   2   3   4   5   6   7   8   9 */
static uint16_t ir1Percent[10] = {0, 60, 65, 70, 75, 80, 85, 90, 95, 100};									/* {85, 90, 95, 100, 0, 60, 65, 70, 75, 80} */

static bool isFactoryMode = false;

#define FACTORY_CLOSE_LONGDELAY_A   1000


bool IsFactoryMode(void)
{
    portENTER_CRITICAL();
    bool flag = isFactoryMode;
    portEXIT_CRITICAL();
    
    return flag;
}

void CurrParaFresh( const BreakerParaInfoDef *const breakerInfo )
{
	/* ���س���ʱ���� */
    if(0==S1_VAL)											/* �жϵ�ǰS1��ť��λ�Ƿ�ΪOFF��  �ڹ���4==S1_VAL */
    {
        currProtectorCfg.longDelay.isEnable = false;		/* ���س���ʱ���ܱ����ر� */	
        currProtectorCfg.longDelay.gear = CURRENT_IN_A;		/* ��λѡ��Ϊabout.h�к궨��ѡ��ĵ�λ */
    }
    else
    {
        currProtectorCfg.longDelay.isEnable = true;			/* ��S1_VAL��= 0����򿪳���ʱ���� */
    }
    currProtectorCfg.longDelay.gear = ir1[S1_VAL];			/* ��λѡ��Ϊ��ǰS1��ť����ĵ�������ֵ */
    currProtectorCfg.longDelay.tsMs = t1Ms[S2_VAL];			/* ���س���ʱ����ʱ�� */
    currProtectorCfg.longDelay.isInverseTime = true;		/* ��ʱ�ޱ������ܴ� */

	/* ��·����ʱ���� */   
	if(0==S3_VAL)											/* �жϵ�ǰS3��ť��λ�Ƿ�ΪOFF��  �ڹ���4==S3_VAL */
    {
        currProtectorCfg.shortDelay.isEnable = false;		/* ��·����ʱ�������ܹر� */
    }
    else
    {
        currProtectorCfg.shortDelay.isEnable = true;		/* ��·����ʱ�������ܴ� */
    }
	currProtectorCfg.shortDelay.gear = ir2Percent[S3_VAL];	/* ��λѡ��Ϊ��ǰS3��ť����ĵ�������ֵ */
    currProtectorCfg.shortDelay.tsMs = t2Ms[S4_VAL];		/* ��·����ʱ����ʱ�� */
    if(S4_VAL < 5)											
    {
        currProtectorCfg.shortDelay.isInverseTime = false;	/* ���S4��ť�ĵ�λ��ǰ5����λ֮�䣬��ʱ�ޱ����رգ���ʱ�ޱ����� */
    }
    else
    {
        currProtectorCfg.shortDelay.isInverseTime = true;	/* ���S4��ť�ĵ�λ�ں�5����λ֮�䣬��ʱ�ޱ����򿪣���ʱ�ޱ����ر� */
    }

	/* ��·˲ʱ�������� */
	if(0==S5_VAL)											/* �жϵ�ǰS5��ť��λ�Ƿ�ΪOFF��  �ڹ���4==S5_VAL */
    {
        currProtectorCfg.shortInstant.isEnable = false;		/* ��·˲ʱ�������ܹر� */
    } 
    else
    {
        currProtectorCfg.shortInstant.isEnable = true;		/* ��·˲ʱ�������ܴ� */
    }
    currProtectorCfg.shortInstant.gear = ir3Percent[S5_VAL];/* ��·˲ʱ�������� */

    if(0==S6_VAL)
    {
        currProtectorCfg.overloadWarning.isEnable = false;	/* �жϵ�ǰS6��ť��λ�Ƿ�ΪOFF�������ǣ������Ԥ�������ܹر� */
    } 
    else
    {
        currProtectorCfg.overloadWarning.isEnable = true;	/* ����򿪹���Ԥ�������� */
    }
    currProtectorCfg.overloadWarning.ir1Percent = ir1Percent[S6_VAL];	/* ����Ԥ����������Χ */
		if((0==S1_VAL)&&(0==S2_VAL)&&(0==S3_VAL)&&(0==S4_VAL)&&(0==S5_VAL)&&(0==S6_VAL))	/* ����ˮ�߲���ʱ����6����λ����ťͳһ����OFF��λ���Թ������� */
		{
			#if (DEV_TYPE_250A == DEV_TYPE)
			currProtectorCfg.longDelay.gear = 100;
			currProtectorCfg.longDelay.tsMs = 1000;
			currProtectorCfg.shortDelay.gear = 1500;
			currProtectorCfg.shortInstant.gear = 2500;
			#elif (DEV_TYPE_400A == DEV_TYPE)
			currProtectorCfg.longDelay.gear = 160;
			currProtectorCfg.longDelay.tsMs = 1000;
			currProtectorCfg.shortDelay.gear = 1500;
			currProtectorCfg.shortInstant.gear = 2500;
			#elif (DEV_TYPE_630A == DEV_TYPE)
			currProtectorCfg.longDelay.gear = 250;
			currProtectorCfg.longDelay.tsMs = 1000;
			currProtectorCfg.shortDelay.gear = 1500;
			currProtectorCfg.shortInstant.gear = 2520;
			#endif
			/* �жϣ�����ԭʼֵУ��������������ֵ�Ƿ������ˮ�߲���ֵ */
			if( (breakerInfo->ia.an > FACTORY_CLOSE_LONGDELAY_A) 
				&& (breakerInfo->ib.an > FACTORY_CLOSE_LONGDELAY_A)
				&& (breakerInfo->ic.an > FACTORY_CLOSE_LONGDELAY_A) )
				{
					currProtectorCfg.longDelay.isEnable = false;		/* ���ǣ���رճ���ʱ�������� */
					currProtectorCfg.shortDelay.isEnable = false;		/* ���ǣ���رն�·����ʱ�������� */
				}
			portENTER_CRITICAL();
			isFactoryMode = true;									/* ��ˮ�߼��ģʽ��־���� */
			portEXIT_CRITICAL();
		}
		
		else
		{
			portENTER_CRITICAL();
			isFactoryMode = false;								/* ��ˮ�߼��ģʽ��־�ر� */
			portEXIT_CRITICAL();
		}
}


uint32_t GetIcwDelayCnt(float an)
{
	uint32_t icwDelayCnt = 0;

//#if (DEV_TYPE>=DEV_TYPE_630A)
////	if(an > 8000*0.95 && an < 8000*1.05)
////	{
////		icwDelayCnt = ICW_DELAY_MS/(1000/AN_COUNT_FREQ);
////	}
//#elif(DEV_TYPE==DEV_TYPE_400A)
//	if(an > 5000*0.95 && an < 5000*1.05)
//	{
//		icwDelayCnt = ICW_DELAY_MS/(1000/AN_COUNT_FREQ);
//	}
//#elif(DEV_TYPE==DEV_TYPE_250A)
//	if(an > 3000*0.95 && an < 3000*1.05)
//	{
//		icwDelayCnt = ICW_DELAY_MS/(1000/AN_COUNT_FREQ);
//	}
//#elif(DEV_TYPE==DEV_TYPE_160A)
//	if(an > 2000*0.95 && an < 2000*1.05)
//	{
//		icwDelayCnt = ICW_DELAY_MS/(1000/AN_COUNT_FREQ);
//	} 
//#endif
	return icwDelayCnt;
}

static void overloadWarningHandler(const BreakerParaInfoDef *const breakerInfo)
{
	/* ���������ܴ� */
    if(!currProtectorCfg.overloadWarning.isEnable)
    {
		/* ������ձ�����־ */
        currProtectorCfg.overloadWarning.isInAlarm = false;
        return;
    }
    /* iWarning = 1.1*��ǰ����ʱ����ֵ*����Ԥ����������Χ/100 */
    float iWarning = 1.1f * (GetLongDelayIr1()*currProtectorCfg.overloadWarning.ir1Percent / 100) ;
    
	/* ����ǰ����ĵ���ֵ > iWarning*/
    if(breakerInfo->ia.an >= iWarning )
    {	
		/* ������־�� */
        currProtectorCfg.overloadWarning.isInAlarm = true;
    }
    else
    {
		/* ��ձ�����־ */
        currProtectorCfg.overloadWarning.isInAlarm = false;
    }
}

void PrintSysInfo( void )
{

    float rmsAdcA = GetAnRawIaAver();
    float rmsAdcB = GetAnRawIbAver();
    float rmsAdcC = GetAnRawIcAver();

    float ia = GetIaAver();
    float ib = GetIbAver();
    float ic = GetIcAver();

    float rmsMvA = rmsAdcA*3300/4096;
    float rmsMvB = rmsAdcB*3300/4096;
    float rmsMvC = rmsAdcC*3300/4096;

    printf("[Ra]:%.2f\t[Rb]:%.2f\t[Rc]:%.2f\r\n[Ia]:%.2f\t[Ib]:%.2f\t[Ic]:%.2f\r\n", 
    rmsAdcA, rmsAdcB, rmsAdcC, ia, ib, ic);

    printf("S1:%d S2:%d S3:%d S4:%d S5:%d S6:%d\r\n", S1_VAL, S2_VAL, S3_VAL, S4_VAL, S5_VAL, S6_VAL);
    printf("\r\n");

}


void CurrProtectorStatusLed(void)
{
    static uint8_t cnt = 0;

    if(0 == cnt % 500)
    {
        if(currProtectorCfg.longDelay.heatIncEvts 
        || currProtectorCfg.shortDelay.heatIncEvts 
        || currProtectorCfg.shortInstant.heatIncEvts)
        {
            LedRedOn();
        }
        else
        {
            LedRedOff();
        }

        if(currProtectorCfg.overloadWarning.isInAlarm)
        {
            LedYellowToggle();
        }
        else
        {
            LedYellowOff();
        }
        cnt = 0;
    }
    cnt++;

}   

void CurrProtectorHandler(const BreakerParaInfoDef *const breakerInfo)
{
	/* �жϵ�ǰ��ť��λֵ��ȷ������ʽ�����ĸ�����ֵ */
    CurrParaFresh(breakerInfo);
	/* ����Ԥ������⴦�� */
    overloadWarningHandler(breakerInfo);
#if 1
	/* ��·˲ʱ������������ */
	if(ShortInstantHandler(breakerInfo))
	{
		return;
	}
#endif
#if 1
	/* ��·����ʱ������������ */
	if(ShortDelayHandler(breakerInfo))
	{
		return;
	}
#endif
#if 1
	if(LongDelayHandler(breakerInfo))
	{
		return;
	}
#endif
}



























