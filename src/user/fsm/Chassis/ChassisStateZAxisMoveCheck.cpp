#include "ChassisStateZAxisMoveCheck.hpp"
#include "ChassisStateWait.hpp"

void ChassisStateZAxisMoveCheck::Init(ChassisController *pOwner)
{
    m_wait_count =  7000;
}

void ChassisStateZAxisMoveCheck::Enter(ChassisController *pOwner)
{
    m_wait_count =  7000;
}

void ChassisStateZAxisMoveCheck::Execute(ChassisController *pOwner)
{
    ZMotorManager *manager = &(pOwner->z_manager);
    ZMotor* motor = &manager->motor[Z2Axis];

    if (motor->m_fdb_state == ZReach)
    {
        motor->m_fdb_state = ZWait;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }

    m_wait_count--;

    if(m_wait_count%100 == 0)
    {
        manager->AddCommand(Z2Axis,ZReadPos);

        float err = abs(motor->m_real_pos - pOwner->m_set_z2_pos);

        if(err<0.1f)
        {
            m_wait_count = 10;
        }

        pOwner->usb_port->AddSendCommand(ZFdb,motor->m_real_pos);
    }

    if (m_wait_count == 0)
    {
        motor->m_fdb_state = ZWait;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }
}

void ChassisStateZAxisMoveCheck::Exit(ChassisController *pOwner)
{
    pOwner->m_last_z2_pos = pOwner->m_set_z2_pos;

    ZMotorManager *manager = &(pOwner->z_manager);
    ZMotor* motor = &manager->motor[Z2Axis];
    pOwner->usb_port->AddSendCommand(ZFdb,motor->m_real_pos);
}
