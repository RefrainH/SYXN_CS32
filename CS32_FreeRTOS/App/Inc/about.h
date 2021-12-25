#ifndef ABOUT_H
#define ABOUT_H


#define DEV_MODEL_BREAKER			0
#define DEV_MODEL_SUKE				1

#define DEV_TYPE_160A				0
#define DEV_TYPE_250A				1
#define DEV_TYPE_400A				2
#define DEV_TYPE_630A				3
#define DEV_TYPE_800A				4

/* devecie model & type config */
#define DEV_MODEL					DEV_MODEL_SUKE
#define DEV_TYPE					DEV_TYPE_250A

/* version contrl */
#define SW_VER_INFO  "JSRCD-SOFT-A036115-2021-A1.42.84 "
#define SW_VER       "A1.42.84"
#define SW_VER_INNER "-00"
#define HW_VER_INFO  "JSRCD-HARD-A036115-2021-1.01.00 "

/* RTC calibration */
#define RTC_CALIB_VAL	            0x3A


/*************************************************************/

#define EDDY  "AC380V"  							//额定电压
#define KJDL  "AC250A"  							//壳架电流
#define SCRQ  "2019-3-24"							//生产日期
#define XYBB  "DL/T 2018"    						//通信协议版本
#define GCDM  "A036115                 "         	//工厂代码
#define HZSJ  "20s~60s"      						//合闸时间
#define DZTX  "AC"           						//剩余电流动作特性

/* series name */
#if (DEV_MODEL==DEV_MODEL_SUKE)
#define SERIES_NAME 	"SYM5E-"
#elif (DEV_MODEL==DEV_MODEL_BREAKER)
#define SERIES_NAME 	"SYM5EL-"
#endif

/* calibration */
#if (DEV_TYPE_160A == DEV_TYPE)
#define CURRENT_IN_A				160
#define IR1_MIN						60
#define IR1_MAX						160
/* calib default */
#define CALIB_BIG_CURR_POINT_1_AN				100
#define CALIB_BIG_CURR_POINT_1_RAW_DEFAULT		125
#define CALIB_BIG_CURR_POINT2_AN				200
#define CALIB_BIG_CURR_POINT2_RAW_DEFAULT		273

#elif (DEV_TYPE_250A == DEV_TYPE)
#define CURRENT_IN_A				250
#define IR1_MIN						100
#define IR1_MAX						250
/* calib default */
#define CALIB_BIG_CURR_POINT_1_AN				80
#define CALIB_BIG_CURR_POINT_1_RAW_DEFAULT		48.071f
#define CALIB_BIG_CURR_POINT2_AN				500
#define CALIB_BIG_CURR_POINT2_RAW_DEFAULT		317.305f

#elif (DEV_TYPE_400A == DEV_TYPE)
#define CURRENT_IN_A				400
#define IR1_MIN						160
#define IR1_MAX						400
/* calib default */
#define CALIB_BIG_CURR_POINT_1_AN				80
#define CALIB_BIG_CURR_POINT_1_RAW_DEFAULT		36.9344f
#define CALIB_BIG_CURR_POINT2_AN				800
#define CALIB_BIG_CURR_POINT2_RAW_DEFAULT		378.557f

#elif (DEV_TYPE_630A == DEV_TYPE)
#define CURRENT_IN_A				630
#define IR1_MIN						250
#define IR1_MAX						630
/* calib default */
#define CALIB_BIG_CURR_POINT_1_AN				80
#define CALIB_BIG_CURR_POINT_1_RAW_DEFAULT		20.8533f
#define CALIB_BIG_CURR_POINT2_AN				800
#define CALIB_BIG_CURR_POINT2_RAW_DEFAULT		197.7367f

#elif (DEV_TYPE_800A == DEV_TYPE)
#define CURRENT_IN_A				800
#define IR1_MIN						320
#define IR1_MAX						800
/* calib default */
#define CALIB_BIG_CURR_POINT_1_AN				100
#define CALIB_BIG_CURR_POINT_1_RAW_DEFAULT		67
#define CALIB_BIG_CURR_POINT2_AN				800
#define CALIB_BIG_CURR_POINT2_RAW_DEFAULT		554

#endif

#define LONG_DELAY_T1_MS_MIN		3000
#define LONG_DELAY_T1_MS_MAX		18000




#endif
