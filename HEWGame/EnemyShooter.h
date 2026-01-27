#pragma once
#include "Enemy.h"

class EnemyShooter : public Enemy
{

private:
	// 各状態のテクスチャを保持しておく変数
	ID3D11ShaderResourceView* m_eTexIdle = nullptr; // 待機用
	ID3D11ShaderResourceView* m_eTexWalk = nullptr; // 移動用
	ID3D11ShaderResourceView* m_eTexJump = nullptr; // ジャンプ用
public:
	EnemyShooter();
	~EnemyShooter();

};


