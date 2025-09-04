#ifndef CHASSISSTATE_Y_AXIS_MOVE_HPP
#define CHASSISSTATE_Y_AXIS_MOVE_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateYAxisMove : public State<ChassisController>
{
private:
    
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateYAxisMove* Instance()
    {
        static ChassisStateYAxisMove instance;
        return &instance;
    }
};

#endif
