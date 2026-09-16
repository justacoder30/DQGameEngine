#include "pch.h"
#include "Utils.h"

namespace dqengine {

float RandomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

} // namespace dqengine
