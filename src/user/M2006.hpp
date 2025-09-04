#ifndef M2006_HPP
#define M2006_HPP

#include "Motor.hpp"
#include "M2006SensorHandler.hpp"
#include "Pid.hpp"

/**
 * @brief DJI Gimbal motor M2006
 */
class M2006 : public Motor
{
public:
    /**
     * @brief  M2006 motor sensor handler
     */
    M2006SensorHandler sensorFeedBack;

    /**
     * @brief  M2006 motor speed control PID
     */
    Pid pidSpeed;

    /**
     * @brief  M2006 motor position control PID
     */
    Pid pidPosition;

    /**
     * @brief M2006 control current set, motor speed control PID result
     * @note min: -10000 max: 10000
     */
    float currentSet;

    /**
     * @brief  M2006 communication CAN type define, CAN1 or CAN2, initialized in M2006()
     */
    MCAN_Type *can;

    /**
     * @brief  M2006 communication CAN ID, from 0x201 to 0x208, initialized in M2006()
     */
    uint32_t canId;

    /**
     * @brief Construct M2006 object
     */
    M2006();

    /**
     * @brief Regist motor in DJIMotorCANManager, called in ShooterController::Init()
     * @param _can CAN type, CAN1 or CAN2
     * @param _canId Motor ID, from 0x201 to 0x208
     */
    void RegistMotor(MCAN_Type *_can, int _canId)
    {
        can = _can;
        canId = _canId;
        sensorFeedBack.RegistSensor(_can, _canId);

        if (_can == HPM_MCAN0)
        {
            DJIMotorCANManager::Instance()->RegisterHandler(0, _canId, this);
        }
        else if (_can == HPM_MCAN1)
        {
            DJIMotorCANManager::Instance()->RegisterHandler(1, _canId, this);
        }
        else if (_can == HPM_MCAN2)
        {
            DJIMotorCANManager::Instance()->RegisterHandler(1, _canId, this);
        }
        else if (_can == HPM_MCAN3)
        {
            DJIMotorCANManager::Instance()->RegisterHandler(1, _canId, this);
        }
    }

    /**
     * @brief motor sensor update function, update the value in pFeedback,
     *        inherited from base class Motor
     */
    virtual void SensorUpdate();

    /**
     * @brief motor control update function, update the PID calculate and send control message.
     *        inherited from base class Motor
     */
    virtual void ControlUpdate();
};

#endif
