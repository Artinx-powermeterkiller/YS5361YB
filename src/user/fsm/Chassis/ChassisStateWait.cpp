#include "ChassisStateWait.hpp"
#include "ChassisStateCMD.hpp"

void ChassisStateWait::Init(ChassisController *pOwner)
{
}

void ChassisStateWait::Enter(ChassisController *pOwner)
{
}

static USBPort* _USBPort = USBPort::Instance();

void ChassisStateWait::Execute(ChassisController *pOwner)
{
    // float deltax = abs(pOwner->m_set_up_x_pos - pOwner->m_last_up_x_pos);
    // float deltay = abs(pOwner->m_set_y_pos - pOwner->m_last_y_pos);
    
    // if (deltax > 0.05f)
    // {
    //     pOwner->chassisFsm.ChangeState(ChassisStateXAxisMove::Instance());
    //     return;
    // }

    if(_USBPort->usb_receive_command_num!=0)
    {
        pOwner->chassisFsm.ChangeState(ChassisStateCMD::Instance());
    }
}

void ChassisStateWait::Exit(ChassisController *pOwner)
{
    ;
}
