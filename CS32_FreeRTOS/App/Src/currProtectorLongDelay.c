#include "currProtectorLongDelay.h"
#include "currProtector.h"
#include "breaker.h"
#include "log.h"
#include "about.h"
#include "breakerAdc.h"
#include "memMgr.h"



static bool isProtected = false;

void ClrLongDelayProtectFlag(void)
{
	isProtected = false;
}

uint16_t GetLongDelayIr1(void)
{
	return currProtectorCfg.longDelay.gear;
}

void SetLongDelayIr1(uint16_t ir1)
{
	currProtectorCfg.longDelay.gear = ir1;
}

uint16_t GetLongDelayT1Ms( void )
{
	return currProtectorCfg.longDelay.tsMs;
}

void SetLongDelayT1Ms(uint16_t ms)
{
	currProtectorCfg.longDelay.tsMs = ms;
}

bool IsLongDelayInverseTime(void)
{
	return currProtectorCfg.longDelay.isInverseTime;
}

void SetLongDelayIsInverseTime(bool isInverse)
{
	currProtectorCfg.longDelay.isInverseTime = isInverse;	
}

bool LongDelayProtector(const BreakerParaInfoDef *const breakerInfo)
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
	uint16_t idz = currProtectorCfg.longDelay.gear*DELAY_ACTION_PERCENT/100;
	double qDlt = 0;
	bool actionFlag = false;
	uint32_t gearIr1Mul6 = 6*GetLongDelayIr1();
	//uint32_t sqr6Ir1An = gearIr1Mul6*gearIr1Mul6*currProtectorCfg.longDelay.tsMs;
	double sqr6Ir1AnTs = 0;

    if(!currProtectorCfg.longDelay.isEnable)
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
        currProtectorCfg.longDelay.heatIncEvts = 0;
		return false;
	}

	if(isProtected)
	{
		return true;
	}

	/* A */
	if(breakerInfo->ia.an > idz)
	{
		if(currProtectorCfg.longDelay.isInverseTime)
		{
			sqr6Ir1AnTs = (double) ( ((double)gearIr1Mul6 / (double)breakerInfo->ia.an) * ((double)gearIr1Mul6 / (double)breakerInfo->ia.an) * (double)currProtectorCfg.longDelay.tsMs );	
			qDlt =  (double)INVERSE_TIME_Q_MAX_STEP / sqr6Ir1AnTs;
			Qa += qDlt;
			if(Qa >= INVERSE_TIME_Q_MAX)
			{
                if(!IsFactoryMode())
                {
                    actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_A_BITMASK);	
                    if(actionFlag)
                    {
                        isProtected = true;
                        return true;
                    }
                }
			}
		}
		else
		{
			Qa = 0;
			if(!isCountDownA)
			{
				isCountDownA = true;
				countDownNumA = currProtectorCfg.longDelay.tsMs / (1000/AN_COUNT_FREQ);
			}
			else
			{
				countDownNumA--;
				if(countDownNumA<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_A_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
						#if LONG_DELAY_LOG
						log_t("LongDelay - fixed protecter do\r\n");
						#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.longDelay.heatIncEvts |= PHASE_A_BITMASK;
	}
	else
	{
		if(currProtectorCfg.longDelay.isInverseTime)
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
			isCountDownA = false;
		}
        currProtectorCfg.longDelay.heatIncEvts  &= ~PHASE_A_BITMASK;
	}

	/* B */
	if(breakerInfo->ib.an > idz)
	{
		if(currProtectorCfg.longDelay.isInverseTime)
		{
			sqr6Ir1AnTs = (double) ( ((double)gearIr1Mul6 / (double)breakerInfo->ib.an) * ((double)gearIr1Mul6 / (double)breakerInfo->ib.an) * (double)currProtectorCfg.longDelay.tsMs );	
			qDlt =  (double)INVERSE_TIME_Q_MAX_STEP / sqr6Ir1AnTs;
			Qb += qDlt;
			if(Qb >= INVERSE_TIME_Q_MAX)
			{
                if(!IsFactoryMode())
                {
                    actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_B_BITMASK);	
                    if(actionFlag)
                    {
                        isProtected = true;
                        return true;
                    }
                }
			}
		}
		else
		{
			Qb = 0;
			if(!isCountDownB)
			{
				isCountDownB = true;
				countDownNumB = currProtectorCfg.longDelay.tsMs / (1000/AN_COUNT_FREQ);
			}
			else
			{
				countDownNumB--;
				if(countDownNumB<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_B_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
					#if LONG_DELAY_LOG
						log_t("LongDelay - fixed protecter do\r\n");
					#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.longDelay.heatIncEvts |= PHASE_B_BITMASK;
	}
	else
	{
		if(currProtectorCfg.longDelay.isInverseTime)
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
			isCountDownB = false;
		}
        currProtectorCfg.longDelay.heatIncEvts &= ~PHASE_B_BITMASK;
	}

	/* C */
	if(breakerInfo->ic.an > idz)
	{
		if(currProtectorCfg.longDelay.isInverseTime)
		{
			sqr6Ir1AnTs = (double) ( ((double)gearIr1Mul6 / (double)breakerInfo->ic.an) * ((double)gearIr1Mul6 / (double)breakerInfo->ic.an) * (double)currProtectorCfg.longDelay.tsMs );	
			qDlt =  (double)INVERSE_TIME_Q_MAX_STEP / sqr6Ir1AnTs;
			Qc += qDlt;
			if(Qc >= INVERSE_TIME_Q_MAX)
			{
                if(!IsFactoryMode())
                {
                    actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_C_BITMASK);
                    if(actionFlag)
                    {
                        isProtected = true;
                        return true;
                    }
                }
			}
		}
		else
		{
			Qc = 0;
			if(!isCountDownC)
			{
				isCountDownC = true;
				countDownNumC = currProtectorCfg.longDelay.tsMs / (1000/AN_COUNT_FREQ);
			}
			else
			{
				countDownNumC--;
				if(countDownNumC<=0)
				{
					actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_C_BITMASK);
					if(actionFlag)
					{
						isProtected = true;
						#if LONG_DELAY_LOG
						log_t("LongDelay - fixed protecter do\r\n");
						#endif
						return true;
					}
				}
			}
		}
        currProtectorCfg.longDelay.heatIncEvts |= PHASE_C_BITMASK;
	}
	else
	{
		if(currProtectorCfg.longDelay.isInverseTime)
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
			isCountDownC = false;
		}
        currProtectorCfg.longDelay.heatIncEvts &= ~PHASE_C_BITMASK;
	}
    
    if(IsFactoryMode())
    {
        if( (Qa>=INVERSE_TIME_Q_MAX) && (Qb>=INVERSE_TIME_Q_MAX) && (Qc>=INVERSE_TIME_Q_MAX) )
        {
						#if (DEV_TYPE_250A == DEV_TYPE)
             if((breakerInfo->ia.an > 270)&&(breakerInfo->ia.an < 330)&&(breakerInfo->ib.an > 270)&&(breakerInfo->ib.an < 330)&&(breakerInfo->ic.an > 270)&&(breakerInfo->ic.an < 330))
								{
										actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_C_BITMASK);
										if(actionFlag)
										{
												isProtected = true;
												return true;
										}								
								}
						#elif (DEV_TYPE_400A == DEV_TYPE)
             if((breakerInfo->ia.an > 432)&&(breakerInfo->ia.an < 528)&&(breakerInfo->ib.an > 432)&&(breakerInfo->ib.an < 528)&&(breakerInfo->ic.an > 432)&&(breakerInfo->ic.an < 528))
								{
										actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_C_BITMASK);
										if(actionFlag)
										{
												isProtected = true;
												return true;
										}								
								}
						#elif (DEV_TYPE_630A == DEV_TYPE)
             if((breakerInfo->ia.an > 675)&&(breakerInfo->ia.an < 825)&&(breakerInfo->ib.an > 675)&&(breakerInfo->ib.an < 825)&&(breakerInfo->ic.an > 675)&&(breakerInfo->ic.an < 825))
								{
										actionFlag = SwitchOffProtector(SWITCH_WARN_REASON_OVERLOAD, PHASE_C_BITMASK);
										if(actionFlag)
										{
												isProtected = true;
												return true;
										}								
								}
						#endif

        }
    }

	return false;
}



void LongDelayProtectorReInit(void)
{
	currProtectorCfg.longDelay.gear = CURRENT_IN_A;
	currProtectorCfg.longDelay.tsMs = 4000;
	currProtectorCfg.longDelay.isInverseTime = false;
	isProtected = false;
}


bool LongDelayHandler(const BreakerParaInfoDef *const breakerInfo)
{
	bool retVal = LongDelayProtector( breakerInfo );

	return retVal;
}























