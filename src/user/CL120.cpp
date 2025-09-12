#include "CL120.hpp"

MotorManager::MotorManager()
{
    std::array<RS485Motor,10> temp_array = {cl57_motor[0],cl57_motor[1],cl57_motor[2],cl57_motor[3],
        cl120_motor[0],cl120_motor[1],cl120_motor[2],cl120_motor[3],cl120_motor[4],cl120_motor[5]};
    device_vector = std::move(temp_array);
}

