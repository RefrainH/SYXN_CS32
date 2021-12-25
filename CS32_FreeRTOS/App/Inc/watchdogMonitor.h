#ifndef WDG_MONITOR_H
#define WDG_MONITOR_H

#include <stdint.h>

#define WDG_MONITOR_EVT_GROUP_TASK_EVT		(1<<0)
#define WDG_MONITOR_EVT_GROUP_TASK_ADC		(1<<1)
#define WDG_MONITOR_EVT_GROUP_TASK_KEY		(1<<2)
#define WDG_MONITOR_EVT_GROUP_TASK_GUI		(1<<3)
#define WDG_MONITOR_EVT_GROUP_TASK_DLT20	(1<<4)
#define WDG_MONITOR_EVT_GROUP_TASK_SWITCH	(1<<5)


#define WDG_MONITOR_EVT_GROUP_ALL ( WDG_MONITOR_EVT_GROUP_TASK_ADC )





void WdgMonitorInit(void);
void WdgMonitorProc( void );
void WdgMonitorEvtGroupSetBits( uint32_t bits );

#endif

