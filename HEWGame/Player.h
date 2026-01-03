#pragma once
#include "Character.h"
class Player :public Character
{

public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;

    // 初期化（親クラスのInitをオーバーライド）
    void Init(ID3D11ShaderResourceView* pTexture) override;

    // 毎フレームの更新処理（入力による移動など）
    void Update() override;
};
