#ifndef CHASSISSTATE_INIT_HPP
#define CHASSISSTATE_INIT_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateInit : public State<ChassisController>
{
private:
    uint32_t wait_count;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateInit* Instance()
    {
        static ChassisStateInit instance;
        return &instance;
    }
};

#endif
