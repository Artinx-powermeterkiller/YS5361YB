#include "GM6020.hpp"

GM6020::GM6020()
{
    pFeedback = &sensorFeedBack;
    controlMode = Motor::RELAX_MODE;
    positionSet = 0.0f;
    speedSet = 0.0f;
    accelerationSet = 0.0f;
    currentSet = 0.0f;
}


void GM6020::SensorUpdate()
{
    sensorFeedBack.Update();
}

static DJIMotorCANManager* _DJIMotorCANManager = DJIMotorCANManager::Instance();

void GM6020::ControlUpdate()
{	
    float _spdFdb = pFeedback->speedFdb;

    if (controlMode == POS_MODE||controlMode == IMU_MODE)
    {
        float err;
			
        if (controlMode == POS_MODE)
        {
            err = positionSet - pFeedback->positionFdb;
        }
        else
        {
            err = positionSet - imuPositionFdb;
        }
        err = Math::LoopFloatConstrain(err, -Math::Pi, Math::Pi);

        m_err = pFeedback->positionFdb;

        pidPosition.UpdateIResult(err);

        speedSet = pidPosition.kp * err + pidPosition.iResult - pidPosition.kd * pFeedback->speedFdb;

        speedSet = Math::LimitMax(speedSet, pidPosition.maxOut);
    }

    pidSpeed.ref = speedSet;
    pidSpeed.fdb = _spdFdb;

    pidSpeed.UpdateResult();

    currentSet = pidSpeed.result;

    if(controlMode == RELAX_MODE)
    {
        currentSet = 0.0f;
    }

    _DJIMotorCANManager->Add2Buffer(can, canId, (int16_t)currentSet);
}
