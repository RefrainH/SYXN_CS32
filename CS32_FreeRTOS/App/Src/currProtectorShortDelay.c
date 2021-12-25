#include "currProtectorShortDelay.h"
#include "currProtector.h"
#include "breaker.h"
#include "currProtectorLongDelay.h"
#include "log.h"
#include "about.h"
#include "breakerAdc.h"
#include "memMgr.h"



static bool isProtected = false;

void ClrShortDelayProtectFlag(void)
{
	isProtected = false;
}

void SetShortDelayGear(uint16_t percent)
{
	currProtectorCfg.shortDelay.gear = percent;
}

uint16_t GetShortDelayGear(void)
{
	return currProtectorCfg.shortDelay.gear;
}

bool IsShortDelayInverseTime(void)
{
	return currProtectorCfg.shortDelay.isInverseTime;
}

void SetShortDelayIsInverseTime(bool isInverse)
{
	currProtectorCfg.shortDelay.isInverseTime = isInverse;	
}

bool ShortDelayProtector(const BreakerParaInfoDef *const breakerInfo)
{
	static double Qa = 0;
	static double Qb = 0;
	static double Qc = 0;
	static bool isCountDownA = false;
	static bool isCountDownB = false;
	static bool isCountDownC = false;
	static int32_t countDownNumA = 0;
	static int32_t countDownNumB = 0;
	static int32_t countDownNumC = 0;
	double qDlt = 0;
	uint16_t Ir1 = GetLongDelayIr1();																			/* 获取当前长延时整定电流值 */
	uint16_t actionAn = currProtectorCfg.shortDelay.gear*Ir1*SHORT_DELAY_ACTION_PERCENT/100/100;				/* 获取当前短路短延时动作电流值 */
    uint16_t actionAnA = actionAn;
    uint16_t actionAnB = actionAn;
    uint16_t actionAnC = actionAn;																				/* (currProtectorCfg.shortDelay.tsMs-25) / [(1000/50) - 1]*/
	uint16_t delayCountDownCnt = (currProtectorCfg.shortDelay.tsMs-PROTECT_COST_MS) / (1000/AN_COUNT_FREQ) - 1; 
	uint16_t delayTotalCountDownCnt = 0;
	bool actionFlag = false;
	uint32_t gearIr1Mul8 = 8*currProtectorCfg.shortDelay.gear*Ir1;												/* 8倍Ir2 */
	//uint32_t sqr8Ir1MulTs = gearIr1Mul8*gearIr1Mul8*currProtectorCfg.shortDelay.tsMs;
	double sqr8Ir1An = 0; 	//(gearIr1Mul8/breakerInfo->ia.an/100)
	static uint8_t disturbCntA = 0;
	static uint8_t disturbCntB = 0;
	static uint8_t disturbCntC = 0;
	uint32_t icwDelayCnt = 0;

	/* 如果短路短延时功能未打开 */
	if(!currProtectorCfg.shortDelay.isEnable)
	{
        Qa = 0;
        Qb = 0;
        Qc = 0;
        isCountDownA = false;
        isCountDownB = false;
        isCountDownC = false;
        countDownNumA = 0;
        countDownNumB = 0;
        countDownNumC = 0;
        disturbCntA = 0;
        disturbCntB = 0;
        disturbCntC = 0;
        currProtectorCfg.shortDelay.heatIncEvts = 0;
		return false;
	}

	if(isProtected)
	{
		return true;
	}


	/** A **/
	icwDelayCnt = GetIcwDelayCnt(breakerInfo->ia.an);
	delayTotalCountDownCnt = delayCountDownCnt + icwDelayCnt;
	if(breakerInfo->ia.an > actionAnA)
	{
		if(!currProtectorCfg.shortDelay.isInverseTime) //定时限
		{
			Qa = 0;
			if(!isCountDownA)
			{
				isCountDownA = true;
				countDownNumA = delayTotalCountDownCnt;
				#if SHORT_DELAY_LOG
				log_t("ShortDelay - fixed delay start, an: %dA, actionAnA: %dA, delay: %dms\r\n", breakerInfo->ia.an, actionAnA, currProtectorCfg.shortDelay.tsMs);
				#endif
			}
			else
			{
				countDownNumA--;
				if(countDownNumA<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_A_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
						#if SHORT_DELAY_LOG
						log_t("ShortDelay - fixed protecter do\r\n");
						#endif
						return true;
					}
					isCountDownA = false;
					countDownNumA = delayTotalCountDownCnt;
				}
			}
		}
		else
		{
			if(breakerInfo->ia.an > gearIr1Mul8) //定时限
			{
				if(!isCountDownA)
				{
					isCountDownA = true;
					countDownNumA = delayTotalCountDownCnt;
					#if SHORT_DELAY_LOG
					log_t("ShortDelay - fixed delay start, an: %dA, actionAnA: %dA, delay: %dms\r\n", breakerInfo->ia.an, actionAnA, currProtectorCfg.shortDelay.tsMs);
					#endif
				}
				else
				{
					countDownNumA--;
					if(countDownNumA<=0)
					{
						actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_A_BITMASK);
						if(actionFlag)
						{
							isProtected = true;
							#if SHORT_DELAY_LOG
							log_t("ShortDelay - fixed protecter do\r\n");
							#endif
							return true;
						}
						isCountDownA = false;
						countDownNumA = delayTotalCountDownCnt;
					}
				}

			}
			else //反时限
			{		
				sqr8Ir1An = ((double)gearIr1Mul8) / ((double)breakerInfo->ia.an) / ((double)100);
				sqr8Ir1An *= sqr8Ir1An;
				qDlt = INVERSE_TIME_Q_MAX_STEP/( sqr8Ir1An * currProtectorCfg.shortDelay.tsMs); 
				Qa += qDlt;
				#if SHORT_DELAY_LOG
				log_t("ShortDelay - qA: %lu, qDlt: %lu, an: %d\r\n", Qa, qDlt, breakerInfo->ia.an);
				#endif
				if(Qa >= INVERSE_TIME_Q_MAX)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_A_BITMASK);	
					if(actionFlag)
					{
						isProtected = true;
						#if SHORT_DELAY_LOG
						log_t("ShortDelay - inverse protecter do\r\n");
						#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts |= PHASE_A_BITMASK;
	}
	else
	{
		if(currProtectorCfg.shortDelay.isInverseTime)
		{
            if(Qa > 0)
            {      
			    Qa -= (double)AN_COUNT_PERIOD_DECAY;
                if(Qa < 0)
                {
                    Qa = 0;
                }
            }
		}
		else
		{
			Qa = 0;
			if(isCountDownA)
			{
				disturbCntA++;
				if(disturbCntA>=5)
				{
					if(countDownNumA <= delayTotalCountDownCnt)
					{
						countDownNumA++;
					}
					disturbCntA = 0;
				}
			}
			else
			{
				disturbCntA = 0;
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts &= ~PHASE_A_BITMASK;
	}

	/** B **/
	icwDelayCnt = GetIcwDelayCnt(breakerInfo->ib.an);
	delayTotalCountDownCnt = delayCountDownCnt + icwDelayCnt;
	if(breakerInfo->ib.an > actionAnB)
	{
		if(!currProtectorCfg.shortDelay.isInverseTime) //定时限
		{
			Qb = 0;
			if(!isCountDownB)
			{
				isCountDownB = true;
				countDownNumB = delayTotalCountDownCnt;
			#if SHORT_DELAY_LOG
				log_t("ShortDelay - fixed delay start, an: %dA, actionAnB: %dA, delay: %dms\r\n", breakerInfo->ib.an, actionAnB, currProtectorCfg.shortDelay.tsMs);
			#endif
				
			}
			else
			{
				countDownNumB--;
				if(countDownNumB<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_B_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
					#if SHORT_DELAY_LOG
						log_t("ShortDelay - fixed protecter do\r\n");
					#endif
						return true;
					}
					isCountDownB = false;
					countDownNumB = delayTotalCountDownCnt;
				}
			}
		}
		else
		{
			if(breakerInfo->ib.an > gearIr1Mul8) //定时限
			{
				if(!isCountDownB)
				{
					isCountDownB = true;
					countDownNumB = delayTotalCountDownCnt;
				#if SHORT_DELAY_LOG
					log_t("ShortDelay - fixed delay start, an: %dA, actionAnB: %dA, delay: %dms\r\n", breakerInfo->ib.an, actionAnB, currProtectorCfg.shortDelay.tsMs);
				#endif
				}
				else
				{
					countDownNumB--;
					if(countDownNumB<=0)
					{
						actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_B_BITMASK);
						if(actionFlag)
						{
							isProtected = true;
							#if SHORT_DELAY_LOG
							log_t("ShortDelay - fixed protecter do\r\n");
							#endif
							return true;
						}
						isCountDownB = false;
						countDownNumB = delayTotalCountDownCnt;
					}
				}

			}
			else //反时限
			{
				sqr8Ir1An = ((double)gearIr1Mul8) / ((double)breakerInfo->ib.an) / ((double)100);
				sqr8Ir1An *= sqr8Ir1An;
				qDlt = INVERSE_TIME_Q_MAX_STEP/( sqr8Ir1An * currProtectorCfg.shortDelay.tsMs); 
				Qb += qDlt;
			#if SHORT_DELAY_LOG
				log_t("ShortDelay - qA: %lu, qDlt: %lu, an: %d\r\n", Qb, qDlt, breakerInfo->ib.an);
			#endif
				if(Qb >= INVERSE_TIME_Q_MAX)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_B_BITMASK);	
					if(actionFlag)
					{
						isProtected = true;
						#if SHORT_DELAY_LOG
						log_t("ShortDelay - inverse protecter do\r\n");
						#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts |= PHASE_B_BITMASK;
	}
	else
	{
		if(currProtectorCfg.shortDelay.isInverseTime)
		{
			if(Qb > 0)
            {      
			    Qb -= (double)AN_COUNT_PERIOD_DECAY;
                if(Qb < 0)
                {
                    Qb = 0;
                }
            }
		}
		else
		{
			Qb = 0;
			if(isCountDownB)
			{
				disturbCntB++;
				if(disturbCntB>=5)
				{
					if(countDownNumB <= delayTotalCountDownCnt)
					{
						countDownNumB++;
					}
					disturbCntB = 0;
				}
			}
			else
			{
				disturbCntB = 0;
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts &= ~PHASE_B_BITMASK;
	}

	/** C **/
	icwDelayCnt = GetIcwDelayCnt(breakerInfo->ic.an);
	delayTotalCountDownCnt = delayCountDownCnt + icwDelayCnt;
	if(breakerInfo->ic.an > actionAnC)
	{
		if(!currProtectorCfg.shortDelay.isInverseTime) //定时限
		{
			Qc = 0;
			if(!isCountDownC)
			{
				isCountDownC = true;
				countDownNumC = delayTotalCountDownCnt;
		#if SHORT_DELAY_LOG
				log_t("ShortDelay - fixed delay start, an: %dA, actionAnC: %dA, delay: %dms\r\n", breakerInfo->ic.an, actionAnC, currProtectorCfg.shortDelay.tsMs);
		#endif
				
			}
			else
			{
				countDownNumC--;
				if(countDownNumC<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_C_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
				#if SHORT_DELAY_LOG
						log_t("ShortDelay - fixed protecter do\r\n");
				#endif
						return true;
					}
					isCountDownC = false;
					countDownNumC = delayTotalCountDownCnt;
				}
			}
		}
		else
		{
			if(breakerInfo->ic.an > gearIr1Mul8) //定时限
			{
				if(!isCountDownC)
				{
					isCountDownC = true;
					countDownNumC = delayTotalCountDownCnt;
			#if SHORT_DELAY_LOG
					log_t("ShortDelay - fixed delay start, an: %dA, actionAnC: %dA, delay: %dms\r\n", breakerInfo->ic.an, actionAnC, currProtectorCfg.shortDelay.tsMs);
			#endif
				}
				else
				{
					countDownNumC--;
					if(countDownNumC<=0)
					{
						actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_C_BITMASK);
						if(actionFlag)
						{
							isProtected = true;
					#if SHORT_DELAY_LOG
							log_t("ShortDelay - fixed protecter do\r\n");
					#endif
							return true;
						}
						isCountDownC = false;
						countDownNumC = delayTotalCountDownCnt;
					}
				}

			}
			else //反时限
			{
				sqr8Ir1An = ((double)gearIr1Mul8) / ((double)breakerInfo->ic.an) / ((double)100);
				sqr8Ir1An *= sqr8Ir1An;
				qDlt = INVERSE_TIME_Q_MAX_STEP/( sqr8Ir1An * currProtectorCfg.shortDelay.tsMs); 
				Qc += qDlt;
		#if SHORT_DELAY_LOG
				log_t("ShortDelay - qA: %lu, qDlt: %lu, an: %d\r\n", Qc, qDlt, breakerInfo->ic.an);
		#endif
				if(Qc >= INVERSE_TIME_Q_MAX)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_SHORT_DELAY, PHASE_C_BITMASK);	
					if(actionFlag)
					{
						isProtected = true;
				#if SHORT_DELAY_LOG
						log_t("ShortDelay - inverse protecter do\r\n");
				#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts |= PHASE_C_BITMASK;
	}
	else
	{
		if(currProtectorCfg.shortDelay.isInverseTime)
		{
			if(Qc > 0)
            {      
			    Qc -= (double)AN_COUNT_PERIOD_DECAY;
                if(Qc < 0)
                {
                    Qc = 0;
                }
            }
		}
		else
		{
			Qc = 0;
			if(isCountDownC)
			{
				disturbCntC++;
				if(disturbCntC>=5)
				{
					if(countDownNumC <= delayTotalCountDownCnt)
					{
						countDownNumC++;
					}
					disturbCntC = 0;
				}
			}
			else
			{
				disturbCntC = 0;
			}
		}
        currProtectorCfg.shortDelay.heatIncEvts &= ~PHASE_C_BITMASK;
	}

	return false;
}



void ShortDelayProtectorReInit(void)
{
	currProtectorCfg.shortDelay.gear = 600;
	currProtectorCfg.shortDelay.tsMs = 300;
	currProtectorCfg.shortDelay.isInverseTime = false;
	isProtected = false;
}

bool ShortDelayHandler(const BreakerParaInfoDef *const breakerInfo)
{
	bool retVal = ShortDelayProtector(breakerInfo);

	return retVal;
}








