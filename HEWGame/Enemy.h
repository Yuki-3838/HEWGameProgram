#pragma once
#include"Character.h"
#include"Animator.h"

enum class ActionState
{
	SERCH,
	ATTACK
};

class Enemy :public Character
{
protected:
	// 各状態のテクスチャを保持しておく変数
	ID3D11ShaderResourceView* m_eTexIdle = nullptr; // 待機用
	ID3D11ShaderResourceView* m_eTexWalk = nullptr; // 移動用
	ID3D11ShaderResourceView* m_eTexAttack = nullptr; // 攻撃用
	ID3D11ShaderResourceView* m_eTexAttackTelegraph = nullptr; // 攻撃予備動作

	bool m_FlipX = true; // 左右反転フラグ
	Animator m_Animator;//アニメーション管理

	//現在再生中のアニメーション状態
	int m_CurrentAnimState = -1;

	//アニメーション切り替え関数
	virtual void SetAnimation(int stateIndex);

	const Character* m_pTarget = nullptr; //ターゲット（プレイヤー）の
	bool isDetection; //プレイヤーの発見状態
	int SCount = 0;   //タイマー
	int SCount2 = 0;
	//サーチ範囲設定
	DirectX::XMFLOAT2 searchSize;
	DirectX::XMFLOAT2 searchPos;
	float m_serchDistance = 1000;
	ActionState m_ActionState;

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void EnemyInit();
	virtual void Update(const TileMap& tile, Character** charaList)override;
	virtual  void UnInit()override;
	void Draw();

	virtual void Attack(Character** charaList)override;
	int TakeDamage() override;
	void Jump() override;
	void SetTarget(const Character& target);
	void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* attack, ID3D11ShaderResourceView* attackTelegraph);

	//アニメーションさせるための描画
	void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;

	void SerchPlayer();
	
};
