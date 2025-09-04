#ifndef CHASSISSTATE_CMD_HPP
#define CHASSISSTATE_CMD_HPP

#include "../StateMachine.hpp"
#include "ChassisController.hpp"

class ChassisStateCMD : public State<ChassisController>
{
private:
    uint32_t m_finished_cmd;
public:
    virtual void Init(ChassisController* pOwner);
    virtual void Enter(ChassisController* pOwner);
    virtual void Execute(ChassisController* pOwner);
    virtual void Exit(ChassisController* pOwner);

    static ChassisStateCMD* Instance()
    {
        static ChassisStateCMD instance;
        return &instance;
    }
};

#endif
