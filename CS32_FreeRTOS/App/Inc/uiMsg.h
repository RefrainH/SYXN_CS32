#ifndef UI_MSG_H
#define UI_MSG_H


#include <stdint.h>



typedef struct
{
	uint32_t msgId;
	uint8_t arg0;
	uint8_t arg1;
}UiMsgDef;



typedef enum
{
	MSG_UI,
	MSG_KEY,
	MSG_TIMER,
	MSG_EVENT,
	
}MsgIdEnum;


typedef enum
{
	MSG_UI_FROM_PARENT,
	MSG_UI_FROM_CHILD,
	
}MsgUiEnumDef;

typedef enum
{
	MSG_KEY_NULL,
	MSG_KEY_FUN_CONFIRM,
	MSG_KEY_UP, 
	MSG_KEY_DOWN,
	MSG_KEY_BACK,
	MSG_KEY_LEAK_SWITCH_TEST,
	MSG_KEY_SWITH,
}MsgKeyEnumDef;

typedef enum
{
	MSG_EVT_NULL,
	MSG_EVT_SWITCH_ON,
	MSG_EVT_SWITCH_OFF,
	MSG_EVT_WARN,

}MsgEventEnumDef;






void UiMsgCreate(void);
long UiMsgPut(	uint32_t msgId, uint8_t arg0, uint8_t arg1);
long UiMsgGet(UiMsgDef *msg);

#endif


