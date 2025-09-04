#include "ChassisStateCMD.hpp"
#include "ChassisStateXAxisMove.hpp"
#include "ChassisStateYAxisMove.hpp"
#include "ChassisStateZAxisMove.hpp"
#include "ChassisStateYawAxisMove.hpp"
#include "ChassisStateWait.hpp"

void ChassisStateCMD::Init(ChassisController *pOwner)
{
    m_finished_cmd = 0;
}

static USBPort *_USBPort = USBPort::Instance();

void ChassisStateCMD::Enter(ChassisController *pOwner)
{
}

void ChassisStateCMD::Execute(ChassisController *pOwner)
{
    uint32_t temp_set = 0;

    switch (_USBPort->usb_receive_command_list[m_finished_cmd].command_type)
    {
    case N:
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case X:
        pOwner->m_set_up_x_pos = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateXAxisMove::Instance());
        break;
    case Y:
        pOwner->m_set_y_pos = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateYAxisMove::Instance());
        break;
    case Z:
        pOwner->m_set_z2_pos = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateZAxisMove::Instance());
        break;
    case W:
        pOwner->m_set_yaw_pos = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateYawAxisMove::Instance());
        break;
    case I:
        temp_set = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32 * 1000;
        pOwner->tdp6260_manager.AddCommand(CurrentSet,temp_set);
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case U:
        temp_set = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32 * 1000;
        pOwner->tdp6260_manager.AddCommand(VoltageSet,temp_set);
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case P:
        pOwner->tdp6260_manager.AddCommand(OutputStateSet,_USBPort->usb_receive_command_list[m_finished_cmd].parameter_u32);
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case S:
        // pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case VX:
        pOwner->m_set_up_x_speed = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case VY:
        pOwner->m_set_y_speed = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    case VZ:
        pOwner->m_set_z2_speed = _USBPort->usb_receive_command_list[m_finished_cmd].parameter_f32;
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    default:
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
        break;
    }
}

void ChassisStateCMD::Exit(ChassisController *pOwner)
{
    m_finished_cmd++;

    if (_USBPort->usb_receive_command_num == m_finished_cmd)
    {
        _USBPort->usb_receive_command_num = m_finished_cmd = 0;
    }
}
