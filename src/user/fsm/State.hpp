#ifndef STATE_HPP
#define STATE_HPP

/**
 * @brief state in FSM
 */
template<typename controller_type>
class State
{
public:

    /**
     * @brief Initialization state
     * @note Usually called in StateMachine<controller_type>::Init()
     */
    virtual void Init(controller_type* pOwner) = 0;

    /**
     * @brief Finish necessary preparatory work before enter this state
     */
    virtual void Enter(controller_type* pOwner) = 0;

    /**
     * @brief Execute normal task in this state
     */
    virtual void Execute(controller_type* pOwner) = 0;

    /**
     * @brief Finish necessary wind up work before exit this state
     */
    virtual void Exit(controller_type* pOwner) = 0;
};

#endif
