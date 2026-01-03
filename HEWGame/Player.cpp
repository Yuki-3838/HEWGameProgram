#include "Player.h"
#include <Windows.h>

// プレイヤーのコンストラクタ
Player::Player()
{
    // プレイヤー固有の初期設定
    m_Speed = 5.0f;
    m_Size.x = 48.0f;
    m_Size.y = 48.0f;
    m_Position.x = 100.0f;
    m_Position.y = 100.0f;
}

Player::~Player()
{

}

void Player::Init(ID3D11ShaderResourceView* pTexture)
{
    // 親クラスのInitを呼んで画像をセット
    GameObject::Init(pTexture);
}

void Player::Update()
{
    // 移動入力処理
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_Position.x -= m_Speed;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_Position.x += m_Speed;
    if (GetAsyncKeyState(VK_UP) & 0x8000) m_Position.y -= m_Speed;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_Position.y += m_Speed;
}