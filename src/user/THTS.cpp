#include "THTS.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

THTSManager::THTSManager():RS485DeviceManager(200)
{
}

void THTSManager::BspInit()
{
    bsp_set_uart7_buffer(rx_buffer);
    bsp_set_uart7_buffer_index(&rx_length);
    bsp_set_uart7_idle_flag(&rx_update_flag);
}

THTS::THTS() : RS485Device(0)
{
}

void THTS::Read(RS485Command*  _cmd)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x04;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x00;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x14;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 8);
}

uint8_t THTS::ReadReceive(uint8_t *_buffer)
{
    union THTSReadUnion
    {
        int8_t i8[2];
        int16_t i16;
    };

    THTSReadUnion read_union;

    for (uint8_t i = 0; i < 16; i++)
    {
        read_union.i8[1] = _buffer[3+i*2];
        read_union.i8[0] = _buffer[4+i*2];

        m_temperature[i] = (float)read_union.i16 * 0.1f;
    }

    for (uint8_t i = 16; i < 20; i++)
    {
        read_union.i8[1] = _buffer[3+i*2];
        read_union.i8[0] = _buffer[4+i*2];

        m_gap[i-16] = (float)read_union.i16 * 0.1f;
    }

    return 45;
}

