#include "Player.h"
#include <Windows.h>
#include<vector>

std::vector<GameObject*> g_GameObjects;

// プレイヤーのコンストラクタ
Player::Player()
{
	// プレイヤー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 15;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;


	m_Size.x = 320.0f;
	m_Size.y = 320.0f;
	m_Position.x = 0.0f;
	m_Position.y = 100.0f;

	m_charaType = State::CharaType::t_Player;
	//例えば0 なら待機、1なら走る、2ならジャンプなど
	SetAnimation(0);

	m_IsDead = false;
}

Player::~Player()
{
}


void Player::Update(const TileMap& tile, Character** charaList)
{
	m_MoveState = State::MoveState::NONE;  //最初は右向き

	// 移動入力処理
	if (GetAsyncKeyState(VK_A) & 0x8000)
	{
		m_MoveState = State::MoveState::LEFT;
		m_FlipX = true;
		m_charDir = CharDir::LEFT;
	}
	if (GetAsyncKeyState(VK_D) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
		m_FlipX = false;
		m_charDir = CharDir::RIGHT;
	}
	//アニメーションの切り替え判定(優先度はジャンプ＞移動＞待機)
	int nextAnim = 0; // 0:待機 (デフォルト)

	// ジャンプ中か
	if (m_JumpState == State::JumpState::RISE || m_JumpState == State::JumpState::DESC)
	{
		nextAnim = 2; // ジャンプ用アニメ
	}
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

	//アニメーション更新
	m_Animator.Update(1.0f / 60.0f);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		Jump();
	}

	if (m_IsAttack == false && GetAsyncKeyState(VK_F) & 0x8000)
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

	Move(tile);
}

void Player::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
	// アニメーターから今のコマ情報を取得
	AnimFrame f = m_Animator.GetCurrentFrame();

	//絵をどれくらい下にずらすか
	float drawOffsetY = 0.0f;
	// SpriteRendererで描画
	if (m_pTexture && pSR)
	{
		pSR->Draw(
			pContext,
			m_pTexture,
			m_Position.x, m_Position.y + drawOffsetY,
			m_Size.x, m_Size.y,
			viewProj,
			f.x, f.y, f.w, f.h, // UV座標
			0.0f,    // 回転なし
			m_FlipX  // 反転フラグ
		);
	}
}


void Player::UnInit()
{
}

void Player::Jump()
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

void Player::Attack(Character** charaList)
{
	//攻撃範囲設定
	DirectX::XMFLOAT2 attackSize = { 200.f,128.0f };
	DirectX::XMFLOAT2 attackPos;
	if (m_charDir == CharDir::RIGHT)//右向き
	{
		attackPos.x = GetPosition().x + GetSize().x;
	}
	if (m_charDir == CharDir::LEFT)//左向き
	{
		attackPos.x = GetPosition().x - attackSize.x;
	}
	attackPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y / 4;

	for (int i = 0; charaList[i] != nullptr; ++i)
	{
		auto obj = charaList[i];
		//オブジェクトじゃなかったらスキップする
		if (!obj)continue;

		if (obj->GetCharaType() != State::CharaType::t_Enemy)continue;  //enemy以外だったらスキップする

		ColRes hit = CollisionRect(*obj, attackPos, attackSize);
		if (Col::Any(hit))
		{
			//敵にダメージを与える
			obj->TakeDamage();
			//ここではenemyをdeleteしない！
		}
	}
}

int Player::TakeDamage()
{
	int damage = 1;
	m_Stats.m_HP -= damage;
	return m_Stats.m_HP;
}

void Player::WallJump()
{
	// test wll
}

void Player::Blink()
{
}

void Player::GetBlink()
{
}

void Player::SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump)
{
	m_pTexIdle = idle;
	m_pTexWalk = walk;
	m_pTexJump = jump;

	// 初期状態として待機画像をセットしておく
	SetAnimation(m_CurrentAnimState);
}

void Player::SetAnimation(int stateIndex)
{
	m_CurrentAnimState = stateIndex;
	// 初期状態として待機画像をセットしておく
	m_pTexture = m_pTexIdle;
	m_CurrentAnimState = stateIndex;
	//画像の構成に合わせて数値を変更してね
	float w = 320.0f;
	float h = 240.0f;
	// 状態に合わせてテクスチャとアニメ設定を切り替える
	switch (stateIndex)
	{
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置)
		//テクスチャの入れ替え
		m_pTexture = m_pTexIdle;
		m_Animator.Init(18, 6, w, h, 0.01f, 0.0f);
		break;
	case 1: //移動
		m_pTexture = m_pTexWalk;
		m_Animator.Init(18, 6, w, h, 0.5f, 0.0f);
		break;
	case 2:
		m_pTexture = m_pTexJump;
		m_Animator.Init(1, 4, w, h, 0.2f, 0.0f);
		break;
	}
}
