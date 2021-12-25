#include "breaker.h"
#include "log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "breakerIo.h"
#include "string.h"
#include "currProtector.h"
#include "breakerIo.h"
#include "currProtectorLongDelay.h"
#include "currProtectorShortDelay.h"
#include "currProtectorShortInstant.h"
#include "tim.h"
#include "iwdg.h"
#include "about.h"
#include "uiMsg.h"

#include "bsp.h"

extern osThreadId TaskEvtsHandle;



#define SWITCH_OFF_TIMEOUT_10MS_CNT	(100*60)
#define SWITCH_STATE_KEEP_CNT		60 //100








osTimerId reSwitchTimerHandle;
//static xQueueHandle  QueueBreakerMsgHandle;

static uint8_t switchStatePre = 0;

#if 0
void BreakerMsgCreate(void)
{
    QueueBreakerMsgHandle = xQueueCreate( 10, sizeof(MsgDef));
    if(NULL == QueueBreakerMsgHandle)
    {
        log_t("Key proc - Key message create fail\r\n");
    }
}
#endif

#if 0
long BreakerMsgPut(	uint32_t msgId, uint8_t arg0, uint8_t arg1)
{
    BaseType_t xStatus = pdFAIL;
    MsgDef uiMsg;
    uiMsg.msgId = msgId;
    uiMsg.arg0 = arg0;
    uiMsg.arg1 = arg1;
    
    xStatus = xQueueSend( QueueBreakerMsgHandle, &uiMsg, (200/portTICK_PERIOD_MS) );
#if LOG_ON
    if(pdTRUE==xStatus)
    {
    	log_t("Put breaker msg success: msgId: %u \targ0: %d \targ1: %d\r\n", uiMsg.msgId, uiMsg.arg0, uiMsg.arg1);
    }
    else
    {
    	log_t("Put breaker msg fail: msgId: %u \targ0: %d \targ1: %d\r\n", uiMsg.msgId, uiMsg.arg0, uiMsg.arg1);
    }
#endif

    return  xStatus;
}
#endif

#if 0
long BreakerMsgPutFromISR(	uint32_t msgId, uint8_t arg0, uint8_t arg1)
{
    BaseType_t xStatus = pdFAIL;
    MsgDef uiMsg;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    
    uiMsg.msgId = msgId;
    uiMsg.arg0 = arg0;
    uiMsg.arg1 = arg1;
    
    
    xQueueSendFromISR( QueueBreakerMsgHandle, &uiMsg, &xHigherPriorityTaskWoken );

    return  xStatus;
}
#endif

#if 0
long BreakerMsgGet(MsgDef *msg)
{
    BaseType_t xStatus = pdFAIL;
    
    xStatus = xQueueReceive( QueueBreakerMsgHandle, msg, (2000/portTICK_PERIOD_MS) );
    #if LOG_ON
    if(pdTRUE==xStatus)
    {
    	log_t("Rcv breaker msg success: msgId: %u \targ0: %d \targ1: %d\r\n", msg->msgId, msg->arg0, msg->arg1);
	}
	else
	{
		log_t("Rcv breaker msg fail: msgId: %u \targ0: %d \targ1: %d\r\n", msg->msgId, msg->arg0, msg->arg1);
	}
	#endif
	
    return  xStatus;
}
#endif


/* 清除三段式保护标志位 */
static void ClrBreakerProtectorFlags(void)
{
	ClrLongDelayProtectFlag();
	ClrShortDelayProtectFlag();
	ClrShortInstantProtectFlag();
}

void SwitchOff(void)
{
    TkOn();
    osDelay(1000);
	ClrBreakerProtectorFlags();
}

bool SwitchOffProtector(SwitchWarnReasonEnum reason, uint8_t phase)
{
	SwitchOff();
    
	return true;
}

void BreakerProtectorInit(void)
{
	/* 清除三段式保护标志位 */
	ClrBreakerProtectorFlags();
	/* 读取外部试跳state的引脚电平状态 */
	switchStatePre = GetSwitchIoState();

	printf("State GpioPin is: %d \r\n",switchStatePre);	

}

void SwitchChgOn2OffHandler( void )
{
	ClrBreakerProtectorFlags();
}

void SwitchChgOff2OnHandler( void )
{
	ClrBreakerProtectorFlags();
}

void SwitchCtrlHandler(void)
{
	static uint8_t keepCnt = 0;
	uint8_t switchStateCurr = GetSwitchIoState();
	
	if(switchStatePre != switchStateCurr)
	{
		keepCnt++;
		if(keepCnt >= SWITCH_STATE_KEEP_CNT)
		{
			if(switchStateCurr)
			{
				SwitchChgOff2OnHandler();
			}
			else
			{
				SwitchChgOn2OffHandler();
			}
			switchStatePre = switchStateCurr;
			keepCnt = 0;
		}
	}
	else
	{
		keepCnt = 0;
	}

}

void BreakerHandler(const BreakerParaInfoDef *const breakerInfo)
{	
	CurrProtectorHandler(breakerInfo);
}

#if 0
void BreakerProc(void) 
{
	MsgDef msg;
	BreakerMsgGet(&msg);

	switch(msg.msgId)
	{
		case MSG_EVENT:
			switch(msg.arg0)
			{
				case MSG_EVT_SWITCH_OFF:
					SwitchOff();
					break;
					
				default:
					break;
			}
			break;

		default:
			break;
	}
}
#endif

