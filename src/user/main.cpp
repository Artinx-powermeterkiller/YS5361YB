/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "hpm_debug_console.h"
#include "board.h"
//#include "CANManager.hpp"
#include "Testbot.hpp"
#include "USBPort.hpp"
#include "bsp/bsp_usb.h"
#include "bsp/bsp_io.h"
#include "bsp/bsp_LED.h"
#include "bsp/bsp_pwm.h"
#include "bsp/bsp_spi.h"
#include "bsp/bsp_i2c.h"
#include "bsp/bsp_mcan.h"
#include "bsp/bsp_uart.h"
// #include "monitor.h"

#define LED_FLASH_PERIOD_IN_MS 1

static uint64_t system_count = 0;
//CANManager* _CANManager = CANManager::Instance();
USBPort* _USBPort = USBPort::Instance();
Testbot testbot;

void system_handler(void)
{
    system_count++;

    if(system_count%500==0)
    {
       bsp_LED_sys_toggle();  
       printf("count:%d\n",(int)system_count);   
    }

    //_CANManager->ReceiveUpdate();
    _USBPort->ReceiveUpdate();

    testbot.Run();

    //_CANManager->SendUpdate();
    _USBPort->SendUpdate();
}

int main(void)
{
    board_init();
    board_init_led_pins();
    board_init_usb_pins();

      bsp_LED_init();
    //bsp_input_gpio_init();
    //bsp_output_gpio_init();

    //bsp_set_pwm_freq(1000.0f);//1KHz
    //bsp_pwm_init();
    //bsp_i2c_init();
    //bsp_spi_init();
    bsp_uart0_init();
    bsp_uart1_init();
    bsp_uart3_init();
    bsp_uart5_init();
    bsp_uart7_init();
    //bsp_mcan0_init();
    //bsp_mcan1_init();
    bsp_mcan2_init();
    //bsp_mcan3_init();

    _USBPort->Init();
    //_CANManager->Init();
    testbot.Init();
    
    board_timer_create(LED_FLASH_PERIOD_IN_MS,system_handler);

    bsp_LED_error_off();
    

    while(1)
    {

    }

    return 0;
}
