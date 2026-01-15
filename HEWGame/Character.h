#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "MapRenderer.h"
#include "TileMap.h"

namespace State
{
    struct collisionState       // trueであれば衝突
    {
        bool isOnGround;    // 設置判定
        bool isOnCeling;    // 天井判定
        bool isOnWallLeft;  // 壁判定左
        bool isOnWallRight; // 壁判定右
    };
}

class Character : public GameObject
{
protected:
    float m_Speed;      // 移動速度
    int   m_HP;         // 体力

    float gravity = 0.5f;
    float m_accelY = 0; // Y軸の加速度
    bool isGround = false;

    State::collisionState m_colState{ false,false,false,false };
    // その他、ジャンプ力や向きなどキャラクター共通の変数をここに追加

public:
    Character();
    virtual ~Character() {}

    virtual void UnInit() = 0;

    virtual void Move() = 0;
    virtual void Attack() = 0;
    virtual void Jump() = 0;

    virtual bool GetColState(const TileMap& tile,const ColRes direction) = 0;

    virtual void Update(TileMap& tile) = 0;

    // キャラクター共通の初期化（必要であれば）
    virtual void Init(ID3D11ShaderResourceView* pTexture) override 
    {
        GameObject::Init(pTexture);
    }
};