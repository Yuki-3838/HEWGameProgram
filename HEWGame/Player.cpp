#include "Player.h"
#include <Windows.h>

// プレイヤーのコンストラクタ
Player::Player()
{
    // プレイヤー固有の初期設定
    m_Speed = 5.0f;
    m_Size.x = 90.0f;
    m_Size.y = 90.0f;
    m_Position.x = 100.0f;
    m_Position.y = 100.0f;
    m_Speed = 5.0f;      // 移動速度
    m_HP = 1;         // 体力
    gravity = 0.5;
    isGround = false;
    state = PState::STAY;
    dir = RIGHT;
}

Player::~Player()
{

}


void Player::Update()
{
    // 移動入力処理
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_Position.x -= m_Speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_Position.x += m_Speed;
    if (GetAsyncKeyState(VK_UP) & 0x8000) m_Position.y -= m_Speed;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_Position.y += m_Speed;
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