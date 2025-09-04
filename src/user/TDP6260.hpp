#ifndef TDP6260_HPP
#define TDP6260_HPP

#include <stdint.h>

enum TDP6260CommandType : uint32_t
{
    CurrentSet = 0,
    VoltageSet = 1,
    OutputStateSet = 2,
};

struct TDP6260Command
{
    uint32_t command_type;
    uint32_t parameter;
};

class TDP6260
{
private:
   
public:
    TDP6260();
    ~TDP6260();

    void UpdateCurrent(uint32_t _set);
    void UpdateVoltage(uint32_t _set);
    void UpdateOutputState(uint32_t _set);

    uint8_t m_address;

    uint8_t m_send_buffer[16];
};


class TDP6260Manager
{
private:
    uint32_t m_tick;
    uint8_t command_num;
    uint8_t finished_command_num;

public:
    TDP6260Manager():command_num(0),m_tick(0){}
    ~TDP6260Manager(){}

    TDP6260 tdp6260;

    TDP6260Command command_list[16];

    void Init();
    void Update();
    void AddCommand(uint32_t command,uint32_t parameter);
    void AddCommand(uint32_t command);
};

#endif
