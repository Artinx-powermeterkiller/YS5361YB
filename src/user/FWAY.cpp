#include <FYAW.hpp>

FYAW::FYAW() : RS485Device(0)
{
}

void FYAW::Read(uint8_t _cmd)
{
    
}

uint8_t FYAW::ReadReceive(uint8_t *_buffer)
{
    return 0;
}

void FYAWManager::Init()
{
}

void FYAWManager::SendUpdate()
{
    
}

void FYAWManager::ReceiceUpdate()
{
}