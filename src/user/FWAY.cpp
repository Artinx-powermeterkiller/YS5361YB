#include "FYAW.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

FYAW::FYAW() : RS485Device(0)
{
}


void FYAW::Read(RS485Command*  _cmd)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x04;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x00;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 8);
}

uint8_t FYAW::ReadReceive(uint8_t *_buffer)
{
    union FYAWReadUnion
    {
        int8_t i8[4];
        float f32;
    };

    FYAWReadUnion read_union;

    for (uint8_t i = 0; i < 4; i++)
    {
        read_union.i8[3-i] = _buffer[3+i];
    }

    m_gap = read_union.f32;

    return 8;
}

FYAWManager::FYAWManager():RS485DeviceManager(500)
{

}

void FYAWManager::BspInit()
{
    bsp_set_uart3_buffer(rx_buffer);
    bsp_set_uart3_buffer_index(&rx_length);
    bsp_set_uart3_idle_flag(&rx_update_flag);
}
