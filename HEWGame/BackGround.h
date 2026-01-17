#pragma once
#include "GameObject.h"
#include "Animator.h"
class BackGround : public GameObject
{
private:
    Animator m_Animator;
public:
    void Init(ID3D11ShaderResourceView* pTexture, int count, int xCount, float w, float h, float speed);
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pRenderer, DirectX::XMMATRIX viewProj) override;
    void Update();
};