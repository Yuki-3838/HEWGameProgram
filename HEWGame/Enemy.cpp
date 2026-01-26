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
	m_Position.x = 1000.0f;
	m_Position.y = 0.0f;

	searchSize = { 500.f, 128.0f };

	m_charaType = State::CharaType::t_Enemy;

	isDetection = false; //プレイヤーの発見状態
	m_FlipX = true;   // 左右反転フラグ(最初は左向き)
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
		m_FlipX = true;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
		m_FlipX = false;
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
		if (m_AttackFrame >= m_AttackHitStart && m_AttackFrame <= m_AttackHitEnd)
		{
			Attack(charaList);
		}
		//攻撃アニメ終了判定
		if (m_AttackFrame >= m_AttackTotalFrame)
		{
			m_IsAttack = false;
			m_AttackFrame = 0;
		}
	}

	//非発見状態
	if (isDetection == false)
	{
		//左右移動にする予定
		SCount++;

		if (SCount == 200)
		{
			m_MoveState = State::MoveState::RIGHT;
			m_FlipX = false; //右を見る
		}

		if (SCount == 400)
		{
			m_MoveState = State::MoveState::LEFT;
			m_FlipX = true; //左を見る

			SCount = 0;
		}


		if (m_FlipX)//右向き
		{
			searchPos.x = GetPosition().x - searchSize.x;
		}
		else//左向き
		{
			// attackPos.x += (m_Size.x / 2) + (attackSize.x / 2);
			searchPos.x = GetPosition().x + GetSize().x;
		}
		//attackPos.y += m_Size.y / 4;
		searchPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y / 4;

		for (int i = 0; charaList[i] != nullptr; ++i)
		{
			auto obj = charaList[i];
			//オブジェクトじゃなかったらスキップする
			if (!obj)continue;

			//Character* chara = dynamic_cast<Character*>(obj);
			//if (!chara)continue;
			if (obj->GetCharaType() != State::CharaType::t_Player)continue;  //player以外だったらスキップする

			//ColRes hit = CollisionRect(attackPos, attackSize, chara->GetPosition(), chara->GetSize());]
			ColRes hit = CollisionRect(*obj, searchPos, searchSize);

			if (Col::Any(hit))
			{
				//敵にダメージを与える
				/*obj->TakeDamage();*/
				//ここではenemyをdeleteしない！

				SCount2++;
				if (SCount2 == 100) //テスト用に見つけてから一拍おいてます
				{
					isDetection = true;
					SCount2 = 0;
				}
			}
		}
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


			if (targetPos.x + 250.0f < enemyPos.x)
			{
				m_MoveState = State::MoveState::LEFT;
				m_FlipX = true;
			}

			else if (targetPos.x - 250.0f > enemyPos.x)
			{
				m_MoveState = State::MoveState::RIGHT;
				m_FlipX = false;
			}
		}
	}
	Move(tile);
}

void Enemy::UnInit()
{
  
}

void Enemy::Draw()
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

void Enemy::SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump)
{
		m_eTexIdle = idle;
		m_eTexWalk = walk;
		m_eTexJump = jump;

		// 初期状態として待機画像をセットしておく
		SetAnimation(m_CurrentAnimState);
}

//アニメーションさせるための描画
void Enemy::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
	// アニメーターから今のコマ情報を取得
	AnimFrame f = m_Animator.GetCurrentFrame();

	////絵をどれくらい下にずらすか
	//float drawOffsetY = 60.0f;   /*+ drawOffsetY*/
	

	// SpriteRendererで描画
	if (m_pTexture && pSR)
	{
		pSR->Draw(
			pContext,
			m_pTexture,
			m_Position.x, m_Position.y,   
			m_Size.x, m_Size.y,
			viewProj,
			f.x, f.y, f.w, f.h, // UV座標
			0.0f,    // 回転なし
			m_FlipX  // 反転フラグ
		);
	}
}

void Enemy::SetAnimation(int stateIndex)
{
	m_CurrentAnimState = stateIndex;
	// 初期状態として待機画像をセットしておく
	m_pTexture = m_eTexIdle;
	m_CurrentAnimState = stateIndex;
	//画像の構成に合わせて数値を変更してね
	float w = 320.0f;
	float h = 320.0f;
	// 状態に合わせてテクスチャとアニメ設定を切り替える
	switch (stateIndex)
	{
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置)
		//テクスチャの入れ替え
		m_pTexture = m_eTexIdle;
		m_Animator.Init(1, 1, w, h, 0.01f, 0.0f);
		break;
	case 1: //移動
		m_pTexture = m_eTexWalk;
		m_Animator.Init(1, 1, w, h, 0.2f, 0.0f);
		break;
	case 2:
		m_pTexture = m_eTexJump;
		m_Animator.Init(1, 1, w, h, 0.2f, 0.0f);
		break;
	}
}
