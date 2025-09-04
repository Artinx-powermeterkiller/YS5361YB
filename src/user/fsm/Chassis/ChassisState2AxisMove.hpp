#ifndef CHASSISSTATE_2AXIS_MOVE_HPP
#define CHASSISSTATE_2AXIS_MOVE_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisState2AxisMove : public State<ChassisController>
{
private:
    float m_temp_vx;
    float m_temp_vy;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisState2AxisMove* Instance()
    {
        static ChassisState2AxisMove instance;
        return &instance;
    }
};

#endif
