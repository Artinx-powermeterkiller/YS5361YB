#include "TDP6260.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

void TDP6260Manager::Init()
{
    tdp6260.m_address = 0x01;
}

const static uint32_t tdp6260_count_interval = 50;

void TDP6260Manager::Update()
{
    if (m_tick % tdp6260_count_interval == 0)
    {
        if (finished_command_num < command_num)
        {
            switch (command_list[finished_command_num].command_type)
            {
            case CurrentSet:
                tdp6260.UpdateCurrent(command_list[finished_command_num].parameter);
                break;
            case VoltageSet:
                tdp6260.UpdateVoltage(command_list[finished_command_num].parameter);
                break;
            case OutputStateSet:
                tdp6260.UpdateOutputState(command_list[finished_command_num].parameter);
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

void TDP6260Manager::AddCommand(uint32_t command, uint32_t parameter)
{
    if (command_num < 64)
    {
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = parameter;
        command_num++;
    }
}

void TDP6260Manager::AddCommand(uint32_t command)
{
    if (command_num < 64)
    {
        command_list[command_num].command_type = command;
        command_list[command_num].parameter = 0.0f;
        command_num++;
    }
}


TDP6260::TDP6260(/* args */)
{
}

TDP6260::~TDP6260()
{
}

void TDP6260::UpdateCurrent(uint32_t _set)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x10;
    m_send_buffer[2] = 0x07;
    m_send_buffer[3] = 0xD2;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;
    m_send_buffer[6] = 0x02;
    m_send_buffer[7] = (_set & 0x0000FF00) >> 8;
    m_send_buffer[8] = _set & 0x000000FF;

    Append_CRC16_Check_Sum(m_send_buffer, 11);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 11);
}

void TDP6260::UpdateVoltage(uint32_t _set)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x10;
    m_send_buffer[2] = 0x07;
    m_send_buffer[3] = 0xD1;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;
    m_send_buffer[6] = 0x02;
    m_send_buffer[7] = (_set & 0x0000FF00) >> 8;
    m_send_buffer[8] = _set & 0x000000FF;

    Append_CRC16_Check_Sum(m_send_buffer, 11);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 11);
}

void TDP6260::UpdateOutputState(uint32_t _set)
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x10;
    m_send_buffer[2] = 0x07;
    m_send_buffer[3] = 0xE0;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = 0x01;
    m_send_buffer[6] = 0x02;
    m_send_buffer[7] = (_set & 0x0000FF00) >> 8;
    m_send_buffer[8] = _set & 0x000000FF;

    Append_CRC16_Check_Sum(m_send_buffer, 11);

    uart_tx_trigger_dma(HPM_HDMA, 5, HPM_UART0, (uint32_t)m_send_buffer, 11);
}