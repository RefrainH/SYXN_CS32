#ifndef LOG_H
#define LOG_H


#include <stdint.h>
#include <stdio.h>


#define LOG_ON	0

#if (LOG_ON)
#define log_t printf
void log_hex_t(uint8_t *data, uint16_t len, uint8_t *tag);
#else
#define log_t(fmt,...)
#define log_hex_t(d,l,t)
#endif


#define UI_LOG_ENABLE	0

#if (UI_LOG_ENABLE)
#define ui_log_t        log_t
#define ui_log_hex_t	log_hex_t
#else
#define ui_log_t(fmt,...)
#define ui_log_hex_t(d,l,t)
#endif








#endif
