#include "M3508SensorHandler.hpp"

const float M3508SensorHandler::RawPos2Rad = 0.00003994074176f;  /* 2Pi / 8191 / 3591 X 187*/
const float M3508SensorHandler::RawRpm2Rps = 0.005453242609f;    /* 2Pi / 60 X 187 / 3591 */
const float M3508SensorHandler::PiDiv19 = 0.1635972783f;      /* PI / 3591 X 187 */

M3508SensorHandler::M3508SensorHandler()
{
    hasAccelerationFdb = false;
    hasPositionFdb = true;
    hasSpeedFdb = true;
    hasTemperatureFdb = true;
    rotorPosition = 0.0f;
    rotorLastPosition = 0.0f;
}

void M3508SensorHandler::Init()
{
    
}

void M3508SensorHandler::Update()
{
    ecd = (uint16_t)(buffer_ptr[0] << 8 | buffer_ptr[1]);           
    speed_rpm = (uint16_t)(buffer_ptr[2] << 8 | buffer_ptr[3]);     
    given_current = (uint16_t)(buffer_ptr[4] << 8 | buffer_ptr[5]); 
    temperate = buffer_ptr[6];  

    lastSpeedFdb = speedFdb;

    rotorLastPosition = rotorPosition;
    lastPositionFdb = positionFdb;
    rotorPosition = ecd * RawPos2Rad - PiDiv19;

    positionFdb += Math::LoopFloatConstrain((rotorPosition - rotorLastPosition), - PiDiv19, PiDiv19);
    speedFdb = speed_rpm * RawRpm2Rps;

    temperatureFdb = (float)temperate;
}
