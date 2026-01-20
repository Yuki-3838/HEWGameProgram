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

void Enemy::Update(const TileMap& tile, Character** charaList)
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

void Enemy::Move(const TileMap& tile)
{
  
	switch (m_MoveState)
	{
	case State::MoveState::LEFT:
		m_Position.x -= m_Stats.m_Speed;
		if (StageCol(tile, ColRes::LEFT))m_Position.x += m_Stats.m_Speed;
		break;
	case State::MoveState::RIGHT:
		m_Position.x += m_Stats.m_Speed;
		if (StageCol(tile, ColRes::RIGHT))m_Position.x -= m_Stats.m_Speed;
		break;
	case State::MoveState::TOP:
		m_Position.y -= m_Stats.m_Speed;
		if (StageCol(tile, ColRes::TOP))m_Position.y += m_Stats.m_Speed;
		break;
	case State::MoveState::BOTTOM:
		m_Position.y += m_Stats.m_Speed;
		if (StageCol(tile, ColRes::BOTTOM))m_Position.y -= m_Stats.m_Speed;
		break;
	}
	switch (m_JumpState)
	{
	case State::JumpState::RISE:
		m_Position.y -= m_Stats.m_AccelY;
		m_Stats.m_AccelY--;
		if (StageCol(tile, ColRes::TOP))
		{
			m_Position.y += m_Stats.m_AccelY + 1;
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		if (m_Stats.m_AccelY == 0)
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		break;
	case State::JumpState::DESC:
		m_Position.y -= m_Stats.m_AccelY;
		m_Stats.m_AccelY -= m_Stats.m_Gravity;
		if (StageCol(tile, ColRes::BOTTOM))
		{
			do
			{
				m_Position.y -= 1;
			} while (StageCol(tile, ColRes::BOTTOM));
			m_JumpState = State::JumpState::NONE;
			m_Stats.m_AccelY = 0;
		}
		break;
	case State::JumpState::NONE:
		m_Position.y += m_Stats.m_Gravity;
		if (!StageCol(tile, ColRes::BOTTOM))
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY++;
		}
		m_Position.y -= m_Stats.m_Gravity;
	}
}

void Enemy::Attack(Character** charaList)
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