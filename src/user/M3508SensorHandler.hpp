#ifndef M3508SENSORHANDLER_HPP
#define M3508SENSORHANDLER_HPP

#include "DJIMotorCANManager.hpp"
#include "MotorFeedback.hpp"
#include "Math.hpp"

/**
 * @brief Sensor Handler for M3508
 */
class M3508SensorHandler : public MotorFeedback
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
     * @note min:-16384 = -20A, max:16384 = 20A
     */
    int16_t given_current;

    /**
     * @brief motor temperate in degree Celsius
     */
    uint8_t temperate;

    /**
     * @brief motor position(after gearbox) in rad
     */
    float rotorPosition;

    /**
     * @brief motor last position(after gearbox) in rad
     */
    float rotorLastPosition;

    /**
     * @brief pointer of CAN for motor communication
     * @note CAN1 or CAN2
     */
    MCAN_Type *can;

    /**
     * @brief motor ID in communication
     * @note 1 to 8 for feedback packet CAN ID 0x201 to 0x208
     */
    uint32_t canId;

    /**
     * @brief constant coefficient that transform raw encoder position
     *        into position in radian
     * @note value: 2Pi / (8192 * 3591 / 187) or 0.00003994074176f
     */
    const static float RawPos2Rad;

    /**
     * @brief constant coefficient that transform raw motor speed in
     *        revolutions per minute into speed in radian per second
     * @note value: 2Pi / (60 * 3591 / 187) or 0.005453242609f
     *
     */
    const static float RawRpm2Rps;

    /**
     * @brief Pi / (3591 / 187), 0.1635972783f, 3591 / 187 ≈ 19.2
     */
    const static float PiDiv19;

public:
    /**
     * @brief initial encoder position
     */
    uint16_t offset;

    /**
     * @brief  Construct M3508SensorHandler object
     */
    M3508SensorHandler();

    /**
     * @brief Regist communicate parameter for sensor
     * @note called in M3508::RegistMotor()
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
    void Init();

        /**
     * @brief update member variables in M3508SensorHandler from buffer_ptr,
     *        and then update member variables in MotorFeedback.
     * @note called in M3508::SensorUpdate()
     */
    void Update();
};

#endif
