#ifndef CX4SY_HPP
#define CX4SY_HPP

#include <stdint.h>
#include "RS485Device.hpp"

enum CX4SYChannel : uint8_t
{
    CX4SYChannel1 = 0,
    CX4SYChannel2 = 1,
    CX4SYChannel3 = 2,
    CX4SYChannel4 = 3,
};

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
    void WriteTemperature(uint8_t channel, uint16_t temperature_set);
    void WriteChannelSet(uint8_t channel, uint16_t channel_set);
    void StartAllChannel();

public:
    CX4SY();
    ~CX4SY() {}

    void Write(RS485Command *_cmd) override;
    void Read(RS485Command *_cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;

    float m_temperature_set[4];
    float m_temperature_fdb[4];
    uint8_t m_channel_set[4];
};

class CX4SYManager : public RS485DeviceManager<CX4SY, 1>
{
private:
public:
    CX4SYManager();
    ~CX4SYManager() {}

    void SetChannel(uint8_t _address, uint8_t _channel, uint8_t _channel_set);
    void SetAllChannelStart(uint8_t _address);
    void SetTemperature(uint8_t _address, uint8_t _channel, float _temperature);
    void ReadTemperature(uint8_t _address,uint8_t channel);
    void BspInit() override;
};

#endif
