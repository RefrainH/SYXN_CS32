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

extern uint8_t S1_VAL;										/* 电位器键值 */
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
static uint16_t t2Ms[10] = 		 {100, 200, 300, 400, 500, 500, 400, 300, 200, 100}; 					    /* {500, 400, 300, 200, 100, 100, 200, 300, 400, 500} 前5：定时限；后5：反时限*/
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
	/* 过载长延时保护 */
    if(0==S1_VAL)											/* 判断当前S1旋钮档位是否为OFF档  仝工：4==S1_VAL */
    {
        currProtectorCfg.longDelay.isEnable = false;		/* 过载长延时功能保护关闭 */	
        currProtectorCfg.longDelay.gear = CURRENT_IN_A;		/* 档位选择为about.h中宏定义选择的档位 */
    }
    else
    {
        currProtectorCfg.longDelay.isEnable = true;			/* 若S1_VAL！= 0，则打开长延时功能 */
    }
    currProtectorCfg.longDelay.gear = ir1[S1_VAL];			/* 档位选择为当前S1旋钮定义的电流整定值 */
    currProtectorCfg.longDelay.tsMs = t1Ms[S2_VAL];			/* 过载长延时整定时限 */
    currProtectorCfg.longDelay.isInverseTime = true;		/* 反时限保护功能打开 */

	/* 短路短延时保护 */   
	if(0==S3_VAL)											/* 判断当前S3旋钮档位是否为OFF档  仝工：4==S3_VAL */
    {
        currProtectorCfg.shortDelay.isEnable = false;		/* 短路短延时保护功能关闭 */
    }
    else
    {
        currProtectorCfg.shortDelay.isEnable = true;		/* 短路短延时保护功能打开 */
    }
	currProtectorCfg.shortDelay.gear = ir2Percent[S3_VAL];	/* 档位选择为当前S3旋钮定义的电流整定值 */
    currProtectorCfg.shortDelay.tsMs = t2Ms[S4_VAL];		/* 短路短延时整定时限 */
    if(S4_VAL < 5)											
    {
        currProtectorCfg.shortDelay.isInverseTime = false;	/* 如果S4旋钮的档位在前5个档位之间，则反时限保护关闭，定时限保护打开 */
    }
    else
    {
        currProtectorCfg.shortDelay.isInverseTime = true;	/* 如果S4旋钮的档位在后5个档位之间，则反时限保护打开，定时限保护关闭 */
    }

	/* 短路瞬时保护功能 */
	if(0==S5_VAL)											/* 判断当前S5旋钮档位是否为OFF档  仝工：4==S5_VAL */
    {
        currProtectorCfg.shortInstant.isEnable = false;		/* 短路瞬时保护功能关闭 */
    } 
    else
    {
        currProtectorCfg.shortInstant.isEnable = true;		/* 短路瞬时保护功能打开 */
    }
    currProtectorCfg.shortInstant.gear = ir3Percent[S5_VAL];/* 短路瞬时整定电流 */

    if(0==S6_VAL)
    {
        currProtectorCfg.overloadWarning.isEnable = false;	/* 判断当前S6旋钮档位是否为OFF档，若是，则过载预报警功能关闭 */
    } 
    else
    {
        currProtectorCfg.overloadWarning.isEnable = true;	/* 否则打开过载预报警功能 */
    }
    currProtectorCfg.overloadWarning.ir1Percent = ir1Percent[S6_VAL];	/* 过载预报警整定范围 */
		if((0==S1_VAL)&&(0==S2_VAL)&&(0==S3_VAL)&&(0==S4_VAL)&&(0==S5_VAL)&&(0==S6_VAL))	/* 过流水线测试时，将6个电位器旋钮统一调成OFF档位，以过测试用 */
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
			/* 判断：经过原始值校正过后的三相电流值是否大于流水线测试值 */
			if( (breakerInfo->ia.an > FACTORY_CLOSE_LONGDELAY_A) 
				&& (breakerInfo->ib.an > FACTORY_CLOSE_LONGDELAY_A)
				&& (breakerInfo->ic.an > FACTORY_CLOSE_LONGDELAY_A) )
				{
					currProtectorCfg.longDelay.isEnable = false;		/* 若是，则关闭长延时保护功能 */
					currProtectorCfg.shortDelay.isEnable = false;		/* 若是，则关闭短路短延时保护功能 */
				}
			portENTER_CRITICAL();
			isFactoryMode = true;									/* 流水线检测模式标志开启 */
			portEXIT_CRITICAL();
		}
		
		else
		{
			portENTER_CRITICAL();
			isFactoryMode = false;								/* 流水线检测模式标志关闭 */
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
	/* 若报警功能打开 */
    if(!currProtectorCfg.overloadWarning.isEnable)
    {
		/* 首先清空报警标志 */
        currProtectorCfg.overloadWarning.isInAlarm = false;
        return;
    }
    /* iWarning = 1.1*当前长延时整定值*过载预报警整定范围/100 */
    float iWarning = 1.1f * (GetLongDelayIr1()*currProtectorCfg.overloadWarning.ir1Percent / 100) ;
    
	/* 若当前计算的电流值 > iWarning*/
    if(breakerInfo->ia.an >= iWarning )
    {	
		/* 报警标志打开 */
        currProtectorCfg.overloadWarning.isInAlarm = true;
    }
    else
    {
		/* 清空报警标志 */
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
	/* 判断当前旋钮档位值，确定三段式保护的各参数值 */
    CurrParaFresh(breakerInfo);
	/* 过载预报警检测处理 */
    overloadWarningHandler(breakerInfo);
#if 1
	/* 短路瞬时保护动作处理 */
	if(ShortInstantHandler(breakerInfo))
	{
		return;
	}
#endif
#if 1
	/* 短路短延时保护动作处理 */
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



























