#pragma once
#include "GameObject.h"

class Character : public GameObject
{
protected:
    float m_Speed;      // 移動速度
    int   m_HP;         // 体力
    // その他、ジャンプ力や向きなどキャラクター共通の変数をここに追加

public:
    Character();
    virtual ~Character() {}

    // キャラクター共通の初期化（必要であれば）
    virtual void Init(ID3D11ShaderResourceView* pTexture) override 
    {
        GameObject::Init(pTexture);
    }
};