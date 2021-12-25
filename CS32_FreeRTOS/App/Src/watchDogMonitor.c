#include "watchdogMonitor.h"
#include "iwdg.h"
#include "freeRTOS.h"
#include "event_groups.h"
#include <stdint.h>

EventGroupHandle_t evtGroupWdgMonitor = NULL;

void WdgMonitorInit(void)
{
	if( NULL != evtGroupWdgMonitor )	
	{
		return;
	}
	evtGroupWdgMonitor = xEventGroupCreate();
	if(NULL == evtGroupWdgMonitor)
	{
		//create evt group fail
	}
}


void WdgMonitorProc( void )
{
 	EventBits_t uxBits;

	uxBits = xEventGroupWaitBits(evtGroupWdgMonitor, 
								WDG_MONITOR_EVT_GROUP_ALL, 
								pdTRUE, 
								pdTRUE, 
								(1000*10/portTICK_PERIOD_MS));

	if( uxBits & WDG_MONITOR_EVT_GROUP_ALL == WDG_MONITOR_EVT_GROUP_ALL)
	{
		IwdgFeed();
	}
}


void WdgMonitorEvtGroupSetBits( uint32_t bits )
{
	xEventGroupSetBits( evtGroupWdgMonitor, bits );
}













