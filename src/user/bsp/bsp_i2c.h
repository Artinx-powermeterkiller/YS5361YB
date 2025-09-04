#ifndef BSP_I2C_H
#define BSP_I2C_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_i2c_init(void);
void bsp_i2c_send(uint16_t slave_address,uint8_t* send_buffer,uint8_t send_length);
void bsp_i2c_receive(uint16_t slave_address,uint8_t* receive_buffer,uint8_t reg,uint8_t receive_length);
bool bsp_get_receive_flag();

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif