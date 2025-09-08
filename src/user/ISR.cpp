//#include "CANManager.hpp"
#include "hpm_mcan_drv.h"

SDK_DECLARE_EXT_ISR_M(IRQn_MCAN0, mcan0_isr);
SDK_DECLARE_EXT_ISR_M(IRQn_MCAN1, mcan1_isr);
SDK_DECLARE_EXT_ISR_M(IRQn_MCAN2, mcan2_isr);
SDK_DECLARE_EXT_ISR_M(IRQn_MCAN3, mcan3_isr);

void mcan0_isr(void)
{
    MCAN_Type *base = HPM_MCAN0;
    uint32_t flags = mcan_get_interrupt_flags(base);
    /* New message is available in RXFIFO0 */
    if ((flags & MCAN_INT_RXFIFO0_NEW_MSG) != 0) {
       // mcan_read_rxfifo(base, 0, (mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXFIFO1 */
    if ((flags & MCAN_INT_RXFIFO1_NEW_MSG) != 0U) {
        //mcan_read_rxfifo(base, 1, (mcan_rx_message_t *) &s_can_rx_buf);
        //show_received_can_message((mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXBUF */
    if ((flags & MCAN_INT_MSG_STORE_TO_RXBUF) != 0U) {
        /* NOTE: Below code is for demonstration purpose, the performance is not optimized
         *       Users should optimize the performance according to real use case.
         */
        //for (uint32_t buf_index = 0; buf_index < MCAN_RXBUF_SIZE_CAN_DEFAULT; buf_index++) {
        //    if (mcan_is_rxbuf_data_available(base, buf_index)) {
        //        mcan_read_rxbuf(base, buf_index, (mcan_rx_message_t *) &s_can_rx_buf);
        //        mcan_clear_rxbuf_data_available_flag(base, buf_index);
        //    }
        //}
    }

    mcan_clear_interrupt_flags(base, flags);
}

void mcan1_isr(void)
{
    MCAN_Type *base = HPM_MCAN1;
    uint32_t flags = mcan_get_interrupt_flags(base);
    /* New message is available in RXFIFO0 */
    if ((flags & MCAN_INT_RXFIFO0_NEW_MSG) != 0) {
       // mcan_read_rxfifo(base, 0, (mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXFIFO1 */
    if ((flags & MCAN_INT_RXFIFO1_NEW_MSG) != 0U) {
        //mcan_read_rxfifo(base, 1, (mcan_rx_message_t *) &s_can_rx_buf);
        //show_received_can_message((mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXBUF */
    if ((flags & MCAN_INT_MSG_STORE_TO_RXBUF) != 0U) {
        /* NOTE: Below code is for demonstration purpose, the performance is not optimized
         *       Users should optimize the performance according to real use case.
         */
        //for (uint32_t buf_index = 0; buf_index < MCAN_RXBUF_SIZE_CAN_DEFAULT; buf_index++) {
        //    if (mcan_is_rxbuf_data_available(base, buf_index)) {
        //        mcan_read_rxbuf(base, buf_index, (mcan_rx_message_t *) &s_can_rx_buf);
        //        mcan_clear_rxbuf_data_available_flag(base, buf_index);
        //    }
        //}
    }

    mcan_clear_interrupt_flags(base, flags);
}

void mcan2_isr(void)
{
    MCAN_Type *base = HPM_MCAN2;
    uint32_t flags = mcan_get_interrupt_flags(base);
    /* New message is available in RXFIFO0 */
    if ((flags & MCAN_INT_RXFIFO0_NEW_MSG) != 0) {
       // mcan_read_rxfifo(base, 0, (mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXFIFO1 */
    if ((flags & MCAN_INT_RXFIFO1_NEW_MSG) != 0U) {
        //mcan_read_rxfifo(base, 1, (mcan_rx_message_t *) &s_can_rx_buf);
        //show_received_can_message((mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXBUF */
    if ((flags & MCAN_INT_MSG_STORE_TO_RXBUF) != 0U) {
        /* NOTE: Below code is for demonstration purpose, the performance is not optimized
         *       Users should optimize the performance according to real use case.
         */
        //for (uint32_t buf_index = 0; buf_index < MCAN_RXBUF_SIZE_CAN_DEFAULT; buf_index++) {
        //    if (mcan_is_rxbuf_data_available(base, buf_index)) {
        //        mcan_read_rxbuf(base, buf_index, (mcan_rx_message_t *) &s_can_rx_buf);
        //        mcan_clear_rxbuf_data_available_flag(base, buf_index);
        //    }
        //}
    }

    mcan_clear_interrupt_flags(base, flags);
}

void mcan3_isr(void)
{
    MCAN_Type *base = HPM_MCAN3;
    uint32_t flags = mcan_get_interrupt_flags(base);
    /* New message is available in RXFIFO0 */
    if ((flags & MCAN_INT_RXFIFO0_NEW_MSG) != 0) {
       // mcan_read_rxfifo(base, 0, (mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXFIFO1 */
    if ((flags & MCAN_INT_RXFIFO1_NEW_MSG) != 0U) {
        //mcan_read_rxfifo(base, 1, (mcan_rx_message_t *) &s_can_rx_buf);
        //show_received_can_message((mcan_rx_message_t *) &s_can_rx_buf);
    }
    /* New message is available in RXBUF */
    if ((flags & MCAN_INT_MSG_STORE_TO_RXBUF) != 0U) {
        /* NOTE: Below code is for demonstration purpose, the performance is not optimized
         *       Users should optimize the performance according to real use case.
         */
        //for (uint32_t buf_index = 0; buf_index < MCAN_RXBUF_SIZE_CAN_DEFAULT; buf_index++) {
        //    if (mcan_is_rxbuf_data_available(base, buf_index)) {
        //        mcan_read_rxbuf(base, buf_index, (mcan_rx_message_t *) &s_can_rx_buf);
        //        mcan_clear_rxbuf_data_available_flag(base, buf_index);
        //    }
        //}
    }

    mcan_clear_interrupt_flags(HPM_MCAN3, flags);
}

