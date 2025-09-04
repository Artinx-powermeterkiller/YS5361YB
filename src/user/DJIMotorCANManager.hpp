#include <stdint.h>
#include "hpm_mcan_drv.h"

#ifndef DJI_MOROR_CAN_MANAGER_HPP
#define DJI_MOROR_CAN_MANAGER_HPP

// Forward declaration
class GM6020;
class M2006;
class M3508;

/**
 * @brief DJI motor CAN Manager
 */
class DJIMotorCANManager
{
private:
    /**
     * @brief GM6020 handler pointers
     * @note using RegisterHandler to set value
     */
    GM6020 *m_GM6020[4][7];

    /**
     * @brief M2006 handler pointers
     * @note using RegisterHandler to set value
     */
    M2006 *m_M2006[4][7];

    /**
     * @brief M3508 handler pointers
     * @note using RegisterHandler to set value
     */
    M3508 *m_M3508[4][7];

    /**
     * @brief CAN receive buffer for CAN message in CAN1 and CAN2,
     *        from 0x201 to 0x20C, each message 8 bytes
     */
    uint8_t m_receive_buffer[4][12][8];

    /**
     * @brief CAN receive update flag in CAN1 and CAN2, from 0x201 to 0x20C,
     *        update in Receive() to define which message is received,
     *        used in ReceiveUpdate() to update MotorFeedback.
     */
    bool m_receive_update_flag[4][12];

    /**
     * @brief DJI motor CAN control message buffer in CAN1 and CAN2,
     *        include 0x1FF, 0X200 and 0x2FF, each motor 2 bytes
     */
    uint8_t m_send_buffer[4][3][8];

    /**
     * @brief CAN control message update flag, update in Add2Buffer()
     *        to define which message need to be send.
     */
    bool m_send_update_flag[4][3];

    /**
     * @brief Construct DJIMotorCANManager object
     * @note private construct function, accessing the unique object
     *       in the project through Instance(). Singleton
     * @see https://refactoringguru.cn/design-patterns/singleton?_gl=1
     */
    DJIMotorCANManager()
    {
        ;
    }

public:
    /**
     * @brief CAN channel enumeration
     */
    enum
    {
        /** CAN0 channel*/
        CAN0_CHANNEL = 0,

        /** CAN1 channel*/
        CAN1_CHANNEL = 1,

        /** CAN2 channel*/
        CAN2_CHANNEL = 2,

        /** CAN2 channel*/
        CAN3_CHANNEL = 3
    };

    /**
     * @brief Instance to access DJIMotorCANManager object
     */
    static DJIMotorCANManager *Instance()
    {
        static DJIMotorCANManager instance;
        return &instance;
    }

    /**
     * @brief Parsing received message and update MotorFeedback for each motor
     */
    void ReceiveUpdate();

    /**
     * @brief Update PID calculate result, update m_send_buffer by Add2Buffer() 
     *        and send control message
     */
    void SendUpdate();

    /**
     * @brief add PID calculate result to m_send_buffer
     * @param _can CAN type, CAN1 or CAN2
     * @param _canId Motor ID, from 0x201 to 0x20C
     * @param _current current set, different range for GM6020, M2006 and M3508
     */
    void Add2Buffer(MCAN_Type *_can, int32_t _canId, int16_t _current);

    /**
     * @brief Update m_receive_buffer, called in CAN1_RX0_IRQHandler() or 
     *        CAN2_RX0_IRQHandler() in main.cpp
     */
    void Receive(uint8_t _can_channel, mcan_rx_message_t *_rx_msg);

    /**
     * @brief Register motor handler to m_GM6020
     * @param _can_channel CAN channel, CAN1_CHANNEL or CAN2_CHANNEL
     * @param _canId Motor ID, from 0x201 to 0x20C
     * @param _GM6020 GM6020 handler pointer
     */
    void RegisterHandler(uint8_t _can_channel, int _canId, GM6020 *_GM6020);

    /**
     * @brief Register motor handler to m_M2006
     * @param _can_channel CAN channel, CAN1_CHANNEL or CAN2_CHANNEL
     * @param _canId Motor ID, from 0x201 to 0x20C
     * @param _M2006 M2006 handler pointer
     */
    void RegisterHandler(uint8_t _can_channel, int _canId, M2006 *_M2006);

    /**
     * @brief Register motor handler to m_M3508
     * @param _can_channel CAN channel, CAN1_CHANNEL or CAN2_CHANNEL
     * @param _canId Motor ID, from 0x201 to 0x20C
     * @param _M3508 M3508 handler pointer
     */
    void RegisterHandler(uint8_t _can_channel, int _canId, M3508 *_M3508);
};

#endif
