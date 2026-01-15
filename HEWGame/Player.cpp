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
    GetColState(tile);
    // 移動入力処理
    if (!m_colState.isOnWallLeft)
    {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_Position.x -= m_Speed;
    }
    if (!m_colState.isOnWallRight)
    {
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_Position.x += m_Speed;
    }
    if (!m_colState.isOnCeling)
    {
        if (GetAsyncKeyState(VK_UP) & 0x8000) m_Position.y -= m_Speed;
    }
    if (!m_colState.isOnGround)
    {
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_Position.y += m_Speed;
    }
    
    

    // 落下処理
    // キャラクターが地面と接触しているかジャンプ中か確認し、していなければ重力を与える
    if (!m_colState.isOnGround)
    {
        m_Position.y += gravity;
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
void Player::GetColState(const TileMap& tile)
{

    // 周囲のタイルを探索　→　タイルのIDを特定　→　壁であれば当たり判定チェック
    // COl 対象の座標同士の当たり判定位置を確認する

    // 周囲のタイル
    float left = GetPosition().x;
    float right = GetPosition().x + GetSize().x;
    float top = GetPosition().y;
    float bottom = (GetPosition().y + GetSize().y);

    int tileX_L = static_cast<int>(left / 32);
    int tileX_R = static_cast<int>(right / 32);
    int tileY_T = static_cast<int>(top / 32);
    int tileY_B = static_cast<int>(bottom / 32);

    // タイルＩＤを特定
    // bottom top を特定
    bool changeGround = false;
    bool changeCeling = false;
    bool changeWallLeft = false;
    bool changeWallRight = false;

    float tileX_Ll = (tileX_L + tileX_R) / 2 - ((tileX_L + tileX_R) / 2 / 2);
    float tileX_Rr = (tileX_L + tileX_R) / 2 + ((tileX_L + tileX_R) / 2 / 2);
    for (int x = tileX_Ll; x <= tileX_Rr; x++)
    {
        if (tile.GetTileID(x, tileY_B) == Kaneda::TILE_WALL && !changeGround)
        {
            m_colState.isOnGround = true;
            changeGround = true;
        }
        if (tile.GetTileID(x, tileY_T) == Kaneda::TILE_WALL && !changeCeling)
        {
            m_colState.isOnCeling = true;
            changeCeling = true;
        }

    }
    // left right を特定
    // ぴったりの判定を取らないため、探索範囲のtop 25% bottom 75%にしぼる　
    float tileY_Bb = (tileY_B + tileY_T) / 2 + (tileY_B + tileY_T) / 2 / 2;   // 本来の75%
    float tileY_Tt = (tileY_B + tileY_T) / 2 - (tileY_B + tileY_T) / 2 / 2;       // 本来の25%
    // 0 100 = 25 75 = 100 / 4 = x x,x * 3
    // 20 40 = 10 30
   
    for (int y = tileY_Tt; y <= tileY_Bb; y++)
    {
        if (tile.GetTileID(tileX_L, y) == Kaneda::TILE_WALL && !changeWallLeft)
        {
            m_colState.isOnWallLeft = true;
            changeWallLeft = true;
        }
        if (tile.GetTileID(tileX_R, y) == Kaneda::TILE_WALL && !changeWallRight)
        {
            m_colState.isOnWallRight = true;
            changeWallRight = true;
        }
    }

    // 一度も当たらなければfalse
    if (!changeGround)
    {
        m_colState.isOnGround = false;
    }
    if (!changeCeling)
    {
        m_colState.isOnCeling = false;
    }
    if (!changeWallLeft)
    {
        m_colState.isOnWallLeft = false;
    }
    if (!changeWallRight)
    {
        m_colState.isOnWallRight = false;
    }
}