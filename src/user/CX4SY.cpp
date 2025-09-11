#include "CX4SY.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

CX4SYManager::CX4SYManager() : RS485DeviceManager(1000)
{
}

void CX4SYManager::BspInit()
{
    bsp_set_uart5_buffer(rx_buffer);
    bsp_set_uart5_buffer_index(&rx_length);
    bsp_set_uart5_idle_flag(&rx_update_flag);

    device_vector[0].m_address = 10;
}

void CX4SYManager::SetChannel(uint8_t _address, uint8_t _channel, uint8_t _channel_set)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CX4SY &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CX4SYCommand::WriteChannelSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = _channel;
        cmd.parameter_optional = _channel_set;

        AddCommand(&cmd);
    }
}

void CX4SYManager::SetAllChannelStart(uint8_t _address)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CX4SY &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CX4SYCommand::StartAllChannel;
        cmd.command_type = RS485WriteCommand;

        AddCommand(&cmd);

        it->m_last_write_read_channel = 0xFF;
    }
}

void CX4SYManager::SetTemperature(uint8_t _address, uint8_t _channel, float _temperature)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CX4SY &a)
                           { return a.m_address == _address; });

    RS485Command cmd;
    uint16_t temp_temperature = _temperature * 10.0f;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CX4SYCommand::WriteTemperature;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = _channel;
        cmd.parameter_optional = temp_temperature;

        AddCommand(&cmd);

        it->m_last_write_read_channel = _channel;
    }
}

void CX4SYManager::ReadTemperature(uint8_t _address, uint8_t _channel)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CX4SY &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CX4SYCommand::ReadTemperature;
        cmd.command_type = RS485ReadCommand;
        cmd.parameter = _channel;

        AddCommand(&cmd);

        it->m_last_write_read_channel = _channel;
    }
}

CX4SY::CX4SY() : RS485Device(0)
{
}

void CX4SY::Read(RS485Command *_cmd)
{
    switch (_cmd->command)
    {
    case CX4SYCommand::ReadTemperature:
        ReadTemperature((uint8_t)(_cmd->parameter));
        break;

    default:
        break;
    }
}

uint8_t CX4SY::ReadReceive(uint8_t *_buffer)
{
    if (m_last_write_read_channel == 0xFF)
    {
        return 8;
    }
    
    switch (_buffer[1])
    {
    case 0x03:
        TemperatureReceive(_buffer);
        return 9;
    default:
        return 8;
    }
}

void CX4SY::TemperatureReceive(uint8_t *_buffer)
{
    union CX4SYReadUnion
    {
        int8_t i8[2];
        int16_t i6;
    };

    CX4SYReadUnion read_union;

    for (uint8_t i = 0; i < 2; i++)
    {
        read_union.i8[2-i] = _buffer[3+i];
    }

    m_temperature_fdb[m_last_write_read_channel] = read_union.i6*0.1f;

    for (uint8_t i = 0; i < 2; i++)
    {
        read_union.i8[2-i] = _buffer[5+i];
    }

    m_temperature_set[m_last_write_read_channel] = read_union.i6*0.1f;
}


void CX4SY::Write(RS485Command *_cmd)
{
    switch (_cmd->command)
    {
    case CX4SYCommand::WriteTemperature:
        WriteTemperature(_cmd->parameter, _cmd->parameter_optional);
        break;
    case CX4SYCommand::WriteChannelSet:
        WriteChannelSet(_cmd->parameter, _cmd->parameter_optional);
        break;
    case CX4SYCommand::StartAllChannel:
        StartAllChannel();
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
    case 0:
        start_address = 0x4848;
        break;
    case 1:
        start_address = 0x4888;
        break;
    case 2:
        start_address = 0x48C8;
        break;
    case 3:
        start_address = 0x4908;
        break;
    default:
        return;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = start_address >> 8;
    m_send_buffer[3] = start_address & 0X00ff;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 8, HPM_UART5, (uint32_t)m_send_buffer, 8);
}

void CX4SY::WriteTemperature(uint8_t channel, uint16_t temperature_set)
{
    uint16_t start_address = 0;

    switch (channel)
    {
    case 1:
        start_address = 0x4849;
        break;
    case 2:
        start_address = 0x4889;
        break;
    case 3:
        start_address = 0x48C9;
        break;
    case 4:
        start_address = 0x4909;
        break;
    default:
        return;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = start_address >> 8;
    m_send_buffer[3] = start_address & 0X00ff;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 8, HPM_UART5, (uint32_t)m_send_buffer, 8);
}

void CX4SY::WriteChannelSet(uint8_t channel, uint16_t channel_set)
{
    uint16_t start_address = 0;

    switch (channel)
    {
    case 1:
        start_address = 0x271A;
        break;
    case 2:
        start_address = 0x271B;
        break;
    case 3:
        start_address = 0x271C;
        break;
    case 4:
        start_address = 0x271D;
        break;
    default:
        return;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x0F;
    m_send_buffer[2] = start_address >> 8;
    m_send_buffer[3] = start_address & 0X00ff;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;
    m_send_buffer[6] = 0x01;
    m_send_buffer[7] = channel_set;

    Append_CRC16_Check_Sum(m_send_buffer, 10);

    uart_tx_trigger_dma(HPM_HDMA, 8, HPM_UART5, (uint32_t)m_send_buffer, 10);
}

void CX4SY::StartAllChannel()
{
    uint16_t start_address = 0X271E;

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x0F;
    m_send_buffer[2] = start_address >> 8;
    m_send_buffer[3] = start_address & 0X00ff;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;
    m_send_buffer[6] = 0x01;
    m_send_buffer[7] = 0x01;

    Append_CRC16_Check_Sum(m_send_buffer, 10);

    uart_tx_trigger_dma(HPM_HDMA, 8, HPM_UART5, (uint32_t)m_send_buffer, 10);
}
