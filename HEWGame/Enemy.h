#pragma once
#include"Character.h"
#include"Animator.h"


class Enemy :public Character
{
private:
	const Character* m_pTarget = nullptr;
	bool isDetection; //プレイヤーの発見状態

	

public:
	// コンストラクタ・デストラクタ
	Enemy();
	~Enemy() override;

	void Update(const TileMap& tile, Character** charaList)override;
	void UnInit()override;
	void Draw();

	void Attack(Character** charaList)override;
	int TakeDamage() override;
	void Jump()override;
	void SetTarget(const Character& target);
	void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump);

	//アニメーションさせるための描画
	void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;
};
