#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "SpriteRenderer.h"
#include "Animator.h"

enum EffectType
{
	Explosion,
    Smoke,
    Dash,
	// 必要に応じて他のエフェクトタイプを追加
};
class Effect
{
private:
    bool m_Active = false;
    DirectX::XMFLOAT2 m_Position;
    float m_Scale;

    //アニメーション管理用
    Animator m_Animator;

    ID3D11ShaderResourceView* m_pTexture = nullptr;

    float m_LifeTimer = 0.0f;
    bool m_FlipX = false;
    float m_Angle = 0.0f;

public:
    Effect() {}
    ~Effect() {}

    // Initでアニメーション情報（コマ数や分割数）も受け取る
    // frameCount: 総コマ数// divX: 横の分割数// texW, texH: テクスチャ全体のサイズ
    void Init(ID3D11ShaderResourceView* tex, float x, float y, float scale,
        int frameCount, int divX, float texW, float texH, float speed, bool flipX, float angle = 0.0f);

    void Update(float deltaTime); // deltaTime (秒) を受け取るようにする
    void Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, DirectX::XMMATRIX viewProj);

    bool IsActive() const { return m_Active; }
};