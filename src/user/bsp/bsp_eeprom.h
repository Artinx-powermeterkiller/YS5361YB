#ifndef BSP_EEPROM_H
#define BSP_EEPROM_H

#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_eeprom_init(void);
void bsp_eeprom_write(uint8_t* _address,uint8_t* _buff,uint32_t _length);
void bsp_eeprom_read(uint8_t* _address,uint8_t* _buff,uint32_t _length);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif