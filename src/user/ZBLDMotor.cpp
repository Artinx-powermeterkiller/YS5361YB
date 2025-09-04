#include "ZBLDMotor.hpp"
#include "crc.hpp"
#include "bsp_uart.h"

ZBLDMotor::ZBLDMotor(/* args */)
{
}

ZBLDMotor::~ZBLDMotor()
{
}

void ZBLDMotor::UpdateState()
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x20;
    m_send_buffer[3] = 0x00;
    m_send_buffer[4] = 0x00;
    m_send_buffer[5] = (uint8_t)m_motor_state;
    Append_CRC16_Check_Sum(m_send_buffer,8);

    uart_tx_trigger_dma(HPM_HDMA,3,HPM_UART7,(uint32_t) m_send_buffer,8); 
}

void ZBLDMotor::UpdateSpeed()
{
    m_send_buffer[0] = m_address;
    m_send_buffer[1] = 0x06;
    m_send_buffer[2] = 0x20;
    m_send_buffer[3] = 0x01;
    m_send_buffer[4] = m_set_speed>>8;
    m_send_buffer[5] = m_set_speed&0x00FF;
    Append_CRC16_Check_Sum(m_send_buffer,8);

    uart_tx_trigger_dma(HPM_HDMA,3,HPM_UART7,(uint32_t) m_send_buffer,8); 
}

