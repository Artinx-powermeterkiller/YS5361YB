#include "ChassisStateYAxisMove.hpp"
#include "ChassisStateYAxisMoveCheck.hpp"

void ChassisStateYAxisMove::Init(ChassisController *pOwner)
{
}

void ChassisStateYAxisMove::Enter(ChassisController *pOwner)
{
}

void ChassisStateYAxisMove::Execute(ChassisController *pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(YAxis, SpeedSet, pOwner->m_set_y_speed);
    manager->AddCommand(YAxis, PosSet, pOwner->m_set_y_pos - pOwner->m_last_y_pos);
    manager->AddCommand(YAxis, Move);
    pOwner->chassisFsm.ChangeState(ChassisStateYAxisMoveCheck::Instance());
    return;
}

void ChassisStateYAxisMove::Exit(ChassisController *pOwner)
{
    ;
}
