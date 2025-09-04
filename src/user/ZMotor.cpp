#include "ZMotor.hpp"
#include "Emm_V5.h"
#include "bsp/bsp_mcan.h"

ZMotor::ZMotor()
{
    m_address = 0xFF;
}

ZMotor::~ZMotor()
{
}

void ZMotor::SetSpeed(float _spd)
{
    m_set_speed = _spd*30.0f;
}

void ZMotor::UpdatePos(float _set)
{
    uint32_t clk = 0;

    if(_set>0.0f)
    {
        clk = _set * 1600.0f;
        Emm_V5_Pos_Control(m_address, 0, m_set_speed, 0, clk, 0, 0);
    }
    else
    {
        clk = (-_set) * 1600.0f;
        Emm_V5_Pos_Control(m_address, 1, m_set_speed, 0, clk, 0, 0);
    }

    m_fdb_state = ZMove;
}

void ZMotor::ReadPos()
{
    Emm_V5_Read_Sys_Params(m_address, S_CPOS);
}

void ZMotor::Receive(uint8_t *_buffer)
{
    uint8_t reg_address = _buffer[0];

    switch (reg_address)
    {
    case 0x36:
        ReceivePos(_buffer);
        break;
    case 0xFD:
        if (_buffer[1] == 0x9F)
        {
            m_fdb_state = ZReach;
        }
    default:
        break;
    }
}

void ZMotor::ReceivePos(uint8_t *_buffer)
{
    uint8_t sign = _buffer[2];

    union RegData
    {
        uint8_t i8[4];
        uint32_t i32;
    };

    RegData reg_data;

    for (uint8_t i = 0; i < 4; i++)
    {
        reg_data.i8[3 - i] = _buffer[3 + i];
    }

    uint32_t temp_pos = 0;

    temp_pos = reg_data.i32;

    m_fdb_pos = (float)temp_pos * 0.000030517578125f;

    if (sign == 1)
    {
        m_fdb_pos = -m_fdb_pos;
    }

    m_real_pos = (m_fdb_pos - m_zero_pos) *0.00390625;
}

void ZMotorManager::Init()
{
    motor[Z1Axis].m_address = 0x01;
    motor[Z1Axis].m_motor_type = Z1Axis;

    motor[Z2Axis].m_address = 0x02;
    motor[Z2Axis].m_motor_type = Z2Axis;

    motor[Z3Axis].m_address = 0x03;
    motor[Z3Axis].m_motor_type = Z3Axis;
}

ZMotor *ZMotorManager::GetMotor(uint8_t _address)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        if (motor[i].m_address == _address)
        {
            return &motor[i];
        }
    }
    return nullptr;
}

const static uint32_t zmotor_count_interval = 10;

void ZMotorManager::Update()
{
    if (m_tick % zmotor_count_interval == 0)
    {
        if (finished_command_num < command_num)
        {
            ZMotor *p_motor = nullptr;
            switch (command_list[finished_command_num].command_type)
            {
            case ZPosSet:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->UpdatePos(command_list[finished_command_num].parameter);
                break;
            case ZReadPos:
                p_motor = &motor[command_list[finished_command_num].motor_type];
                p_motor->ReadPos();
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

void ZMotorManager::RxUpdate()
{
    mcan_rx_message_t can0_rx0_buff[8];
    uint8_t receive_num = 0;
    uint8_t address = 0;
    ZMotor * motor = nullptr;

    mcan_read_rxfifo_mutiframe(HPM_MCAN2, 0, &can0_rx0_buff[0], 8, &receive_num);

    if (receive_num != 0)
    {
        for (uint8_t i = 0; i < receive_num; i++)
        {
            address = can0_rx0_buff[i].ext_id>>8;
            motor = GetMotor(address);

            motor->Receive(can0_rx0_buff[i].data_8);
        }
    }
}

void ZMotorManager::AddCommand(uint32_t motor, uint32_t command, float parameter)
{
    if (command_num < 64)
    {
        command_list[command_num].motor_type = motor;
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = parameter;
        command_num++;
    }
}

void ZMotorManager::AddCommand(uint32_t motor, uint32_t command)
{
    if (command_num < 64)
    {
        command_list[command_num].motor_type = motor;
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = 0.0f;
        command_num++;
    }
}
