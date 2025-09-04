#ifndef CHASSISSTATE_YAW_AXIS_MOVE_HPP
#define CHASSISSTATE_YAW_AXIS_MOVE_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateYawAxisMove : public State<ChassisController>
{
private:
    
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateYawAxisMove* Instance()
    {
        static ChassisStateYawAxisMove instance;
        return &instance;
    }
};

#endif
