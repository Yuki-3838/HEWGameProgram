#pragma once
#include"Character.h"


class Enemy :public Character
{
private:

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update(const TileMap& tile, Character** charaList)override;
	void UnInit()override;

	void Attack(Character** charaList)override;
	void TakeDamage(int) override;
	int ApplyDamage() override;
	void Jump()override;
};
