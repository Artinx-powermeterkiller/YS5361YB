#include "CX4SY.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

CX4SYManager::CX4SYManager() : RS485DeviceManager(500)
{
}

void CX4SYManager::BspInit()
{
    bsp_set_uart5_buffer(rx_buffer);
    bsp_set_uart5_buffer_index(&rx_length);
    bsp_set_uart5_idle_flag(&rx_update_flag);
}

CX4SY::CX4SY() : RS485Device(0)
{
}

void CX4SY::Read(RS485Command*  _cmd)
{
    switch (_cmd->command)
    {
    case CX4SYCommand::ReadTemperature :
        ReadTemperature((uint8_t)(_cmd->parameter));
        break;

    default:
        break;
    }
}

uint8_t CX4SY::ReadReceive(uint8_t *_buffer)
{
}

void CX4SY::Write(RS485Command* _cmd)
{
    switch (_cmd->command)
    {
    case CX4SYCommand::WriteTemperature :
        WriteTemperature(_cmd->parameter,_cmd->parameter_optional);
        break;
    case CX4SYCommand::WriteChannelSet :
        WriteChannelSet(_cmd->);
        break;
    case CX4SYCommand::StartAllChannel :
        //StartAllChannel();
        break;
    default:
        break;
    }
}

void CX4SY::ReadTemperature(uint8_t channel)
{
    uint16_t start_address = 0; 

    switch (channel)
    {
    case 1:
        start_address = 0x4848;
        break;
    case 2:
        start_address = 0x4849;
        break;
    case 3:
        start_address = 0x484A;
        break;
    case 4:
        start_address = 0x484B;
        break;
    default:
        return;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = start_address>>8;
    m_send_buffer[3] = start_address & 0X00ff;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 8, HPM_UART5, (uint32_t)m_send_buffer, 8);
}
