#ifndef M3508_HPP
#define M3508_HPP

#include "Motor.hpp"
#include "M3508SensorHandler.hpp"
#include "Pid.hpp"

/**
 * @brief DJI Gimbal motor M3508
 */
class M3508 : public Motor
{
public:
    /**
     * @brief  M3508 motor sensor handler
     */
    M3508SensorHandler sensorFeedBack;

    /**
     * @brief  M3508 motor speed control PID
     */
    Pid pidSpeed;

    /**
     * @brief  M3508 motor position control PID
     */
    Pid pidPosition;

    /**
     * @brief M3508 control current set, motor speed control PID result
     * @note min: -16384 max: 16384
     */
    float currentSet;

    /**
     * @brief  M3508 communication CAN type define, CAN1 or CAN2, initialized in M3508()
     */
    MCAN_Type *can;

    /**
     * @brief  M3508 communication CAN ID, from 0x201 to 0x208, initialized in M3508()
     */
    uint32_t canId;

    /**
     * @brief Construct M3508 object
     */
    M3508();

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
            DJIMotorCANManager::Instance()->RegisterHandler(2, _canId, this);
        }
        else if (_can == HPM_MCAN3)
        {
            DJIMotorCANManager::Instance()->RegisterHandler(3, _canId, this);
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
