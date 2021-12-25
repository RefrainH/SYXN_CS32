#ifndef USR_LIB_H
#define USR_LIB_H


#include <stdint.h>
#include <stdbool.h>









uint8_t CountMod256(uint8_t *data, uint16_t len);
uint32_t bcd2int(uint8_t *bcd, uint8_t len, bool isBig);
float sqrSumAverSqrt(uint32_t *data, uint32_t len);
uint32_t CountMax(uint32_t *data, uint32_t len);
void CountMaxOffset(uint32_t *data, uint16_t len);

void InverteDataSequence(uint8_t *data, uint16_t len);
uint32_t DateTimeBcd2Unix(uint8_t *bcd);
void int2Bcd(uint32_t num, uint8_t *bcd, uint8_t *len, bool isBig);
void CountMaxMin(const uint16_t *const data, uint16_t len, uint16_t *dMax, uint16_t *dMin);
uint16_t CountMaxMinRange(const uint16_t *const data, uint16_t len);
uint16_t CountAver(uint16_t *src, uint16_t len);
uint8_t GetPreMday(uint8_t pre);
void CountMaxMin32(const uint32_t *const data, uint16_t len, uint32_t *dMax, uint32_t *dMin);
unsigned short CRC16( unsigned char *puchMsg, unsigned short usDataLen );
unsigned short CRC16_Ext( uint16_t crc16, unsigned char *puchMsg, unsigned short usDataLen );


#endif
