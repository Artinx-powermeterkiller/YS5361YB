#include "ChassisStateInit.hpp"
//#include "ChassisStateZero.hpp"

void ChassisStateInit::Init(ChassisController* pOwner)
{
    wait_count = 10000;
}

void ChassisStateInit::Enter(ChassisController* pOwner)
{
    wait_count = 10000;
}

void ChassisStateInit::Execute(ChassisController* pOwner)
{
    if(wait_count > 0 )
    {
        wait_count--;
    }
    else
    {
        //pOwner->chassisFsm.ChangeState(ChassisStateZero::Instance());
    }
}

void ChassisStateInit::Exit(ChassisController* pOwner)
{
    ;
}
