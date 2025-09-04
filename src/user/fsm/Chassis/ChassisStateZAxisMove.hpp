#ifndef CHASSISSTATE_Z_AXIS_MOVE_HPP
#define CHASSISSTATE_Z_AXIS_MOVE_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateZAxisMove : public State<ChassisController>
{
private:
    
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateZAxisMove* Instance()
    {
        static ChassisStateZAxisMove instance;
        return &instance;
    }
};

#endif
