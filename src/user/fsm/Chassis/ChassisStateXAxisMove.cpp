#include "ChassisStateXAxisMove.hpp"
#include "ChassisStateXAxisMoveCheck.hpp"

void ChassisStateXAxisMove::Init(ChassisController *pOwner)
{

}         

void ChassisStateXAxisMove::Enter(ChassisController *pOwner)
{

}

void ChassisStateXAxisMove::Execute(ChassisController *pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);
    manager->AddCommand(UpXAxis, SpeedSet, pOwner->m_set_up_x_speed);
    manager->AddCommand(UpXAxis, PosSet, pOwner->m_set_up_x_pos - pOwner->m_last_up_x_pos);
    manager->AddCommand(UpXAxis, Move);
    pOwner->chassisFsm.ChangeState(ChassisStateXAxisMoveCheck::Instance());
    return;
}

void ChassisStateXAxisMove::Exit(ChassisController *pOwner)
{
    ;
}
