#pragma once
#include"Character.h"


enum class EState
{
	STY,
	JUMP,
	STAN,
	ATTACK,
	CONTACT,
	RIGHT,
	LEFT
};

class Enemy :public Character
{
private:
	int hp;
	bool isGround;
	bool isNoDamage;
	EState state;
	Object m_enemy;
	bool isContact;
	int speed;
	int gravity;

public:
	void Init(HWND hWnd)override;
	void Update()override;
	void Draw()override;
	void UnInit()override;

	void Move()override;
	void Attack()override;
	void Jump()override;
};
