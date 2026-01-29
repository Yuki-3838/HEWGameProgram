#include "EnemyShooter.h"

EnemyShooter::EnemyShooter()
{
	// エネミー固有の初期設定
	EnemyInit();
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 15;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;

	m_Size.x = 64 * 2;
	m_Size.y = 64 * 2;
	m_Position.x = 1600.0f;
	m_Position.y = 0.0f;

	m_AttackTotalFrame = 30;  //攻撃アニメの総フレーム数
	m_AttackHitStart = 1;     //攻撃判定が発生する開始フレーム
	m_AttackHitEnd = 30;      //攻撃判定が発生する終了フレーム

	m_charaType = State::CharaType::t_EnemyShooter;
}

EnemyShooter::~EnemyShooter()
{
}

void EnemyShooter::Update(const TileMap& tile, Character** charaList)
{
	//アニメーション更新
	m_Animator.Update(1.0f / 1.0f);
	// 移動状態初期化
	m_MoveState = State::MoveState::NONE;
	// 移動入力処理
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_MoveState = State::MoveState::LEFT;
		m_charDir = State::CharDir::LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
		m_charDir = State::CharDir::RIGHT;
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

	// エネミーのAI処理

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
	}
}

void EnemyShooter::Attack(Character** charaList)
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

void EnemyShooter::SetAnimation(int stateIndex)
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
		scale = 1.25f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, animW, animH, 0.01f, 0.0f, true, offX, offY, scale);
		break;
	case 1: //移動
		m_pTexture = m_eTexWalk;
		animW = w;
		animH = h;
		scale = 1.25f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.02f, 0.0f, true, offX, offY, scale);
		break;
	case 2: // 攻撃予備動作
		m_pTexture = m_eTexAttackTelegraph;
		animW = w;
		animH = h;
		scale = 1.25f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.2f, 0.0f, true, offX, offY, scale);
		break;
	case 3: // 攻撃
		m_pTexture = m_eTexAttack;
		animW = w;
		animH = h;
		scale = 1.25f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(4, 2, w, h, 0.2f, 0.0f, false, offX, offY, scale);
		break;
	}
}

