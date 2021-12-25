#ifndef CURR_PROTECTOR_SHORT_INSTANT_H
#define CURR_PROTECTOR_SHORT_INSTANT_H


#include <stdint.h>
#include <stdbool.h>
#include "breakerAdc.h"


#define SHORT_INSTANT_ACTION_PERCENT	100
#define SHORT_INSTANT_LOG	0



typedef enum
{
	IR3_MUL_IR1_4,
	IR3_MUL_IR1_5,
	IR3_MUL_IR1_6,
	IR3_MUL_IR1_7,
	IR3_MUL_IR1_8,
	IR3_MUL_IR1_9,
	IR3_MUL_IR1_10,
	IR3_MUL_IR1_11,
	IR3_MUL_IR1_12,
	IR3_MUL_IR1_13,
	IR3_MUL_IR1_14,


	IR3_MUL_IR1_CNT,
	IR3_MUL_IR1_LAST=IR3_MUL_IR1_CNT-1,
}Ir3MulIr1Enum;



bool shortInstantProtector(const BreakerParaInfoDef *const breakerInfo);
void SetShortInstantGear(uint16_t percent);
uint16_t GetShortInstantGear(void);
uint8_t GetShortInstantProtectorGearIdx(void);
void SetShortInstantProtectorGearIdx(uint8_t idx);
void ShortInstantProtectorReInit(void);
void ClrShortInstantProtectFlag(void);
uint8_t GetIr3DivIr1Idx( uint16_t div );
bool ShortInstantHandler(const BreakerParaInfoDef *const breakerInfo);



#endif
