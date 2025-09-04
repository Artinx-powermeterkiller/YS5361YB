#include "M3508.hpp"

M3508::M3508()
{
    pFeedback = &sensorFeedBack;
    controlMode = Motor::RELAX_MODE;
    positionSet = 0.0f;
    speedSet = 0.0f;
    accelerationSet = 0.0f;
    currentSet = 0.0f;
}
void M3508::SensorUpdate()
{
    sensorFeedBack.Update();
}

static DJIMotorCANManager* _DJIMotorCANManager = DJIMotorCANManager::Instance();

void M3508::ControlUpdate()
{
    if (controlMode == POS_MODE)
    {
        pidPosition.ref = positionSet;
        pidPosition.fdb = pFeedback->positionFdb;

        pidPosition.UpdateResult();

        speedSet = pidPosition.result;
    }

    pidSpeed.ref = speedSet;
    pidSpeed.fdb = pFeedback->speedFdb;

    pidSpeed.UpdateResult();

    currentSet = pidSpeed.result;
    
    if(controlMode == RELAX_MODE)
    {
        currentSet = 0.0f;
    }

    _DJIMotorCANManager->Add2Buffer(can, canId, (int16_t)currentSet);
}
