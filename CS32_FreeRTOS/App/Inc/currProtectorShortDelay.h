#ifndef CURR_PROTECTOR_SHORT_DELAY_H
#define CURR_PROTECTOR_SHORT_DELAY_H



#include <stdint.h>
#include <stdbool.h>
#include "currProtector.h"

#define SHORT_DELAY_LOG		0

#define PROTECT_COST_MS		25 

typedef enum
{
	IR2_MUL_2_IR1,
	IR2_MUL_3_IR1,
	IR2_MUL_4_IR1,
	IR2_MUL_5_IR1,
	IR2_MUL_6_IR1,
	IR2_MUL_7_IR1,
	IR2_MUL_8_IR1,
	IR2_MUL_9_IR1,
	IR2_MUL_10_IR1,
	IR2_MUL_11_IR1,
	IR2_MUL_12_IR1,

	IR2_MUL_IR1_CNT,
	IR2_MUL_IR1_LAST=IR2_MUL_IR1_CNT-1,
}IR2MulIr1Enum;


typedef enum
{
	IR2_DELAY_MS_100,
	IR2_DELAY_MS_200,
	IR2_DELAY_MS_300,
	IR2_DELAY_MS_400,
	IR2_DELAY_MS_500,

	IR2_DELAY_MS_CNT,
	IR2_DELAY_MS_LAST=IR2_DELAY_MS_CNT-1,
}IR2DelayEnum;







bool IsShortDelayProtectorOn(void);
void SetShortDelayProtectorState(bool isOn);
uint8_t GetShortDelayProtectorGearIdx(void);
void SetShortDelayProtectorGearIdx(uint8_t idx);
uint8_t GetShortDelayProtectorDelayIdx(void);
void SetShortDelayProtectorDelayIdx(uint8_t idx);
bool IsShortDelayInverseTime(void);
void SetShortDelayIsInverseTime(bool isInverse);
bool ShortDelayProtector(const BreakerParaInfoDef *const breakerInfo);
void ShortDelayProtectorReInit(void);
void ClrShortDelayProtectFlag(void);
uint8_t GetIr2DivIr1Idx( uint16_t div );
uint8_t GetShortDelayT2mSIdx(uint16_t ms);
void SetShortDelayT2mS(        uint16_t ms );
bool ShortDelayHandler(const BreakerParaInfoDef *const breakerInfo);






#endif

