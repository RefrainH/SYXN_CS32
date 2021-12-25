/**
  * @file    cs32f0xx_rtc.h  
  * @brief   This file contains all the functions prototypes for the RTC firmware 
  *          library. This is not applicable for CS32F036 devices.
  * @author  ChipSea MCU Group
  * @version V1.0.1
  * @date   2020.12.17         
  * @copyright CHIPSEA TECHNOLOGIES (SHENZHEN) CORP.
  * @note 
  * <h2><center>&copy; COPYRIGHT 2020 ChipSea</center></h2>
  *
  */

#ifndef __CS32F0XX_RTC_H__
#define __CS32F0XX_RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "cs32f0xx.h"

/** @addtogroup CS32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup RTC
  * @{
  */ 


/** 
  * @struct rtc_init_t
  * @brief RTC Init structures definition  
  */ 
typedef struct
{
    uint32_t hour_format;    //!< Specifies the RTC hour format                             
    uint32_t rtc_divider1;   //!< Specifies the RTC first pre-divider factor  lower than 0x7F                             
    uint32_t rtc_divider2;   //!< Specifies the RTC second pre-divider factor lower than 0x1FFF                             
}rtc_init_t;

/** 
  * @struct rtc_time_t
  * @brief RTC Time structure definition  
  */
typedef struct
{
    uint8_t rtc_hours;    //!< Specifies the RTC hour, HourFormat_12 (0~11), HourFormat_24 (0~23)                      
    uint8_t rtc_minutes;  //!< Specifies the RTC minutes (0~59)                          
    uint8_t rtc_seconds;  //!< Specifies the RTC seconds (0~59)                        
    uint8_t am_pm_sel;    //!< Specifies the RTC AM/PM                        
}rtc_time_t; 

/** 
  * @struct rtc_date_t
  * @brief  RTC Date structure definition  
  */
typedef struct
{
    uint8_t rtc_weekday;  //!< Specifies weekday                        
    uint8_t rtc_month;    //!< Specifies month
    uint8_t rtc_date;     //!< Specifies date (value in the 1-31)
    uint8_t rtc_year;     //!< Specifies year (value in the 0-99)                         
}rtc_date_t;

/**
  * @struct rtc_alarm_t
  * @brief  RTC Alarm structure definition  
  */
typedef struct
{
    rtc_time_t alarm_time;       //!< Specifies the RTC alarm time members
    uint32_t alarm_mask;         //!< Specifies the RTC alarm masks                                     
    uint32_t date_weekday_sel;   //!< Specifies the RTC alarm is on date or weekday  
    uint8_t date_weekday;        //!< Specifies the RTC alarm date/weekday          
    uint8_t reserved0;
    uint16_t reserved1;
}rtc_alarm_t;

/** @defgroup RTC_Constants
  * @{
  */ 

/** 
  * @brief  RTC hour format definition  
  */ 
#define RTC_HOUR_FORMAT_24              ((uint32_t)0x00000000)  //!< Specifies the RTC Hour Format ,vaule in 0~23
#define RTC_HOUR_FORMAT_12              ((uint32_t)0x00000040)  //!< Specifies the RTC Hour Format, vaule in 0~11
#define RTC_HOUR_FORMAT_CHECK(format)   (((format) == RTC_HOUR_FORMAT_12) || \
                                         ((format) == RTC_HOUR_FORMAT_24))

/** 
  * @brief check first pre-divider factor
  */ 
#define RTC_PDIV1_CHECK(prediv)   ((prediv) <= 0x7F)  //!< pre-divider factor
 
/** 
  * @brief check second pre-divider factor
  */ 
#define RTC_PDIV2_CHECK(prediv)    ((prediv) <= 0x7FFF)  //!< pre-divider factor

/** 
  * @brief check rtc time definitions 
  */ 
#define RTC_HOUR12_CHECK(hour)            (((hour) > 0) && ((hour) <= 12))  //!< check hour
#define RTC_HOUR24_CHECK(hour)            ((hour) <= 23)                    //!< check hour
#define RTC_MINUTES_CHECK(minutes)        ((minutes) <= 59)                 //!< check minute
#define RTC_SECONDS_CHECK(seconds)        ((seconds) <= 59)                 //!< check second

/** 
  * @brief rtc AM/PM defintion 
  */ 
#define RTC_AM_HOUR12                     ((uint8_t)0x00)  //!< select AM
#define RTC_PM_HOUR12                     ((uint8_t)0x40)  //!< select PM
#define RTC_AM_PM_HOUR12_CHECK(pm) (((pm) == RTC_AM_HOUR12) || ((pm) == RTC_PM_HOUR12))

/** 
  * @brief year chenck
  */ 
#define RTC_YEAR_CHECK(year)              ((year) <= 99)  //!< check year 

/** @defgroup Month_Date_Definitions 
  * @{
  */ 
#define MONTH_JANUARY              ((uint8_t)0x01)  //!< January
#define MONTH_FEBRUARY             ((uint8_t)0x02)  //!< February
#define MONTH_MARCH                ((uint8_t)0x03)  //!< March
#define MONTH_APRIL                ((uint8_t)0x04)  //!< April
#define MONTH_MAY                  ((uint8_t)0x05)  //!< May
#define MONTH_JUNE                 ((uint8_t)0x06)  //!< Jun
#define MONTH_JULY                 ((uint8_t)0x07)  //!< July
#define MONTH_AUGUST               ((uint8_t)0x08)  //!< August
#define MONTH_SEPTEMBER            ((uint8_t)0x09)  //!< September 
#define MONTH_OCTOBER              ((uint8_t)0x10)  //!< October
#define MONTH_NOVEMBER             ((uint8_t)0x11)  //!< November
#define MONTH_DECEMBER             ((uint8_t)0x12)  //!< December
#define MONTH_CHECK(month)         (((month) >= 1) && ((month) <= 12))
#define DATE_CHECK(date)           (((date) >= 1) && ((date) <= 31))

/**
  * @}
  */ 

/** @defgroup WeekDay_Definitions 
  * @{
  */   
#define	WEEKDAY_MONDAY             ((uint8_t)0x01)  //!< Monday
#define	WEEKDAY_TUESDAY            ((uint8_t)0x02)  //!< Tuesday
#define	WEEKDAY_WEDNESDAY          ((uint8_t)0x03)  //!< Wednesday
#define	WEEKDAY_THURSDAY           ((uint8_t)0x04)  //!< Thursday
#define	WEEKDAY_FRIDAY             ((uint8_t)0x05)  //!< Friday
#define	WEEKDAY_SATURDAY           ((uint8_t)0x6)   //!< Saturday
#define	WEEKDAY_SUNDAY             ((uint8_t)0x07)  //!< Sunday
#define WEEKDAY_CHECK(weekday)  (((weekday) == WEEKDAY_MONDAY)    || \
                                 ((weekday) == WEEKDAY_TUESDAY)   || \
                                 ((weekday) == WEEKDAY_WEDNESDAY) || \
                                 ((weekday) == WEEKDAY_THURSDAY)  || \
                                 ((weekday) == WEEKDAY_FRIDAY)    || \
                                 ((weekday) == WEEKDAY_SATURDAY)  || \
                                 ((weekday) == WEEKDAY_SUNDAY))
/**
  * @}
  */ 

/** 
  * @brief rtc Date/WeekDay select Definitions
  */ 
#define RTC_DATE_WEEKDAY_SEL_DATE        ((uint32_t)0x00000000)   //!<select date 
#define RTC_DATE_WEEKDAY_SEL_WEEKDAY     ((uint32_t)0x40000000)   //!<select weekday 
#define RTC_DATE_WEEKDAY_SEL_CHECK(sel)  (((sel) == RTC_DATE_WEEKDAY_SEL_DATE) || \
                                          ((sel) == RTC_DATE_WEEKDAY_SEL_WEEKDAY))
                                            
/** 
  * @brief rtc alarm mask Definitions
  */ 
#define RTC_ALARM_MASK_NONE                ((uint32_t)0x00000000) //!< all fields are not masked 
#define RTC_ALARM_MASK_DATE_WEEKDAY        ((uint32_t)0x80000000) //!< date_weekday field s masked
#define RTC_ALARM_MASK_HOURS               ((uint32_t)0x00800000) //!< hours field is masked
#define RTC_ALARM_MASK_MINUTES             ((uint32_t)0x00008000) //!< minutes field is masked
#define RTC_ALARM_MASK_SECONDS             ((uint32_t)0x00000080) //!< seconds field is masked
#define RTC_ALARM_MASK_ALL                 ((uint32_t)0x80808080) //!< all fields are masked 
#define RTC_ALARM_MASK_CHECK(mask)  (((mask) & 0x7F7F7F7F) == (uint32_t)RESET)

/** 
  * @brief rtc alarm definitions.
  */ 
#define RTC_ALARM_ENABLE                       ((uint32_t)0x00000100)     //!< alarm field is enabled
#define RTC_ALARM_ENABLE_CHECK(alarm)      ((alarm) == RTC_ALARM_ENABLE)  //!< check enable flag
#define RTC_ALARM_CMD_CHECK(alarm)         (((alarm) & (RTC_ALARM_ENABLE)) != (uint32_t)RESET)

/** 
  * @brief rtc alarm SubSeconds masks definitions.
  */ 
#define RTC_SUB_SECOND_MASK_ALL           ((uint8_t)0x00)  //!< RTC_SSEC All Alarm  SSEC fields are masked. 
#define RTC_SUB_SECOND_MASK_SSEC14_1      ((uint8_t)0x01)  //!< SSEC[14:1] are don't care, Only SSEC[0] valid.                                                                
#define RTC_SUB_SECOND_MASK_SSEC14_2      ((uint8_t)0x02)  //!< SSEC[14:2] are don't care, Only SSEC[1:0]valid.                                                                 
#define RTC_SUB_SECOND_MASK_SSEC14_3      ((uint8_t)0x03)  //!< SSEC[14:3] are don't care, Only SSEC[2:0]valid.                                                                
#define RTC_SUB_SECOND_MASK_SSEC14_4      ((uint8_t)0x04)  //!< SSEC[14:4] are don't care, Only SSEC[3:0]valid.                                                                 
#define RTC_SUB_SECOND_MASK_SSEC14_5      ((uint8_t)0x05)  //!< SSEC[14:5] are don't care, Only SSEC[4:0]valid.                                                              
#define RTC_SUB_SECOND_MASK_SSEC14_6      ((uint8_t)0x06)  //!< SSEC[14:6] are don't care, Only SSEC[5:0]valid.                                                              
#define RTC_SUB_SECOND_MASK_SSEC14_7      ((uint8_t)0x07)  //!< SSEC[14:7] are don't care, Only SSEC[6:0]valid.                                                               
#define RTC_SUB_SECOND_MASK_SSEC14_8      ((uint8_t)0x08)  //!< SSEC[14:8] are don't care, Only SSEC[7:0]valid.                                                               
#define RTC_SUB_SECOND_MASK_SSEC14_9      ((uint8_t)0x09)  //!< SSEC[14:9] are don't care, Only SSEC[8:0]valid.                                                             
#define RTC_SUB_SECOND_MASK_SSEC14_10     ((uint8_t)0x0A)  //!< SSEC[14:10] are don't care, Only SSEC[9:0]valid.                                                               
#define RTC_SUB_SECOND_MASK_SSEC14_11     ((uint8_t)0x0B)  //!< SSEC[14:11] are don't care, Only SSEC[10:0]valid.                                                              
#define RTC_SUB_SECOND_MASK_SSEC14_12     ((uint8_t)0x0C)  //!< SSEC[14:12] are don't care, Only SSEC[11:0]valid.                                                                
#define RTC_SUB_SECOND_MASK_SSEC14_13     ((uint8_t)0x0D)  //!< SSEC[14:13] are don't care, Only SSEC[12:0]valid.                                                                
#define RTC_SUB_SECOND_MASK_SSEC14        ((uint8_t)0x0E)  //!< SSEC[14] are don't care, Only SSEC[13:0]valid.                                                               
#define RTC_SUB_SECOND_MASK_NONE          ((uint8_t)0x0F)  //!< SSEC[14:0] valid and must match to activate alarm. 
                                                               
#define RTC_SUB_SECOND_MASK_CHECK(mask)      (((mask) == RTC_SUB_SECOND_MASK_ALL)       || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_1)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_2)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_3)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_4)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_5)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_6)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_7)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_8)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_9)  || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_10) || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_11) || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_12) || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14_13) || \
                                              ((mask) == RTC_SUB_SECOND_MASK_SSEC14)    || \
                                              ((mask) == RTC_SUB_SECOND_MASK_NONE))

/**
  * @brief rtc SubSeconds value check
  */   
#define RTC_SUB_SECOND_VALUE_CHECK(value) ((value) <= 0x00007FFF)  //!< checek subsecond value 

/** 
  * @brief rtc wakeup timer clock predivider definitions.
  */
#define RTC_WAKEUP_CLOCK_RTCCLK_DIV16        ((uint32_t)0x00000000)  //!< predivider RTCCLK/16
#define RTC_WAKEUP_CLOCK_RTCCLK_DIV8         ((uint32_t)0x00000001)  //!< predivider RTCCLK/8
#define RTC_WAKEUP_CLOCK_RTCCLK_DIV4         ((uint32_t)0x00000002)  //!< predivider RTCCLK/4
#define RTC_WAKEUP_CLOCK_RTCCLK_DIV2         ((uint32_t)0x00000003)  //!< predivider RTCCLK/2
#define RTC_WAKEUP_CLOCK_SPRE_16BITS         ((uint32_t)0x00000004)  //!< clock selected (see RTC_CTR) (usually 1HZ)
#define RTC_WAKEUP_CLOCK_SPRE_17BITS         ((uint32_t)0x00000006)  //!< clock selected (see RTC_CTR) (usually 1HZ)
#define RTC_WAKEUP_CLOCK_CHECK(clock) (((clock) == RTC_WAKEUP_CLOCK_RTCCLK_DIV16) || \
                                       ((clock) == RTC_WAKEUP_CLOCK_RTCCLK_DIV8)  || \
                                       ((clock) == RTC_WAKEUP_CLOCK_RTCCLK_DIV4)  || \
                                       ((clock) == RTC_WAKEUP_CLOCK_RTCCLK_DIV2)  || \
                                       ((clock) == RTC_WAKEUP_CLOCK_SPRE_16BITS)  || \
                                       ((clock) == RTC_WAKEUP_CLOCK_SPRE_17BITS))
#define RTC_WAKEUP_COUNTER_CHECK(counter)  ((counter) <= 0xFFFF)

/** 
  * @brief rtc Time recording edge select
  */ 
#define RTC_RECORD_RISING_EDGE          ((uint32_t)0x00000000)  //!< select rising edge triggle
#define RTC_RECORD_FALLING_EDGE         ((uint32_t)0x00000008)  //!< select falling edge triggle

#define RTC_RECORD__EDGE_CHECK(edge) (((edge) == RTC_RECORD_RISING_EDGE) || \
                                      ((edge) == RTC_RECORD_FALLING_EDGE))

/** 
  * @brief rtc output source selection definitions
  */ 
#define RTC_OUTPUT_SOURCE_DISABLE             ((uint32_t)0x00000000)  //!< rtc output clock suorce disable
#define RTC_OUTPUT_SOURCE_ALARM               ((uint32_t)0x00200000)  //!< select alarm output clock suorce
#define RTC_OUTPUT_SOURCE_WAKEUP              ((uint32_t)0x00600000)  //!<  select wakeup output clock suorce.
 
#define RTC_OUTPUT_SOURCE_CHECK(output) (((output) == RTC_OUTPUT_SOURCE_DISABLE) || \
                                         ((output) == RTC_OUTPUT_SOURCE_ALARM)   || \
                                         ((output) == RTC_OUTPUT_SOURCE_WAKEUP))

/**   
  * @brief rtc output polarity definitions
  */ 
#define RTC_OUTPUT_POLARITY_High           ((uint32_t)0x00000000) //!< set alarm polarity output high
#define RTC_OUTPUT_POLARITY_LOW            ((uint32_t)0x00100000) //!< set alarm polarity output low

#define RTC_OUTPUT_POLARITY_CHECK(pol) (((pol) == RTC_OUTPUT_POLARITY_High) || \
                                        ((pol) == RTC_OUTPUT_POLARITY_LOW))

/** 
  * @brief rtc calibration output selection definitions
  */ 
#define RTC_CALI_OUTPUT_512HZ            ((uint32_t)0x00000000)  //!< select calibration output 512 HZ
#define RTC_CALI_OUTPUT_1HZ              ((uint32_t)0x00080000)  //!< select calibration output 1 HZ

#define RTC_CALI_OUTPUT_CHECK(output)  (((output) == RTC_CALI_OUTPUT_512HZ) || \
                                        ((output) == RTC_CALI_OUTPUT_1HZ))

/** 
  * @brief rtc calibration period definitions 
  */ 
#define RTC_CALI_PERIOD_32SECOND   ((uint32_t)0x00000000) //!< if RTCCLK = 32768 Hz, then the calibation period is 32s, else 2exp20 RTCCLK seconds. 
#define RTC_CALI_PERIOD_16SECOND   ((uint32_t)0x00002000) //!< if RTCCLK = 32768 Hz, then the calibation period is 16s, else 2exp19 RTCCLK seconds.                                                              
#define RTC_CALI_PERIOD_8SECOND    ((uint32_t)0x00004000) //!< if RTCCLK = 32768 Hz, then the claibation period is 8s, else 2exp18 RTCCLK seconds. 
#define RTC_CALI_PERIOD_CHECK(period) (((period) == RTC_CALI_PERIOD_32SECOND) || \
                                       ((period) == RTC_CALI_PERIOD_16SECOND) || \
                                       ((period) == RTC_CALI_PERIOD_8SECOND))                                         

/**
  * @brief rtc insert rtc clock definitions 
  */ 
#define RTC_CALI_INSERT_CLOCK_SET    ((uint32_t)0x00008000)  //!< set Insert RTC clock bit                                                                                                                             
#define RTC_CALI_INSERT_CLOCK_RESET  ((uint32_t)0x00000000)  //!< reset Insert RTC clock bit
#define RTC_CALI_INSERT_CLOCK_CHECK(plus) (((plus) == RTC_CALI_INSERT_CLOCK_SET) || \
                                           ((plus) == RTC_CALI_INSERT_CLOCK_RESET))

/** 
  * @brief swallow RTC clock value check
  */ 
#define  RTC_SWALLOW_CLOCK_VALUE_CHECK(value) ((value) <= 0x000001FF)  //!< check swallow clock value

/**  
  * @brief rtc daylight saving Definitions
  */ 
#define RTC_DAYLIGHT_SAVING_DEC1H   ((uint32_t)0x00020000)  //!< select winter time (sub one hour)
#define RTC_DAYLIGHT_SAVING_INC1H   ((uint32_t)0x00010000)  //!< select summer time (add one hour) 
#define RTC_DAYLIGHT_SAVING_CHECK(saving) (((saving) == RTC_DAYLIGHT_SAVING_DEC1H) || \
                                           ((saving) == RTC_DAYLIGHT_SAVING_INC1H))

#define RTC_DAYLIGHT_SAVING_FLAG_RESET        ((uint32_t)0x00000000) //!< no effect
#define RTC_DAYLIGHT_SAVING_FLAG_SET          ((uint32_t)0x00040000) //!< select daylight-saving

#define RTC_DAYLIGHT_SAVING_FLAG_CHECK(operation) (((operation) == RTC_DAYLIGHT_SAVING_FLAG_RESET) || \
                                                   ((operation) == RTC_DAYLIGHT_SAVING_FLAG_SET))

/** 
  * @brief rtc inbreak1 level Definitions
  */ 
#define RTC_INBREAK1_RISING_EDGE            ((uint32_t)0x00000000) //!< select rising edge trigger
#define RTC_INBREAK1_FALLING_EDGE           ((uint32_t)0x00000001) //!< select rising edge trigger
#define RTC_INBREAK1_LOW_LEVEL              ((uint32_t)0x00000000) //!< select low level trigger
#define RTC_INBREAK1_HIGH_LEVEL             ((uint32_t)0x00000001) //!< select high level trigger

#define RTC_INBREAK1_LEVEL_CHECK(trigger) (((trigger) == RTC_INBREAK1_RISING_EDGE)  || \
                                           ((trigger) == RTC_INBREAK1_FALLING_EDGE) || \
                                           ((trigger) == RTC_INBREAK1_LOW_LEVEL)    || \
                                           ((trigger) == RTC_INBREAK1_HIGH_LEVEL)) 

/** 
  * @brief rtc inbreak filter definitions
  */ 
#define RTC_INBREAK_FILTER_DISABLE   ((uint32_t)0x00000000) //!< inbreak filter is disabled 
#define RTC_INBREAK_FILTER_2SAMPLE   ((uint32_t)0x00000800) //!< filter is activated after 2 consecutive samples.                                                          
#define RTC_INBREAK_FILTER_4SAMPLE   ((uint32_t)0x00001000) //!< filter is activated after 4 consecutive samples.                                                          
#define RTC_INBREAK_FILTER_8SAMPLE   ((uint32_t)0x00001800) //!< filter is activated after 8 consecutive samples.
                                                         
#define RTC_INBREAK_FILTER_CHECK(filter) (((filter) == RTC_INBREAK_FILTER_DISABLE) || \
                                          ((filter) == RTC_INBREAK_FILTER_2SAMPLE) || \
                                          ((filter) == RTC_INBREAK_FILTER_4SAMPLE) || \
                                          ((filter) == RTC_INBREAK_FILTER_8SAMPLE))

/**  
  * @brief rtc inbreak sampling frequencies Definitions
  */ 
#define RTC_INBREAK_SAMPLE_FREQ_PDIV32768     ((uint32_t)0x00000000)  //!< sampling frequency =  RTCCLK / 32768                                                                      
#define RTC_INBREAK_SAMPLE_FREQ_PDIV16384     ((uint32_t)0x00000100)  //!< sampling frequency =  RTCCLK / 16384                                                                      
#define RTC_INBREAK_SAMPLE_FREQ_PDIV8192      ((uint32_t)0x00000200)  //!< sampling frequency =  RTCCLK / 8192                                                                      
#define RTC_INBREAK_SAMPLE_FREQ_PDIV4096      ((uint32_t)0x00000300)  //!< sampling frequency =  RTCCLK / 4096                                                                   
#define RTC_INBREAK_SAMPLE_FREQ_PDIV2048      ((uint32_t)0x00000400)  //!< sampling frequency =  RTCCLK / 2048                                                                     
#define RTC_INBREAK_SAMPLE_FREQ_PDIV1024      ((uint32_t)0x00000500)  //!< sampling frequency =  RTCCLK / 1024                                                                    
#define RTC_INBREAK_SAMPLE_FREQ_PDIV512       ((uint32_t)0x00000600)  //!< sampling frequency =  RTCCLK / 512                                                                     
#define RTC_INBREAK_SAMPLE_FREQ_PDIV256       ((uint32_t)0x00000700)  //!< sampling frequency =  RTCCLK / 256                                                                     

#define RTC_INBREAK_SAMPLE_FREQ_CHECK(freq) (((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV32768) || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV16384) || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV8192)  || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV4096)  || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV2048)  || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV1024)  || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV512)   || \
                                             ((freq) == RTC_INBREAK_SAMPLE_FREQ_PDIV256))
                                           
  /**  
  * @brief rtc inbreak precharge duration definitions
  */ 
#define RTC_INBREAK_PRECHARGE_1RTCCLK ((uint32_t)0x00000000)  //!< pre_charged before 1 RTCCLK cycle duration                                                                        
#define RTC_INBREAK_PRECHARGE_2RTCCLK ((uint32_t)0x00002000)  //!< pre_charged before 2 RTCCLK cycle duration                                                                       
#define RTC_INBREAK_PRECHARGE_4RTCCLK ((uint32_t)0x00004000)  //!< pre_charged before 4 RTCCLK cycle duration                                                                  
#define RTC_INBREAK_PRECHARGE_8RTCCLK ((uint32_t)0x00006000)  //!< pre_charged before 8 RTCCLK cycle duration
                                                                       
#define RTC_INBREAK_PRECHARGE_CHECK(duration) (((duration) == RTC_INBREAK_PRECHARGE_1RTCCLK) || \
                                               ((duration) == RTC_INBREAK_PRECHARGE_2RTCCLK) || \
                                               ((duration) == RTC_INBREAK_PRECHARGE_4RTCCLK) || \
                                               ((duration) == RTC_INBREAK_PRECHARGE_8RTCCLK))

/** 
  * @brief rtc inbreak(x) detect enable definitions 
  */ 
#define RTC_INBREAK1_ENABLE            RTC_MF_INBRK1E //!< inbreak1 detect enable                                               
#define RTC_INBREAK2_ENABLE            RTC_MF_INBRK2E //!< inbreak2 detect enable                                                     
#define RTC_INBREAK3_ENABLE            RTC_MF_INBRK3E //!< inbreak3 detect enable, only for cs32F072 devices                                                                                               
#define RTC_INBREAK_ENABLE_CHECK(tamper) ((((tamper) & (uint32_t)0xFFFFFFD6) == 0x00) && ((tamper) != (uint32_t)RESET))

/** 
  * @brief rtc output type definitions
  */ 
#define RTC_OUTPUT_OPEN_DRAIN           ((uint32_t)0x00000000) //!< output type open_drain
#define RTC_OUTPUT_PUSH_PULL            ((uint32_t)0x00040000) //!< output type push_pull
#define RTC_OUTPUT_TYPE_CHECK(type) (((type) == RTC_OUTPUT_OPEN_DRAIN) || \
                                     ((type) == RTC_OUTPUT_PUSH_PULL))

/** 
  * @brief rtc increase one second definitions
  */ 
#define RTC_ADJ_INC1S_RESET      ((uint32_t)0x00000000) //!< no effect
#define RTC_ADJ_INC1S_SET        ((uint32_t)0x80000000) //!< add one second to the clock/calendar
#define RTC_ADJ_INC1S_CHECK(sel) (((sel) == RTC_ADJ_INC1S_RESET) || \
                                  ((sel) == RTC_ADJ_INC1S_SET))

/** 
  * @brief rtc substract fraction Of second value
  */ 
#define RTC_ADJ_SSEC_CHECK(fs) ((fs) <= 0x00007FFF) //!<check substract a fraction of a second 

/** 
  * @brief rtc always on registers definitions
  */
#define RTC_AO0_DR                         ((uint32_t)0x00000000) //!< No. 0 rtc always on register
#define RTC_AO1_DR                         ((uint32_t)0x00000001) //!< No. 1 rtc always on register
#define RTC_AO2_DR                         ((uint32_t)0x00000002) //!< No. 2 rtc always on register
#define RTC_AO3_DR                         ((uint32_t)0x00000003) //! <No. 3 rtc always on register
#define RTC_AO4_DR                         ((uint32_t)0x00000004) //!< No. 4 rtc always on register
#define RTC_AO_CHECK(aor)                 (((aor) == RTC_AO0_DR) || \
                                           ((aor) == RTC_AO1_DR) || \
                                           ((aor) == RTC_AO2_DR) || \
                                           ((aor) == RTC_AO3_DR) || \
                                           ((aor) == RTC_AO4_DR)) 

/**
  * @brief rtc BIN/DCB format definitions 
  */ 
#define RTC_FORMAT_BIN                    ((uint32_t)0x000000000) //!< data format BIN
#define RTC_FORMAT_BCD                    ((uint32_t)0x000000001) //!< data format BCD
#define RTC_FORMAT_CHECK(format) (((format) == RTC_FORMAT_BIN) || ((format) == RTC_FORMAT_BCD))

/**  
  * @brief rtc flags definitions
  */ 
#define RTC_FLAG_CALBSY                   RTC_STS_CALBSY    //!< calibration busy flag
#define RTC_FLAG_INBREAK3F                RTC_STS_INBREAK3F //!< RTC_INBREAK3 detection flag
#define RTC_FLAG_INBREAK2F                RTC_STS_INBREAK2F //!< RTC_INBREAK2 detection flag
#define RTC_FLAG_INBREAK1F                RTC_STS_INBREAK1F //!< RTC_INBREAK1 detection flag
#define RTC_FLAG_TSOVF                    RTC_STS_TSOVF     //!< Time-stamp overflow flag
#define RTC_FLAG_TSF                      RTC_STS_TSF       //!< Time-stamp flag
#define RTC_FLAG_WUTF                     RTC_STS_WKF       //!< wakeup flag
#define RTC_FLAG_ALRF                     RTC_STS_ALRF      //!< alarm flag
#define RTC_FLAG_INITF                    RTC_STS_INITF     //!< initialization mode enter flag
#define RTC_FLAG_RSF                      RTC_STS_RSF       //!< synchronization flag
#define RTC_FLAG_INITS                    RTC_STS_INITS     //!< initialization status flag
#define RTC_FLAG_SHF_BUSY                 RTC_STS_SHF_BUSY  //!< shift busy flag
#define RTC_FLAG_WUTWF                    RTC_ISR_WUTWF     //!<  wakeup 
#define RTC_FLAG_ALRWAF                   RTC_STS_ALRWAF    //!< alarm write access flag

#define RTC_FLAG_GET_CHECK(flag) (((flag) == RTC_FLAG_TSOVF)     || ((flag) == RTC_FLAG_TSF)       || \
                                  ((flag) == RTC_FLAG_WUTF)      || ((flag) == RTC_FLAG_ALRWAF)    || \
                                  ((flag) == RTC_FLAG_ALRF)      || ((flag) == RTC_FLAG_INITF)     || \
                                  ((flag) == RTC_FLAG_RSF)       || ((flag) == RTC_FLAG_WUTWF)     || \
                                  ((flag) == RTC_FLAG_INBREAK1F) || ((flag) == RTC_FLAG_INBREAK2F) || \
                                  ((flag) == RTC_FLAG_INBREAK3F) || ((flag) == RTC_FLAG_CALBSY)    || \
                                  ((flag) == RTC_FLAG_SHF_BUSY))
#define RTC_FLAG_CLEAR_CHECK(flag) (((flag) != (uint32_t)RESET) && (((flag) & 0xFFFF02DF) == (uint32_t)RESET))


/** 
  * @brief rtc interrupt source definitions
  */ 
#define RTC_INTERRUPT_TR                      ((uint32_t)0x00008000) //!< time recording interrupt enable
#define RTC_INTERRUPT_WUT                     ((uint32_t)0x00004000) //!< wakeup interrupt
#define RTC_INTERRUPT_ALR                     ((uint32_t)0x00001000) //!< alarm interrupt enable
#define RTC_INTERRUPT_INBRK                   ((uint32_t)0x00000004) //!< inbreak interrupt enable
#define RTC_INTERRUPT_INBRK1                  ((uint32_t)0x00020000) //!< enable the inbreak1 interrupt
#define RTC_INTERRUPT_INBRK2                  ((uint32_t)0x00040000) //!< enable the inbreak2 interrupt 
#define RTC_INTERRUPT_INBRK3                  ((uint32_t)0x00080000) //!< enable the inbreak3 interrupt

#define RTC_CONFIG_INTERRUPT_CHECK(flag) (((flag) != (uint32_t)RESET) && (((flag) & 0xFFFF2FFB) == (uint32_t)RESET))
#define RTC_GET_INTERRUPT_CHECK(flag) (((flag) == RTC_INTERRUPT_TR)     || ((flag) == RTC_INTERRUPT_ALR) || \
                                       ((flag) == RTC_INTERRUPT_INBRK1) || ((flag) == RTC_INTERRUPT_WUT) || \
                                       ((flag) == RTC_INTERRUPT_INBRK2) || ((flag) == RTC_INTERRUPT_INBRK3))                           

#define RTC_CLEAR_INTERRUPT_CHECK(flag) (((flag) != (uint32_t)RESET) && (((flag) & 0xFFF12FFF) == (uint32_t)RESET))

/**
  * @}
  */ 

/**
  * @fn error_status_t rtc_def_init(void)
  * @brief  Deinitializes the RTC registers to their default reset values.                     
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_def_init(void);

/**
  * @fn error_status_t rtc_init(rtc_init_t* ptr_init_struct)
  * @brief  Initializes the RTC registers according to the specified parameters in ptr_init_struct.
  *         RTC Prescaler register can be written in initialization step only.
  * @param  ptr_init_struct: pointer to a rtc_init_t structure that contains 
  *         the configuration information for the RTC peripheral.
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_init(rtc_init_t* ptr_init_struct);

/**
  * @fn void rtc_struct_init(rtc_init_t* ptr_init_struct)
  * @brief  Fills each rtc init_struct member with its default value.
  * @param  ptr_init_struct: pointer to a ptr_init_struct structure which will be initialized.         
  * @return None
  */
void rtc_struct_init(rtc_init_t* ptr_init_struct);

/**
  * @fn void rtc_ccode_register_set(enable_state_t enable_flag)
  * @brief  Enables or disables the RTC registers write protection, CCODE (RTC control code register).            
  *         It can be Writ a wrong key reactivates the write protection.
  *         The protection mechanism is not affected by system reset.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_ccode_register_set(enable_state_t enable_flag);

/**
  * @fn error_status_t rtc_init_mode_enter(void)
  * @brief  Enters the RTC Initialization mode.
  *         When the RTC Initialization mode is write protected, can be use the 
  *         rtc_ccode_register_set(DISABLE) before calling this function.
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_init_mode_enter(void);

/**
  * @fn void rtc_init_mode_exit(void)
  * @brief  Exits the RTC Initialization mode.      
  * @param  None
  * @return None
  */
void rtc_init_mode_exit(void);

/**
  * @fn error_status_t rtc_synchro_wait(void)
  * @brief  Waits until the RTC Time and Date registers (RTC_TIME and RTC_DATE) are 
  *         synchronized with RTC APB clock.
  * @note   When the RTC Initialization mode is write protected, can use the 
  *         rtc_ccode_register_set(DISABLE) before calling this function.  
  * @param  None
  * @return error_status_t value:SUCCESS or ERROR.
  */
error_status_t rtc_synchro_wait(void);

/**
  * @fn error_status_t rtc_outside_clock_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables RTC OPCLKEN (outside precise clock enable) detection.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_outside_clock_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rtc_direct_access_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables direct access register.
  * @note   When direct access is enabled the calendar value are taken 
  *         directly from the Calendar counter.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
*/
void rtc_direct_access_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn error_status_t rtc_time_set(uint32_t format_flag, rtc_time_t* ptr_time_struct)
  * @brief  Set the RTC current time.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).        
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that contains 
  *         the time configuration information for the RTC.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_time_set(uint32_t format_flag, rtc_time_t* ptr_time_struct);

/**
  * @fn void rtc_time_struct_init(rtc_time_t* ptr_time_struct)
  * @brief  rtc structure member with its default value (00h:00min:00sec).  *         
  * @param  ptr_time_struct: pointer to a rtc_time_t structure which will be 
  *         initialized.
  * @return None
  */
void rtc_time_struct_init(rtc_time_t* ptr_time_struct);

/**
  * @fn void rtc_time_get(uint32_t format_flag, rtc_time_t* ptr_time_struct)
  * @brief  Get the RTC current Time.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that will 
  *         contain the returned current time configuration.
  * @return None
  */
void rtc_time_get(uint32_t format_flag, rtc_time_t* ptr_time_struct);

/**
  * @fn uint32_t rtc_subsecond_get(void)
  * @brief  Gets the RTC current Calendar Subseconds value.
  * @param  None
  * @return RTC current Calendar Subseconds value.
  */
uint32_t rtc_subsecond_get(void);

/**
  * @fn error_status_t rtc_date_set(uint32_t format_flag, rtc_date_t* ptr_date_struct)
  * @brief  Set the RTC current date.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that contains 
  *         the date configuration information for the RTC.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_date_set(uint32_t format_flag, rtc_date_t* ptr_date_struct);

/**
  * @fn void rtc_date_struct_init(rtc_date_t* ptr_date_struct)
  * @brief  rtc date struct member default value (Monday, January 01 xx00).          
  * @param  ptr_date_struct: pointer to a rtc_date_t structure which will be 
  *         initialized.
  * @return None
  */
void rtc_date_struct_init(rtc_date_t* ptr_date_struct);

/**
  * @fn void rtc_date_get(uint32_t format_flag, rtc_date_t* ptr_date_struct)
  * @brief  Get the RTC current date.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that will 
  *         contain the returned current date configuration.
  * @return None
  */
void rtc_date_get(uint32_t format_flag, rtc_date_t* ptr_date_struct);

/**
  * @fn void rtc_alarm_set(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
  * @brief  Set the specified RTC Alarm.The Alarm register can only be written 
  *         when the corresponding Alarm is disabled (Use the rtc_alarm_enable_ctrl(DISABLE)).       
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  alarm_flag: Alarm enable(RTC_ALARM_ENABLE).
  * @param  ptr_alarm_struct: pointer to a rtc_alarm_t structure that 
  *         contains the alarm configuration parameters.
  * @return None
  */
void rtc_alarm_set(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct);

/**
  * @fn void rtc_alarm_struct_init(rtc_alarm_t* ptr_alarm_struct)
  * @brief  Init alarm struct member  default value (00h:00mn:00sec / 1st day of the month).
  * @param  ptr_alarm_struct: pointer to a @ref rtc_alarm_t structure which
  *         will be initialized.
  * @return None
  */
void rtc_alarm_struct_init(rtc_alarm_t* ptr_alarm_struct);

/**
  * @fn void rtc_alarm_get(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct)
  * @brief  Get the RTC Alarm value and masks.
  * @param  format_flag: It can be Binary data format (RTC_FORMAT_BIN),
  *         or BCD data format (RTC_FORMAT_BCD).  
  * @param  alarm_flag: Alarm enable (RTC_ALARM_ENABLE).
  * @param  ptr_alarm_struct: pointer to a rtc_alarm_t structure that will 
  *         contains the output alarm configuration values.
  * @return None
  */
void rtc_alarm_get(uint32_t format_flag, uint32_t alarm_flag, rtc_alarm_t* ptr_alarm_struct);

/**
  * @fn error_status_t rtc_alarm_enable_ctrl(uint32_t alarm_flag, enable_state_t enable_flag)
  * @brief  Enables or disables the specified RTC Alarm.
  * @param  alarm_flag: Alarm enable(RTC_ALARM_ENABLE).
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return error_status_t value: SUCCESS or ERROR.
  */
error_status_t rtc_alarm_enable_ctrl(uint32_t alarm_flag, enable_state_t enable_flag);

/**
  * @fn void rtc_alarm_subsecond_config(uint32_t alarm_flag, uint32_t subsecond_value, uint8_t subsecond_mask)
  * @brief  Configure the RTC Alarm Subseconds value and mask.
  * @note   This function is performed only when the Alarm is disabled. 
  * @param  alarm_flag: Alarm enable (RTC_ALARM_ENABLE).
  * @param  subsecond_value: It can be a value from 0 to 0x00007FFF.
  * @param  subsecond_mask:  specifies the Subseconds Mask bits (RTC_ACFG2 SSEC[14:0]).
  * @return None
  */
void rtc_alarm_subsecond_config(uint32_t alarm_flag, uint32_t subsecond_value, uint8_t subsecond_mask);

/**
  * @fn uint32_t rtc_alarm_subsecond_get(void)
  * @brief  Gets the RTC Alarm Subseconds value.
  * @param  None
  * @return RTC Alarm Subseconds value.
  */
uint32_t rtc_alarm_subsecond_get(void);

/**
  * @fn void rtc_wakeup_clock_config(uint32_t wakeup_clk)
  * @brief  Configures the RTC Wakeup clock source .   
  *         The WakeUp Clock source can only be changed when the RTC WakeUp
  *         is disabled (Use the rtc_wakeup_enable_ctrl(DISABLE)).
  * @param  wakeup_clk: It can be one of the following values:           
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV16
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV8
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV4
  *          @arg RTC_WAKEUP_CLOCK_RTCCLK_DIV2
  *          @arg RTC_WAKEUP_CLOCK_SPRE_16BITS
  *          @arg RTC_WAKEUP_CLOCK_SPRE_17BITS.
  * @return None
  */
void rtc_wakeup_clock_config(uint32_t wakeup_clk); 

/**
  * @fn void rtc_wakeup_value_set(uint32_t counter) 
  * @brief  Configures the RTC Wakeup counter .
  * @note   The RTC WakeUp counter can only be written when the RTC WakeUp
  *         is disabled (Use the rtc_wakeup_enable_ctrl(DISABLE)).
  * @param  counter: It can be a value from 0x0000 to 0xFFFF.  *           
  * @return None
  */
void rtc_wakeup_value_set(uint32_t counter); 

/**
  * @fn uint32_t rtc_wakeup_value_get(void)
  * @brief  Get the RTC WakeUp timer counter value.
  * @param  None
  * @return The RTC WakeUp Counter value.
  */
uint32_t rtc_wakeup_value_get(void); 

/**
  * @fn error_status_t rtc_wakeup_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables the RTC WakeUp timer.
  * @param  enable_flag: It can be ENABLE or DISABLE.
  * @return None
  */
error_status_t rtc_wakeup_enable_ctrl(enable_state_t enable_flag); 

/**
  * @fn void rtc_daylight_saving_config(uint32_t save_time_value, uint32_t flag)
  * @brief  Adds or substract one hour from the current time.
  * @param  save_time_value: It can be one of the following values:
  *          @arg RTC_DAYLIGHT_SAVING_DEC1H(substract one hour)
  *          @arg RTC_DAYLIGHT_SAVING_INC1H(add one hour)
  * @param  flag: Specifies the value to be written in the SAVEF bit Reset or Set.
  * @return None
  */
void rtc_daylight_saving_config(uint32_t save_time_value, uint32_t flag);

/**
  * @fn uint32_t rtc_daylight_saving_flag_get(void)
  * @brief  Returns the RTC Day Light Saving flag.
  * @param  None
  * @return RTC_CTR Day Light Saving stored operation.
  */
uint32_t rtc_daylight_saving_flag_get(void);

/**
  * @fn void rtc_output_config(uint32_t config_value, uint32_t config_polarity)
  * @brief  Configures the RTC output source.
  * @param  config_value: It can be one of the following values:
  *          @arg RTC_OUTPUT_SOURCE_DISABLE(rtc output clock suorce disable)
  *          @arg RTC_OUTPUT_SOURCE_ALARM(select alarm output clock suorce)
  *          @arg RTC_OUTPUT_SOURCE_WAKEUP(select wakeup output clock suorce)
  * @param  config_polarity: It can be one of the following:
  *          @arg RTC_OUTPUT_POLARITY_High(output pin is high)                                          
  *          @arg RTC_OUTPUT_POLARITY_LOW(output pin is low)                        
  * @return None
  */
void rtc_output_config(uint32_t config_value, uint32_t config_polarity);

/**
  * @fn void rtc_cali_output_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or disables calibration Output.
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_cali_output_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rtc_cali_output_config(uint32_t config_value)
  * @brief  Configure the Calibration Pinout (RTC_CTR_CALSRC) Selection (1Hz or 512Hz).
  * @param  config_value: It can be one of the following values:
  *          @arg RTC_CALI_OUTPUT_512HZ(regular waveform at 512Hz) 
  *          @arg RTC_CALI_OUTPUT_1HZ(regular waveform at 1Hz)
  * @return None
*/
void rtc_cali_output_config(uint32_t config_value);

/**
  * @fn error_status_t rtc_swallow_config(uint32_t period_value,uint32_t insert_value, uint32_t swallow_value) 
  * @brief  Configures swallow rtc clock Settings.
  * @param  period_value: It can be can be one of the following values:
  *          @arg RTC_CALI_PERIOD_32SECOND(calibration periode is 32s)
  *          @arg RTC_CALI_PERIOD_16SECOND(calibration periode is 16s)
  *          @arg RTC_CALI_PERIOD_8SECOND(calibartion periode is 8s)
  * @param  insert_value: It can be one of the following values:
  *          @arg RTC_CALI_INSERT_CLOCK_SET(Add one RTCCLK puls every 2**11 pulses)
  *          @arg RTC_CALI_INSERT_CLOCK_RESET(No RTCCLK pulses are added)
  * @param  swallow_value: Select the value of SWACLK[8:0] bits,value from 0 to 0x000001FF.
  * @return :SUCCESS or ERROR.
*/
error_status_t rtc_swallow_config(uint32_t period_value,uint32_t insert_value, uint32_t swallow_value); 
                                  
/**
  * @fn void rtc_time_recording_enable_ctrl(uint32_t edge_flag, enable_state_t enable_flag)
  * @brief  Enables or Disables the RTC recording functionality. 
  * @param  edge_flag: It can be one of the following:
  *          @arg RTC_RECORD_RISING_EDGE(the Time stamp event occurs on the rising edge)                                          
  *          @arg RTC_RECORD_FALLING_EDGE(the Time stamp event occurs on the falling edge)
  * @param  enable_flag: It can be: ENABLE or DISABLE.
  * @return None
  */                                 
void rtc_time_recording_enable_ctrl(uint32_t edge_flag, enable_state_t enable_flag);

/**
  * @fn void rtc_time_recording_get(uint32_t format_flag, rtc_time_t* ptr_time_struct, rtc_date_t* ptr_date_struct)
  * @brief  Get the RTC time recording value and masks.
  * @param  format_flag: It can be one of the following values:
  *          @arg RTC_FORMAT_BIN(Binary data format) 
  *          @arg RTC_FORMAT_BCD(BCD data format)
  * @param  ptr_time_struct: pointer to a rtc_time_t structure that will 
  *         contains the recording time values. 
  * @param  ptr_date_struct: pointer to a rtc_date_t structure that will 
  *         contains the recording date values.     
  * @return None
  */
void rtc_time_recording_get(uint32_t format_flag, rtc_time_t* ptr_time_struct, rtc_date_t* ptr_date_struct);

/**
  * @fn uint32_t rtc_time_recording_subsecond_get(void)
  * @brief  Get the RTC recording  Subseconds(SSEC) value.
  * @param  None
  * @return RTC current  Subseconds value.
  */
uint32_t rtc_time_recording_subsecond_get(void);

/**
  * @fn void rtc_inbreak_enable_config(uint32_t set_value, uint32_t trigger_flag)
  * @brief  Configures the select inbreak input enable pin and triggle condition.
  * @param  set_value: Selected inbreak pin. It can be any combination of the following values:
  *          @arg RTC_INBREAK1_ENABLE(Select inbreak 1)
  *          @arg RTC_INBREAK2_ENABLE(Select inbreak 2)
  * @param  trigger_flag: Specifies the trigger on the inbreak pin condition 
  *          It can be one of the following values:           
  *           @arg TC_INBREAK1_RISING_EDGE(Rising Edge of the inbreak pin)
  *           @arg RTC_INBREAK1_FALLING_EDGE(Falling Edge of the inbreak pin)
  *           @arg RTC_INBREAK1_LOW_LEVEL(Low Level of the inbreak pin)
  *           @arg RTC_INBREAK1_HIGH_LEVEL(High Level of the inbreak pin)  
  * @return None
  */
void rtc_inbreak_enable_config(uint32_t set_value, uint32_t trigger_flag);

/**
  * @fn void rtc_inbreak_enable_ctrl(uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or Disables the inbreak detection.
  * @param  set_value: Selected inbreak pin. It can be any combination of the following values:
  *          @arg RTC_INBREAK1_ENABLE(Select inbreak 1)
  *          @arg RTC_INBREAK2_ENABLE(Select inbreak 2)
  * @param  enable_flag: It can be: ENABLE or DISABLE.                  
  * @return None
  */
void rtc_inbreak_enable_ctrl(uint32_t set_value, enable_state_t enable_flag);

/**
  * @fn void rtc_inbreak_filter_config(uint32_t set_value)
  * @brief  Configures the inbreak Filter.
  * @param  set_value: Specifies the inbreak filter,it can be one of the following values:
  *          @arg RTC_INBREAK_FILTER_DISABLE(inbreak filter is disabled)
  *          @arg RTC_INBREAK_FILTER_2SAMPLE(inbreak is activated after 2 consecutive samples at the active level) 
  *          @arg RTC_INBREAK_FILTER_4SAMPLE(inbreak is activated after 4 consecutive samples at the active level) 
  *          @arg RTC_INBREAK_FILTER_8SAMPLE(inbreak is activated after 8 consecutive samples at the active level)   
  * @return None
  */
void rtc_inbreak_filter_config(uint32_t set_value);

/**
  * @fn void rtc_inbreak_sample_freq_config(uint32_t freq_vale)
  * @brief  Configures the inbreak sampling frequency.
  * @param  freq_vale: Specifies the inbreak sampling frequency,it can be one of the following values:
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV32768(each of the inbreak inputs are sampled with freq RTCCLK / 32768)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV16384(each of the inbreak inputs are sampled with freq RTCCLK / 16384)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV8192(each of the inbreak inputs are sampled with freq RTCCLK / 8192)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV4096(each of the inbreak inputs are sampled with freq RTCCLK / 4096)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV2048(each of the inbreak inputs are sampled with freq RTCCLK / 2048)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV1024(each of the inbreak inputs are sampled with freq RTCCLK / 1024)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV512(each of the inbreak inputs are sampled with freq RTCCLK / 512)
  *          @arg RTC_INBREAK_SAMPLE_FREQ_PDIV256(each of the inbreak inputs are sampled with freq RTCCLK / 256)                                                       
  * @return None
  */
void rtc_inbreak_sample_freq_config(uint32_t freq_vale);

/**
  * @fn void rtc_inbreak_precharge_duration(uint32_t set_value)
  * @brief  Configures the inbreak Pins input Precharge Duration.
  * @param  set_value: it can be one of the following values:
  *          @arg RTC_INBREAK_PRECHARGE_1RTCCLK(inbreak pins are precharged before sampling during 1 RTCCLK cycle)
  *          @arg RTC_INBREAK_PRECHARGE_2RTCCLK(inbreak pins are precharged before sampling during 2 RTCCLK cycle) 
  *          @arg RTC_INBREAK_PRECHARGE_4RTCCLK(inbreak pins are precharged before sampling during 4 RTCCLK cycle) 
  *          @arg RTC_INBREAK_PRECHARGE_8RTCCLK(inbreak pins are precharged before sampling during 8 RTCCLK cycle)  
  * @return None
  */
void rtc_inbreak_precharge_duration(uint32_t set_value);

/**
  * @fn void rtc_recording_inbreak_detect_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables recording on inbreak detection event. 
  * @param  enable_flag: it can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_recording_inbreak_detect_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rtc_inbreak_pull_up_enable_ctrl(enable_state_t enable_flag)
  * @brief  Enables or Disables the Precharge of Tamper pin.
  * @param  enable_flag: new state of tamper pull up.
  *         This parameter can be: ENABLE or DISABLE.                   
  * @return None
  */
void rtc_inbreak_pull_up_enable_ctrl(enable_state_t enable_flag);

/**
  * @fn void rtc_ao_register_write(uint32_t ao_reg, uint32_t set_value)
  * @brief Writes a data in the specified AO (always on) registers.
  * @param  ao_reg: AO (always on) registers number, it can be: A00~A04.
  * @param  set_value: Data to be written in the specified AO (always on) registers.                    
  * @return None
  */
void rtc_ao_register_write(uint32_t ao_reg, uint32_t set_value);

/**
  * @fn uint32_t rtc_ao_register_read(uint32_t ao_reg)
  * @brief  Reads data from the specified AO (always on) registers.
  * @param  ao_reg: AO (always on) registers number, it can be: A00~A04.                                          
  * @return the value of the specified AO (always on) registers.
  */
uint32_t rtc_ao_register_read(uint32_t ao_reg);

/**
  * @fn void rtc_output_type_config(uint32_t output_type)
  * @brief  Configures the RTC Output Pin mode. 
  * @param  output_type: specifies the RTC Output (PC13) pin mode.
  *          It can be one of the following values:
  *           @arg RTC_OUTPUT_OPEN_DRAIN(RTC output (PC13) is configured in open drain mode)                                     
  *           @arg RTC_OUTPUT_PUSH_PULL(RTC Output (PC13) is configured in Push Pull mode)                                    
  * @return None
  */
void rtc_output_type_config(uint32_t output_type);

/**
  * @fn error_status_t rtc_adj_register_config(uint32_t second_flag, uint32_t subsecond_value)
  * @brief Configures the adjustment(ADJ) register.
  * @param  second_flag: Select to add or not 1 second to the time Calendar.
  *          It can be one of the following values :
  *           @arg RTC_ADJ_INC1S_SET(add one second to the clock calendar)
  *           @arg RTC_ADJ_INC1S_RESET(no effect)
  * @param  subsecond_value: It can be one any value from 0 to 0x7FFF.
  * @return An error_status_t: SUCCESS or ERROR.
*/
error_status_t rtc_adj_register_config(uint32_t second_flag, uint32_t subsecond_value);

/**
  * @fn void rtc_interrupt_config(uint32_t set_value, enable_state_t enable_flag)
  * @brief  Enables or disables the specified RTC interrupts.
  * @param  set_value: specifies the RTC interrupt sources to be enabled or disabled. 
  * @param  enable_flag: new state of the specified RTC interrupts, it can be: ENABLE or DISABLE.
  * @return None
  */
void rtc_interrupt_config(uint32_t set_value, enable_state_t enable_flag);

/**
  * @fn bit_status_t rtc_flag_status_get(uint32_t set_flag) 
  * @brief  Checks whether the specified RTC flag is set or not.
  * @param  set_flag: specifies the RTC_STS register status bit.
  * @return The new state of  set_flag (SET or RESET).
  */
bit_status_t rtc_flag_status_get(uint32_t set_flag);

/**
  * @fn void rtc_flag_clear(uint32_t set_flag)
  * @brief  Clear the Flags in the RTC_STS register.
  * @param  set_flag: specifies the clear bit in the RTC_STS register.
  * @return None
  */
void rtc_flag_clear(uint32_t set_flag);

/**
  * @fn bit_status_t rtc_interrupt_status_get(uint32_t status_bit)
  * @brief  Get the specified RTC interrupt status in RTC_STS register.
  * @param  status_bit: specifies the RTC interrupt source in RTC_STS register.
  * @return The new state of status_bit (SET or RESET).
  */
bit_status_t rtc_interrupt_status_get(uint32_t status_bit);

/**
  * @fn void rtc_interrupt_flag_clear(uint32_t status_bit)
  * @brief  Clears the RTC's interrupt pending bits.
  * @param  status_bit: specifies the RTC interrupt pending bit to clear.
  * @return None
  */
void rtc_interrupt_flag_clear(uint32_t status_bit);
#define UNUSED(x) (void)x
#ifdef __cplusplus
}
#endif

#endif 

/**
  * @}
  */ 

/**
  * @}
  */ 

