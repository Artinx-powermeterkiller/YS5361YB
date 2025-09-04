#ifndef TESTBOT_HPP
#define TESTBOT_HPP

#include "RobotEngine.hpp"
#include "fsm/Chassis/ChassisController.hpp"

class Testbot : public RobotEngine
{
private:
      ChassisController chassis_controller;
public:
    Testbot() : RobotEngine(this)
    {
    }
};

#endif
