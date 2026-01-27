#pragma once
#include "Enemy.h"

class EnemyShooter : public Enemy
{
protected:
	void SetAnimation(int stateIndex) override;
	
public:
	EnemyShooter();
	~EnemyShooter() override;

	void Update(const TileMap& tile, Character** charaList) override;
	void UnInit() override;
	void Attack(Character** charaList) override;
};


