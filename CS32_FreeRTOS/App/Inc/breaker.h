#ifndef BREAKER_H
#define BREAKER_H

#include <stdint.h>
#include <stdbool.h>
#include "breakerAdc.h"
#include "time.h"


#define WARN_DISSHARKE_MS	500
#define WARN_DISSHARKE_CNT   (WARN_DISSHARKE_MS/(1000/AN_COUNT_FREQ))

typedef enum
{
	SWITCH_CTRL_STATE_NULL,
	SWITCH_CTRL_STATE_OFFING,
	SWITCH_CTRL_STATE_ONING,
	SWITCH_CTRL_STATE_OFF_SUCCESS,
	SWITCH_CTRL_STATE_OFF_FAIL,
	SWITCH_CTRL_STATE_ON_SUCCESS,
	SWITCH_CTRL_STATE_ON_FAIL,
	
}SwitchCtrlStateEnum;

typedef struct
{
	uint32_t msgId;
	uint8_t arg0;
	uint8_t arg1;
}MsgDef;

typedef enum
{
	SWITCH_WARN_REASON_NORMAL = 0x00,
	SWITCH_WARN_REASON_IDN_WARN = 0x01,
	SWITCH_WARN_REASON_IDN = 0x02,
	SWITCH_WARN_REASON_LACKN = 0x04,
	SWITCH_WARN_REASON_OVERLOAD = 0x05,
	SWITCH_WARN_REASON_SHORT_DELAY = 0x06,
	SWITCH_WARN_REASON_LACKPHASE = 0x07,
	SWITCH_WARN_REASON_UNDERVOL = 0x08,
	SWITCH_WARN_REASON_OVERVOL = 0x09,
	SWITCH_WARN_REASON_GND = 0x0a,
	SWITCH_WARN_REASON_POWROFF = 0x0b,
	//SWITCH_WARN_REASON_FIXED_TEST = 0x0c,
	SWITCH_WARN_REASON_REMOTE = 0x0d,
	SWITCH_WARN_REASON_KEY = 0x0e,
	SWITCH_WARN_REASON_LOCK = 0x0f,
	SWITCH_WARN_REASON_HAND = 0x12,
	SWITCH_WARN_REASON_TRANSFORMER = 0x10,
	SWITCH_WARN_REASON_SWITCH_ON_FAIL = 0x11,
	SWITCH_WARN_REASON_SET_CHG = 0x13,
	SWITCH_WARN_REASON_SHORT_INSTANT = 0x16,
	SWITCH_WARN_REASON_SWITCH_OFF_FAIL = 0x17,
	SWITCH_WARN_REASON_RESWITCH_ON = 0x18,
	SWITCH_WARN_REASON_SOFT_CONTROL = 0x1E,
	SWITCH_WARN_REASON_HW_CONTROL = 0x1D,
}SwitchWarnReasonEnum; 

typedef struct
{
	uint8_t reson:5; 
	uint8_t switchState:2;
	uint8_t rsv:1;
}StateChar1;


typedef enum
{
	WARN_EVT_NULL     			= 0x00000000,
	WARN_EVT_NORMAL 			= 0x00000001,
	WARN_EVT_IDN_WARN 			= 0x00000002,
	WARN_EVT_IDN				= 0x00000004,
	WARN_EVT_LACKN				= 0x00000008,
	WARN_EVT_OVERLOAD			= 0x00000010,
	WARN_EVT_SHORT_DELAY		= 0x00000020,
	WARN_EVT_LACKPHASE			= 0x00000040,
	WARN_EVT_UNDERVOL			= 0x00000080,
	WARN_EVT_OVERVOL			= 0x00000100,
	WARN_EVT_GND				= 0x00000200,
	WARN_EVT_POWROFF			= 0x00000400,
	//WARN_EVT_FIXED_TEST = 0x0c,
	WARN_EVT_REMOTE				= 0x00000800,
	WARN_EVT_KEY				= 0x00001000,
	WARN_EVT_LOCK				= 0x00002000,
	WARN_EVT_HAND				= 0x00004000,
	WARN_EVT_TRANSFORMER		= 0x00008000,
	WARN_EVT_ON_FAIL			= 0x00010000,
	WARN_EVT_SET_CHG			= 0x00020000,
	WARN_EVT_SHORT_INSTANT		= 0x00040000,
	WARN_EVT_OFF_FAIL			= 0x00080000,
	WARN_EVT_RESWITCH_ON		= 0x00100000,
	WARN_EVT_SOFT_CONTROL		= 0x00200000,
	WARN_EVT_HW_CONTROL			= 0x00400000,
}WarnEvtEnum;


#define CHECKSELF_TYPE_FIXED	0x0C
#define CHECKSELF_TYPE_ORDER	0x0D

#define LEAK_TEST_REASON_KEY		0
#define LEAK_TEST_REASON_REMOTE		1


bool SwitchOffProtector(SwitchWarnReasonEnum reason, uint8_t phase);
void BreakerProtectorInit(void);
void ToggleSwitch(void);
void ReSwitchOn(SwitchWarnReasonEnum reason);
void BreakerHandler(const BreakerParaInfoDef *const breakerInfo);
//long BreakerMsgPut(	uint32_t msgId, uint8_t arg0, uint8_t arg1);
//long BreakerMsgPutFromISR(	uint32_t msgId, uint8_t arg0, uint8_t arg1);
//long BreakerMsgGet(MsgDef *msg);
//void BreakerProc(void);


void SetBreakerSwitchState(uint8_t state);
//void BreakerMsgCreate(void);
SwitchCtrlStateEnum GetSwitchCtrlState( void );
void SwitchChgOn2OffHandler( void );
void SwitchChgOff2OnHandler( void );
void SwitchCtrlHandler(void);



#endif

