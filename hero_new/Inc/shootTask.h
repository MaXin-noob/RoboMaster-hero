#ifndef SHOOTTASK_H
#define SHOOTTASK_H
#include "main.h"
typedef enum
{
	frictiongear_open,
	frictiongear_close,
} frictiongearMode_e;

typedef enum
{
	pluck_open,
	pluck_close,
	pluck_lock,
} pluckMode_e;

extern float pluck_Angle_Data;
extern float pluck_Last_Angle_Data;

#endif
