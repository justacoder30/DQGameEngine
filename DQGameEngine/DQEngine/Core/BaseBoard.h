#pragma once
#include "Board.h"

namespace dqengine {

class BaseBoard : public Board
{
public:
	using Board::Query;
    void Query(ShapeComponent* obj, std::vector<ShapeComponent*>& result) override;
	void Insert(ShapeComponent* obj) override;
	void Clear() override;
    void Remove(ShapeComponent* obj) override;

private:
	std::vector<ShapeComponent*> colliders;
};


} // namespace dqengine
