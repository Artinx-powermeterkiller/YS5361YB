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
    FYAWManager fyaw_manager;
    THTSManager thts_manager;
    USBPort* usb_port;
    
    float m_set_up_x_pos; // mm
    float m_set_y_pos; // mm
    float m_set_yaw_pos; // degree
    float m_set_down_x_pos;
    float m_set_z2_pos;//mm

    float m_set_up_x_speed; //mm/s
    float m_set_yaw_speed; // deg/s
    float m_set_y_speed; //mm/s
    float m_set_xy_speed;//mm/s
    float m_set_z2_speed; //mm/s

    float m_last_up_x_pos;
    float m_last_y_pos;
    float m_last_yaw_pos;
    float m_last_down_x_pos;
    float m_last_z2_pos;

    ChassisController();

    virtual void Init();
    virtual void Update();
};

#endif
