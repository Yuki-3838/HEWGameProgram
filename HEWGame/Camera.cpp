#include "Camera.h"

using namespace DirectX;

// コンストラクタ
Camera::Camera(int screenWidth, int screenHeight)
{
    m_Position.x = 0.0f;
    m_Position.y = 0.0f;

    // 2Dゲーム用の平行投影行列（画面左上を0,0、右下をscreenWidth, screenHeightにする）
    m_ProjMatrix = XMMatrixOrthographicOffCenterLH(
        0.0f,
        static_cast<float>(screenWidth),
        static_cast<float>(screenHeight),
        0.0f,
        0.0f,
        1.0f
    );

    // ビュー行列を初期位置で更新
    SetPosition(0.0f, 0.0f);
}

void Camera::SetPosition(float x, float y)
{
    m_Position.x = x;
    m_Position.y = y;

    // カメラの座標に基づいてビュー行列を作成
    // 2Dでは単純に世界の座標をカメラの逆方向にずらす処理になる
    m_ViewMatrix = XMMatrixTranslation(-m_Position.x, -m_Position.y, 0.0f);
}

void Camera::Move(float dx, float dy)
{
    SetPosition(m_Position.x + dx, m_Position.y + dy);
}

XMMATRIX Camera::GetViewProjection() const
{
    // View行列 * Projection行列を返却
    return m_ViewMatrix * m_ProjMatrix;
}