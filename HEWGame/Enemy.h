#pragma once
#include"Character.h"


enum class EState
{
	STAY,
	JUMP,
	STAN,
	ATTACK,
	CONTACT,
};

enum EDir
{
	RIGHT,	//âE
	LEFT,   //ç∂
};


class Enemy :public Character
{
private:
	EState state;
	EDir dir;
	bool isContact;
	bool isDead;

	GameObject* m_enemy = nullptr;

public:
	void Update(TileMap& tile)override;
	void UnInit()override;

	void Move()override;
	void Attack()override;
	void Jump()override;
};
