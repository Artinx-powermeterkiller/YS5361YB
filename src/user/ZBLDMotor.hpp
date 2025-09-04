#ifndef ZBLD_MOTOR_HPP
#define ZBLD_MOTOR_HPP

#include <stdint.h>

enum class ZBLDMotorState
{
    Forward = 0x01,
    Backward = 0x02,
    Stop = 0x05,
    FreeStop = 0x06
};

class ZBLDMotorManager;

class ZBLDMotor
{
private:
   
public:
    ZBLDMotor(/* args */);
    ~ZBLDMotor();

    void UpdateState();
    void UpdateSpeed();

    uint8_t m_address;
   ZBLDMotorState m_motor_state;
   uint16_t m_set_speed;
   uint8_t m_send_buffer[8];

};

#endif
