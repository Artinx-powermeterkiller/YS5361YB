#include "BoardMemory.hpp"
#include <string.h>

BoardPacket::BoardPacket(uint8_t _id,uint8_t _length):m_board_id(_id),
    m_board_length(_length)
{
    m_my_mask_begin = MY_ADDRESS + m_board_id * MIN_BOARD_PACKET_GAP;
    m_my_mask_end = m_my_mask_begin + m_board_length;

    uint64_t temp_mask = 0;

    for (uint8_t i = 0; i < m_board_length; i++)
    {
        temp_mask |= (0x8000000000000000>>(m_my_mask_begin+i));
    }

    m_my_mask = temp_mask;
    m_receive_flag = m_send_flag = false;
}

BoardPacket::BoardPacket(uint8_t _address,uint8_t _id,uint8_t _length):m_board_id(_id),
    m_board_length(_length)
{
    m_board_address = _address;
    m_my_mask_begin = _address + m_board_id * MIN_BOARD_PACKET_GAP;
    m_my_mask_end = m_my_mask_begin + m_board_length;

    uint64_t temp_mask = 0;

    for (uint8_t i = 0; i < m_board_length; i++)
    {
        temp_mask |= (0x8000000000000000>>(m_my_mask_begin+i));
    }

    m_my_mask = temp_mask;
    m_receive_flag = m_send_flag = false;
}

bool BoardPacket::CheckMask(uint64_t _mask)
{
    uint64_t temp_mask = 0;
    uint64_t front_mask = 0;
    uint64_t back_mask = 0;

    switch (m_board_address)
    {
    case GimbalBoardAddress:
       _mask &= GimbalBoardBank;
        break;
    case ChassisBoardAddress:
       _mask &= ChassisBoardBank;
        break;
    case SuperCapBoardAddress:
       _mask &= SuperCapBoardBank;
        break;
    case IMUBoardAddress:
       _mask &= IMUBoardBank;
        break;
    default:
        break;
    }

    if(m_board_id!=0)
    {
        temp_mask = ~(0xFFFFFFFFFFFFFFFF>>m_my_mask_begin);
        front_mask = _mask&temp_mask;

        if(front_mask!=0)
        {
            return false;
        }

        temp_mask = ~((0xFFFFFFFFFFFFFFFF>>m_my_mask_end)<<m_my_mask_end);
        back_mask = _mask&temp_mask;

        if(back_mask!=0)
        {
            //return false;
        }
    }

    if(_mask == m_my_mask)
    {
        return true;
    }
    return false;
}

void BoardPacket::SetMask(uint64_t* mask)
{
    (*mask)|=m_my_mask;
}

void BoardPacket::ClearMask(uint64_t* mask)
{
    (*mask)&=(~(m_my_mask));
}

BoardMemory::BoardMemory(/* args */)
{
    m_receive_reset_count = 10;

    for (uint8_t i = 0; i < 64; i++)
    {
        m_board_memory_ptr[i] = &m_board_memory[8*i];
    }
    
    m_send_mask = m_receive_mask = 0;
}

void BoardMemory::Register(BoardPacket* _packet)
{
    m_packet_list[m_packet_num] = _packet;
    m_packet_num ++;
}

void BoardMemory::Receive(mcan_rx_message_t* _rx_msg)
{
    uint8_t index = _rx_msg->std_id - CAN_BIAS_ID;

    memcpy(m_board_memory_ptr[index],_rx_msg->data_8,8);

    m_receive_mask|= (0x8000000000000000>>(index));
}

uint64_t test_mask = 0;
uint64_t test_mask_result = 0;

/**
 * @brief Check receive mask, update fully received packets,
 *        clear missed receive mask and reset.
 * 
 */
void BoardMemory::ReceiveUpdate()
{
    if(m_receive_mask!=0)
    {
        for (size_t i = 0; i < m_packet_num; i++)
        {
            if(m_packet_list[i]->CheckMask(m_receive_mask))
            {
                m_packet_list[i]->Receive(m_board_memory_ptr[m_packet_list[i]->GetMaskBegin()]);
                m_packet_list[i]->ClearMask(&m_receive_mask);
                m_packet_list[i]->m_receive_flag = true;
            }
        }
    }

    if(m_receive_mask!=0)
    {
        m_receive_reset_count--;
    }
    else
    {
        m_receive_reset_count = 100;
    }

    if(m_receive_reset_count==0)
    {
        m_receive_mask = 0;
    }
}

void BoardMemory::SendUpdate()
{
    for (size_t i = 0; i < m_packet_num; i++)
    {
        if(m_packet_list[i]->m_send_flag)
        {
            m_packet_list[i]->Send(m_board_memory_ptr[m_packet_list[i]->GetMaskBegin()]);
            m_packet_list[i]->SetMask(&m_send_mask);
        }
    }

    uint8_t send_count = 0;

    for (uint8_t i = 0; i < 64; i++)
    {
        if(send_count==2)
        {
            break;
        }
		
        if((m_send_mask&(0x8000000000000000>>i))!=0)
        {
           uint32_t put_index = 0;
           mcan_tx_frame_t tx_buf;

           tx_buf.std_id = CAN_BIAS_ID + i;
           tx_buf.dlc = 0x08;

           memcpy(tx_buf.data_8,m_board_memory_ptr[i],8);
           mcan_transmit_via_txfifo_nonblocking(BoardCAN, &tx_buf,&put_index);

           m_send_mask&=~((0x8000000000000000>>i));

           send_count++;
        }
    }
}
