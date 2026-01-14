#pragma once
#include "GameObject.h"

class Character : public GameObject
{
protected:
    float m_Speed;      // 移動速度
    int   m_HP;         // 体力

    const float gravity;

    bool isGround = false;
    // その他、ジャンプ力や向きなどキャラクター共通の変数をここに追加

public:
    Character();
    virtual ~Character() {}

    virtual void UnInit() = 0;

    virtual void Move() = 0;
    virtual void Attack() = 0;
    virtual void Jump() = 0;

    // キャラクター共通の初期化（必要であれば）
    virtual void Init(ID3D11ShaderResourceView* pTexture) override 
    {
        GameObject::Init(pTexture);
    }
};