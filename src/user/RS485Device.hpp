#ifndef RS485_DEVICE_HPP
#define RS485_DEVICE_HPP

#include <stdint.h>
#include <array>

class RS485Device
{
private:
public:
    RS485Device(uint8_t _address) : m_address(_address) {}
    ~RS485Device() {}

    virtual void Read(uint8_t _cmd) {}
    virtual uint8_t ReadReceive(uint8_t *_buffer) = 0;

    uint8_t m_address;
    uint8_t m_send_buffer[16];
};

template <class T, std::size_t N>
class RS485DeviceManager
{
private:
public:
    RS485DeviceManager() {}
    ~RS485DeviceManager() {}

    std::array<T, N> device_vector;

    uint8_t rx_buffer[256];
    uint8_t rx_update_flag;
    uint8_t rx_length;

    virtual void Init() = 0;
    virtual void SendUpdate()
    {
        for (auto it = device_vector.begin(); it != device_vector.end(); ++it)
        {
            (*it).Read(0);
        }
    }
    virtual void ReceiceUpdate() = 0;
};

#endif
