#include "GM6020SensorHandler.hpp"

const float GM6020SensorHandler::RawPosToRad = 0.0007669903939f;
const float GM6020SensorHandler::RawRpmToRadps = 0.1047197551f;

GM6020SensorHandler::GM6020SensorHandler()
{
    hasAccelerationFdb = false;
    hasPositionFdb = true;
    hasSpeedFdb = true;
    hasTemperatureFdb = true;
}

void GM6020SensorHandler::Init()
{
 
}

void GM6020SensorHandler::Update()
{
    /* update member variables in GM6020SensorHandler from buffer_ptr-----------------*/
    ecd = (uint16_t)(buffer_ptr[0] << 8 | buffer_ptr[1]);           
    speed_rpm = (uint16_t)(buffer_ptr[2] << 8 | buffer_ptr[3]);     
    given_current = (uint16_t)(buffer_ptr[4] << 8 | buffer_ptr[5]); 
    temperate = buffer_ptr[6];

    /* update last time speed and position -------------------------------------------*/
    lastPositionFdb = positionFdb;
    lastSpeedFdb = speedFdb;

    /* update member variables in MotorFeedback --------------------------------------*/
    positionFdb = Math::LoopFloatConstrain((float)(ecd - offset) * RawPosToRad, 
        - Math::Pi, Math::Pi);
    speedFdb = speed_rpm * RawRpmToRadps;
    temperatureFdb = (float)temperate;
}
