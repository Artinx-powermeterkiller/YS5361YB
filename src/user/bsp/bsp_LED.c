#include "bsp_led.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"

void bsp_LED_init(void)
{
    HPM_IOC->PAD[IOC_PAD_PA10].FUNC_CTL = IOC_PA10_FUNC_CTL_GPIO_A_10;

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOA, 10, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOA, 10);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 10, 0);

    HPM_IOC->PAD[IOC_PAD_PA11].FUNC_CTL = IOC_PA11_FUNC_CTL_GPIO_A_11;

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOA, 11, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOA, 11);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 11, 0);

    HPM_IOC->PAD[IOC_PAD_PA22].FUNC_CTL = IOC_PA22_FUNC_CTL_GPIO_A_22;

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOA, 22, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOA, 22);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 22, 0);

    HPM_IOC->PAD[IOC_PAD_PA23].FUNC_CTL = IOC_PA23_FUNC_CTL_GPIO_A_23;

    gpiom_set_pin_controller(HPM_GPIOM, GPIOM_ASSIGN_GPIOA, 23, gpiom_soc_gpio0);
    gpio_set_pin_output(HPM_GPIO0, GPIO_OE_GPIOA, 23);
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 23, 0);


    bsp_LED_sys_on();
    bsp_LED_error_on();
}

void bsp_LED_sys_toggle(void)
{
    gpio_toggle_pin(HPM_GPIO0, GPIO_DO_GPIOA, 10);
}

void bsp_LED_sys_on(void)
{
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 10, 1);
}

void bsp_LED_sys_off(void)
{
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 10, 0);
}

void bsp_LED_error_toggle(void)
{
    gpio_toggle_pin(HPM_GPIO0, GPIO_DO_GPIOA, 11);
}

void bsp_LED_error_on(void)
{
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 11, 1);
}

void bsp_LED_error_off(void)
{
    gpio_write_pin(HPM_GPIO0, GPIO_DO_GPIOA, 11, 0);
}
