#pragma once
#include "GameObject.h"
#include "Animator.h"
#include <DirectXMath.h>

class BackGround : public GameObject
{
private:
    Animator m_Animator;
    float m_ParallaxFactor = 0.0f; // カメラ移動に対する倍率（手前ほど大きく）
    float m_OffsetX = 0.0f;        // 横オフセット（ラップ描画用）
    bool  m_IsParallax = false;    // true: パララックスモード、false: アニメモード

public:
    //背景用
    // pTexture, 分割数, 横分割数, フレーム幅, フレーム高さ, フレーム時間
    void Init(ID3D11ShaderResourceView* pTexture, int count, int xCount, float w, float h, float speed);

    //パララックス背景用
    // width/heightは画面サイズ、parallaxFactor はパララックス係数
    void Init(ID3D11ShaderResourceView* pTexture, float width, float height, float parallaxFactor);

    // 描画
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pRenderer, DirectX::XMMATRIX viewProj) override;

    // 更新（アニメモード用）
    void Update();

    // 更新（パララックスモード用、カメラ位置を渡す）
    void Update(const DirectX::XMFLOAT2& cameraPos);
};  
