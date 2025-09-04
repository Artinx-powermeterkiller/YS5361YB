#ifndef Z_MOTOR_HPP
#define Z_MOTOR_HPP

#include <stdint.h>

enum ZMotoCommandType : uint32_t
{
    ZPosSet = 0,
    ZSpeedSet = 1,
    ZReadPos = 2
};

enum ZMotorType : uint32_t
{
    Z1Axis = 0,
    Z2Axis = 1,
    Z3Axis = 2
};

enum ZMotorFdbState : uint8_t
{
    ZWait = 0,
    ZMove = 1,
    ZReach = 2
};

struct ZMotorCommand
{
    uint32_t motor_type;
    uint32_t command_type;
    float parameter;
};

class ZMotor
{
private:
public:
    ZMotor();
    ~ZMotor();

    void UpdatePos(float _set);
    void ReadPos();
    void SetSpeed(float _spd);

    void Receive(uint8_t *_buffer);
    void ReceivePos(uint8_t *_buffer);
    
    uint8_t m_address;

    uint16_t m_read_reg_address;
    uint32_t m_motor_type;
    uint32_t m_set_speed;
    float m_fdb_pos; // mm
    float m_zero_pos;
    float m_real_pos;
    uint16_t m_fdb_speed;
    uint8_t m_fdb_state;
};

class ZMotorManager
{
private:
    uint32_t m_tick;
    uint8_t command_num;
    uint8_t finished_command_num;

public:
    ZMotorManager() : command_num(0), finished_command_num(0) {}
    ~ZMotorManager() {}

    ZMotor motor[3];
    ZMotorCommand command_list[16];

    void Init();
    void Update();
    void RxUpdate();

    ZMotor *GetMotor(uint8_t _address);

    void AddCommand(uint32_t motor, uint32_t command, float parameter);
    void AddCommand(uint32_t motor, uint32_t command);
};

#endif

