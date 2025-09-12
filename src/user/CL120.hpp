#ifndef CL120_HPP
#define CL120_HPP

#include <stdint.h>
#include "RS485Device.hpp"

class RS485Motor : public RS485Device
{
private:
public:
    RS485Motor();
    ~RS485Motor(){}
};


enum CL57Command : uint32_t
{
    PosSet = 0,
    SpeedSet = 1,
    Move = 2,
    Stop = 3,
    EmergencyStop = 4,
    ReadPos = 5,
    ReadState = 6
};

class CL57Motor : public RS485Motor
{
private:
public:
    CL57Motor();
    ~CL57Motor(){}

    uint16_t speed_set;

    void WritePos(uint16_t _pos);
    void WriteState(uint32_t _state);
    void WriteSpeed(float _speed);

    void Write(RS485Command *_cmd) override;
    void Read(RS485Command *_cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class CL120Motor : public RS485Motor
{
private:
    /* data */
public:
    CL120Motor(/* args */);
    ~CL120Motor(){}

    void Write(RS485Command *_cmd) override;
    void Read(RS485Command *_cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class MotorManager : public RS485DeviceManager<RS485Motor,10>
{
private:
    CL57Motor cl57_motor[4];
    CL120Motor cl120_motor[6];
public:
    MotorManager();
    ~MotorManager() {}

    void BspInit() override;
};

#endif
