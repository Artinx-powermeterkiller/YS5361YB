#ifndef BSP_LED_H
#define BSP_LED_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_LED_init(void);
void bsp_LED_sys_toggle(void);
void bsp_LED_sys_on(void);
void bsp_LED_sys_off(void);
void bsp_LED_error_toggle(void);
void bsp_LED_error_on(void);
void bsp_LED_error_off(void);


#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif