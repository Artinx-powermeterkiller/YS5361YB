#include "USBPort.hpp"
#include <stdio.h>

USBPort::USBPort(/* args */) : usb_receive_command_num(0), usb_send_command_num(0)
{
}

USBPort::~USBPort()
{
}

void USBPort::Init()
{
    USB_Init();
}

uint32_t Power10(uint32_t _power)
{
    uint32_t _return = 1;

    for (uint32_t i = 0; i < _power; i++)
    {
        _return *= 10;
    }

    return _return;
}

uint32_t USBPort::ReceiveU32ParameterHandle(uint32_t __start, uint32_t __end)
{
    uint32_t length = __end - __start + 1;
    uint32_t digits[length] = {0};
    uint32_t _parameter = 0;

    for (size_t i = 0; i < length; i++)
    {
        digits[i] = receive_buffer[__start + i] - '0';
    }

    for (size_t i = 0; i < length; i++)
    {
        _parameter += digits[i] * Power10(length - i - 1);
    }

    return _parameter;
}

float USBPort::ReceiveF32ParameterHandle(uint32_t __start, uint32_t __end, uint32_t _point)
{
    uint32_t digits_length = _point - __start;
    uint32_t digits[digits_length] = {0};
    uint32_t digits_parameter = 0;

    uint32_t point_length = __end - _point;
    uint32_t point[point_length] = {0};
    float point_parameter = 0;
    uint32_t temp_point = 0;

    for (size_t i = 0; i < digits_length; i++)
    {
        digits[i] = receive_buffer[__start + i] - '0';
    }

    for (size_t i = 0; i < digits_length; i++)
    {
        digits_parameter += digits[i] * Power10(digits_length - i - 1);
    }

    for (size_t i = 0; i < point_length; i++)
    {
        point[i] = receive_buffer[_point + i + 1] - '0';
    }

    for (size_t i = 0; i < point_length; i++)
    {
        temp_point += point[i] * Power10(point_length - i - 1);
    }

    point_parameter = ((float)temp_point) / ((float)Power10(point_length));

    return (float)digits_parameter + point_parameter;
}

void USBPort::ReceiveParameterHandle(uint32_t _start, uint32_t _end)
{
    uint32_t temp_command = 0;
    uint32_t temp_parameter_u32 = 0;
    float temp_parameter_f32 = 0;

    uint8_t *start = &receive_buffer[_start];
    int32_t length = _end - _start;
    bool f32_flag = false;
    uint32_t point = 0;
    uint32_t temp_start = _start + 2;

    if (length < 0)
    {
        return;
    }

    switch (start[0])
    {
    case 'N':
        temp_command = USBReceiveCommandType::N;
        temp_parameter_u32 = 0;
        AddReceiveCommand(temp_command, temp_parameter_u32);
        return;
    case 'X':
        temp_command = USBReceiveCommandType::X;
        break;
    case 'Y':
        temp_command = USBReceiveCommandType::Y;
        break;
    case 'Z':
        temp_command = USBReceiveCommandType::Z;
        break;
    case 'W':
        temp_command = USBReceiveCommandType::W;
        break;
    case 'I':
        temp_command = USBReceiveCommandType::I;
        break;
    case 'U':
        temp_command = USBReceiveCommandType::U;
        break;
    case 'P':
        temp_command = USBReceiveCommandType::P;
        break;
    case 'S':
        temp_command = USBReceiveCommandType::S;
        break;
    case 'V':
        temp_start++;
        if (start[1] == 'X')
        {
            temp_command = USBReceiveCommandType::VX;
        }
        else if (start[1] == 'Y')
        {
            temp_command = USBReceiveCommandType::VY;
        }
        else if (start[1] == 'Z')
        {
            temp_command = USBReceiveCommandType::VZ;
        }
        else if (start[1] == 'W')
        {
            temp_command = USBReceiveCommandType::VW;
        }
        break;
    default:
        break;
    }

    for (size_t i = 0; i < length; i++)
    {
        if (start[i] == '.')
        {
            f32_flag = true;
            point = i;
        }
    }

    uint32_t temp_end = _end;
    point += _start;

    if (f32_flag)
    {
        temp_parameter_f32 = ReceiveF32ParameterHandle(temp_start, temp_end, point);
        AddReceiveCommand(temp_command, temp_parameter_f32);
    }
    else
    {
        temp_parameter_u32 = ReceiveU32ParameterHandle(temp_start, temp_end);
        AddReceiveCommand(temp_command, temp_parameter_u32);
    }
}

void USBPort::ReceiveCommandHandle(uint32_t _start, uint32_t _end)
{
    uint32_t parameters_num = 0;
    uint32_t parameters_end[17] = {0};

    for (uint32_t i = _start; i < _end; i++)
    {
        if (receive_buffer[i] == ',')
        {
            parameters_end[1 + parameters_num] = i - 1;
            parameters_num++;
        }
    }

    parameters_end[1 + parameters_num] = _end;
    parameters_num++;

    for (uint32_t i = 0; i < parameters_num; i++)
    {
        if (i == 0)
        {
            ReceiveParameterHandle(parameters_end[i], parameters_end[i + 1]);
        }
        else
        {
            ReceiveParameterHandle(parameters_end[i] + 2, parameters_end[i + 1]);
        }
    }
}

uint8_t USBPort::SendCommandHandle(uint8_t *_buffer, USBCommand *_cmd)
{
    uint8_t cmd_buffer[4];
    uint8_t cmd_length = 0;
    uint8_t parameter_buffer[16];
    uint8_t parameter_length = 0;

    switch (_cmd->command_type)
    {
    case USBSendCommandType::NU:
        cmd_buffer[0] = 'N';
        break;
    case USBSendCommandType::OK:
        cmd_buffer[0] = 'O';
        cmd_buffer[1] = 'K';
        cmd_length = 2;
        break;
    case USBSendCommandType::StateFdb:
        cmd_buffer[0] = 'S';
        cmd_length = 1;
        break;
    case USBSendCommandType::XFdb:
        cmd_buffer[0] = 'X';
        cmd_buffer[1] = 'F';
        cmd_length = 2;
        break;
    case USBSendCommandType::YFdb:
        cmd_buffer[0] = 'Y';
        cmd_buffer[1] = 'F';
        cmd_length = 2;
        break;
    case USBSendCommandType::ZFdb:
        cmd_buffer[0] = 'Z';
        cmd_buffer[1] = 'F';
        cmd_length = 2;
        break;
    case USBSendCommandType::WFdb:
        cmd_buffer[0] = 'W';
        cmd_buffer[1] = 'F';
        cmd_length = 2;
        break;
    default:
        break;
    }

    parameter_length = SendParameterHandle(parameter_buffer, _cmd);

    for (uint8_t i = 0; i < cmd_length; i++)
    {
        _buffer[i] = cmd_buffer[i];
    }

    _buffer[cmd_length] = ' ';

    for (uint8_t i = 0; i < parameter_length; i++)
    {
        _buffer[cmd_length + i + 1] = parameter_buffer[i];
    }

    return cmd_length + parameter_length + 1;
}

uint8_t USBPort::SendParameterHandle(uint8_t *_buffer, USBCommand *_cmd)
{
    uint8_t parameter_buffer[16];
    uint8_t parameter_length = 0;

    switch (_cmd->command_type)
    {
    case USBSendCommandType::NU:
        parameter_buffer[0] = '0';
        parameter_length = 1;
        break;
    case USBSendCommandType::OK:
    case USBSendCommandType::StateFdb:
        parameter_length = SendU32ParameterHandle(parameter_buffer, _cmd->parameter_u32);
        break;
    case USBSendCommandType::XFdb:
    case USBSendCommandType::YFdb:
    case USBSendCommandType::ZFdb:
    case USBSendCommandType::WFdb:
        parameter_length = SendF32ParameterHandle(parameter_buffer, _cmd->parameter_f32);
        break;
    default:
        break;
    }

    for (uint8_t i = 0; i < parameter_length; i++)
    {
        _buffer[i] = parameter_buffer[i];
    }

    return parameter_length;
}

uint8_t USBPort::SendU32ParameterHandle(uint8_t *_buffer, uint32_t _parameter)
{
    return sprintf((char *)_buffer, "%d", _parameter);
}

uint8_t USBPort::SendF32ParameterHandle(uint8_t *_buffer, float _parameter)
{
    return sprintf((char *)_buffer, "%.1f", _parameter);
}

// X 1,Y 2\R\N
// X 1,Y 2\R\N

void USBPort::ReceiveUpdate()
{
    USB_Receive(receive_buffer, 256);

    uint32_t cmd_num = 0;
    uint32_t cmd_begin[16] = {0};
    uint32_t cmd_end[16] = {0};

    for (uint32_t i = 0; i < 256; i++)
    {
        if (receive_buffer[i] == '\n')
        {
            cmd_begin[cmd_num + 1] = i + 1;
            cmd_end[cmd_num] = i - 2;
            cmd_num++;
        }
    }

    for (uint32_t i = 0; i < cmd_num; i++)
    {
        ReceiveCommandHandle(cmd_begin[i], cmd_end[i]);
    }
}

void USBPort::SendUpdate()
{
    if (usb_send_command_num != 0)
    {
        for (uint8_t i = 0; i < usb_send_command_num; i++)
        {
            send_buffer_length += SendCommandHandle(&send_buffer[send_buffer_length], &(usb_send_command_list[i]));
            send_buffer[send_buffer_length] = ',';
            send_buffer_length++;
        }

        send_buffer_length--;
        send_buffer[send_buffer_length] = '\r';
        send_buffer[send_buffer_length + 1] = '\n';

        send_buffer_length += 2;

        usb_send_command_num = 0;
    }

    if (send_buffer_length != 0)
    {
        USB_Transmit(send_buffer, send_buffer_length);
        send_buffer_length = 0;
    }
}

void USBPort::AddSendCommand(uint32_t _command_type, uint32_t _parameter)
{
    usb_send_command_list[usb_send_command_num].command_type = _command_type;
    usb_send_command_list[usb_send_command_num].parameter_u32 = _parameter;
    usb_send_command_num++;
}

void USBPort::AddSendCommand(uint32_t _command_type, float _parameter)
{
    usb_send_command_list[usb_send_command_num].command_type = _command_type;
    usb_send_command_list[usb_send_command_num].parameter_f32 = _parameter;
    usb_send_command_num++;
}

void USBPort::AddReceiveCommand(uint32_t _command_type, uint32_t _parameter)
{
    usb_receive_command_list[usb_receive_command_num].command_type = _command_type;
    usb_receive_command_list[usb_receive_command_num].parameter_u32 = _parameter;

    usb_receive_command_num++;
}

void USBPort::AddReceiveCommand(uint32_t _command_type, float _parameter)
{
    usb_receive_command_list[usb_receive_command_num].command_type = _command_type;
    usb_receive_command_list[usb_receive_command_num].parameter_f32 = _parameter;

    usb_receive_command_num++;
}
