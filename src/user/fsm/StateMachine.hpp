#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include "State.hpp"

/**
 * @brief finite state machine template
 */
template <typename controller_type>
class StateMachine
{
protected:
    /**
     * @brief The pointer of controller that have this state machine as member variables.
     * @note Must be set by StateMachine::StateMachine(controller_type *m_pOwner)
     */
    controller_type *m_pOwner;

    /**
     * @brief Current execution status
     */
    State<controller_type> *m_pCurrentState;

    /**
     * @brief The execution status before the last state switch
     */
    State<controller_type> *m_pPreviousState;

    /**
     * @brief The global status, always execute
     * @note No used
     */
    State<controller_type> *m_pGlobalState;

public:
    /**
     * @brief Construct state machine
     * @note Called in the constructor function of m_pOwner
     */
    StateMachine(controller_type *m_pOwner) : m_pOwner(m_pOwner),
                                              m_pCurrentState(nullptr),
                                              m_pPreviousState(nullptr),
                                              m_pGlobalState(nullptr)
    {
    }

    /**
     * @brief Directly set current state, not execute State<controller_type>::Exit()
     *        of m_pCurrentState or State<controller_type>::Enter() of m_pPreviousState
     * @note In order to standardize the behavior of FSM, only called in Init()
     */
    void SetCurrentState(State<controller_type> *_state) { m_pCurrentState = _state; }

    /**
     * @brief Directly set global state
     * @note No used
     */
    void SetGlobalState(State<controller_type> *_state) { m_pGlobalState = _state; }

    /**
     * @brief Directly set previous state
     * @note In order to standardize the behavior of FSM, forbid to use generally.
     */
    void SetPreviousState(State<controller_type> *_state) { m_pPreviousState = _state; }

    /**
     * @brief Execute State<controller_type>::Execute() of m_pGlobalState and m_pCurrentState
     */
    void Update()
    {
        if (m_pGlobalState)
        {
            m_pGlobalState->Execute(m_pOwner);
        }

        if (m_pCurrentState)
        {
            m_pCurrentState->Execute(m_pOwner);
        }
    }

    /**
     * @brief Change state for state machine, execute State<controller_type>::Exit() of m_pPreviousState
     *        and State<controller_type>::Enter() of m_pCurrentState
     * @param _state the current state after state change
     * @note If _state is nullptr or  _state equal to m_pCurrentState, nothing happend
     */
    void ChangeState(State<controller_type> *_state)
    {
        if (!_state)
        {
            return;
        }

        if (_state == m_pCurrentState)
        {
            return;
        }

        m_pPreviousState = m_pCurrentState;

        if (m_pPreviousState)
        {
            m_pPreviousState->Exit(m_pOwner);
        }

        m_pCurrentState = _state;

        if (m_pCurrentState)
        {
            m_pCurrentState->Enter(m_pOwner);
        }
    }

    /**
     * @brief Change current state to previous state
     */
    void RevertToPreviousState() { ChangeState(m_pPreviousState); }

    /**
     * @brief Get current state
     * @retval Pointer of current state
     */
    State<controller_type> *GetCurrentState() const { return m_pCurrentState; }

    /**
     * @brief Get previous state
     * @retval Pointer of previous state
     */
    State<controller_type> *GetPreviousState() const { return m_pPreviousState; }

    /**
     * @brief Get global state
     * @retval Pointer of global state
     */
    State<controller_type> *GetGlobalState() const { return m_pGlobalState; }

    /**
     * @brief Initialization StateMachine, must set m_pCurrentState before Update()
     * @note Called in the initialization function of m_pOwner
     */
    virtual void Init() = 0;
};

#endif
