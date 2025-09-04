#ifndef CL57_MOTOR_HPP
#define CL57_MOTOR_HPP

#include <stdint.h>

// X 195 y 400

// 1R = 4mm

enum CL57CommandType : uint32_t
{
    PosSet = 0,
    SpeedSet = 1,
    Move = 2,
    Stop = 3,
    EmergencyStop = 4,
    ReadPos = 5,
    ReadState = 6
};

enum CL57MotorType : uint32_t
{
    UpXAxis = 0,
    YAxis = 1,
    YawAxis = 2,
    DownAxis = 3
};

enum CL57MotorState :uint8_t
{
    CL57Wait = 0,
    CL57Move = 1,
    CL57Reach = 2
};

struct CL57Command
{
    uint32_t motor_type;
    uint32_t command_type;
    float parameter;
};

class CL57Motor
{
private:
public:
    CL57Motor();
    ~CL57Motor();

    void UpdatePos(float _set);
    void UpdateState(uint32_t _set);
    void UpdateSpeed(float _set);

    void ReadPos();
    void ReadState();

    uint8_t Receive(uint8_t *_buffer);
    uint8_t ReadReceive(uint8_t *_buffer);

    uint8_t m_address;
    uint8_t m_zero_pos_set_flag;
    uint8_t m_motor_state;
    uint16_t m_read_reg_address;
    uint32_t m_motor_type;
    float m_fdb_pos; // mm deg
    float m_fdb_zero_pos;
    float m_real_pos;
    uint16_t m_fdb_speed;
    uint16_t m_fdb_state;

    uint8_t m_send_buffer[16];
};

class CL57MotorManager
{
private:
    uint32_t m_tick;
    uint8_t command_num;
    uint8_t finished_command_num;

public:
    CL57MotorManager() : command_num(0), m_tick(0), finished_command_num(0) {}
    ~CL57MotorManager() {}

    CL57Motor motor[4];
    CL57Command command_list[64];

    uint8_t rx_buffer[256];
    uint8_t rx_update_flag;
    uint8_t rx_length;

    void Init();
    void Update();
    void RxUpdate();
    CL57Motor *GetMotor(uint8_t _address);
    void AddCommand(uint32_t motor, uint32_t command, float parameter);
    void AddCommand(uint32_t motor, uint32_t command);
    void EmergencyStop();
};

#endif
