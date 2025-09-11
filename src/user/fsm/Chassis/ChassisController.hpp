#ifndef CHASSISCONTROLLER_HPP
#define CHASSISCONTROLLER_HPP

#define FLT_EPSILON 1.192e-07F
#include <math.h>
#include "../../RobotEngine.hpp"
#include "../StateMachine.hpp"
//#include "../../M3508.hpp"
//#include "../../BoardMemory.hpp"
//#include "../../math_first_order_filter.h"
//#include "../../ZBLDMotor.hpp"
#include "../../CL57.hpp"
#include "../../bsp/bsp_io.h"
#include "../../USBPort.hpp"
#include "../../THTS.hpp"
#include "../../FYAW.hpp"
#include "../../CX4SY.hpp"
#include "../../SBT90.hpp"

// Forward declaration
class ChassisController;

class ChassisFsm : public StateMachine<ChassisController>
{
public:
    ChassisFsm(ChassisController* _pOwner) : StateMachine<ChassisController>(_pOwner){}
    void HandleInput();

    virtual void Init();
};

class ChassisController : public ControllerEntity
{
private:
    
public:
    ChassisFsm chassisFsm;

    CL57MotorManager cl57_manager;
    THTSManager thts_manager;
    CX4SYManager cx4sy_manager;
    FYAWManager fyaw_manager;
    SBT90Manager sbt90_manager;
    USBPort* usb_port;
    
    ChassisController();

    virtual void Init();
    virtual void Update();
};

#endif
