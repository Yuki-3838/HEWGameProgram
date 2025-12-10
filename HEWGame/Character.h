#pragma once
#include"Object.h"
class Character
{
protected:
	int hp;
	float speed;
	const float gravity = -0.5f;

	bool isGround = false;
	bool isNotDamage;


public:
	virtual void Init(HWND hWnd) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void UnInit() = 0;

	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void Jump() = 0;
};

