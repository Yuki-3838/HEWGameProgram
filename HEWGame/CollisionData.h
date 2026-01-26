#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteRenderer.h"

// GameObjectを継承しない独立したクラス
class CollisionData
{
private:
    DirectX::XMFLOAT2 m_Position; // 座標
    DirectX::XMFLOAT2 m_Size;     // サイズ
    ID3D11ShaderResourceView* m_pTexture; // デバッグ表示用画像

public:
    CollisionData();
    ~CollisionData();

    // 初期化：画像、座標(x,y)、サイズ(w,h)を受け取る
    void Init(ID3D11ShaderResourceView* pTexture, float x, float y, float width, float height);

    // 親（プレイヤー）の位置に合わせて移動
    void UpdatePos(float parentX, float parentY, float offsetX, float offsetY);

    // 描画：SpriteRendererを使って自分で描画する
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj);

    // 当たり判定関数(CollisionRect)に渡すためのゲッター
    DirectX::XMFLOAT2 GetPosition() const { return m_Position; }
    DirectX::XMFLOAT2 GetSize() const { return m_Size; }
};
