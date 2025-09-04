#ifndef GM6020_SENSOR_HANDLER_HPP
#define GM6020_SENSOR_HANDLER_HPP

#include "DJIMotorCANManager.hpp"
#include "MotorFeedback.hpp"
#include "Math.hpp"
#include "math_first_order_filter.h"

/**
 * @brief Sensor Handler for GM6020
 */
class GM6020SensorHandler : public MotorFeedback
{
private:
    /**
     * @brief motor encoder position
     * @note min:0x0000, max:0x1FFF
     */
    uint16_t ecd;

    /**
     * @brief motor speed in rpm
     * @note in general, it refers to the speed of the motor after gearbox
     */
    int16_t speed_rpm;

    /**
     * @brief motor given current
     * @note min:-16384 = -3A, max:16384 = 3A
     */
    int16_t given_current;

    /**
     * @brief motor temperate in degree Celsius
     */
    uint8_t temperate;

    /**
     * @brief pointer of CAN for motor communication
     * @note CAN1 or CAN2
     */
    MCAN_Type *can;

    /**
     * @brief motor ID in communication
     * @note 1 to 7 for feedback packet CAN ID 0x205 to 0x20B
     */
    uint32_t canId;

    /**
     * @brief motor speed first order low pass filter
     * @note not yet used
     */
    FirstOrderFilter speed_filter;

    /**
     * @brief constant coefficient that transform raw encoder position
     *        into position in radian
     * @note value: 2Pi / 8192 or 0.0007669903939f
     */
    const static float RawPosToRad;

    /**
     * @brief constant coefficient that transform raw motor speed in
     *        revolutions per minute into speed in radian per second
     * @note value: 2Pi / 60 or 0.1047197551f
     */
    const static float RawRpmToRadps;

public:
    /**
     * @brief initial encoder position
     */
    uint16_t offset;

    /**
     * @brief  Construct GM6020SensorHandler object
     */
    GM6020SensorHandler();

    /**
     * @brief Regist communicate parameter for sensor
     * @note called in GM6020::RegistMotor()
     * @param _can pointer of CAN for motor communication
     * @param _canId motor ID in communication
     */
    void RegistSensor(MCAN_Type *_can, int _canId)
    {
        can = _can;
        canId = _canId;
    }

    /**
     * @brief initialization
     * @note not yet used
     */
    virtual void Init();

    /**
     * @brief update member variables in GM6020SensorHandler from buffer_ptr,
     *        and then update member variables in MotorFeedback.
     * @note called in GM6020::SensorUpdate()
     */
    virtual void Update();
};

#endif
