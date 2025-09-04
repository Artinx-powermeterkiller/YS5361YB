#ifndef ENTITYTYPE_HPP
#define ENTITYTYPE_HPP

enum EntityType
{
    ET_SENSOR = 0,
    ET_CONTROLLER,
    ET_ACTUATOR,

    ET_LENGTH
};

enum EntityClassType
{
    ECT_NULL = 0,
    ECT_GimbalController,
	ECT_ChassisController,
    ECT_ShooterController,
    ECT_LENGTH
};

#endif
