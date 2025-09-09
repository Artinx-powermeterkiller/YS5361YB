#ifndef THTS_HPP
#define THTS_HPP

#include <stdint.h>
#include <RS485Device.hpp>

// class THTS
// {
// private:
// public:
//     THTS();
//     ~THTS(){}

//     void ReadAll();
//     uint8_t ReadReceive(uint8_t *_buffer);

//     float m_gap[4]; // mm
//     float m_temperature[16]; // ℃
//     uint8_t m_address;

//     uint8_t m_send_buffer[16];
// };

// class THTSManager
// {
// private:

// public:
//     THTSManager() {}
//     ~THTSManager() {}

//     THTS thts;

//     uint8_t rx_buffer[256];
//     uint8_t rx_update_flag;
//     uint8_t rx_length;

//     void Init();
//     void SendUpdate();
//     void ReceiceUpdate();
// };

class THTS : public RS485Device
{
private:
public:
    THTS();
    ~THTS(){}

    void Read(RS485Command*  _cmd) override;
    uint8_t ReadReceive(uint8_t *_buffer) override;

    float m_gap[4]; // mm
    float m_temperature[16]; // ℃
};

class THTSManager : public RS485DeviceManager<THTS,1>
{
private:

public:
    THTSManager();
    ~THTSManager() {}

    void BspInit() override;
};

#endif
