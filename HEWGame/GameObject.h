#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteRenderer.h"

class GameObject 
{
protected:
    DirectX::XMFLOAT2 m_Position; // 座標
    DirectX::XMFLOAT2 m_Size;     // 表示サイズ
    ID3D11ShaderResourceView* m_pTexture; // テクスチャへのポインタ

public:
    GameObject();
    virtual ~GameObject();

    // 基本的な初期化
    virtual void Init(ID3D11ShaderResourceView* pTexture);

    // 更新処理（子クラスで中身を書く）
    //virtual void Update() = 0;

    // 共通の描画処理
    virtual void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj);

    // 座標の設定・取得
    void SetPosition(float x, float y) { m_Position = { x, y }; }
    void SetSize(float w, float h) { m_Size.x = w; m_Size.y = h; }
    DirectX::XMFLOAT2 GetPosition() const { return m_Position; }
    DirectX::XMFLOAT2 GetSize() const { return m_Size; }
};