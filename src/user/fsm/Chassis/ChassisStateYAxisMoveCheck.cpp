#include "ChassisStateYAxisMoveCheck.hpp"
#include "ChassisStateWait.hpp"

void ChassisStateYAxisMoveCheck::Init(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateYAxisMoveCheck::Enter(ChassisController* pOwner)
{
    wait_count = 100;
}

void ChassisStateYAxisMoveCheck::Execute(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    CL57Motor* motor = &manager->motor[YAxis];

    if(wait_count > 0 )
    {
        wait_count--;
    }
    else
    {
        manager->AddCommand(YAxis,ReadState);
        manager->AddCommand(YAxis,ReadPos);
        pOwner->usb_port->AddSendCommand(YFdb,motor->m_real_pos);
        wait_count = 100;
    }

    if(motor->m_motor_state == CL57Reach)
    {
        motor->m_motor_state = CL57Wait;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }
}

void ChassisStateYAxisMoveCheck::Exit(ChassisController* pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(YAxis,ReadPos);
    pOwner->m_last_y_pos = pOwner->m_set_y_pos;
}
