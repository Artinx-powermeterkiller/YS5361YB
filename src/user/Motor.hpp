#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "MotorFeedback.hpp"

/**
 * @brief DJI motor base class
 */
class Motor
{
public:
    /**
     * @brief  motor control mode enumeration.
     */
    enum MotorControlModeType
    {
        /** relax mode, force motor has no output*/
        RELAX_MODE,

        /** speed control mode, speedSet is the speed PID set value, MotorFeedback::speedFdb is the speed PID feedback value*/
        SPD_MODE,

        /** position control mode, positionSet is the speed PID set value, MotorFeedback::positionFdb is the speed PID feedback value, speed control is also used in POS_MODE*/
        POS_MODE,

        /** position control without speed control mode, no used*/
        POS_FOR_NO_SPD_MODE,

        /** IMU control mode, using IMU position to replace value in pFeedback as PID feedback value, no used*/
        IMU_MODE
    };

    /**
     * @brief Construct Motor object
     * @note pFeedback must be set in subclass constructor.
     */
    Motor() : controlMode{RELAX_MODE},
              pFeedback{nullptr}
    {
    }

    /**
     * @brief motor control mode
     */
    MotorControlModeType controlMode;

    /**
     * @brief motor feedback pointer, must be define before SensorUpdate() and ControlUpdate()
     */
    MotorFeedback *pFeedback;

    /**
     * @brief motor speed set in rad/s
     */
    float speedSet;

    /**
     * @brief motor position set in rad
     */
    float positionSet;

    /**
     * @brief motor acceleration set in rad/s^2, not used
     */
    float accelerationSet;

    /**
     * @brief imu position feedback in rad, used in IMU_MODE
     */
    float imuPositionFdb;

    /**
     * @brief motor sensor update function, update the value in pFeedback.
     * @note pure virtual function, must be override in subclass.
     */
    virtual void SensorUpdate() = 0;

    /**
     * @brief motor control update function, update the PID calculate and send control message.
     * @note pure virtual function, must be override in subclass.
     */
    virtual void ControlUpdate() = 0;
};

#endif
