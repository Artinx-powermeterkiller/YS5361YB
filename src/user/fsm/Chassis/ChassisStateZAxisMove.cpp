#include "ChassisStateZAxisMove.hpp"
#include "ChassisStateZAxisMoveCheck.hpp"

void ChassisStateZAxisMove::Init(ChassisController *pOwner)
{
}

void ChassisStateZAxisMove::Enter(ChassisController *pOwner)
{
}

void ChassisStateZAxisMove::Execute(ChassisController *pOwner)
{
    ZMotorManager *manager = &(pOwner->z_manager);
    manager->motor[Z2Axis].SetSpeed(pOwner->m_set_z2_speed);
    manager->AddCommand(Z2Axis, ZPosSet, pOwner->m_set_z2_pos - pOwner->m_last_z2_pos);
    pOwner->chassisFsm.ChangeState(ChassisStateZAxisMoveCheck::Instance());
    return;
}

void ChassisStateZAxisMove::Exit(ChassisController *pOwner)
{
    ;
}
