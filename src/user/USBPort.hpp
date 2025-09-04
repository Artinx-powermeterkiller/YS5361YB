#ifndef USB_PORT_HPP
#define USB_PORT_HPP

#include "bsp/bsp_usb.h"

enum USBReceiveCommandType : uint32_t
{
    N = 0,
    X = 1,
    Y = 2,
    Z = 3,
    W = 4,
    I = 5,
    U = 6,
    P = 7,
    S = 8,
    VX = 9,
    VY = 10,
    VZ = 11,
    VW = 12
};

enum USBSendCommandType : uint32_t
{
    NU = 0,
    OK = 1,
    StateFdb = 2,
    XFdb = 3,
    YFdb = 4,
    ZFdb = 5,
    WFdb = 6

};

struct USBCommand
{
    uint32_t command_type;

    union
    {
        uint32_t parameter_u32;
        float parameter_f32;
    };
};

class USBPort
{
private:
    void AddReceiveCommand(uint32_t _command_type, uint32_t _parameter);
    void AddReceiveCommand(uint32_t _command_type, float _parameter);

    void ReceiveCommandHandle(uint32_t _start,uint32_t _end);
    void ReceiveParameterHandle(uint32_t _start,uint32_t _end);

    uint32_t ReceiveU32ParameterHandle(uint32_t _start,uint32_t _end);
    float ReceiveF32ParameterHandle(uint32_t _start,uint32_t _end,uint32_t _point);

    uint8_t SendCommandHandle(uint8_t* _buffer,USBCommand* _cmd);
    uint8_t SendParameterHandle(uint8_t* _buffer,USBCommand* _cmd);
    uint8_t SendU32ParameterHandle(uint8_t* _buffer,uint32_t _parameter);
    uint8_t SendF32ParameterHandle(uint8_t* _buffer,float _parameter);

    USBPort(/* args */);

public:
    ~USBPort();

    static USBPort *Instance()
    {
        static USBPort instance;
        return &instance;
    }

    void Init();
    void ReceiveUpdate();
    void SendUpdate();

    void AddSendCommand(uint32_t _command_type, uint32_t _parameter);
    void AddSendCommand(uint32_t _command_type, float _parameter);

    USBCommand usb_receive_command_list[32];
    USBCommand usb_send_command_list[32];

    uint8_t send_buffer[256];
    uint8_t receive_buffer[256];

    uint8_t send_buffer_length;
    uint8_t receive_buffer_length;

    uint32_t usb_receive_command_num;
    uint32_t usb_send_command_num;
};

#endif
