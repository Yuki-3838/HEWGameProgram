#include "Character.h"

Character::Character()
{
	
}

// 蠖薙◆繧雁愛螳壻ｽ懈・荳ｭ
bool Character::StageCol(const TileMap& tile, const ColRes direction)
{
	// 蜻ｨ蝗ｲ縺ｮ繧ｿ繧､繝ｫ繧呈爾邏｢
	float left = GetPosition().x;
	float right = GetPosition().x + GetSize().x;
	float top = GetPosition().y;
	float bottom = GetPosition().y + GetSize().y;

	int tileX_L = static_cast<int>(left / 64);
	int tileX_R = static_cast<int>(right / 64);
	int tileY_T = static_cast<int>(top / 64);
	int tileY_B = static_cast<int>(bottom / 64);
	ColRes res;
	// 蠑輔″貂｡縺輔ｌ縺滓婿蜷代↓蟇ｾ縺励※縲∝ｽ薙◆縺｣縺ｦ縺・ｌ縺ｰtrue 蠖薙◆縺｣縺ｦ縺・↑縺代ｌ縺ｰfalse繧定ｿ斐☆
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

	// 鬮伜ｺｦ縺ｫ髢｢縺吶ｋ蜃ｦ逅・
	switch (m_JumpState)
	{
		// 荳頑・蜃ｦ逅・
	case State::JumpState::RISE:
		//縲荳頑・縺励・ｼ托ｽ・★縺､荳頑・蜉騾溷ｺｦ繧抵ｼ第ｸ幃溘☆繧・
		m_Position.y -= m_Stats.m_AccelY;
		m_Stats.m_AccelY--;
		// 螟ｩ莠輔↓陦晉ｪ√＠縺溷ｴ蜷医∽ｸ矩剄縺ｫ遘ｻ陦後☆繧・
		if (StageCol(tile, ColRes::TOP))
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		// 荳頑・蜉騾溷ｺｦ縺鯉ｼ舌↓縺ｪ縺｣縺溷ｴ蜷医∽ｸ矩剄縺ｫ遘ｻ陦後☆繧・
		if (m_Stats.m_AccelY == 0)
		{
			m_JumpState = State::JumpState::DESC;
			m_Stats.m_AccelY = -1;
		}
		break;

		// 荳矩剄蜃ｦ逅・
	case State::JumpState::DESC:
		// 荳矩剄縺吶ｋ
		m_Position.y -= m_Stats.m_AccelY;
		// 譛螟ｧ荳矩剄蜉騾溷ｺｦ蜃ｺ縺ｪ縺・ｴ蜷医∽ｸ矩剄蜉騾溷ｺｦ繧抵ｼ大刈騾・
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

		// 關ｽ荳句・逅・
	case State::JumpState::FALLING:
		// 荳矩剄縺吶ｋ
		m_Position.y -= m_Stats.m_AccelY;
		// 譛螟ｧ荳矩剄蜉騾溷ｺｦ蜃ｺ縺ｪ縺・ｴ蜷医∽ｸ矩剄蜉騾溷ｺｦ繧抵ｼ大刈騾・
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

		// 騾壼ｸｸ譎ょ・逅・
	case State::JumpState::NONE:
		// 驥榊鴨繧剃ｸ弱∴縲∝慍髱｢縺ｫ逹蝨ｰ縺励※縺・↑縺代ｌ縺ｰ荳矩剄縺ｫ遘ｻ陦・
		m_Position.y += m_Stats.m_Gravity;
		if (!StageCol(tile, ColRes::BOTTOM))
		{
			m_JumpState = State::JumpState::FALLING;
			m_Stats.m_AccelY = 1;
		}
		m_Position.y -= m_Stats.m_Gravity;
		m_Stats.m_DefPosY = m_Position.y;
	}
}
