#ifndef CHASSISSTATE_Y_AXIS_MOVE_CHECK_HPP
#define CHASSISSTATE_Y_AXIS_MOVE_CHECK_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateYAxisMoveCheck : public State<ChassisController>
{
private:
    uint32_t wait_count;
public:
    virtual void Init(ChassisController *pOwner);
    virtual void Enter(ChassisController *pOwner);
    virtual void Execute(ChassisController *pOwner);
    virtual void Exit(ChassisController *pOwner);

    static ChassisStateYAxisMoveCheck *Instance()
    {
        static ChassisStateYAxisMoveCheck instance;
        return &instance;
    }
};

#endif
