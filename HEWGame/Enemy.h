#pragma once
#include"Character.h"


class Enemy :public Character
{
private:

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update(const TileMap& tile)override;
	void UnInit()override;

	void Move(const TileMap& tile)override;
	void Attack()override;
	void Jump()override;
};
