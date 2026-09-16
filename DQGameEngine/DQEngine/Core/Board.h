#pragma once
#include "Components/ShapeComponent.h"

namespace dqengine {

class Board
{
public:
    virtual ~Board() = default;
    virtual void Query(ShapeComponent* obj, std::vector<ShapeComponent*>& result) = 0;
    std::vector<ShapeComponent*> Query(ShapeComponent* obj) {
        std::vector<ShapeComponent*> result;
        Query(obj, result);
        return result;
    }
	virtual void Insert(ShapeComponent* obj) = 0;
	virtual void Clear() = 0;
    virtual void Remove(ShapeComponent* obj) = 0;
};


} // namespace dqengine
