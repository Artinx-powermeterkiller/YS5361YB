#ifndef RS485_DEVICE_HPP
#define RS485_DEVICE_HPP

#include <stdint.h>
#include <array>
#include <algorithm>

enum RS485CommandType : uint8_t
{
    RS485WriteCommand = 0,
    RS485ReadCommand = 1,
};

struct RS485Command
{
    uint8_t address;
    uint8_t command;
    uint8_t command_type;
    uint16_t parameter;
    uint16_t parameter_optional;
};

class RS485Device
{
private:
public:
    RS485Device(uint8_t _address) : m_address(_address) {}
    ~RS485Device() {}

    virtual void Read(RS485Command *_cmd) {}
    virtual void Write(RS485Command *_cmd) {}
    virtual uint8_t ReadReceive(uint8_t *_buffer) = 0;

    uint8_t m_address;
    uint8_t m_send_buffer[16];
};

template <class T, std::size_t N>
class RS485DeviceManager
{
private:
    uint32_t m_tick;
    uint32_t m_interval;

public:
    RS485DeviceManager(uint32_t _interval)
    {
        m_tick = 0;
        m_interval = _interval;
        m_command_num = 0;
    }
    ~RS485DeviceManager() {}

    std::array<T, N> device_vector;

    uint8_t rx_buffer[256];
    uint8_t rx_update_flag;
    uint8_t rx_length;

    RS485Command m_write_command_vector[8];
    uint8_t m_command_num;
    uint8_t m_finished_command_num;

    uint8_t AddCommand(uint8_t _address)
    {
        if(m_command_num<8)
        {
            m_write_command_vector[m_command_num].address = _address;
            m_write_command_vector[m_command_num].command_type = RS485ReadCommand;
            m_command_num++;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    uint8_t AddCommand(RS485Command* _cmd)
    {
        if(m_command_num<8)
        {
            m_write_command_vector[m_command_num].address = _cmd->address;
            m_write_command_vector[m_command_num].command = _cmd->command;
            m_write_command_vector[m_command_num].command_type = _cmd->command_type;
            m_write_command_vector[m_command_num].parameter = _cmd->parameter;
            m_write_command_vector[m_command_num].parameter_optional = _cmd->parameter_optional;
            m_command_num++;
            return 1;
        }
        else
        {
            return 0;
        }
    }

    virtual void Init()
    {
        int i = 1;
        for (auto it = device_vector.begin(); it != device_vector.end(); ++it, ++i)
        {
            (*it).m_address = i;
        }

        BspInit();

        rx_update_flag = 0;
        rx_length = 0;
    }

    virtual void BspInit() = 0;

    virtual void SendUpdate()
    {
        m_tick++;

        if (m_tick % m_interval == 0)
        {
            if (m_command_num > m_finished_command_num)
            {
                uint8_t address = m_write_command_vector[m_finished_command_num].address;

                auto it = std::find_if(device_vector.begin(), device_vector.end(), [address](const T &a)
                                       { return a.m_address == address; });

                if (it != device_vector.end())
                {
                    if (m_write_command_vector[m_finished_command_num].command_type == RS485WriteCommand)
                    {
                        it->Write(&m_write_command_vector[m_finished_command_num]);
                    }
                    else if (m_write_command_vector[m_finished_command_num].command_type == RS485ReadCommand)
                    {
                        it->Read(&m_write_command_vector[m_finished_command_num]);
                    }
                }

                m_finished_command_num++;

                if(m_finished_command_num ==  m_command_num)
                {
                    m_finished_command_num = m_command_num = 0;
                }

                return;
            }
        }
    }

    virtual void ReceiceUpdate()
    {
        uint8_t head = 0;
        uint8_t temp_length = 0;
        uint8_t address = 0xFF;

        if (rx_update_flag == 1)
        {
            while (head < rx_length)
            {
                address = rx_buffer[head];

                auto it = std::find_if(device_vector.begin(), device_vector.end(), [address](const T &a)
                                       { return a.m_address == address; });

                if (it != device_vector.end())
                {
                    temp_length = it->ReadReceive(&rx_buffer[head]);
                    head += temp_length;
                }
                else
                {
                    head++;
                }
            }
            rx_update_flag = 0;
            //rx_length -= head;
        }
    }
};

#endif
