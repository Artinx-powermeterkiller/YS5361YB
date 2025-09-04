#include "ChassisStateZero.hpp"
#include "ChassisStateWait.hpp"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"

void ChassisStateZero::Init(ChassisController* pOwner)
{
    y_finish_flag = 0;
    up_x_finish_flag = 0;
    yaw_finish_flag = 0;
    z_finish_flag = 0;
    down_x_finish_flag = 0;

    y_wait_count = 500;
    up_x_wait_count = 500;
    yaw_wait_count = 500;
}

void ChassisStateZero::Enter(ChassisController* pOwner)
{
    y_finish_flag = 0;
    up_x_finish_flag = 0;
    yaw_finish_flag = 0;
    z_finish_flag = 0;
    down_x_finish_flag = 0;

    y_wait_count = 500;
    up_x_wait_count = 500;
    yaw_wait_count = 500;

    CL57MotorManager* manager = &(pOwner->cl57_manager);

    manager->AddCommand(YAxis,SpeedSet,3.0f);
    manager->AddCommand(YAxis,PosSet,-500.0f);
    manager->AddCommand(YAxis,Move);

    manager->AddCommand(UpXAxis,SpeedSet,3.0f);
    manager->AddCommand(UpXAxis,PosSet,-500.0f);
    manager->AddCommand(UpXAxis,Move);

    manager->AddCommand(YawAxis,SpeedSet,12.0f);
    manager->AddCommand(YawAxis,PosSet,-50.0f);
    manager->AddCommand(YawAxis,Move);

    ZMotorManager *zmanager = &(pOwner->z_manager);
    zmanager->AddCommand(Z2Axis,ZReadPos);
}

void ChassisStateZero::Execute(ChassisController* pOwner)
{
    CL57MotorManager* manager = &(pOwner->cl57_manager);

    if(gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 0) == 0 && y_finish_flag ==0)
    {
        manager->AddCommand(YAxis,Stop);

        y_finish_flag = 1;
    }
    else if(y_finish_flag == 1)
    {
        y_wait_count--;
        
        if(y_wait_count == 0)
        {
            manager->AddCommand(YAxis,SpeedSet,1.0f);
            manager->AddCommand(YAxis,PosSet,10.0f);
            manager->AddCommand(YAxis,Move);
            y_finish_flag = 2;
        }
    }
    else if (gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 0) == 1 && y_finish_flag == 2)
    {
        manager->AddCommand(YAxis,Stop);
        manager->AddCommand(YAxis,ReadPos);
        y_finish_flag = 3;
    }
    else if(y_finish_flag == 3)
    {
        if(manager->motor[YAxis].m_fdb_pos!=0)
        {
            manager->motor[YAxis].m_fdb_zero_pos = manager->motor[YAxis].m_fdb_pos;
            manager->motor[YAxis].m_real_pos = 0.0f;
            manager->motor[YAxis].m_zero_pos_set_flag = 1;
            y_finish_flag = 4;
        }
    }

    if(gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 1) == 0 && up_x_finish_flag ==0)
    {
        manager->AddCommand(UpXAxis,Stop);

        up_x_finish_flag = 1;
    }
    else if(up_x_finish_flag == 1)
    {
        up_x_wait_count--;
        
        if(up_x_wait_count == 0)
        {
            manager->AddCommand(UpXAxis,SpeedSet,1.0f);
            manager->AddCommand(UpXAxis,PosSet,10.0f);
            manager->AddCommand(UpXAxis,Move);
            up_x_finish_flag = 2;
        }
    }
    else if (gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 1) == 1 && up_x_finish_flag == 2)
    {
        manager->AddCommand(UpXAxis,Stop);
        manager->AddCommand(UpXAxis,ReadPos);
        up_x_finish_flag = 3;
    }
    else if(up_x_finish_flag == 3)
    {
        if(manager->motor[UpXAxis].m_fdb_pos!=0)
        {
            manager->motor[UpXAxis].m_fdb_zero_pos = manager->motor[UpXAxis].m_fdb_pos;
            manager->motor[UpXAxis].m_real_pos = 0.0f;
            manager->motor[UpXAxis].m_zero_pos_set_flag = 1;
            up_x_finish_flag = 4;
        }
    }

    if(gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 2) == 0 && yaw_finish_flag ==0)
    {
        manager->AddCommand(YawAxis,Stop);

        yaw_finish_flag = 1;
    }
    else if(yaw_finish_flag == 1)
    {
        yaw_wait_count--;
        
        if(yaw_wait_count == 0)
        {
            manager->AddCommand(YawAxis,SpeedSet,6.0f);
            manager->AddCommand(YawAxis,PosSet,20.0f);
            manager->AddCommand(YawAxis,Move);
            yaw_finish_flag = 2;
        }
    }
    else if (gpio_read_pin(HPM_GPIO0, GPIO_DI_GPIOY, 2) == 1 && yaw_finish_flag == 2)
    {
        manager->AddCommand(YawAxis,Stop);
        manager->AddCommand(YawAxis,ReadPos);
        yaw_finish_flag = 3;
    }
    else if(yaw_finish_flag == 3)
    {
        if(manager->motor[YawAxis].m_fdb_pos!=0)
        {
            manager->motor[YawAxis].m_fdb_zero_pos = manager->motor[YawAxis].m_fdb_pos;
            manager->motor[YawAxis].m_real_pos = 0.0f;
            manager->motor[YawAxis].m_zero_pos_set_flag = 1;
            yaw_finish_flag  = 4;
        }
    }
  
    if(up_x_finish_flag == 4 && y_finish_flag == 4 && yaw_finish_flag == 4)  
    {
        pOwner->chassisFsm.ChangeState(ChassisStateWait::Instance());
    }
}

void ChassisStateZero::Exit(ChassisController* pOwner)
{
    pOwner->m_set_up_x_speed = 20.0f;
    pOwner->m_set_y_speed = 20.0f;
    pOwner->m_set_yaw_speed = 24.0f;
    pOwner->m_set_xy_speed = 20.0f;
    pOwner->m_set_z2_speed = 5.0f;

    pOwner->usb_port->AddSendCommand(USBSendCommandType::OK,(uint32_t)1);

    ZMotorManager *manager = &(pOwner->z_manager);
    ZMotor* motor = &manager->motor[Z2Axis];
    motor->m_zero_pos = motor->m_fdb_pos;
    motor->m_real_pos = 0.0f;
}
