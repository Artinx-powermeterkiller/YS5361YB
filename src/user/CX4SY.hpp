#ifndef CX4SY_HPP
#define FYAW_HPP

#include <stdint.h>
#include "RS485Device.hpp"

enum CX4SYCommand : uint8_t
{
    WriteTemperature = 0,
    ReadTemperature = 1,
    WriteChannelSet = 2,
    StartAllChannel = 3,
};

class CX4SY : public RS485Device
{
private:
    void ReadTemperature(uint8_t channel);
    void WriteTemperature(uint8_t channel,uint16_t temperature_set);
    void WriteChannelSet(uint8_t channel);
    void StartAllChannel();
public:
    CX4SY();
    ~CX4SY(){}

    void Write(RS485Command* _cmd);
    void Read(RS485Command* _cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;

    float m_temperature_set[4];
    float m_temperature_fdb[4];
    uint8_t m_channel_set[4];
};

class CX4SYManager : public RS485DeviceManager<CX4SY,1>
{
private:

public:
    CX4SYManager();
    ~CX4SYManager() {}

    void BspInit() override;
};

#endif
