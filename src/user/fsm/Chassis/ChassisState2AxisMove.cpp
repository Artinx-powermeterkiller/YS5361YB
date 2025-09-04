#include "ChassisState2AxisMove.hpp"

void ChassisState2AxisMove::Init(ChassisController *pOwner)
{
    m_temp_vy = 0.0f;
    m_temp_vx = 0.0f;
}

void ChassisState2AxisMove::Enter(ChassisController *pOwner)
{
    float abs_x = abs(pOwner->m_set_up_x_pos - pOwner->m_last_up_x_pos);
    float abs_y = abs(pOwner->m_set_y_pos - pOwner->m_last_y_pos);
    float abs_xy = sqrt(abs_x * abs_x + abs_y * abs_y);

    m_temp_vy = pOwner->m_set_xy_speed * abs_y / abs_xy;
    m_temp_vx = pOwner->m_set_xy_speed * abs_x / abs_xy;
}

void ChassisState2AxisMove::Execute(ChassisController *pOwner)
{
    CL57MotorManager *manager = &(pOwner->cl57_manager);

    manager->AddCommand(UpXAxis, SpeedSet, m_temp_vx);
    manager->AddCommand(UpXAxis, PosSet, -(pOwner->m_set_up_x_pos - pOwner->m_last_up_x_pos));
    manager->AddCommand(UpXAxis, Move);
    manager->AddCommand(YAxis, SpeedSet, m_temp_vy);
    manager->AddCommand(YAxis, PosSet, -(pOwner->m_set_y_pos - pOwner->m_last_y_pos));
    manager->AddCommand(YAxis, Move);

    //pOwner->chassisFsm.ChangeState(ChassisStateMoveCheck::Instance());
}

void ChassisState2AxisMove::Exit(ChassisController *pOwner)
{
    m_temp_vy = 0.0f;
    m_temp_vx = 0.0f;
}
