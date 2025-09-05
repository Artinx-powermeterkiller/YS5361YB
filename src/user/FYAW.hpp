#ifndef FYAW_HPP
#define FYAW_HPP

#include <stdint.h>
#include <RS485Device.hpp>

class FYAW : public RS485Device
{
private:
public:
    FYAW();
    ~FYAW(){}

    void ReadGap();
    void Read(uint8_t _cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;

    float m_gap; // mm
};

class FYAWManager : public RS485DeviceManager<FYAW,4>
{
private:

public:
    FYAWManager() {}
    ~FYAWManager() {}

    void Init() override;
    void SendUpdate() override;
    void ReceiceUpdate() override;
};

#endif
