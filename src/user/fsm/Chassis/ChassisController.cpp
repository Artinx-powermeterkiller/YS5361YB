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

    fyaw_manager.Init();
    cx4sy_manager.Init();
    thts_manager.Init();
    sbt90_manager.Init();

    usb_port = USBPort::Instance();

    chassisFsm.Init();
}

uint32_t fyaw_count = 0;
bool set_flag = false;

void ChassisController::Update()
{
    
    fyaw_manager.ReceiceUpdate();
    cx4sy_manager.ReceiceUpdate();
    sbt90_manager.ReceiceUpdate();
    thts_manager.ReceiceUpdate();
    
    chassisFsm.HandleInput();
    chassisFsm.Update();

    if(fyaw_manager.m_command_num == 0)
    {
        fyaw_manager.AddCommand((fyaw_count%4)+1);
        fyaw_count++;
    }

    if(cx4sy_manager.m_command_num == 0)
    {  
        cx4sy_manager.ReadTemperature(10,CX4SYChannel1);
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
}

void ChassisFsm::HandleInput()
{
}

void ChassisFsm::Init()
{
    ChassisStateInit::Instance()->Init(m_pOwner);

    SetCurrentState(ChassisStateInit::Instance());
}
