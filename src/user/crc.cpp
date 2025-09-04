// Copyright (c) 2022 ChenJun
// Licensed under the Apache-2.0 License.

#include "crc.hpp"

void InvertUint8(unsigned char *dBuf, unsigned char *srcBuf) {
	int i;
	unsigned char tmp[4];
	tmp[0] = 0;
	for (i = 0; i < 8; i++) {
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (7 - i);
	}
	dBuf[0] = tmp[0];

}

void InvertUint16(unsigned short *dBuf, unsigned short *srcBuf) {
	int i;
	unsigned short tmp[4];
	tmp[0] = 0;
	for (i = 0; i < 16; i++) {
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}

unsigned short CRC16_MODBUS(unsigned char *data, unsigned int datalen) {
	unsigned short wCRCin = 0xFFFF;
	unsigned short wCPoly = 0x8005;
	unsigned char wChar = 0;

	while (datalen--) {
		wChar = *(data++);
		InvertUint8(&wChar, &wChar);
		wCRCin ^= (wChar << 8);
		for (int i = 0; i < 8; i++) {
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	InvertUint16(&wCRCin, &wCRCin);
	return (wCRCin);
}

void Append_CRC16_Check_Sum(uint8_t * pchMessage, uint32_t dwLength)
{
		uint16_t CRC16 = CRC16_MODBUS(pchMessage,dwLength-2);
	
		pchMessage[dwLength - 2] = (uint8_t)(CRC16 & 0x00ff);
	  pchMessage[dwLength - 1] = (uint8_t)((CRC16 >> 8) & 0x00ff);
}
