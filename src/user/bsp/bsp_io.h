#ifndef BSP_IO_H
#define BSP_IO_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_input_gpio_init(void);
uint32_t* bsp_get_input_buffer(void);
uint8_t bsp_get_input_state(uint8_t ch);
void bsp_output_gpio_init(void);
void bsp_set_output_gpio(uint8_t _pin, uint8_t _value);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif