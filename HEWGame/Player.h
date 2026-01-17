#pragma once
#include "Character.h"
#include"Input.h"


class Player :public Character
{
private:

public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;

    // 毎フレームの更新処理（入力による移動など）
    void Update(const TileMap& tile)override;
    // ステージとの当たり判定取得

    void UnInit()override;

    void Move(const TileMap& tile)override;
    void Jump()override;
    void Attack()override;

    void WallJump();
    void Blink();
    void GetBlink();

};
