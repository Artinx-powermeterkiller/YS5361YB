#ifndef BOARD_MEMORY_HPP
#define BOARD_MEMORY_HPP

#include <stdint.h>
#include "hpm_mcan_drv.h"

enum BoardAddress : uint8_t
{
    GimbalBoardAddress  = 0x00,
    ChassisBoardAddress = 0x10,
    SuperCapBoardAddress = 0x20,
    IMUBoardAddress = 0x30
};

enum BoardBank: uint64_t
{
    GimbalBoardBank  = 0xFFFF000000000000,
    ChassisBoardBank = 0x0000FFFF00000000,
    SuperCapBoardBank = 0x00000000FFFF0000,
    IMUBoardBank = 0x000000000000FFFF
};

#define BoardCAN HPM_MCAN0

const uint16_t CAN_BIAS_ID = 0x0400;
const uint8_t MAX_BOARD_PACKET_LENGTH = 64;
const uint8_t MIN_BOARD_PACKET_GAP = 2;
const uint8_t MY_ADDRESS = GimbalBoardAddress;

class BoardPacket
{
protected:
    uint8_t m_board_id;
    uint8_t m_board_address;
    uint8_t m_board_length;
    uint8_t m_my_mask_begin;
    uint8_t m_my_mask_end;
    uint64_t m_my_mask;
public:
    BoardPacket(uint8_t _id,uint8_t _length);
	BoardPacket(uint8_t _address,uint8_t _id,uint8_t _length);
    ~BoardPacket(){}

    bool m_receive_flag;
    bool m_send_flag;

    bool CheckMask(uint64_t _mask);
    void SetMask(uint64_t* mask);
    void ClearMask(uint64_t* mask);
    uint8_t GetMaskBegin(void){return m_my_mask_begin;}
    virtual void Send(uint8_t* _buffer) = 0;
    virtual void Receive(uint8_t* _buffer) = 0;
};

class BoardMemory
{
private:
    uint8_t m_receive_reset_count;
    uint8_t m_board_memory[512];
    uint8_t* m_board_memory_ptr[64];

    uint64_t m_send_mask;
    uint64_t m_receive_mask;
    BoardPacket* m_packet_list[16];
    uint8_t m_packet_num;

    BoardMemory(/* args */);
public:
    static BoardMemory* Instance()
    {
        static BoardMemory instance;
        return &instance;
    }
    ~BoardMemory(){;}

    void Register(BoardPacket* _packet);
    void Receive(mcan_rx_message_t* _rx_msg);
    void ReceiveUpdate();
    void SendUpdate();
};

#endif
