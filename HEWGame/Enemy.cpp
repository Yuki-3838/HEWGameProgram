#include "Enemy.h"

Enemy::Enemy()
{
	// エネミー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 15;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;


	m_Size.x = 128.0f;
	m_Size.y = 256.0f;
	m_Position.x = 500.0f;
	m_Position.y = 0.0f;

	m_charaType = State::CharaType::t_Enemy;
}

Enemy::~Enemy()
{
}

void Enemy::Update(const TileMap& tile)

{
	m_MoveState = State::MoveState::NONE;
	// 移動入力処理
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_MoveState = State::MoveState::LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		Jump();
	}

	Move(tile);
}

void Enemy::UnInit()
{
  
}


void Enemy::Attack()
{

}

void Enemy::TakeDamage(int)
{
}

int Enemy::ApplyDamage()
{
	return 0;
}

void Enemy::Jump()
{
	//Y軸の加速度がなければ追加
// ジャンプ　→　地面についていたら可能
// 処理　　
	if (m_JumpState == State::JumpState::NONE)
	{
		m_Stats.m_AccelY = m_Stats.m_JumpPw;
		m_JumpState = State::JumpState::RISE;
	}
}