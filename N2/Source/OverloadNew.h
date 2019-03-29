#ifndef OVERLOAD_NEW_H
#define OVERLOAD_NEW_H

#include "MemoryLeak.h"


#define new new(__FUNCTION__, __LINE__)

#endif
