#ifndef M2006SENSORHANDLER_HPP
#define M2006SENSORHANDLER_HPP

#include "DJIMotorCANManager.hpp"
#include "MotorFeedback.hpp"
#include "Math.hpp"

/**
 * @brief Sensor Handler for M2006
 */
class M2006SensorHandler : public MotorFeedback
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
     * @note min:-10000 = -10A, max:10000 = 10A
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
     * @note value: 2Pi / (8192 * 36) or 0.0000213052887f
     */
    const static float RawPosToRad;

    /**
     * @brief constant coefficient that transform raw motor speed in
     *        revolutions per minute into speed in radian per second
     * @note value: 2Pi / (60 * 36) or 0.002908882087f
     */
    const static float RawRpmToRadps;

    /**
     * @brief Pi/36, 0.0872664826f
     */
    const static float PiDiv36;
public:
    /**
     * @brief initial encoder position
     */
    uint16_t offset;

    /**
     * @brief  Construct M2006SensorHandler object
     */
    M2006SensorHandler();

    /**
     * @brief Regist communicate parameter for sensor
     * @note called in M2006::RegistMotor()
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
     * @brief update member variables in M2006SensorHandler from buffer_ptr,
     *        and then update member variables in MotorFeedback.
     * @note called in M2006::SensorUpdate()
     */
    virtual void Update();
};

#endif
