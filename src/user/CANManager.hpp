#ifndef CAN_MANAGER
#define CAN_MANAGER

#include "hpm_mcan_drv.h"
#include "DJIMotorCANManager.hpp"
#include "BoardMemory.hpp"

class CANManager
{
private:
    CANManager(/* args */);

    DJIMotorCANManager* _DJIMotorCANManager;
    BoardMemory* _BoardMemory;

    uint8_t receive_num[4][2];

    mcan_rx_message_t can0_rx1_buff[8];
    mcan_rx_message_t can0_rx0_buff[8];

    mcan_rx_message_t can1_rx1_buff[8];
    mcan_rx_message_t can1_rx0_buff[8];

    mcan_rx_message_t can2_rx1_buff[8];
    mcan_rx_message_t can2_rx0_buff[8];

    mcan_rx_message_t can3_rx1_buff[8];
    mcan_rx_message_t can3_rx0_buff[8];

public:
    static CANManager *Instance()
    {
        static CANManager instance;
        return &instance;
    }

    void Init();

    void ReceiveUpdate();
    void SendUpdate();

    ~CANManager();
};


#endif