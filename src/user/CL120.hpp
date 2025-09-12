#ifndef CL120_HPP
#define CL120_HPP

#include <stdint.h>
#include "RS485Device.hpp"

enum CL57Command : uint8_t
{
    CL57PosSet = 0,
    CL57SpeedSet = 1,
    CL57StateSet = 2,
};

enum CL120Command : uint8_t
{
    CL120StateSet = 0,
    CL120SpeedSet = 1,
};

class CL57Motor : public RS485Device
{
private:
public:
    CL57Motor();
    ~CL57Motor(){}

    uint16_t speed_set;

    void WritePos(uint32_t _pos); // mm
    void WriteState(uint16_t _state);
    void WriteSpeed(uint16_t _speed); // rpm

    void Write(RS485Command *_cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class CL120Motor : public RS485Device
{
private:
    /* data */
public:
    CL120Motor(/* args */);
    ~CL120Motor(){}

    void WriteState(uint16_t _state);
    void WriteSpeed(uint16_t _speed);

    void Write(RS485Command *_cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class CL57Manager : public RS485DeviceManager<CL57Motor,4>
{
private:

public:
    CL57Manager();
    ~CL57Manager() {}

    void SetSpeed(uint8_t _address,float _speed);
    void SetPos(uint8_t _address,float _pos);
    void SetState(uint8_t _address,uint8_t _state);

    void BspInit() override;
};

class CL120Manager : public RS485DeviceManager<CL120Motor,6>
{
private:

public:
    CL120Manager();
    ~CL120Manager() {}

    void SetSpeed(uint8_t _address,float _speed);
    void SetState(uint8_t _address,uint8_t _state);

    void BspInit() override;
};

#endif
