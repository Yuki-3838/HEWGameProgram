#pragma once
#include "Enemy.h"

class EnemyShooter : public Enemy
{
protected:
	void SetAnimation(int stateIndex) override;
public:
	EnemyShooter();
	~EnemyShooter();

	void Update(const TileMap& tile, Character** charaList)override;
	void Attack(Character** charaList)override;
};


