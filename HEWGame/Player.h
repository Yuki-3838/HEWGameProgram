#pragma once
#include "Character.h"
#include"Animator.h"
#include"Sound.h"

class Player :public Character
{
private:
    // 各状態のテクスチャを保持しておく変数
    ID3D11ShaderResourceView* m_pTexIdle = nullptr; // 待機用
    ID3D11ShaderResourceView* m_pTexWalk = nullptr; // 移動用
    ID3D11ShaderResourceView* m_pTexJump = nullptr; // ジャンプ用

    Animator m_Animator;//アニメーション管理
    bool m_FlipX = false; // 左右反転フラグ

    //現在再生中のアニメーション状態
    int m_CurrentAnimState = -1;

    //アニメーション切り替え関数
    void SetAnimation(int stateIndex);

    Sound* m_pSound = nullptr;
    EffectManager* m_pEffectManager = nullptr;
    Effect* m_pRunningEffect = nullptr;
public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;

    // 毎フレームの更新処理（入力による移動など）
    void Update(const TileMap& tile, Character** charaList)override;

    //アニメーションさせるための描画
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;
    // ステージとの当たり判定取得

    void Jump()override;
    void Attack(Character** charaList)override;
    int TakeDamage()override;

    // ダッシュ処理
    void DashMove(const TileMap& tile);

    void WallJump();
    void Blink();
    void GetBlink();
    void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump, ID3D11ShaderResourceView* fall, ID3D11ShaderResourceView* attack);
    void SetSound(Sound* pSound) { m_pSound = pSound; }
    void SetEffectManager(EffectManager* em) { m_pEffectManager = em; }
};
