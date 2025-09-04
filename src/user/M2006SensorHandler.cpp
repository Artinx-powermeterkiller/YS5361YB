#include "M2006SensorHandler.hpp"

const float M2006SensorHandler::RawPosToRad = 0.00002130788978f;
const float M2006SensorHandler::RawRpmToRadps = 0.002908882087f;
const float M2006SensorHandler::PiDiv36 = 0.0872664826f;

M2006SensorHandler::M2006SensorHandler()
{
    hasAccelerationFdb = false;
    hasPositionFdb = true;
    hasSpeedFdb = true;
    hasTemperatureFdb = true;
    rotorPosition = 0.0f;
    rotorLastPosition = 0.0f;
}

void M2006SensorHandler::Init()
{
   
}

void M2006SensorHandler::Update()
{
    ecd = (uint16_t)(buffer_ptr[0] << 8 | buffer_ptr[1]);           
    speed_rpm = (uint16_t)(buffer_ptr[2] << 8 | buffer_ptr[3]);     
    given_current = (uint16_t)(buffer_ptr[4] << 8 | buffer_ptr[5]); 
    temperate = buffer_ptr[6];  

    lastSpeedFdb = speedFdb;

    rotorLastPosition = rotorPosition;
    lastPositionFdb = positionFdb;
    rotorPosition = ecd * RawPosToRad - PiDiv36;

    positionFdb += Math::LoopFloatConstrain((rotorPosition - rotorLastPosition), - PiDiv36, PiDiv36);
    speedFdb = speed_rpm * RawRpmToRadps;

    temperatureFdb = (float)temperate;
}
