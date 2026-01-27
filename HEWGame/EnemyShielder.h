#pragma once
#include "Enemy.h"

class EnemyShielder : public Enemy
{
public:
	EnemyShielder();
	~EnemyShielder();

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


