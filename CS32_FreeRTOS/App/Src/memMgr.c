#include "memMgr.h"
#include "currProtector.h"
#include "string.h"
#include "usrLib.h"
#include "calibMeterMem.h"
#include "currProtectorLongDelay.h"
#include "currProtectorShortDelay.h"
#include "currProtectorShortInstant.h"
#include "about.h"
#include "breakerIo.h"


void MemMgrInit(void)
{
	CalibMeterReInit();
}


