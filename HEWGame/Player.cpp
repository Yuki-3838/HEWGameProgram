#include "Player.h"
#include <Windows.h>

// プレイヤーのコンストラクタ
Player::Player()
{
    // プレイヤー固有の初期設定
    m_Speed = 5.0f;
    m_Size.x = 0.0f;
    m_Size.y =0.0f;
    m_Position.x = 100.0f;
    m_Position.y = 100.0f;
    m_HP = 1;         // 体力
    gravity = 1;
    isGround = false;
    state = PState::STAY;
    dir = RIGHT;

}

Player::~Player()
{
    if (m_player) delete m_player;
}


void Player::Update(TileMap& tile)
{
    // ステージとの当たり判定処理
    // 入力を受け取る　→　入力方向へタイルを探索し、移動できるかチェック　→　結果を処理
    // 移動入力処理
    if (GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if (GetColState(tile, ColRes::TOP))
        {
            //m_Position.y -= m_Speed;
        }
        else
        {
            //m_Position.y += 5;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if (GetColState(tile, ColRes::BOTTOM))
        {
            //m_Position.y += m_Speed;
        }
        else
        {
            ///m_Position.y -= 5;
        }
    }
    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if (GetColState(tile, ColRes::LEFT))
        {
            //m_Position.x -= m_Speed;
        }
        else
        {
           // m_Position.x += 1;
        }
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if (GetColState(tile, ColRes::RIGHT))
        {
            //m_Position.x += m_Speed;
        }
        else
        {
           // m_Position.x -= 1;
        }
    }
    // 落下処理
    // キャラクターが地面と接触しているかジャンプ中か確認し、していなければ重力を与える
    //if (!m_colState.isOnGround)
    {
        //m_Position.y += gravity;
    }
}


void Player::UnInit()
{
}

void Player::Move()
{
}

void Player::Jump()
{
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
bool Player::GetColState(const TileMap& tile, const ColRes direction)
{

    // 周囲のタイルを探索　→　タイルのIDを特定　→　壁であれば当たり判定チェック
    // COl 対象の座標同士の当たり判定位置を確認する

    // 周囲のタイル このままだと＋の方向は余分にとっている、また、少ししかかぶっていないところも取っている
    // 探索したタイルがほとんどプレイヤーと接触していなければ無視する？
    // そのタイルの10
    switch (direction)
    {
    case ColRes::TOP:
        m_Position.y -= m_Speed;
        break;
    case ColRes::BOTTOM:
        m_Position.y += m_Speed;
        break;
    case ColRes::LEFT:
        m_Position.x -= m_Speed;
        break;
    case ColRes::RIGHT:
        m_Position.x += m_Speed;
        break;

    }
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
                    m_Position.y += m_Speed;
                    return false;
                }
            }
        }
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
                        m_Position.y -= m_Speed;
                        return false;
                    }
                }
            }
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
                    m_Position.x += m_Speed;
                    return false;
                }
            }
        }
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
                    m_Position.x -= m_Speed;
                    return false;
                }
            }
        }
        break;
    }

    return true;
}