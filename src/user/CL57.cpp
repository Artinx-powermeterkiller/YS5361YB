#include "CL57.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

void CL57MotorManager::Init()
{
    motor[UpXAxis].m_address = 0x02;
    motor[UpXAxis].m_motor_type = UpXAxis;

    motor[YAxis].m_address = 0x01;
    motor[YAxis].m_motor_type = YAxis;

    motor[YawAxis].m_address = 0x03;
    motor[YawAxis].m_motor_type = YawAxis;
    // motor[UpXAxis].m_address = 0x04;

    bsp_set_uart7_buffer(rx_buffer);
    bsp_set_uart7_buffer_index(&rx_length);
    bsp_set_uart7_idle_flag(&rx_update_flag);

    rx_update_flag = 0;
    rx_length = 0;
}

CL57Motor *CL57MotorManager::GetMotor(uint8_t _address)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        if (motor[i].m_address == _address)
        {
            return &motor[i];
        }
    }
    return nullptr;
}

const static uint32_t cl57_count_interval = 20;

void CL57MotorManager::Update()
{
    if (m_tick % cl57_count_interval == 0)
    {
        if (finished_command_num < command_num)
        {
            CL57Motor *p_motor = nullptr;
            switch (command_list[finished_command_num].command_type)
            {
            case PosSet:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->UpdatePos(command_list[finished_command_num].parameter);
                break;
            case SpeedSet:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->UpdateSpeed(command_list[finished_command_num].parameter);
                break;
            case Move:
            case Stop:
            case CL57CommandType::EmergencyStop:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->UpdateState(command_list[finished_command_num].command_type);
                break;
            case ReadPos:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->ReadPos();
                break;
            case ReadState:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->ReadState();
                break;
            default:
                break;
            }
            finished_command_num++;
        }
        else
        {
            finished_command_num = command_num = 0;
        }
    }

    m_tick++;
}

void CL57MotorManager::RxUpdate()
{
    uint8_t head = 0;
    uint8_t temp_length = 0;
    if (rx_update_flag == 1)
    {
        while (head < rx_length)
        {
            CL57Motor *motor = GetMotor(rx_buffer[head]);

            if (motor != nullptr)
            {
                temp_length = motor->Receive(&rx_buffer[head]);
                head += temp_length;
            }
            else
            {
                head++;
            }
        }
        rx_update_flag = 0;
        rx_length -= head;
    }
}

void CL57MotorManager::AddCommand(uint32_t motor, uint32_t command, float parameter)
{
    if (command_num < 64)
    {
        command_list[command_num].motor_type = motor;
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = parameter;
        command_num++;
    }
}

void CL57MotorManager::AddCommand(uint32_t motor, uint32_t command)
{
    if (command_num < 64)
    {
        command_list[command_num].motor_type = motor;
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = 0.0f;
        command_num++;
    }
}

CL57Motor::CL57Motor(/* args */)
{
    m_address = m_zero_pos_set_flag = m_zero_pos_set_flag = m_read_reg_address =
        m_fdb_pos = m_fdb_zero_pos = m_fdb_speed = m_fdb_state = 0;
}

CL57Motor::~CL57Motor()
{
}

void CL57Motor::UpdatePos(float _set)
{
    _set = -_set; // todo

    uint32_t pos = 0;

    if (m_motor_type != YawAxis)
    {
        if (_set > 0)
        {
            pos = _set * 250.0f;
        }
        else
        {
            pos = 0xFFFFFFFF + (int32_t)(_set * 250.0f);
        }
    }
    else
    {
        if (_set > 0)
        {
            pos = _set * 2.777778f;
        }
        else
        {
            pos = 0xFFFFFFFF + (int32_t)(_set * 2.777778f);
        }
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x10;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x37;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;
    m_send_buffer[6] = 0x04;
    m_send_buffer[7] = pos >> 24;
    m_send_buffer[8] = (pos & 0x00FF0000) >> 16;
    m_send_buffer[9] = (pos & 0x0000FF00) >> 8;
    m_send_buffer[10] = pos & 0x000000FF;

    Append_CRC16_Check_Sum(m_send_buffer, 13);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 13);
}

void CL57Motor::UpdateSpeed(float _set)
{
    uint32_t speed = 0;

    if (m_motor_type != YawAxis)
    {
        speed = _set * 15.0f;
    }
    else
    {
        speed = _set * 0.166666667f;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x36;
    m_send_buffer[4] = speed >> 8;
    m_send_buffer[5] = speed & 0x00FF;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 8);
}

void CL57Motor::UpdateState(uint32_t _set)
{
    uint16_t state_set = 0;

    switch (_set)
    {
    case Move:
        state_set = 0x01;
        m_motor_state = CL57Move;
        break;
    case Stop:
        state_set = 0x24;
        m_motor_state = CL57Wait;
        break;
    case EmergencyStop:
        state_set = 0x44;
        m_motor_state = CL57Wait;
        break;
    default:
        break;
    }

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x4E;
    m_send_buffer[4] = state_set >> 8;
    m_send_buffer[5] = state_set & 0x00FF;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 8);
}

void CL57Motor::ReadPos()
{
    m_read_reg_address = 0x0008;

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x08;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x02;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 8);
}

void CL57Motor::ReadState()
{
    m_read_reg_address = 0x0004;

    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x03;
    m_send_buffer[2] = 0x00;
    m_send_buffer[3] = 0x04;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;

    Append_CRC16_Check_Sum(m_send_buffer, 8);

    uart_tx_trigger_dma(HPM_HDMA, 3, HPM_UART7, (uint32_t)m_send_buffer, 8);
}

uint8_t CL57Motor::Receive(uint8_t *_buffer)
{
    uint8_t _return = 0;

    switch (_buffer[1])
    {
    case 0x03:
        _return = ReadReceive(_buffer);
        break;
    case 0x06:
        _return = 8;
        break;
    case 0x10:
        _return = 8;
        break;
    case 0x83:
    case 0x86:
    case 0x90:
        break;
    default:
        _return = 1;
        break;
    }

    return _return;
}

uint8_t CL57Motor::ReadReceive(uint8_t *_buffer)
{
    uint8_t reg_length = _buffer[2];
    int32_t temp_pos = 0;
    int16_t temp_state = 0;

    union RegData
    {
        int8_t i8[4];
        int16_t i16[2];
        int32_t i32;
    };

    RegData reg_data;

    for (uint8_t i = 0; i < reg_length; i++)
    {
        reg_data.i8[3 - i] = _buffer[3 + i];
    }

    switch (m_read_reg_address)
    {
    case 0x0004:
        temp_state = reg_data.i16[1];

        if (temp_state & 0x0001 != 0)
        {
           m_motor_state = CL57Reach;
        }
        break;
    case 0x0008:
        temp_pos = -reg_data.i32;

        if (m_motor_type != YawAxis)
        {
            m_fdb_pos = temp_pos * 0.004f;
        }
        else
        {
            m_fdb_pos = temp_pos * 0.36f;
        }

        m_real_pos = m_fdb_pos - m_fdb_zero_pos;
        break;
    default:
        break;
    }

    return reg_length + 5;
}
