#ifndef MOTORFEEDBACK_HPP
#define MOTORFEEDBACK_HPP

/**
 * @brief DJI motor feedback base class
 */
class MotorFeedback
{
public:
    /**
     * @brief CAN receive buffer address in DJIMotorCANManager::m_receive_buffer
     */
    unsigned char *buffer_ptr;

    /**
     * @brief flag that speed updated
     * @note not yet used
     */
    bool hasSpeedFdb;

    /**
     * @brief motor speed feedback in rad/s
     */
    float speedFdb;

    /**
     * @brief last time motor speed feedback in rad/s
     * @note not yet used
     */
    float lastSpeedFdb;

    /**
     * @brief flag that position updated
     * @note not yet used
     */
    bool hasPositionFdb;

    /**
     * @brief motor absolute position feedback in rad
     * @note min: -Pi, max: Pi
     */
    float positionFdb;

    /**
     * @brief last time motor speed feedback in rad/s
     * @note  used in absolute position
     */
    float lastPositionFdb;

    /**
     * @brief flag that acceleration updated
     * @note not yet used
     */
    bool hasAccelerationFdb;

    /**
     * @brief motor acceleration feedback in rad/s^2
     * @note  not yet used
     */
    float accelerationFdb;

    /**
     * @brief last time motor speed feedback in rad/s
     * @note  not yet used
     */
    float lastAccelerationFdb;

    /**
     * @brief flag that temperature updated
     */
    bool hasTemperatureFdb;

    /**
     * @brief motor temperature feedback in degree Celsius
     */
    float temperatureFdb;

    /**
     * @brief Construct MotorFeedback object
     * @note buffer_ptr must be set by SetBufferPtr()
     */
    MotorFeedback() : buffer_ptr(nullptr),
                      hasSpeedFdb(false),
                      speedFdb(0.0f),
                      lastSpeedFdb(0.0f),

                      hasPositionFdb(false),
                      positionFdb(0.0f),
                      lastPositionFdb(0.0f),

                      hasAccelerationFdb(false),
                      accelerationFdb(0.0f),
                      lastAccelerationFdb(0.0f),

                      hasTemperatureFdb(false),
                      temperatureFdb(0.0f)
    {
    }

    /**
     * @brief Set the buffer_ptr
     * 
     * @param _buffer_ptr the address in DJIMotorCANManager::m_receive_buffer
     */
    void SetBufferPtr(unsigned char *_buffer_ptr) { buffer_ptr = _buffer_ptr; }
};

#endif
