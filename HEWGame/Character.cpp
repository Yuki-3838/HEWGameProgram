#include "Character.h"

Character::Character()
{
	
}

// “–‚½‚è”»’èì¬’†
bool Character::StageCol(const TileMap& tile, const ColRes direction)
{
	// üˆÍ‚Ìƒ^ƒCƒ‹‚ğ’Tõ
	float left = GetPosition().x;
	float right = GetPosition().x + GetSize().x;
	float top = GetPosition().y;
	float bottom = GetPosition().y + GetSize().y;

	int tileX_L = static_cast<int>(left / 32);
	int tileX_R = static_cast<int>(right / 32);
	int tileY_T = static_cast<int>(top / 32);
	int tileY_B = static_cast<int>(bottom / 32);
	ColRes res;
	// ˆø‚«“n‚³‚ê‚½•ûŒü‚É‘Î‚µ‚ÄA“–‚½‚Á‚Ä‚¢‚ê‚Îtrue “–‚½‚Á‚Ä‚¢‚È‚¯‚ê‚Îfalse‚ğ•Ô‚·
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
				DirectX::XMFLOAT2 XMPos(x * 32, tileY_B * 32);
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
				DirectX::XMFLOAT2 XMPos(tileX_L * 32, y * 32);
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
				DirectX::XMFLOAT2 XMPos(tileX_R * 32, y * 32);
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