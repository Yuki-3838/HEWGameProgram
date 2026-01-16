#include "Player.h"
#include <Windows.h>

// プレイヤーのコンストラクタ
Player::Player()
{
	// プレイヤー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 5.0f;
	m_Stats.m_Gravity = 1;
	m_Stats.m_JumpPw = 12;


	m_Size.x = 64.0f;
	m_Size.y = 128.0f;
	m_Position.x = 0.0f;
	m_Position.y = 0.0f;

	state = PState::STAY;
	dir = RIGHT;

}

Player::~Player()
{
	if (m_player) delete m_player;
}


void Player::Update(TileMap& tile)
{
	m_MoveState = State::MoveState::NONE;
	// 移動入力処理
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		m_MoveState = State::MoveState::TOP;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		m_MoveState = State::MoveState::BOTTOM;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_MoveState = State::MoveState::LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		Jump();
	}

	Move(tile);
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
		if (!StageCol(tile, ColRes::LEFT))m_Position.x += m_Stats.m_Speed;
		break;
	case State::MoveState::RIGHT:
		m_Position.x += m_Stats.m_Speed;
		if (!StageCol(tile, ColRes::RIGHT))m_Position.x -= m_Stats.m_Speed;
		break;
	case State::MoveState::TOP:
		m_Position.y -= m_Stats.m_Speed;
		if(!StageCol(tile,ColRes::TOP))m_Position.y += m_Stats.m_Speed;
		break;
	case State::MoveState::BOTTOM:
		m_Position.y += m_Stats.m_Speed;
		if(!StageCol(tile,ColRes::BOTTOM))m_Position.y -= m_Stats.m_Speed;
		break;
	}
	switch (m_JumpState)
	{
	case State::JumpState::RISE:
		m_Position.y -= m_Stats.m_AccelY;
		m_Stats.m_AccelY--;
		if (!StageCol(tile, ColRes::TOP))
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
		m_Stats.m_AccelY--;
		if (!StageCol(tile, ColRes::BOTTOM))
		{
			m_Position.y += m_Stats.m_AccelY + 1;
			m_JumpState = State::JumpState::NONE;
			m_Stats.m_AccelY = 0;
		}
		break;
	case State::JumpState::NONE:
		m_Position.y += m_Stats.m_Speed;
		if (StageCol(tile, ColRes::BOTTOM))
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY++;
		}
		m_Position.y -= m_Stats.m_Speed;
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
	}
}

void Player::Attack()
{
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

// 当たり判定作成中
bool Player::StageCol(const TileMap& tile,const ColRes direction)
{

	// 周囲のタイルを探索　→　タイルのIDを特定　→　壁であれば当たり判定チェック
	// COl 対象の座標同士の当たり判定位置を確認する

	// 周囲のタイル このままだと＋の方向は余分にとっている、また、少ししかかぶっていないところも取っている
	// 探索したタイルがほとんどプレイヤーと接触していなければ無視する？
	// そのタイルの10
	float left = GetPosition().x;
	float right = GetPosition().x + GetSize().x;
	float top = GetPosition().y;
	float bottom = GetPosition().y + GetSize().y;

	int tileX_L = static_cast<int>(left / 32);
	int tileX_R = static_cast<int>(right / 32);
	int tileY_T = static_cast<int>(top / 32);
	int tileY_B = static_cast<int>(bottom / 32);
	ColRes res;
	switch (direction)
	{
	case ColRes::TOP:
		for (int x = tileX_L; x <= tileX_R; x++)
		{
			if (tile.GetTileID(x, tileY_T) == Kaneda::TILE_WALL)
			{
				DirectX::XMFLOAT2 XMPos(x * 32, tileY_T * 32);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::BOTTOM)
				{
					m_colState.isOnCeling = true;
					return false;
				}
			}
		}
		m_colState.isOnCeling = false;
		break;
	case ColRes::BOTTOM:
		for (int x = tileX_L; x <= tileX_R; x++)
		{
			if (tile.GetTileID(x, tileY_B) == Kaneda::TILE_WALL)
			{
				DirectX::XMFLOAT2 XMPos(x * 32, tileY_B * 32);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::TOP)
				{
					m_colState.isOnGround = true;
					return false;
				}
			}
		}
		m_colState.isOnGround = false;
		break;
	case ColRes::LEFT:
		for (int y = tileY_T; y <= tileY_B; y++)
		{
			if (tile.GetTileID(tileX_L, y) == Kaneda::TILE_WALL)
			{
				DirectX::XMFLOAT2 XMPos(tileX_L * 32, y * 32);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::RIGHT)
				{
					m_colState.isOnWallLeft = true;
					return false;
				}
			}
		}
		m_colState.isOnWallLeft = false;
		break;
	case ColRes::RIGHT:
		for (int y = tileY_T; y <= tileY_B; y++)
		{
			if (tile.GetTileID(tileX_R, y) == Kaneda::TILE_WALL)
			{
				DirectX::XMFLOAT2 XMPos(tileX_R * 32, y * 32);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::LEFT)
				{
					m_colState.isOnWallRight = true;
					return false;
				}
			}
		}
		m_colState.isOnWallRight = false;
		break;
	}
	return true;
}