#include "Player.h"
#include <Windows.h>


// プレイヤーのコンストラクタ
Player::Player()
{
	// プレイヤー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 15;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;


	m_Size.x = 192.0f;
	m_Size.y = 192.0f;
	m_Position.x = 0.0f;
	m_Position.y = 640.0f;

	m_charaType = State::CharaType::t_Player;
	//例えば0 なら待機、1なら走る、2ならジャンプなど
	SetAnimation(0);

	isDead = false;
}

Player::~Player()
{
	//aaaaaaaaaaa
}


void Player::Update(const TileMap& tile)
{
	m_MoveState = State::MoveState::NONE;  //最初は右向き
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
	m_Animator.Update(1.0f / 2.0f);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		Jump();
	}

	if (GetAsyncKeyState(VK_Z) & 0x8000)
	{
		Attack();
	}

	Move(tile);
}

void Player::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
	// アニメーターから今のコマ情報を取得
	AnimFrame f = m_Animator.GetCurrentFrame();

	//絵をどれくらい下にずらすか
	float drawOffsetY = 0;
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

void Player::Move(const TileMap& tile)
{
	switch (m_MoveState)
	{
	case State::MoveState::LEFT:
		m_Position.x -= m_Stats.m_Speed;
		if (StageCol(tile, ColRes::LEFT))m_Position.x += m_Stats.m_Speed;
		if (m_pEffectManager)
		{
			// 足元に出したいので Y座標を調整
			m_pEffectManager->Play(EffectType::Smoke, m_Position.x, m_Position.y+64);
		}
		break;
	case State::MoveState::RIGHT:
		m_Position.x += m_Stats.m_Speed;
		if (StageCol(tile, ColRes::RIGHT))m_Position.x -= m_Stats.m_Speed;
		if (m_pEffectManager)
		{
			// 足元に出したいので Y座標を調整
			m_pEffectManager->Play(EffectType::Smoke, m_Position.x, m_Position.y+64);
		}
		break;
	}

	// 高度に関する処理
	switch (m_JumpState)
	{
		// 上昇処理
	case State::JumpState::RISE:
		//　上昇し、１ｆずつ上昇加速度を１減速する
		m_Position.y -= m_Stats.m_AccelY;
		m_Stats.m_AccelY--;
		// 天井に衝突した場合、下降に移行する
		if (StageCol(tile, ColRes::TOP))
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		// 上昇加速度が０になった場合、下降に移行する
		if (m_Stats.m_AccelY == 0)
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		break;

		// 下降処理
	case State::JumpState::DESC:
		// 下降する
		m_Position.y -= m_Stats.m_AccelY;
		// 最大下降加速度出ない場合、下降加速度を１加速
		if (m_Stats.m_AccelY > -m_Stats.m_AccelYMax)
		{
			m_Stats.m_AccelY -= m_Stats.m_Gravity;
		}
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

		// 通常時処理
	case State::JumpState::NONE:
		// 重力を与え、地面に着地していなければ下降に移行
		m_Position.y += m_Stats.m_Gravity;
		if (!StageCol(tile, ColRes::BOTTOM))
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY++;
		}
		m_Position.y -= m_Stats.m_Gravity;
	}
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
		if (m_pSound)
		{
			m_pSound->Play(SOUND_LABEL_SE_JUMP);
		}
	}
}

void Player::Attack()
{
	ApplyDamage();
}

void Player::TakeDamage(int damage)
{
	m_Stats.m_HP - damage;
	if (m_Stats.m_HP <= 0)
	{
		m_Stats.m_HP = 0;
		isDead = true;
	}
}

int Player::ApplyDamage()
{
	return 0;
}

void Player::WallJump()
{
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
			m_Animator.Init(18, 6, w, h, 0.01f,0.0f);
			break; 
		case 1: //移動
			m_pTexture = m_pTexWalk;
			m_Animator.Init(18, 6, w, h, 0.5f,0.0f);
			break;
		case 2:
			m_pTexture = m_pTexJump;
			m_Animator.Init(1, 4, w, h, 0.2f, 0.0f);
			break;
	}
}