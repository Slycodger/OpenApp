#define _UNIQUE_TYPE
#include "uniqueType.h"
#include <vector>
using namespace openApp;

StaticList<UniqueType*> UniqueType::globalUniqueTypeInstances = StaticList<UniqueType*>(32);
size_t UniqueType::globalUniqueTypeCount = 0;