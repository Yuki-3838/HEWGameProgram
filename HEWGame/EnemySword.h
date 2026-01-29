#pragma once
#include "Enemy.h"

class EnemySword : public Enemy
{
protected:
    void SetAnimation(int stateIndex) override;

public:
    EnemySword();
    ~EnemySword() override;

    void Update(const TileMap& tile, Character** charaList) override;
    void UnInit() override;
    void Attack(Character** charaList) override;

};
