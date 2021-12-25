#ifndef CALIB_METER_MEM_H
#define CALIB_METER_MEM_H

#include <stdint.h>
#include "about.h"



//big curr (second point calib)

//#define CALIB_BIG_CURR_POINT2		250
#define CALIB_METER_ITEM_CURR_SMALL_DEFAULT			100
#define CALIB_METER_ITEM_IDN_HIGH_FRIST_DEFAULT		100
#define CALIB_METER_ITEM_IDN_HIGH_SECOND_DEFAULT	400
#define CALIB_METER_ITEM_IDN_LOW_FRIST_DEFAULT		0	
#define CALIB_METER_ITEM_IDN_LOW_SECORD_DEFAULT		CALIB_METER_ITEM_IDN_HIGH_FRIST_DEFAULT


#pragma pack(1)
typedef struct
{
	float fftAn;
	float An;
}calibParaDef;

typedef struct
{
	calibParaDef frist;
	calibParaDef second;
}CalibInfoDef;

typedef struct
{
	CalibInfoDef ia;
	CalibInfoDef ib;
	CalibInfoDef ic;
  /* add extended para here */
    
}CalibMeterInfoExDef; /* !!!only support extension for keeping compatibility */


#pragma pack()


extern CalibMeterInfoExDef  calibMeterEx;



void CalibMeterReInit( void );

#endif
