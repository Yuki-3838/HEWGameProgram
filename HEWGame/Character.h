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
    enum class MoveState
    {
        NONE,
        LEFT,   // 左
        RIGHT,  // 右
        TOP,
        BOTTOM
    };
    enum class JumpState
    {
        NONE,
        RISE,
        DESC
    };
}
struct Stats
{
    int m_HP;           // 体力
    float m_Speed;      // X移動速度
    float m_Gravity;    // 重力
    float m_JumpPw;     // ジャンプ力
    float m_AccelX = 0;     // X軸の加速度
    float m_AccelY = 0;     // Y軸の加速度
};

class Character : public GameObject
{
protected:
    Stats m_Stats;
    State::collisionState m_colState{ false,false,false,false };
    State::MoveState m_MoveState = State::MoveState::NONE;
    State::JumpState m_JumpState = State::JumpState::NONE;

public:
    Character();
    virtual ~Character() {}

    virtual void UnInit() = 0;

    virtual void Move(const TileMap& tile) = 0;
    virtual void Attack() = 0;
    virtual void Jump() = 0;

    virtual bool StageCol(const TileMap& tile,const ColRes direction) = 0;

    virtual void Update(TileMap& tile) = 0;

    // キャラクター共通の初期化（必要であれば）
    virtual void Init(ID3D11ShaderResourceView* pTexture) override 
    {
        GameObject::Init(pTexture);
    }
};