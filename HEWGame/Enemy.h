#pragma once
#include"Character.h"


class Enemy :public Character
{
private:
	const Character* m_pTarget = nullptr;
	bool isDetection; //プレイヤーの発見状態

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update(const TileMap& tile, Character** charaList)override;
	void UnInit()override;

	
	void Attack(Character** charaList)override;
	int TakeDamage() override;
	void Jump()override;
	void SetTarget(const Character& target);
};
