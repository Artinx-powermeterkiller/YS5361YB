#ifndef BSP_PWM_H
#define BSP_PWM_H

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

void bsp_pwm_init(void);
void bsp_set_pwm_freq(float _freq);
void bsp_set_pwm_duty(uint8_t _ch,float _duty);
void bsp_set_pwm_pulse_width(uint8_t _ch, float _pulse_width);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif