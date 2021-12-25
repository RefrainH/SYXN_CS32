                                           #include "currProtectorShortInstant.h"
#include "currProtector.h"
#include "breaker.h"
#include "log.h"
#include "currProtectorLongDelay.h"
#include "about.h"
#include "breakerAdc.h"
#include "memMgr.h"


static uint32_t overCntA = 0;
static uint32_t overCntB = 0;
static uint32_t overCntC = 0;


void ClrShortInstantProtectFlag(void)
{
	currProtectorCfg.shortInstant.isProtected = false;
}

void SetShortInstantGear(uint16_t percent)
{
	currProtectorCfg.shortInstant.gear = percent;
}

uint16_t GetShortInstantGear(void)
{
	return currProtectorCfg.shortInstant.gear;
}

/* 瞬时保护功能 */
bool shortInstantProtector(const BreakerParaInfoDef *const breakerInfo)
{
	uint16_t Ir1 = GetLongDelayIr1();																	/* 获取当前长延时整定电流范围 */
	uint16_t actionAn = currProtectorCfg.shortInstant.gear*Ir1*SHORT_INSTANT_ACTION_PERCENT/100/100;	/* 短路瞬时整定档位*Ir1/100 = 当前短路瞬时动作值 */
    uint16_t actionAnA = actionAn;
    uint16_t actionAnB = actionAn;
    uint16_t actionAnC = actionAn;
	uint32_t icwDelayCnt = 0;
	bool actionFlag = false;
	
	/* 如果短瞬保护功能未打开 */
	if( !currProtectorCfg.shortInstant.isEnable )
	{
	    overCntA = 0;
        overCntB = 0;
        overCntC = 0;
        currProtectorCfg.shortInstant.heatIncEvts = 0;
		return false;
	}
	/* 检测isProtected标志是否已经打开 */
	if(currProtectorCfg.shortInstant.isProtected)
	{	
		return true;
	}
	/* A相瞬时检测判断-当前电流计算值是否大于判断值 */
	if(breakerInfo->ia.an >= actionAnA)
	{
		/* 根据当前程序，icwDelayCnt被置为0 */
		icwDelayCnt = GetIcwDelayCnt(breakerInfo->ia.an);
		overCntA++;											/* overCntA累计 */
		if(overCntA>=SHORT_INSTANT_CNT_DEF+icwDelayCnt)		/* 若在某一短时间内计数次数>“ SHORT_INSTANT_CNT_DEF+icwDelayCnt ” */
		{
			actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_INSTANT, PHASE_A_BITMASK);	/* actionFlag被设置为True */
			if(actionFlag)
			{
				overCntA = 0;
				currProtectorCfg.shortInstant.isProtected = true;
				#if SHORT_INSTANT_LOG
				log_t("ShortInstant - switch off, iaAn: %d, actionAnA: %d\r\n", breakerInfo->ia.an, actionAnA);
				#endif
				return true;
			}
		}
        currProtectorCfg.shortInstant.heatIncEvts |= PHASE_A_BITMASK;
	}
	else
	{
		overCntA = 0;
        currProtectorCfg.shortInstant.heatIncEvts &= ~PHASE_A_BITMASK;
	}

	if(breakerInfo->ib.an >= actionAnB)
	{
		icwDelayCnt = GetIcwDelayCnt(breakerInfo->ib.an);
		overCntB++;
		if(overCntB>=SHORT_INSTANT_CNT_DEF+icwDelayCnt)
		{
			actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_INSTANT, PHASE_B_BITMASK);
			if(actionFlag)
			{
				overCntB = 0;
				currProtectorCfg.shortInstant.isProtected = true;
				#if SHORT_INSTANT_LOG
				log_t("ShortInstant - switch off, ibAn: %d, actionAnB: %d\r\n", breakerInfo->ib.an, actionAnB);
				#endif			
				return true;
			}
		}
        currProtectorCfg.shortInstant.heatIncEvts |= PHASE_B_BITMASK;
	}
	else
	{
		overCntB = 0;
        currProtectorCfg.shortInstant.heatIncEvts &= ~PHASE_B_BITMASK;
	}

	if(breakerInfo->ic.an >= actionAnC)
	{
		icwDelayCnt = GetIcwDelayCnt(breakerInfo->ic.an);
		overCntC++;
		if(overCntC>=SHORT_INSTANT_CNT_DEF+icwDelayCnt)
		{
			actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_INSTANT, PHASE_C_BITMASK);	
			if(actionFlag)
			{
				overCntC = 0;
				currProtectorCfg.shortInstant.isProtected = true;
				#if SHORT_INSTANT_LOG
				log_t("ShortInstant - switch off, icAn: %d, actionAnC: %d\r\n", breakerInfo->ic.an, actionAnC);
				#endif			
				return true;
			}
		}
        currProtectorCfg.shortInstant.heatIncEvts |= PHASE_C_BITMASK;
	}
	else
	{
		overCntC = 0;
        currProtectorCfg.shortInstant.heatIncEvts &= ~PHASE_C_BITMASK;
	}

	return false;
}


void ShortInstantProtectorReInit(void)
{
	currProtectorCfg.shortInstant.gear = 1000;
	currProtectorCfg.shortInstant.isProtected = false;
}


/* 瞬时保护动作处理 */
bool ShortInstantHandler(const BreakerParaInfoDef *const breakerInfo)
{
	bool retVal = shortInstantProtector(breakerInfo);

	return retVal;
}





















