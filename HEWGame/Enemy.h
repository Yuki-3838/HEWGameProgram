#pragma once
#include"Character.h"


enum class EState
{
	STAY,
	JUMP,
	STUN,
	ATTACK,
	IsDetected
};

enum EDir
{
	RIGHT,	//右
	LEFT,   //左
};


class Enemy :public Character
{
private:
	EState state;
	EDir dir;

	bool isDead;

	GameObject* m_enemy = nullptr;

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update()override;
	void UnInit()override;

	void Move()override;
	void Attack()override;
	void Jump()override;
};
