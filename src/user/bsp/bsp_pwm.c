#include "board.h"
#include "bsp_pwm.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "hpm_mchtmr_drv.h"
#include "hpm_pwm_drv.h"
#include "hpm_trgm_drv.h"
#include "hpm_clock_drv.h"

uint32_t pwm_sourec_clk, pwm_reload,pwm_freq;

void bsp_pwm_init(void)
{
        uint8_t cmp_index = 0;
        pwm_cmp_config_t cmp_config[4] = {0};
        pwm_config_t pwm_config = {0};

        HPM_IOC->PAD[IOC_PAD_PB02].FUNC_CTL = IOC_PB02_FUNC_CTL_PWM0_P_2;
        HPM_IOC->PAD[IOC_PAD_PB03].FUNC_CTL = IOC_PB03_FUNC_CTL_PWM0_P_3;
        HPM_IOC->PAD[IOC_PAD_PB04].FUNC_CTL = IOC_PB04_FUNC_CTL_PWM0_P_4;
        HPM_IOC->PAD[IOC_PAD_PB05].FUNC_CTL = IOC_PB05_FUNC_CTL_PWM0_P_5;

        pwm_stop_counter(HPM_PWM0);
        pwm_enable_reload_at_synci(HPM_PWM0);
        trgm_output_update_source(HPM_TRGM0, TRGM_TRGOCFG_PWM0_SYNCI, 1);
        trgm_output_update_source(HPM_TRGM0, TRGM_TRGOCFG_PWM0_SYNCI, 0);

        pwm_get_default_pwm_config(HPM_PWM0, &pwm_config);

        pwm_config.enable_output = true;
        pwm_config.dead_zone_in_half_cycle = 0;
        pwm_config.invert_output = false;

        pwm_set_reload(HPM_PWM0, 0, pwm_reload);
        pwm_set_start_count(HPM_PWM0, 0, 0);

        cmp_config[0].mode = pwm_cmp_mode_output_compare;
        cmp_config[0].cmp = pwm_reload + 1;
        cmp_config[0].update_trigger = pwm_shadow_register_update_on_modify;

        if (status_success != pwm_setup_waveform(HPM_PWM0, 2, &pwm_config, cmp_index, &cmp_config[0], 1))
        {
                printf("failed to setup waveform\n");
                while (1)
                        ;
        }

        cmp_config[1].mode = pwm_cmp_mode_output_compare;
        cmp_config[1].cmp = pwm_reload + 1;
        cmp_config[1].update_trigger = pwm_shadow_register_update_on_modify;

        if (status_success != pwm_setup_waveform(HPM_PWM0, 3, &pwm_config, cmp_index + 1, &cmp_config[1], 1))
        {
                printf("failed to setup waveform\n");
                while (1)
                        ;
        }

        cmp_config[2].mode = pwm_cmp_mode_output_compare;
        cmp_config[2].cmp = pwm_reload + 1;
        cmp_config[2].update_trigger = pwm_shadow_register_update_on_modify;

        if (status_success != pwm_setup_waveform(HPM_PWM0, 4, &pwm_config, cmp_index + 2, &cmp_config[2], 1))
        {
                printf("failed to setup waveform\n");
                while (1)
                        ;
        }

        cmp_config[3].mode = pwm_cmp_mode_output_compare;
        cmp_config[3].cmp = pwm_reload + 1;
        cmp_config[3].update_trigger = pwm_shadow_register_update_on_modify;

        if (status_success != pwm_setup_waveform(HPM_PWM0, 5, &pwm_config, cmp_index + 3, &cmp_config[3], 1))
        {
                printf("failed to setup waveform\n");
                while (1)
                        ;
        }

        pwm_start_counter(HPM_PWM0);
        pwm_issue_shadow_register_lock_event(HPM_PWM0);
}

void bsp_set_pwm_freq(float _freq)
{
        pwm_sourec_clk = clock_get_frequency(clock_mot0);
        pwm_reload = pwm_sourec_clk / _freq -1;
        pwm_freq = _freq;
}

void bsp_set_pwm_duty(uint8_t _ch, float _duty)
{
    pwm_update_raw_cmp_edge_aligned(HPM_PWM0,_ch,pwm_reload - pwm_reload * _duty);
}

void bsp_set_pwm_pulse_width(uint8_t _ch, float _pulse_width)
{
}
