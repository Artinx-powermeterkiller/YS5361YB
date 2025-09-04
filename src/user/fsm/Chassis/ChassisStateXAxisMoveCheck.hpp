#ifndef CHASSISSTATE_X_AXIS_MOVE_CHECK_HPP
#define CHASSISSTATE_X_AXIS_MOVE_CHECK_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateXAxisMoveCheck : public State<ChassisController>
{
private:
    uint32_t wait_count;
public:
    virtual void Init(ChassisController *pOwner);
    virtual void Enter(ChassisController *pOwner);
    virtual void Execute(ChassisController *pOwner);
    virtual void Exit(ChassisController *pOwner);

    static ChassisStateXAxisMoveCheck *Instance()
    {
        static ChassisStateXAxisMoveCheck instance;
        return &instance;
    }
};

#endif
