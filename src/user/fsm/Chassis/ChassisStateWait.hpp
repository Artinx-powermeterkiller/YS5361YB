#ifndef CHASSISSTATE_WAIT_HPP
#define CHASSISSTATE_WAIT_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateWait : public State<ChassisController>
{
private:
    
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateWait* Instance()
    {
        static ChassisStateWait instance;
        return &instance;
    }
};

#endif
