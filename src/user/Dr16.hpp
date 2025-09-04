#ifndef DR16_HPP
#define DR16_HPP

#include "bsp/bsp_DR16.h"

/**
 * @brief DJI DR16 receiver
 */
class Dr16
{
public:
    /**
     * @brief Keyboard key state
     */
    enum PcKeyStateType
    {
        /** Key up*/
        KEY_UP = 0,

        /** Key down*/
        KEY_DOWN,

        /** Key Fall, from up to down*/
        KEY_FALL,

        /** Key down, from down to up*/
        KEY_RISE,
    };

    /**
     * @brief Keyboard key type
     */
    enum PcKeyType
    {
        PC_KEY_W = 0,
        PC_KEY_S,
        PC_KEY_A,
        PC_KEY_D,
        PC_KEY_SHIFT,
        PC_KEY_CTRL,
        PC_KEY_Q,
        PC_KEY_E,
        PC_KEY_R,
        PC_KEY_F,
        PC_KEY_G,
        PC_KEY_Z,
        PC_KEY_X,
        PC_KEY_C,
        PC_KEY_V,
        PC_KEY_B,

        /** Mouse left switch*/
        PC_M_L,

        /** Mouse right switch*/
        PC_M_R,
    };

    /**
     * @brief remote control switch state
     */
    enum RcSwStatusType
    {
        /** switch in top state*/
        RC_SW_UP = 1,

        /** switch in middle state*/
        RC_SW_MID = 3,

        /** switch in down state*/
        RC_SW_DOWN = 2,

        /** switch from up to middle*/
        RC_SW_U2M = 4,

        /** switch from middle to down*/
        RC_SW_M2D = 5,

        /** switch from down to middle*/
        RC_SW_D2M = 6,

        /** switch from middle to up*/
        RC_SW_M2U = 7,
    };

    /**
     * @brief remote control switch type
     */
    enum RcSwType
    {
        /** remote control left switch*/
        RC_SW_L = 1,

        /** remote control right switch*/
        RC_SW_R = 0,
    };

private:
    /**
     * @brief receiver raw buffer pointer
     */
    RC_raw_t *pRcRaw;

    /**
     * @brief Left horizontal axis value
     * @note min:-1.0f max:1.0f
     */
    float m_LHAxis;

    /**
     * @brief Left vertical axis value
     * @note min:-1.0f max:1.0f
     */
    float m_LVAxis;

    /**
     * @brief Right horizontal axis value
     * @note min:-1.0f max:1.0f
     */
    float m_RHAxis;

    /**
     * @brief Right vertical axis value
     * @note min:-1.0f max:1.0f
     */
    float m_RVAxis;

    /**
     * @brief dead zone for each joy stick
     */
    float m_JoyStickDeadzone;

    /**
     * @brief keyboard key state
     */
    uint16_t m_KbdState;

    /**
     * @brief keyboard key state in last update cycle
     */
    uint16_t m_PrevKbdState;

    /**
     * @brief keyboard key state that which key fall in this cycle compare with last cycle
     */
    uint16_t m_KbdFall;

    /**
     * @brief keyboard key state that which key rise in this cycle compare with last cycle
     */
    uint16_t m_KbdRise;

    /**
     * @brief mouse left button state in this cycle
     */
    uint8_t m_MouseLState;

    /**
     * @brief mouse left button state in last cycle
     */
    uint8_t m_PrevMouseLState;

    /**
     * @brief mouse right button state in this cycle
     */
    uint8_t m_MouseRState;

    /**
     * @brief mouse right button state in last cycle
     */
    uint8_t m_PrevMouseRState;

    /**
     * @brief time difference between last cycle and this cycle in ms
     */
    uint32_t m_LastUpdateTick;

    /**
     * @brief time difference between last DR16 UART receive and this DR16 UART receive in ms
     */
    uint32_t m_LastDr16UpdateTick;

    /**
     * @brief time difference between last judgement system UART receive and this receive in ms
     */
    uint32_t m_LastJSUpdateTick;

    /**
     * @brief time difference threshold value in ms, determine offline for DR16
     */
    uint32_t m_TimeoutTick;

    /**
     * @brief remote control switch state in this cycle, [0] for right switch,[1] for left switch
     */
    char m_SwState[2];

    /**
     * @brief remote control switch state in last cycle, [0] for right switch,[1] for left switch
     */
    char m_PrevSwState[2];

    /**
     * @brief Construct Dr16 object
     * @note private construct function, accessing the unique object
     *       in the project through Instance(). Singleton
     * @see https://refactoringguru.cn/design-patterns/singleton?_gl=1
     */
    Dr16();

    /**
     * @brief update value for each axis from pRcRaw and normalized
     * @note normalization: mapping raw range(364 to 1684) to unified range(-1.0 to 1.0)
     */
    void UpdateNormalizedAxis(void);

    /**
     * @brief update keyboard events
     */
    void UpdateKbdEvents(void);

    /**
     * @brief update transmitter keyboard events, not used
     */
    void UpdateTransmitterKbdEvents(void);

    /**
     * @brief normalization each axis
     * @retval normalized result(-1.0f to 1.0f, resolution ratio: 1/1340 or 0.000746269)
     */
    float NormalizeAxis(int16_t ch);

public:
    /**
     * @brief remote control initialization
     */
    void Init();

    /**
     * @brief remote control enable
     */
    void Enable();

    /**
     * @brief remote control disable
     */
    void Disable();

    /**
     * @brief remote control restart
     */
    void Restart(uint16_t dma_buf_num);

    /**
     * @brief get raw remote control buffer
     * @retval pointer of buffer
     */
    RC_raw_t *GetRcRaw();

    /**
     * @brief update received buffer
     */
    void ReceiveUpdate();

    /* Left Horizontal, Left Vertical, Right Horizontal, Right Vertical, normalized */
    /**
     * @brief get normalized left horizontal axis value
     * @retval normalized left horizontal axis value, -1.0f to 1.0f
     */
    float GetLHAxis() const { return m_LHAxis; }

    /**
     * @brief get normalized left vertical axis value
     * @retval normalized left vertical axis value, -1.0f to 1.0f
     */
    float GetLVAxis() const { return m_LVAxis; }

    /**
     * @brief get normalized right horizontal axis value
     * @retval normalized right horizontal axis value, -1.0f to 1.0f
     */
    float GetRHAxis() const { return m_RHAxis; }

    /**
     * @brief get normalized right vertical axis value
     * @retval normalized right vertical axis value, -1.0f to 1.0f
     */
    float GetRVAxis() const { return m_RVAxis; }

    /**
     * @brief get mouse X axis(left-right) speed
     */
    int16_t GetMouseX();

    /**
     * @brief get mouse Y axis(front-rear) speed
     */
    int16_t GetMouseY();

    /**
     * @brief get mouse Z axis(up-down) speed, not used
     */
    int16_t GetMouseZ();

    /**
     * @brief get dead zone for each joy stick
     * @retval dead zone for each joy stick
     */
    float GetJoyStickDeadZone() const { return m_JoyStickDeadzone; }

    /**
     * @brief set dead zone for each joy stick
     * @param deadzone dead zone for each joy stick
     */
    void SetJoyStickDeadZone(float deadzone) { m_JoyStickDeadzone = deadzone; }

    /**
     * @brief get last update tick
     * @retval last update tick
     */
    uint32_t GetLastUpdateTick() const { return m_LastUpdateTick; }

    /**
     * @brief set timeout threshold
     * @param _timeout timeout threshold
     */
    void SetTimeout(uint32_t _timeout) { m_TimeoutTick = _timeout; }

    /**
     * @brief get timeout threshold
     * @retval timeout threshold
     */
    uint32_t GetTimeout() const { return m_TimeoutTick; }

    /**
     * @brief query remote control switch state equal to target or not
     * @param target target switch type
     * @param queryType query switch state
     * @retval true or false
     * @note for RcSwStatusType::RC_SW_U2M, RcSwStatusType::RC_SW_M2D,
     *       RcSwStatusType::RC_SW_D2M and RcSwStatusType::RC_SW_M2U,
     *       only trigger once when state change
     */
    bool QuerySwState(RcSwType target, RcSwStatusType queryType);

    /**
     * @brief query keyboard and mouse state equal to target or not
     * @param target target switch type
     * @param queryType query switch state
     * @retval true or false
     * @note for PcKeyStateType::KEY_FALL and PcKeyStateType::KEY_RISE,
     *       only trigger once when state change
     */
    bool QueryPcKeyState(PcKeyType target, PcKeyStateType queryType);

    /**
     * @brief is timeout or not
     * @retval true or false
     */
    bool IsTimeout();

    /**
     * @brief is Dr16 timeout or not
     * @retval true or false
     */
    bool Dr16Timeout();

    /**
     * @brief Instance to access Dr16 object
     */
    static Dr16 *Instance()
    {
        static Dr16 instance;
        return &instance;
    }
};

#endif
