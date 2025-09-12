#include "CL120.hpp"
#include "crc.hpp"
#include "bsp_uart.h"'

CL57Manager::CL57Manager() : RS485DeviceManager(400)
{
    SetTick(200);
}

CL57Motor::CL57Motor() : RS485Device(0)
{
}

void CL57Motor::WritePos(uint32_t _pos)
{
    uint32_t pos = 0;

    if (_pos > 0)
    {
        pos = _pos * 5000.0f;
    }
    else
    {
        pos = 0xFFFFFFFF + (int32_t)(_pos * 5000.0f);
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x10;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x37;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;
    m_send_buffer[6] = 0x04;
    m_send_buffer[7] = _pos >> 24;
    m_send_buffer[8] = (_pos & 0x00FF0000) >> 16;
    m_send_buffer[9] = (_pos & 0x0000FF00) >> 8;
    m_send_buffer[10] = _pos & 0x000000FF;

    Append_CRC16_Check_Sum(m_send_buffer, 13);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 13);
}

void CL57Motor::WriteState(uint16_t _state)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x4E;
    m_send_buffer[4] = _state >> 8;
    m_send_buffer[5] = _state & 0x00FF;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 8);
}

void CL57Motor::WriteSpeed(uint16_t _speed)
{
    uint16_t speed = 0;

    speed = _speed * 15.0f;

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x36;
    m_send_buffer[4] = _speed >> 8;
    m_send_buffer[5] = _speed & 0x00FF;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 8);
}

void CL57Motor::Write(RS485Command *_cmd)
{
    switch (_cmd->command)
    {
    case CL57Command::CL57PosSet:
        WritePos((uint32_t)_cmd->parameter + ((uint32_t)_cmd->parameter_optional) << 16);
        break;
    case CL57Command::CL57SpeedSet:
        WriteState(_cmd->parameter);
        break;
    case CL57Command::CL57StateSet:
        WriteSpeed(_cmd->parameter);
        break;
    default:
        break;
    }
}

uint8_t CL57Motor::ReadReceive(uint8_t *_buffer)
{
    return 8; // todo
}

void CL57Manager::BspInit()
{
    bsp_set_uart0_buffer(rx_buffer);
    bsp_set_uart0_buffer_index(&rx_length);
    bsp_set_uart0_idle_flag(&rx_update_flag);
}

void CL57Manager::SetSpeed(uint8_t _address, float _speed)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CL57Motor &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    uint16_t speed = _speed * 15.0f;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CL57Command::CL57SpeedSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = speed;

        AddCommand(&cmd);
    }
}

void CL57Manager::SetPos(uint8_t _address, float _pos)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CL57Motor &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    uint32_t pos = _pos * 5000.0f;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CL57Command::CL57PosSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = pos & 0x0000FFFF;
        cmd.parameter_optional = pos >> 16;

        AddCommand(&cmd);
    }
}

void CL57Manager::SetState(uint8_t _address, uint8_t _state)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CL57Motor &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CL57Command::CL57StateSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = _state;

        AddCommand(&cmd);
    }
}

CL120Manager::CL120Manager() : RS485DeviceManager(400)
{
    SetTick(0);
}

CL120Motor::CL120Motor() : RS485Device(0)
{
}

void CL120Motor::WriteState(uint16_t _state)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x20;
    m_send_buffer[3] = 0x00;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = (uint8_t)_state;
    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 8);
}

void CL120Motor::WriteSpeed(uint16_t _speed)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x20;
    m_send_buffer[3] = 0x01;
    m_send_buffer[4] = _speed >> 8;
    m_send_buffer[5] = _speed & 0x00FF;
    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 8);
}

void CL120Motor::Write(RS485Command *_cmd)
{
    switch (_cmd->command)
    {
    case CL120Command::CL120StateSet:
        WriteState(_cmd->parameter);
        break;
    case CL120Command::CL120SpeedSet:
        WriteSpeed(_cmd->parameter);
        break;
    default:
        break;
    }
}

uint8_t CL120Motor ::ReadReceive(uint8_t *_buffer)
{
    return 8; // TODO
}

void CL120Manager::BspInit()
{
    bsp_set_uart0_buffer(rx_buffer);
    bsp_set_uart0_buffer_index(&rx_length);
    bsp_set_uart0_idle_flag(&rx_update_flag);
}

void CL120Manager::SetSpeed(uint8_t _address, float _speed)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CL120Motor &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    uint16_t speed = _speed * 1.0f;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CL120Command::CL120SpeedSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = speed;

        AddCommand(&cmd);
    }
}

void CL120Manager::SetState(uint8_t _address, uint8_t _state)
{
    auto it = std::find_if(device_vector.begin(), device_vector.end(), [_address](const CL120Motor &a)
                           { return a.m_address == _address; });

    RS485Command cmd;

    if (it != device_vector.end())
    {
        cmd.address = _address;
        cmd.command = CL120Command::CL120StateSet;
        cmd.command_type = RS485WriteCommand;
        cmd.parameter = _state;

        AddCommand(&cmd);
    }
}
