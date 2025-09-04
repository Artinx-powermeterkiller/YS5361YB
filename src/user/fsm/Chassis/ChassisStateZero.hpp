#ifndef CHASSISSTATE_ZERO_HPP
#define CHASSISSTATE_ZERO_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateZero : public State<ChassisController>
{
private:
    uint8_t y_finish_flag;
    uint8_t y_wait_count;

    uint8_t up_x_finish_flag;
    uint8_t up_x_wait_count;

    uint8_t yaw_finish_flag;
    uint8_t yaw_wait_count;

    uint8_t z_finish_flag;
    uint8_t down_x_finish_flag;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateZero* Instance()
    {
        static ChassisStateZero instance;
        return &instance;
    }
};

#endif
