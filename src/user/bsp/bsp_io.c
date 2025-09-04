#include "board.h"
#include "bsp_io.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "hpm_mchtmr_drv.h"
#define DEBOUNCE_THRESHOLD_IN_MS 5
#define MCHTMR_CLK_NAME (clock_mchtmr0)

static volatile uint64_t gpio_isr_rel_time[4] = {0, 0, 0, 0}; /* mark the real time from mchtimer */
static volatile uint64_t gpio_isr_pre_time[4] = {0, 0, 0, 0}; /* mark the last time marked on gpio_isr */
static volatile uint32_t level_value[4] = {0, 0, 0, 0};       /* mark the level */
static uint32_t debounce_threshold;                          /* debounce threshold */

void isr_gpio(void)
{
    if (gpio_check_clear_interrupt_flag(HPM_GPIO0, GPIO_IF_GPIOY, 3))
    {
        gpio_isr_rel_time[3] = mchtmr_get_count(HPM_MCHTMR);
        if (gpio_isr_rel_time[3] - gpio_isr_pre_time[3] > debounce_threshold)
        {
            gpio_isr_pre_time[3] = gpio_isr_rel_time[3];
            level_value[3] = gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 3);
        }
    }
    else if (gpio_check_clear_interrupt_flag(HPM_GPIO0, GPIO_IF_GPIOY, 2))
    {
        gpio_isr_rel_time[2] = mchtmr_get_count(HPM_MCHTMR);
        if (gpio_isr_rel_time[2] - gpio_isr_pre_time[2] > debounce_threshold)
        {
            gpio_isr_pre_time[2] = gpio_isr_rel_time[2];
            level_value[2] = gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 2);
        }
    }
    else if (gpio_check_clear_interrupt_flag(HPM_GPIO0, GPIO_IF_GPIOY, 1))
    {
        gpio_isr_rel_time[1] = mchtmr_get_count(HPM_MCHTMR);
        if (gpio_isr_rel_time[1] - gpio_isr_pre_time[1] > debounce_threshold)
        {
            gpio_isr_pre_time[1] = gpio_isr_rel_time[1];
            level_value[1] = gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 1);
        }
    }
    else if (gpio_check_clear_interrupt_flag(HPM_GPIO0, GPIO_IF_GPIOY, 0))
    {
        gpio_isr_rel_time[0] = mchtmr_get_count(HPM_MCHTMR);
        if (gpio_isr_rel_time[0] - gpio_isr_pre_time[0] > debounce_threshold)
        {
            gpio_isr_pre_time[0] = gpio_isr_rel_time[0];
            level_value[0] = gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 0);
        }
    }
}

uint32_t *bsp_get_input_buffer(void)
{
    return level_value;
}

uint8_t bsp_get_input_state(uint8_t ch)
{
    return level_value[ch];
}

void bsp_input_gpio_init(void)
{
    gpio_interrupt_trigger_t trigger;

    trigger = gpio_interrupt_trigger_edge_both;

    HPM_PIOC->PAD[IOC_PAD_PY03].FUNC_CTL = PIOC_PY03_FUNC_CTL_SOC_GPIO_Y_03;
    HPM_IOC->PAD[IOC_PAD_PY03].FUNC_CTL = IOC_PY03_FUNC_CTL_GPIO_Y_03;
    HPM_IOC->PAD[IOC_PAD_PY03].PAD_CTL = IOC_PAD_PAD_CTL_HYS_SET(1) | IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_OD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 3, gpiom_soc_gpio0);
    gpio_set_pin_input(HPM_GPIO0, GPIO_OE_GPIOY, 3);

    gpio_config_pin_interrupt(HPM_GPIO0, GPIOM_ASSIGN_GPIOY, 3, trigger);
    gpio_enable_pin_interrupt(HPM_GPIO0, GPIO_IE_GPIOY, 3);

    HPM_PIOC->PAD[IOC_PAD_PY02].FUNC_CTL = PIOC_PY02_FUNC_CTL_SOC_GPIO_Y_02;
    HPM_IOC->PAD[IOC_PAD_PY02].FUNC_CTL = IOC_PY02_FUNC_CTL_GPIO_Y_02;
    HPM_IOC->PAD[IOC_PAD_PY02].PAD_CTL = IOC_PAD_PAD_CTL_HYS_SET(1) | IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_OD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 2, gpiom_soc_gpio0);
    gpio_set_pin_input(HPM_GPIO0, GPIO_OE_GPIOY, 2);

    gpio_config_pin_interrupt(HPM_GPIO0, GPIOM_ASSIGN_GPIOY, 2, trigger);
    gpio_enable_pin_interrupt(HPM_GPIO0, GPIO_IE_GPIOY, 2);

    HPM_PIOC->PAD[IOC_PAD_PY01].FUNC_CTL = PIOC_PY01_FUNC_CTL_SOC_GPIO_Y_01;
    HPM_IOC->PAD[IOC_PAD_PY01].FUNC_CTL = IOC_PY01_FUNC_CTL_GPIO_Y_01;
    HPM_IOC->PAD[IOC_PAD_PY01].PAD_CTL = IOC_PAD_PAD_CTL_HYS_SET(1) | IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_OD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 1, gpiom_soc_gpio0);
    gpio_set_pin_input(HPM_GPIO0, GPIO_OE_GPIOY, 1);

    gpio_config_pin_interrupt(HPM_GPIO0, GPIOM_ASSIGN_GPIOY, 1, trigger);
    gpio_enable_pin_interrupt(HPM_GPIO0, GPIO_IE_GPIOY, 1);

    HPM_PIOC->PAD[IOC_PAD_PY00].FUNC_CTL = PIOC_PY00_FUNC_CTL_SOC_GPIO_Y_00;
    HPM_IOC->PAD[IOC_PAD_PY00].FUNC_CTL = IOC_PY00_FUNC_CTL_GPIO_Y_00;
    HPM_IOC->PAD[IOC_PAD_PY00].PAD_CTL = IOC_PAD_PAD_CTL_HYS_SET(1) | IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(1) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_OD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 0, gpiom_soc_gpio0);
    gpio_set_pin_input(HPM_GPIO0, GPIO_OE_GPIOY, 0);

    gpio_config_pin_interrupt(HPM_GPIO0, GPIOM_ASSIGN_GPIOY, 0, trigger);
    gpio_enable_pin_interrupt(HPM_GPIO0, GPIO_IE_GPIOY, 0);

    uint32_t mchtmr_freq;
    mchtmr_freq = clock_get_frequency(MCHTMR_CLK_NAME);
    debounce_threshold = DEBOUNCE_THRESHOLD_IN_MS * mchtmr_freq / 1000;

    intc_m_enable_irq_with_priority(IRQn_GPIO0_Y, 5);

    level_value[0] = level_value[1] = level_value[2] = level_value[3] = 0;
}

SDK_DECLARE_EXT_ISR_M(IRQn_GPIO0_Y, isr_gpio)

void bsp_output_gpio_init(void)
{
    HPM_PIOC->PAD[IOC_PAD_PY07].FUNC_CTL = PIOC_PY03_FUNC_CTL_SOC_GPIO_Y_03;
    HPM_IOC->PAD[IOC_PAD_PY07].FUNC_CTL = IOC_PY03_FUNC_CTL_GPIO_Y_03;
    HPM_IOC->PAD[IOC_PAD_PY07].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(0) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_DS_SET(7) | IOC_PAD_PAD_CTL_SR_SET(0) | IOC_PAD_PAD_CTL_SPD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 7, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOY, 7);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 7, 0);

    HPM_PIOC->PAD[IOC_PAD_PY06].FUNC_CTL = PIOC_PY02_FUNC_CTL_SOC_GPIO_Y_02;
    HPM_IOC->PAD[IOC_PAD_PY06].FUNC_CTL = IOC_PY02_FUNC_CTL_GPIO_Y_02;
    HPM_IOC->PAD[IOC_PAD_PY06].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(0) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_DS_SET(7) | IOC_PAD_PAD_CTL_SR_SET(0) | IOC_PAD_PAD_CTL_SPD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 5, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOY, 6);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 6, 0);

    HPM_PIOC->PAD[IOC_PAD_PY05].FUNC_CTL = PIOC_PY01_FUNC_CTL_SOC_GPIO_Y_01;
    HPM_IOC->PAD[IOC_PAD_PY05].FUNC_CTL = IOC_PY01_FUNC_CTL_GPIO_Y_01;
    HPM_IOC->PAD[IOC_PAD_PY05].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(0) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_DS_SET(7) | IOC_PAD_PAD_CTL_SR_SET(0) | IOC_PAD_PAD_CTL_SPD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY,5, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOY, 5);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 5, 0);

    HPM_PIOC->PAD[IOC_PAD_PY04].FUNC_CTL = PIOC_PY00_FUNC_CTL_SOC_GPIO_Y_00;
    HPM_IOC->PAD[IOC_PAD_PY04].FUNC_CTL = IOC_PY00_FUNC_CTL_GPIO_Y_00;
    HPM_IOC->PAD[IOC_PAD_PY04].PAD_CTL = IOC_PAD_PAD_CTL_PE_SET(1) | IOC_PAD_PAD_CTL_PS_SET(0) | IOC_PAD_PAD_CTL_PRS_SET(0) | IOC_PAD_PAD_CTL_DS_SET(7) | IOC_PAD_PAD_CTL_SR_SET(0) | IOC_PAD_PAD_CTL_SPD_SET(0);

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOY, 4, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOY, 4);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 4, 0);
}

void bsp_set_output_gpio(uint8_t _pin, uint8_t _value)
{
    switch (_pin)
    {
    case 0:
        gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 4, _value);
        break;
    case 1:
        gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 5, _value);
        break;
    case 2:
        gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 6, _value);
        break;
    case 3:
        gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOY, 7, _value);
        break;
    default:
        printf("Unknown gpio output set error");
        break;
    }
}
