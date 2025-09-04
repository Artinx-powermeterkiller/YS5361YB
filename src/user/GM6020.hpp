#ifndef GM6020_HPP
#define GM6020_HPP

#include "Motor.hpp"
#include "Math.hpp"
#include "GM6020SensorHandler.hpp"
#include "Pid.hpp"

/**
 * @brief DJI Gimbal motor GM6020
 */
class GM6020 : public Motor
{
public:

    /**
     * @brief  GM6020 motor sensor handler
     */
    GM6020SensorHandler sensorFeedBack;

    /**
     * @brief  GM6020 motor speed control PID
     */
    Pid pidSpeed;

    /**
     * @brief  GM6020 motor position control PID
     * @note  Pid::UpdateResult() not used but expand at GM6020::ControlUpdate() 
     *        to adapt IMU_MODE
     */
    Pid pidPosition;

    /**
     * @brief GM6020 control current(or voltage, voltage in actually) set,
     *        motor speed control PID result
     * @note min: -25000 max: 25000 for voltage, min: -16384 max 16384 for current
     */
    float currentSet;

    /**
     * @brief  GM6020 communication CAN type define, CAN1 or CAN2, initialized in GM6020()
     */
    MCAN_Type* can;

    /**
     * @brief  GM6020 communication CAN ID, from 0x205 to 0x20B, initialized in GM6020()
     */
    uint32_t canId;

    /**
     * @brief  GM6020 rotational angular velocity by IMU, high precision and 
     *         suppress oscillations, not used
     */
	float ImuW;

    /**
     * @brief  Flag that use rotational angular velocity by IMU, not used
     */
    bool UseImuW;

    /**
     * @brief  unknow purpose
     */
    float m_err;

     /**
     * @brief  first order lowpass filter for speed control to suppress oscillations, not used
     */
	FirstOrderFilter m_filter;

    /**
     * @brief Construct GM6020 object
     */
    GM6020();

    /**
     * @brief Regist motor in DJIMotorCANManager, called in GimbalController::Init()
     * @param _can CAN type, CAN1 or CAN2
     * @param _canId Motor ID, from 0x205 to 0x20B
     */
    void RegistMotor(MCAN_Type* _can, int _canId)
    {
        can = _can;
        canId = _canId;
        sensorFeedBack.RegistSensor(_can, _canId);
		
		if(_can == HPM_MCAN0)
		{
			DJIMotorCANManager::Instance()->RegisterHandler(0,_canId,this);
		}
		else if(_can == HPM_MCAN1)
		{
			DJIMotorCANManager::Instance()->RegisterHandler(1,_canId,this);
		}
                else if(_can == HPM_MCAN2)
		{
			DJIMotorCANManager::Instance()->RegisterHandler(1,_canId,this);
		}
                else if(_can == HPM_MCAN3)
		{
			DJIMotorCANManager::Instance()->RegisterHandler(1,_canId,this);
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
