#include "Character.h"

Character::Character()
{
	
}

// 当たり判定作成中
bool Character::StageCol(const TileMap& tile, const ColRes direction)
{
	// 周囲のタイルを探索
	float left = GetPosition().x;
	float right = GetPosition().x + GetSize().x;
	float top = GetPosition().y;
	float bottom = GetPosition().y + GetSize().y;

	int tileX_L = static_cast<int>(left / 64);
	int tileX_R = static_cast<int>(right / 64);
	int tileY_T = static_cast<int>(top / 64);
	int tileY_B = static_cast<int>(bottom / 64);
	ColRes res;
	// 引き渡された方向に対して、当たっていればtrue 当たっていなければfalseを返す
	switch (direction)
	{
	case ColRes::TOP:
		for (int x = tileX_L; x <= tileX_R; x++)
		{
			if (tile.GetTileID(x, tileY_T) == Kaneda::TILE_WALL)
			{
				DirectX::XMFLOAT2 XMPos(x * 64, tileY_T * 64);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::BOTTOM)
				{
					m_colState.isOnCeling = true;
					return true;
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
				DirectX::XMFLOAT2 XMPos(x * 64, tileY_B * 64);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::TOP)
				{
					m_colState.isOnGround = true;
					return true;
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
				DirectX::XMFLOAT2 XMPos(tileX_L * 64, y * 64);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::RIGHT)
				{
					m_colState.isOnWallLeft = true;
					return true;
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
				DirectX::XMFLOAT2 XMPos(tileX_R * 64, y * 64);
				DirectX::XMFLOAT2 XMSize(tile.GetTileSize(), tile.GetTileSize());
				res = CollisionRect(*this, XMPos, XMSize);
				if (res & ColRes::LEFT)
				{
					m_colState.isOnWallRight = true;
					return true;
				}
			}
		}
		m_colState.isOnWallRight = false;
		break;
	}
	return false;
}

void Character::Move(const TileMap& tile)
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

		// 落下処理
	case State::JumpState::FALLING:
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
			m_JumpState = State::JumpState::FALLING;
			m_Stats.m_AccelY++;
		}
		m_Position.y -= m_Stats.m_Gravity;
		m_Stats.m_DefPosY = m_Position.y;
	}
}