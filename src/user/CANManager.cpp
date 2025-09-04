#include "CANManager.hpp"

#include "board.h"
#include "bsp/bsp_mcan.h"

CANManager::CANManager(/* args */)
{
    _DJIMotorCANManager = DJIMotorCANManager::Instance();
    _BoardMemory = BoardMemory::Instance();
}

CANManager::~CANManager()
{

}

void CANManager::Init()
{
    bsp_mcan0_init();
    bsp_mcan1_init();
    bsp_mcan2_init();
    bsp_mcan3_init();
}

void CANManager::ReceiveUpdate()
{
    mcan_read_rxfifo_mutiframe(HPM_MCAN0,0,&can0_rx0_buff[0],8,&receive_num[0][0]);
    mcan_read_rxfifo_mutiframe(HPM_MCAN0,1,&can0_rx1_buff[0],8,&receive_num[0][1]);

    mcan_read_rxfifo_mutiframe(HPM_MCAN1,0,&can1_rx0_buff[0],8,&receive_num[1][0]);
    mcan_read_rxfifo_mutiframe(HPM_MCAN1,1,&can1_rx1_buff[0],8,&receive_num[1][1]);


    mcan_read_rxfifo_mutiframe(HPM_MCAN2,0,&can2_rx0_buff[0],8,&receive_num[2][0]);
    mcan_read_rxfifo_mutiframe(HPM_MCAN2,1,&can2_rx1_buff[0],8,&receive_num[2][1]);

    mcan_read_rxfifo_mutiframe(HPM_MCAN3,0,&can3_rx0_buff[0],8,&receive_num[3][0]);
    mcan_read_rxfifo_mutiframe(HPM_MCAN3,1,&can3_rx1_buff[0],8,&receive_num[3][1]); 

    for(uint8_t i = 0; i<receive_num[0][1];i++)
    {
      _DJIMotorCANManager->Receive(0,&can0_rx1_buff[i]);
    }

    receive_num[0][1] = 0;

    for(uint8_t i = 0; i<receive_num[1][1];i++)
    {
      _DJIMotorCANManager->Receive(1,&can1_rx1_buff[i]);
    }

    receive_num[1][1] = 0;

    for(uint8_t i = 0; i<receive_num[2][1];i++)
    {
      _DJIMotorCANManager->Receive(2,&can2_rx1_buff[i]);
    }

    receive_num[2][1] = 0;

    for(uint8_t i = 0; i<receive_num[3][1];i++)
    {
      _DJIMotorCANManager->Receive(3,&can3_rx1_buff[i]);
    }

    receive_num[3][1] = 0;

    _DJIMotorCANManager->ReceiveUpdate();
}

void CANManager::SendUpdate()
{
    _DJIMotorCANManager->SendUpdate();
}
