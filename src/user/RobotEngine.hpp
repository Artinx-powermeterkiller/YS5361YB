#ifndef ROBOTENGINE_HPP
#define ROBOTENGINE_HPP

#include "EntityType.hpp"
#include "Time.hpp"

#define MAX_ENTITIES 32

class RobotEngine;

/**
 * @brief An abstract collection of certain components of a robot
 * @note Only used in ControllerEntity in this version
 * @todo Flatten in next version
 */
class Entity
{
private:
    RobotEngine* m_pOwner;
    EntityType m_EntityType;
    uint8_t m_EntityId;
    uint32_t m_LastUpdateTick;
    uint32_t m_TicksToUpdate;
    uint32_t m_DefaultTicksToUpdate;
    EntityClassType m_ClassType;
    bool m_IsActive;
    bool m_NeedUpdate;
    
protected:
    Entity(EntityType _type, EntityClassType _classType, RobotEngine* _pRobotEngine);

public:
    RobotEngine* GetOwner(){ return m_pOwner; }
    EntityType GetEntityType(){ return m_EntityType; }
    uint8_t GetEntityId(){ return m_EntityId; }
    uint32_t GetLastUpdateTick(){ return m_LastUpdateTick; }
    uint32_t GetTicksToUpdate(){ return m_TicksToUpdate; }
    uint32_t GetDefaultTicksToUpdate(){ return m_DefaultTicksToUpdate; }
    EntityClassType GetClassType(){ return m_ClassType; }
    bool IsActive(){ return m_IsActive; }
    bool NeedUpdate(){ return m_NeedUpdate; }
    void SetActive(bool _state){ m_IsActive = _state; }
    void SetOwner(RobotEngine* _pOwner){ m_pOwner = _pOwner; }
    void SetId(uint8_t _id){ m_EntityId = _id; }
    void SetDefaultTicksToUpdate(uint32_t _defaultTicks){ m_DefaultTicksToUpdate = _defaultTicks; }

    /**
     * @brief  Initialization entity, must called before Update(),
     *         called in RobotEngine::Init() automatically.
     * @note   pure virtual function, must be override in subclass.
     */
    virtual void Init() = 0;

    /**
     * @brief  Update entity, called in RobotEngine::Run() automatically.
     * @note   pure virtual function, must be override in subclass.
     */
    virtual void Update() = 0;

    void ClearUpdateFlag();
    void Suspend(uint32_t _ticks);
};

class RobotEngine
{
private:
    Entity* m_EntityList[ET_LENGTH][MAX_ENTITIES];
    
    uint8_t m_EntityCount[ET_LENGTH];

    bool m_IsRunning;
    static bool IsInstantiated;
    static RobotEngine* instance;
    
public:
    RobotEngine(RobotEngine* instance, uint32_t _msPerTick = 1);
    void AddEntity(Entity* _newEntity, EntityType _type);

    /**
     * @brief  Initialization entity in RobotEngine::m_EntityList       
     */
    void Init();

    /**
     * @brief  Update entity in RobotEngine::m_EntityList       
     */
    void Run();

    Entity* GetEntity(EntityClassType _classType);
    void GetEntity(EntityClassType _classType, uint32_t _maxLen, Entity** entityFound, uint32_t& numberFound);
    uint32_t GetTick(){ return Time::GetTick(); }
    uint32_t GetMsPerTick(){ return Time::GetMsPerTick(); }
    static RobotEngine* Instance(){ return instance; }
};

class ControllerEntity : public Entity
{
public:
    ControllerEntity(EntityClassType _classType);
    virtual void Init(){}
};

#endif
