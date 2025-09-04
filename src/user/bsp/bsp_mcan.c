#include "bsp_mcan.h"
#include "hpm_mcan_drv.h"
#include "hpm_gpio_drv.h"
#include "board.h"
#include <stdio.h>

mcan_filter_elem_t can3_filters[4];

void bsp_mcan3_init()
{
    HPM_IOC->PAD[IOC_PAD_PA14].FUNC_CTL = IOC_PA14_FUNC_CTL_MCAN3_RXD;
    HPM_IOC->PAD[IOC_PAD_PA15].FUNC_CTL = IOC_PA15_FUNC_CTL_MCAN3_TXD;

    MCAN_Type *ptr = HPM_MCAN3;
    mcan_config_t can_config;
    mcan_get_default_config(ptr, &can_config);
    can_config.baudrate = 1000000; /* 1Mbps */
    can_config.mode = mcan_mode_normal;

    can3_filters[0].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can3_filters[0].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO1_IF_MATCH;
    can3_filters[0].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can3_filters[0].start_id = 0x201;
    can3_filters[0].end_id = 0x20B;

    can3_filters[1].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can3_filters[1].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO0_IF_MATCH;
    can3_filters[1].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can3_filters[1].start_id = 0x400;
    can3_filters[1].end_id = 0x440;

    can3_filters[2].filter_type = MCAN_FILTER_TYPE_SPECIFIED_ID_FILTER;
    can3_filters[2].filter_config = MCAN_FILTER_ELEM_CFG_STORE_INTO_RX_BUFFER_OR_AS_DBG_MSG;
    can3_filters[2].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can3_filters[2].match_id = 0x100;
    can3_filters[2].offset = 0;
    can3_filters[2].filter_event = 0;
    can3_filters[2].store_location = 0;

    can_config.all_filters_config.ext_id_filter_list.mcan_filter_elem_count = 0;
    can_config.all_filters_config.std_id_filter_list.filter_elem_list = &can3_filters[0];
    can_config.all_filters_config.std_id_filter_list.mcan_filter_elem_count = 3;
    can_config.all_filters_config.global_filter_config.accept_non_matching_std_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.accept_non_matching_ext_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.reject_remote_std_frame = true;
    can_config.all_filters_config.global_filter_config.reject_remote_ext_frame = true;

    board_init_can(ptr);
    uint32_t can_src_clk_freq = board_init_can_clock(ptr);

    uint32_t interrupt_mask = 0;
    hpm_stat_t status = mcan_init(ptr, &can_config, can_src_clk_freq);
    if (status != status_success)
    {
        printf("CAN initialization failed, error code: %d\n", status);
        return;
    }
    mcan_enable_interrupts(ptr, interrupt_mask);
    mcan_enable_txbuf_transmission_interrupt(ptr, ~0UL);

    intc_m_enable_irq_with_priority(IRQn_MCAN3, 2);
}

mcan_filter_elem_t can2_filters[4];

void bsp_mcan2_init()
{
    HPM_IOC->PAD[IOC_PAD_PA24].FUNC_CTL = IOC_PA24_FUNC_CTL_MCAN2_TXD;
    HPM_IOC->PAD[IOC_PAD_PA25].FUNC_CTL = IOC_PA25_FUNC_CTL_MCAN2_RXD;

    MCAN_Type *ptr = HPM_MCAN2;
    mcan_config_t can_config;
    mcan_get_default_config(ptr, &can_config);
    can_config.baudrate = 1000000; /* 1Mbps */
    can_config.mode = mcan_mode_normal;

    can2_filters[0].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can2_filters[0].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO0_IF_MATCH;
    can2_filters[0].can_id_type = MCAN_CAN_ID_TYPE_EXTENDED;
    can2_filters[0].start_id = 0x00000;
    can2_filters[0].end_id = 0x10000;

    can_config.all_filters_config.ext_id_filter_list.filter_elem_list = &can2_filters[0];
    can_config.all_filters_config.ext_id_filter_list.mcan_filter_elem_count = 1;
    //can_config.all_filters_config.std_id_filter_list.filter_elem_list = &can2_filters[0];
    can_config.all_filters_config.std_id_filter_list.mcan_filter_elem_count = 0;
    can_config.all_filters_config.global_filter_config.accept_non_matching_std_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.accept_non_matching_ext_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.reject_remote_std_frame = true;
    can_config.all_filters_config.global_filter_config.reject_remote_ext_frame = true;

    board_init_can(ptr);
    uint32_t can_src_clk_freq = board_init_can_clock(ptr);

    uint32_t interrupt_mask = 0;
    hpm_stat_t status = mcan_init(ptr, &can_config, can_src_clk_freq);
    if (status != status_success)
    {
        printf("CAN initialization failed, error code: %d\n", status);
        return;
    }
    mcan_enable_interrupts(ptr, interrupt_mask);
    mcan_enable_txbuf_transmission_interrupt(ptr, ~0UL);

    intc_m_enable_irq_with_priority(IRQn_MCAN2, 1);
}

mcan_filter_elem_t can1_filters[4];

void bsp_mcan1_init()
{
    HPM_IOC->PAD[IOC_PAD_PA20].FUNC_CTL = IOC_PA20_FUNC_CTL_MCAN1_RXD;
    HPM_IOC->PAD[IOC_PAD_PA21].FUNC_CTL = IOC_PA21_FUNC_CTL_MCAN1_TXD;

    MCAN_Type *ptr = HPM_MCAN1;
    mcan_config_t can_config;
    mcan_get_default_config(ptr, &can_config);
    can_config.baudrate = 1000000; /* 1Mbps */
    can_config.mode = mcan_mode_normal;

    can1_filters[0].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can1_filters[0].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO1_IF_MATCH;
    can1_filters[0].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can1_filters[0].start_id = 0x201;
    can1_filters[0].end_id = 0x20B;

    can1_filters[1].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can1_filters[1].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO0_IF_MATCH;
    can1_filters[1].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can1_filters[1].start_id = 0x400;
    can1_filters[1].end_id = 0x440;

    can1_filters[2].filter_type = MCAN_FILTER_TYPE_SPECIFIED_ID_FILTER;
    can1_filters[2].filter_config = MCAN_FILTER_ELEM_CFG_STORE_INTO_RX_BUFFER_OR_AS_DBG_MSG;
    can1_filters[2].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can1_filters[2].match_id = 0x100;
    can1_filters[2].offset = 0;
    can1_filters[2].filter_event = 0;
    can1_filters[2].store_location = 0;

    can_config.all_filters_config.ext_id_filter_list.mcan_filter_elem_count = 0;
    can_config.all_filters_config.std_id_filter_list.filter_elem_list = &can1_filters[0];
    can_config.all_filters_config.std_id_filter_list.mcan_filter_elem_count = 3;
    can_config.all_filters_config.global_filter_config.accept_non_matching_std_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.accept_non_matching_ext_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.reject_remote_std_frame = true;
    can_config.all_filters_config.global_filter_config.reject_remote_ext_frame = true;

    board_init_can(ptr);
    uint32_t can_src_clk_freq = board_init_can_clock(ptr);

    uint32_t interrupt_mask = 0;
    hpm_stat_t status = mcan_init(ptr, &can_config, can_src_clk_freq);
    if (status != status_success)
    {
        printf("CAN initialization failed, error code: %d\n", status);
        return;
    }
    mcan_enable_interrupts(ptr, interrupt_mask);
    mcan_enable_txbuf_transmission_interrupt(ptr, ~0UL);

    intc_m_enable_irq_with_priority(IRQn_MCAN1, 2);
}

mcan_filter_elem_t can0_filters[4];

void bsp_mcan0_init()
{
    HPM_IOC->PAD[IOC_PAD_PA01].FUNC_CTL = IOC_PA01_FUNC_CTL_MCAN0_RXD;
    HPM_IOC->PAD[IOC_PAD_PA00].FUNC_CTL = IOC_PA00_FUNC_CTL_MCAN0_TXD;

    MCAN_Type *ptr = HPM_MCAN0;
    mcan_config_t can_config;
    mcan_get_default_config(ptr, &can_config);
    can_config.baudrate = 1000000; /* 1Mbps */
    can_config.mode = mcan_mode_normal;

    can0_filters[0].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can0_filters[0].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO1_IF_MATCH;
    can0_filters[0].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can0_filters[0].start_id = 0x201;
    can0_filters[0].end_id = 0x20B;

    can0_filters[1].filter_type = MCAN_FILTER_TYPE_RANGE_FILTER;
    can0_filters[1].filter_config = MCAN_FILTER_ELEM_CFG_STORE_IN_RX_FIFO0_IF_MATCH;
    can0_filters[1].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can0_filters[1].start_id = 0x400;
    can0_filters[1].end_id = 0x440;

    can0_filters[2].filter_type = MCAN_FILTER_TYPE_SPECIFIED_ID_FILTER;
    can0_filters[2].filter_config = MCAN_FILTER_ELEM_CFG_STORE_INTO_RX_BUFFER_OR_AS_DBG_MSG;
    can0_filters[2].can_id_type = MCAN_CAN_ID_TYPE_STANDARD;
    can0_filters[2].match_id = 0x100;
    can0_filters[2].offset = 0;
    can0_filters[2].filter_event = 0;
    can0_filters[2].store_location = 0;

    can_config.all_filters_config.ext_id_filter_list.mcan_filter_elem_count = 0;
    can_config.all_filters_config.std_id_filter_list.filter_elem_list = &can0_filters[0];
    can_config.all_filters_config.std_id_filter_list.mcan_filter_elem_count = 3;
    can_config.all_filters_config.global_filter_config.accept_non_matching_std_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.accept_non_matching_ext_frame_option =
        MCAN_ACCEPT_NON_MATCHING_FRAME_OPTION_IN_RXFIFO1;
    can_config.all_filters_config.global_filter_config.reject_remote_std_frame = true;
    can_config.all_filters_config.global_filter_config.reject_remote_ext_frame = true;

    board_init_can(ptr);
    uint32_t can_src_clk_freq = board_init_can_clock(ptr);

    uint32_t interrupt_mask = 0;
    hpm_stat_t status = mcan_init(ptr, &can_config, can_src_clk_freq);
    if (status != status_success)
    {
        printf("CAN initialization failed, error code: %d\n", status);
        return;
    }
    mcan_enable_interrupts(ptr, interrupt_mask);
    mcan_enable_txbuf_transmission_interrupt(ptr, ~0UL);

    intc_m_enable_irq_with_priority(IRQn_MCAN0, 2);
}