#ifndef CURR_PROTECTOR_H
#define CURR_PROTECTOR_H


#include <stdint.h>
#include "breakerAdc.h"
#include "stdbool.h"

#define ICW_DELAY_MS			750
#define SHORT_INSTANT_CNT_DEF	3



#define DELAY_ACTION_PERCENT		(110)
#define SHORT_DELAY_ACTION_PERCENT	(100)

#define INVERSE_TIME_Q_MAX		(90000)
#define Q_DECAY_S				(15*60)

#define AN_COUNT_PERIOD_DECAY	(INVERSE_TIME_Q_MAX/Q_DECAY_S/AN_COUNT_FREQ)

#define INVERSE_TIME_Q_MAX_STEP	(INVERSE_TIME_Q_MAX*1000/AN_COUNT_FREQ)


#pragma pack(1)
typedef struct
{
	uint16_t gear;  //短瞬: 百分比
	bool isProtected;
    bool isEnable;
    uint8_t heatIncEvts;
}ShortInstantProtectorDef;

typedef struct
{
	//bool	isOnoff;
	uint16_t gear;          //长延时：实际档位值；短延时：百分比；
	uint16_t tsMs;			//短延时时间设定值ts
	bool isInverseTime; 	//反时限
	//bool isHeatRemember;	//热记忆
	bool isEnable;
    uint8_t heatIncEvts;  
}DelayProtectorDef;

typedef struct
{
    bool isEnable;
    uint16_t ir1Percent;
    bool isInAlarm;
}OverloadWarningDef;


typedef struct
{
	DelayProtectorDef longDelay;
	DelayProtectorDef shortDelay;
	ShortInstantProtectorDef shortInstant;
    OverloadWarningDef overloadWarning;
}CurrProtectorCfgDef;


#pragma pack()





extern CurrProtectorCfgDef currProtectorCfg;




void CurrProtectorHandler(const BreakerParaInfoDef *const breakerInfo);
void CurrProtectorReInit(void);
uint32_t GetIcwDelayCnt(float an);
void CurrProtectorStatusLed(void);
bool IsFactoryMode(void);
void PrintSysInfo( void );




#endif
