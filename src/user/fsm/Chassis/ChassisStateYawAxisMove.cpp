#include "ChassisStateYawAxisMove.hpp"
#include "ChassisStateYawAxisMoveCheck.hpp"

void ChassisStateYawAxisMove::Init(ChassisController *pOwner)
{

}         

void ChassisStateYawAxisMove::Enter(ChassisController *pOwner)
{

}

void ChassisStateYawAxisMove::Execute(ChassisController *pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(YawAxis, SpeedSet, pOwner->m_set_yaw_speed);
    manager->AddCommand(YawAxis, PosSet, pOwner->m_set_yaw_pos - pOwner->m_last_yaw_pos);
    manager->AddCommand(YawAxis, Move);
    pOwner->chassisFsm.ChangeState(ChassisStateYawAxisMoveCheck::Instance());
    return;
}

void ChassisStateYawAxisMove::Exit(ChassisController *pOwner)
{
    ;
}
