#ifndef CL120_HPP
#define CL120_HPP

#include <stdint.h>
#include "RS485Device.hpp"

class CL120 : public RS485Device
{
private:
public:
    CL120();
    ~CL120(){}

    void Write() override;
    void Read(RS485Command*  _cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;
};

class CL120Manager : public RS485DeviceManager<CL120,6>
{
private:

public:
    CL120Manager();
    ~CL120Manager() {}

    void BspInit() override;
};

#endif
