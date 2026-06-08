#include "pch.h"
#include "Rect.h"

void Rect::setRect()
{
	top = y;
	bottom = y + h;
	left = x;
	right = x + w;
}

Rect::Rect()
{
}

Rect::Rect(const Vector& pos, const Vector& size)
{
	this->pos = pos;
	this->size = size;

	x = pos.x;
	y = pos.y;
	w = size.x;
	h = size.y;

	setRect();
}

Rect::Rect(const float& _x, const float& _y, const float& _w, const float& _h) : x(_x), y(_y), w(_w), h(_h)
{
	this->pos = Vector(x, y);
	this->size = Vector(w, h);

	x = pos.x;
	y = pos.y;
	w = size.x;
	h = size.y;

	top = y;
	bottom = y + h;
	left = x;
	right = x + w;
}

bool Rect::CheckCollide(const Rect& other)
{
	return (top < other.bottom &&
		bottom > other.top &&
		left < other.right &&
		right > other.left);
}

Vector Rect::GetCenter()
{
	return Vector(x + w / 2, y + h / 2);
}

Vector Rect::GetOverlap(const Rect& other)
{
	float overlapX = std::min(right, other.right) - std::max(left, other.left);
	float overlapY = std::min(bottom, other.bottom) - std::max(top, other.top);

	return Vector(overlapX, overlapY);	
}

Vector Rect::GetMTV(const Rect& other)
{
	Vector overlap = GetOverlap(other);
	if (overlap.x < overlap.y)
		return Vector((x < other.x) ? -overlap.x : overlap.x, 0);
	else
		return Vector(0, (y < other.y) ? -overlap.y : overlap.y);
}
