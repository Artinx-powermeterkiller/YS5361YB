#ifndef CHASSISSTATE_YAW_AXIS_MOVE_CHECK_HPP
#define CHASSISSTATE_YAW_AXIS_MOVE_CHECK_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateYawAxisMoveCheck : public State<ChassisController>
{
private:
    uint32_t wait_count;
public:
    virtual void Init(ChassisController *pOwner);
    virtual void Enter(ChassisController *pOwner);
    virtual void Execute(ChassisController *pOwner);
    virtual void Exit(ChassisController *pOwner);

    static ChassisStateYawAxisMoveCheck *Instance()
    {
        static ChassisStateYawAxisMoveCheck instance;
        return &instance;
    }
};

#endif
