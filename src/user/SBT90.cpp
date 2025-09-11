#include "SBT90.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

SBT90Manager::SBT90Manager() : RS485DeviceManager(100)
{
}

void SBT90Manager::BspInit()
{
    bsp_set_uart1_buffer(rx_buffer);
    bsp_set_uart1_buffer_index(&rx_length);
    bsp_set_uart1_idle_flag(&rx_update_flag);
}

SBT90::SBT90() : RS485Device(0)
{
}

void SBT90::Read(RS485Command *_cmd)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x0B;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x08;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 6, HPM_UART1, (uint32_t)m_send_buffer, 8);
}

uint8_t SBT90::ReadReceive(uint8_t *_buffer)
{
    union SBT90ReadUnion
    {
        int8_t i8[4];
        uint32_t u32;
    };

    SBT90ReadUnion read_union;

    for (size_t i = 0; i < 4; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            read_union.i8[3-j] = _buffer[3 + j + 4 * i];
        }
        force[i] = read_union.u32 *0.01f;
    }

    return 21;
}
