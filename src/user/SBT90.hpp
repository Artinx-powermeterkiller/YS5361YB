#ifndef SBT90_HPP
#define SBT90_HPP

#include <stdint.h>
#include "RS485Device.hpp"

class SBT90 : public RS485Device
{
private:
public:
    SBT90();
    ~SBT90(){}

    uint32_t force[4];

    void Read(RS485Command*  _cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class SBT90Manager : public RS485DeviceManager<SBT90,1>
{
private:

public:
    SBT90Manager();
    ~SBT90Manager() {}

    void BspInit() override;
};

#endif
