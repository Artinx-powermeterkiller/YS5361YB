#ifndef CHASSISSTATE_X_AXIS_MOVE_HPP
#define CHASSISSTATE_X_AXIS_MOVE_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateXAxisMove : public State<ChassisController>
{
private:
    
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateXAxisMove* Instance()
    {
        static ChassisStateXAxisMove instance;
        return &instance;
    }
};

#endif
