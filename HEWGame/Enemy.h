#pragma once
#include"Character.h"
#include"Animator.h"


enum class EnemyState
{
	NONE,
	STAY,
	DASH
};

class Enemy :public Character
{
protected:

	// 各状態のテクスチャを保持しておく変数
	ID3D11ShaderResourceView* m_eTexIdle = nullptr; // 待機用
	ID3D11ShaderResourceView* m_eTexWalk = nullptr; // 移動用
	ID3D11ShaderResourceView* m_eTexJump = nullptr; // ジャンプ用

	bool m_FlipX = true; // 左右反転フラグ
	Animator m_Animator;//アニメーション管理

	//現在再生中のアニメーション状態
	int m_CurrentAnimState = -1;
	
	EnemyState m_eState;

	const Character* m_pTarget = nullptr; //ターゲット（プレイヤー）の
	bool isDetection; //プレイヤーの発見状態
	int SCount = 0;   //タイマー
	int SCount2 = 0;
	//サーチ範囲設定
	DirectX::XMFLOAT2 searchSize;
	DirectX::XMFLOAT2 searchPos;

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update(const TileMap& tile, Character** charaList)override;
	void UnInit()override;

	void Attack(Character** charaList)override;
	int TakeDamage() override;
	void Jump()override;
	void SetTarget(const Character& target);
	void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump);

	//アニメーションさせるための描画
	void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;

	//アニメーション切り替え関数
	void SetAnimation(int stateIndex);
};
