#pragma once
enum State
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall,
	Death,
	Roll,
	Hurt,
	Attack1,
	Attack2,
	Attack3,
	AirAttack1,
	AirAttack2,
	AirAttack3,
	AirAttackEnd,
};

enum Direction
{
	Left,
	Right,
	Up,
	Down
};