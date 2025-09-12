#include "ChassisController.hpp"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "Math.hpp"
#include "ChassisStateInit.hpp"
#include "ChassisStateWait.hpp"

ChassisController::ChassisController() : ControllerEntity(ECT_ChassisController), chassisFsm(this)
{
}


void ChassisController::Init()
{
    SetDefaultTicksToUpdate(1);

    cl120_manager.Init();
    cl57_manager.Init();
    fyaw_manager.Init();
    cx4sy_manager.Init();
    thts_manager.Init();
    sbt90_manager.Init();

    usb_port = USBPort::Instance();

    chassisFsm.Init();
}

uint32_t motor_count = 0;
uint32_t fyaw_count = 0;
uint32_t cx4sy_count = 0;
uint32_t debug_count = 0;

bool set_flag = false;

void ChassisController::Update()
{
    fyaw_manager.ReceiceUpdate();
    cx4sy_manager.ReceiceUpdate();
    sbt90_manager.ReceiceUpdate();
    thts_manager.ReceiceUpdate();
    cl57_manager.ReceiceUpdate();

    if(cl57_manager.m_command_num == 0)
    {
        cl57_manager.SetSpeed(0x01,100);
        cl57_manager.SetSpeed(0x02,100);
        cl57_manager.SetSpeed(0x03,100);
        cl57_manager.SetSpeed(0x04,100);
    }

    //if(set_flag == true)
    //{
    //    cl120_manager.SetSpeed(0x05,100);
    //    cl120_manager.SetSpeed(0x06,200);
    //    cl120_manager.SetSpeed(0x07,300);
    //    cl120_manager.SetSpeed(0x08,400);
    //    cl120_manager.SetSpeed(0x09,500);
    //    cl120_manager.SetSpeed(0x0A,600);
    //    set_flag = false;
    //}

    chassisFsm.HandleInput();
    chassisFsm.Update();

    if(fyaw_manager.m_command_num == 0)
    {
        fyaw_manager.AddCommand((fyaw_count%4)+1);
        fyaw_count++;
    }

    if(cx4sy_manager.m_command_num == 0)
    {  
        cx4sy_manager.ReadTemperature(10,(CX4SYChannel1+cx4sy_count)%4);
        cx4sy_count++;
    }

    if(sbt90_manager.m_command_num == 0)
    {
        sbt90_manager.AddCommand(1);
    }

    if(thts_manager.m_command_num == 0)
    {
        thts_manager.AddCommand(1);
    }

    fyaw_manager.SendUpdate();
    cx4sy_manager.SendUpdate();
    sbt90_manager.SendUpdate();
    thts_manager.SendUpdate();
    cl57_manager.SendUpdate();
    cl120_manager.SendUpdate();
}

void ChassisFsm::HandleInput()
{
}

void ChassisFsm::Init()
{
    ChassisStateInit::Instance()->Init(m_pOwner);

    SetCurrentState(ChassisStateInit::Instance());
}
