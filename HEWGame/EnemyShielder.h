#pragma once
#include "Enemy.h"

class EnemyShielder : public Enemy
{
protected:
	void SetAnimation(int stateIndex) override;
public:
	EnemyShielder();
	~EnemyShielder();

	void Update(const TileMap& tile, Character** charaList)override;
	void Attack(Character** charaList)override;
};


