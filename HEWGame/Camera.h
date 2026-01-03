#pragma once
#include <DirectXMath.h>

class Camera {
private:
    DirectX::XMFLOAT2 m_Position;    // カメラの中心座標
    DirectX::XMMATRIX m_ViewMatrix;  // ビュー行列
    DirectX::XMMATRIX m_ProjMatrix;  // 射影行列（画面サイズ）

public:
    Camera(int screenWidth, int screenHeight);

    // カメラの座標更新
    void SetPosition(float x, float y);
    void Move(float dx, float dy);

    // Renderer等に渡す最終的な行列 (View * Projection) を取得
    DirectX::XMMATRIX GetViewProjection() const;

    // Getter
    float GetX() const { return m_Position.x; }
    float GetY() const { return m_Position.y; }
};