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

	m_charaType = State::CharaType::t_EnemySword;

	isDetection = false; //プレイヤーの発見状態
}

Enemy::~Enemy()
{

}

void Enemy::EnemyInit()
{
	m_ActionState = ActionState::SERCH;
	m_serchDistance = 1000;
}

void Enemy::Update(const TileMap& tile, Character** charaList)
{
	/*if (m_ActionState == ActionState::SERCH)
	{


		if (targetPos.x + 250.0f < enemyPos.x)
		{
			m_MoveState = State::MoveState::LEFT;
			m_charDir = State::CharDir::LEFT;
		}

		else if (targetPos.x - 250.0f > enemyPos.x)
		{
			m_MoveState = State::MoveState::RIGHT;

		}

	}
	Move(tile);

	//アニメーションの切り替え判定(優先度はダッシュ＞溜め＞攻撃＞ジャンプ＞移動＞待機)
	int nextAnim = 0; // 0:待機 (デフォルト)

	//攻撃中か
	if (m_IsAttack)
	{
		nextAnim = 3; //攻撃用アニメ
	}
	// 攻撃予備動作中か
	//else if ()
	//{
	//	nextAnim = 2; //攻撃予備動作用アニメ
	//}
	// 移動中か
	else if (m_MoveState == State::MoveState::LEFT || m_MoveState == State::MoveState::RIGHT)
	{
		nextAnim = 1; // 移動用アニメ
	}
	else
	{
		nextAnim = 0; // 待機アニメ
	}

	// 状態が変わった時だけセットする
	if (nextAnim != m_CurrentAnimState)
	{
		SetAnimation(nextAnim);
	}*/
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

		if (obj->GetCharaType() == State::CharaType::t_Player)continue;  //player以外だったらスキップする

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

void Enemy::SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* attack, ID3D11ShaderResourceView* attackTelegraph)
{
		m_eTexIdle = idle;
		m_eTexWalk = walk;
		m_eTexAttack = attack;
		m_eTexAttackTelegraph = attackTelegraph;

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
	float drawX = m_Position.x + f.renderOffsetX;
	float drawY = m_Position.y + f.renderOffsetY;
	float drawW = f.w * f.scale;
	float drawH = f.h * f.scale;

	// SpriteRendererで描画
	if (m_pTexture && pSR)
	{
		
		m_FlipX = (m_charDir != State::CharDir::RIGHT); // enumyの向きに合わせてbool型の反転設定

		pSR->Draw(
			pContext,
			m_pTexture,
			drawX, drawY,
			drawW, drawH,
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
	float animW = 0; // 今回設定するアニメの幅
	float animH = 0; // 今回設定するアニメの高さ
	float uvOffsetY = 0.0f; // UVのYオフセット
	float scale = 1.0f;

	float offX;
	float offY;
	// 状態に合わせてテクスチャとアニメ設定を切り替える
	switch (stateIndex)
	{
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置, ループするかどうか)
		//テクスチャの入れ替え
		m_pTexture = m_eTexIdle;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, animW, animH, 0.01f, 0.0f, true, offX, offY, scale);
		break;
	case 1: //移動
		m_pTexture = m_eTexWalk;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.02f, 0.0f, true, offX, offY, scale);
		break;
	case 2: // 攻撃予備動作
		m_pTexture = m_eTexAttackTelegraph;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.2f, 0.0f, true, offX, offY, scale);
		break;
	case 3: // 攻撃
		m_pTexture = m_eTexAttack;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.2f, 0.0f, false, offX, offY, scale);
		break;
	}
}

void Enemy::SerchPlayer()
{
	int correction;

	int nowSerchDistance = 0;
	DirectX::XMFLOAT2 startpos;
	DirectX::XMFLOAT2 endpos;
	startpos.y = m_Position.y;
	endpos.y = m_Position.y + m_Size.y;
	if (m_charDir == State::CharDir::RIGHT)
	{
		nowSerchDistance = m_serchDistance;
		startpos.x = m_Position.x + m_Size.x;
		endpos.x = startpos.x + nowSerchDistance;
		for (int x = startpos.x; x < endpos.x; x -= m_Size.x)
		{
			for (int y = startpos.y; y < endpos.y; y += 10)
			{
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					int a = 0;
				}
			}
		}
	}
	else if (m_charDir == State::CharDir::LEFT)
	{
		nowSerchDistance = m_serchDistance * -1;
		startpos.x = m_Position.x;
		endpos.x = startpos.x + nowSerchDistance;

		for (int x = startpos.x; x > endpos.x; x -= m_Size.x)
		{
			for (int y = startpos.y; y < endpos.y; y+=10)
			{
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					int a = 0;
				}
			}
		}
	}
	
	
}

