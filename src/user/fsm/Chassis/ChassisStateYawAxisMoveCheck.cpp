#include "ChassisStateYawAxisMoveCheck.hpp"
#include "ChassisStateWait.hpp"

void ChassisStateYawAxisMoveCheck::Init(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateYawAxisMoveCheck::Enter(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateYawAxisMoveCheck::Execute(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    CL57Motor* motor = &manager->motor[YawAxis];

    if(wait_count > 0 )
    {
        wait_count--;
    }
    else
    {
        manager->AddCommand(YawAxis,ReadState);
        manager->AddCommand(YawAxis,ReadPos);
        pOwner->usb_port->AddSendCommand(WFdb,motor->m_real_pos);
        wait_count = 100;
    }

    if(motor->m_motor_state == CL57Reach)
    {
        motor->m_motor_state = CL57Wait;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }
}

void ChassisStateYawAxisMoveCheck::Exit(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(YawAxis,ReadPos);
    pOwner->m_last_yaw_pos = pOwner->m_set_yaw_pos;
}
