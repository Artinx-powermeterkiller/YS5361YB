#include "ChassisStateXAxisMoveCheck.hpp"
#include "ChassisStateWait.hpp"

void ChassisStateXAxisMoveCheck::Init(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateXAxisMoveCheck::Enter(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateXAxisMoveCheck::Execute(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    CL57Motor* motor = &manager->motor[UpXAxis];

    if(wait_count > 0 )
    {
        wait_count--;
    }
    else
    {
        manager->AddCommand(UpXAxis,ReadState);
        manager->AddCommand(UpXAxis,ReadPos);
        pOwner->usb_port->AddSendCommand(XFdb,motor->m_real_pos);
        wait_count = 100;
    }

    if(motor->m_motor_state == CL57Reach)
    {
        motor->m_motor_state = CL57Wait;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }
}

void ChassisStateXAxisMoveCheck::Exit(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(UpXAxis,ReadPos);
    pOwner->m_last_up_x_pos = pOwner->m_set_up_x_pos;
}
