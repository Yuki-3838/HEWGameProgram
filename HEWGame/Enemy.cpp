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
	//a
	m_charaType = State::CharaType::t_Enemy;

	isDetection = false; //プレイヤーの発見状態
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

	if (m_IsAttack == false && GetAsyncKeyState(VK_Z) & 0x8000)
	{
		m_IsAttack = true;
		m_AttackFrame = 0;
	}

	//攻撃処理
	if (m_IsAttack)
	{
		m_AttackFrame++;
		//攻撃判定のあるフレームならAttack関数を呼び出す
		if (m_AttackFrame >= AttackHitStart && m_AttackFrame <= AttackHitEnd)
		{
			Attack(charaList);
		}
		//攻撃アニメ終了判定
		if (m_AttackFrame >= AttackTotalFrame)
		{
			m_IsAttack = false;
			m_AttackFrame = 0;
		}
	}

	//非発見状態
	if (isDetection == false)
	{
		//左右移動にする予定
	}

	//発見状態時
	if (isDetection == true)
	{
		m_MoveState = State::MoveState::NONE;
		if (m_pTarget)
		{
			DirectX::XMFLOAT2 targetPos = m_pTarget->GetPosition();
			DirectX::XMFLOAT2 enemyPos = GetPosition();

			targetPos.x += m_pTarget->GetSize().x * 0.5f;
			enemyPos.x += GetSize().x * 0.5f;

			if (targetPos.x < enemyPos.x)
				m_MoveState = State::MoveState::LEFT;
			else
				m_MoveState = State::MoveState::RIGHT;

		}
	}
	Move(tile);
}

void Enemy::UnInit()
{
  
}


void Enemy::Attack(Character** charaList)
{
	//攻撃範囲設定
	DirectX::XMFLOAT2 attackSize = { 200.f,128.0f };
	DirectX::XMFLOAT2 attackPos;
	if (m_charDir == State::CharDir::RIGHT)//右向き
	{
		attackPos.x = GetPosition().x + GetSize().x;
	}
	if (m_charDir == State::CharDir::LEFT)//左向き
	{
		attackPos.x = GetPosition().x - attackSize.x;
	}
	attackPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y / 4;

	for (int i = 0; charaList[i] != nullptr; ++i)
	{
		auto obj = charaList[i];
		//オブジェクトじゃなかったらスキップする
		if (!obj)continue;

		if (obj->GetCharaType() != State::CharaType::t_Player)continue;  //player以外だったらスキップする

		ColRes hit = CollisionRect(*obj, attackPos, attackSize);
		if (Col::Any(hit))
		{
			//敵にダメージを与える
			obj->TakeDamage();
			//ここではplayerをdeleteしない！
		}
	}
}

int Enemy::TakeDamage()
{
	int damage = 1;
	m_Stats.m_HP -= damage;
	if (m_Stats.m_HP <= 0)
	{
		m_Stats.m_HP = 0;
		m_IsDead = true;
	}
	return m_Stats.m_HP;
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

void Enemy::SetTarget(const Character& target)
{
	m_pTarget = &target;
}
