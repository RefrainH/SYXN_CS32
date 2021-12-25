#include "log.h"




#if (LOG_ON)

void log_hex_t(uint8_t *data, uint16_t len, uint8_t *tag)
{
	uint16_t i = 0;
	uint8_t hexStr[3] = {0};
	
	log_t("%s - ", tag);
	for(i=0; i<len; i++)
	{
		sprintf((char *)hexStr, "%02x", data[i]);
		log_t("%s", hexStr);
	}
	log_t("\r\n");
}



#endif













