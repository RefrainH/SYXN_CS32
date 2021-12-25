#include "bsp.h"
#include "about.h"
#include "string.h"


CalibMeterInfoExDef calibMeterEx;



void CalibMeterReInit( void )
{
    /* ia */
    calibMeterEx.ia.frist.An = CALIB_BIG_CURR_POINT_1_AN;
    calibMeterEx.ia.frist.fftAn = CALIB_BIG_CURR_POINT_1_RAW_DEFAULT;
    calibMeterEx.ia.second.An = CALIB_BIG_CURR_POINT2_AN;
    calibMeterEx.ia.second.fftAn = CALIB_BIG_CURR_POINT2_RAW_DEFAULT;

    /* ib */
    calibMeterEx.ib.frist.An = CALIB_BIG_CURR_POINT_1_AN;
    calibMeterEx.ib.frist.fftAn = CALIB_BIG_CURR_POINT_1_RAW_DEFAULT;
    calibMeterEx.ib.second.An = CALIB_BIG_CURR_POINT2_AN;
    calibMeterEx.ib.second.fftAn = CALIB_BIG_CURR_POINT2_RAW_DEFAULT;

    /* ic */
    calibMeterEx.ic.frist.An = CALIB_BIG_CURR_POINT_1_AN;
    calibMeterEx.ic.frist.fftAn = CALIB_BIG_CURR_POINT_1_RAW_DEFAULT;
    calibMeterEx.ic.second.An = CALIB_BIG_CURR_POINT2_AN;
    calibMeterEx.ic.second.fftAn = CALIB_BIG_CURR_POINT2_RAW_DEFAULT;
}



