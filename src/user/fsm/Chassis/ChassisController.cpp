#include "ChassisController.hpp"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "Math.hpp"
#include "ChassisStateInit.hpp"
#include "ChassisStateZero.hpp"
#include "ChassisStateWait.hpp"
#include "ChassisStateXAxisMove.hpp"
#include "ChassisStateXAxisMoveCheck.hpp"

ChassisController::ChassisController() : ControllerEntity(ECT_ChassisController), chassisFsm(this)
{
}


void ChassisController::Init()
{
    SetDefaultTicksToUpdate(1);

    m_set_up_x_pos = m_set_y_pos = m_set_yaw_pos = m_set_down_x_pos = 0.0f;
    m_last_up_x_pos = m_last_y_pos = m_last_yaw_pos = m_last_down_x_pos = 0.0f;

    m_set_up_x_speed = m_set_y_speed = 1.0f;

    cl57_manager.Init();
    tdp6260_manager.Init();
    z_manager.Init();

    usb_port = USBPort::Instance();

    chassisFsm.Init();
}

uint32_t test_chassis_count = 0;
uint32_t state_count = 0;
bool set_flag = false;
float debug_set_speed = 500.0f;
float debug_delta_speed = 0.05f;

void ChassisController::Update()
{
    cl57_manager.RxUpdate();
    z_manager.RxUpdate();
    
    chassisFsm.HandleInput();
    chassisFsm.Update();

    cl57_manager.Update();
    z_manager.Update();
    tdp6260_manager.Update();

    test_chassis_count++;
}

void ChassisFsm::HandleInput()
{
}

void ChassisFsm::Init()
{
    ChassisStateInit::Instance()->Init(m_pOwner);
    ChassisStateZero::Instance()->Init(m_pOwner);
    ChassisStateXAxisMove::Instance()->Init(m_pOwner);
    ChassisStateXAxisMoveCheck::Instance()->Init(m_pOwner);

    SetCurrentState(ChassisStateInit::Instance());
}
