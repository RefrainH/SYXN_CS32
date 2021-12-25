#ifndef CURR_PROTECTOR_LONG_DELAY_H
#define CURR_PROTECTOR_LONG_DELAY_H


#include <stdint.h>
#include "breakerAdc.h"
#include <stdbool.h>

#define LONG_DELAY_LOG	0









uint16_t GetLongDelayIr1(void);
uint8_t GetLongDelayProtectorGearIdx(void);
void SetLongDelayProtectorGearIdx(uint8_t idx);
uint8_t GetLongDelayProtectorDelayIdx(void);
void SetLongDelayProtectorDelayIdx(uint8_t idx);
bool IsLongDelayInverseTime(void);
void SetLongDelayIsInverseTime(bool isInverse);
bool LongDelayProtector(const BreakerParaInfoDef *const breakerInfo);
void LongDelayProtectorReInit(void);
void ClrLongDelayProtectFlag(void);
uint8_t GetLongDelayT1sIdx(uint16_t ms);
bool LongDelayHandler(const BreakerParaInfoDef *const breakerInfo);
void SetLongDelayIr1(uint16_t ir1);
uint16_t GetLongDelayT1Ms( void );






#endif

