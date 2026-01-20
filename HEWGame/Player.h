#pragma once
#include "Character.h"
#include"Animator.h"
#include"Sound.h"
#include "EffectManager.h"
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
public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;

    // 毎フレームの更新処理（入力による移動など）
    void Update(const TileMap& tile)override;

    //アニメーションさせるための描画
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;
    // ステージとの当たり判定取得

    void UnInit()override;

    void Move(const TileMap& tile)override;
    void Jump()override;
    void Attack()override;
    void TakeDamage(int)override;
    int ApplyDamage();

    void WallJump();
    void Blink();
    void GetBlink();
    void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump);
    void SetSound(Sound* pSound) { m_pSound = pSound; }
    void SetEffectManager(EffectManager* em) { m_pEffectManager = em; }
};
