#ifndef CHASSISSTATE_Z_AXIS_MOVE_CHECK_HPP
#define CHASSISSTATE_Z_AXIS_MOVE_CHECK_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateZAxisMoveCheck : public State<ChassisController>
{
private:
    uint32_t m_wait_count;

public:
    virtual void Init(ChassisController *pOwner);
    virtual void Enter(ChassisController *pOwner);
    virtual void Execute(ChassisController *pOwner);
    virtual void Exit(ChassisController *pOwner);

    static ChassisStateZAxisMoveCheck *Instance()
    {
        static ChassisStateZAxisMoveCheck instance;
        return &instance;
    }
};

#endif
