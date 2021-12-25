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

/* ˲ʱ�������� */
bool shortInstantProtector(const BreakerParaInfoDef *const breakerInfo)
{
	uint16_t Ir1 = GetLongDelayIr1();																	/* ��ȡ��ǰ����ʱ����������Χ */
	uint16_t actionAn = currProtectorCfg.shortInstant.gear*Ir1*SHORT_INSTANT_ACTION_PERCENT/100/100;	/* ��·˲ʱ������λ*Ir1/100 = ��ǰ��·˲ʱ����ֵ */
    uint16_t actionAnA = actionAn;
    uint16_t actionAnB = actionAn;
    uint16_t actionAnC = actionAn;
	uint32_t icwDelayCnt = 0;
	bool actionFlag = false;
	
	/* �����˲��������δ�� */
	if( !currProtectorCfg.shortInstant.isEnable )
	{
	    overCntA = 0;
        overCntB = 0;
        overCntC = 0;
        currProtectorCfg.shortInstant.heatIncEvts = 0;
		return false;
	}
	/* ���isProtected��־�Ƿ��Ѿ��� */
	if(currProtectorCfg.shortInstant.isProtected)
	{	
		return true;
	}
	/* A��˲ʱ����ж�-��ǰ��������ֵ�Ƿ�����ж�ֵ */
	if(breakerInfo->ia.an >= actionAnA)
	{
		/* ���ݵ�ǰ����icwDelayCnt����Ϊ0 */
		icwDelayCnt = GetIcwDelayCnt(breakerInfo->ia.an);
		overCntA++;											/* overCntA�ۼ� */
		if(overCntA>=SHORT_INSTANT_CNT_DEF+icwDelayCnt)		/* ����ĳһ��ʱ���ڼ�������>�� SHORT_INSTANT_CNT_DEF+icwDelayCnt �� */
		{
			actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_INSTANT, PHASE_A_BITMASK);	/* actionFlag������ΪTrue */
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


/* ˲ʱ������������ */
bool ShortInstantHandler(const BreakerParaInfoDef *const breakerInfo)
{
	bool retVal = shortInstantProtector(breakerInfo);

	return retVal;
}





















