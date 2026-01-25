#pragma once
#include "GameObject.h"
#include "Collision.h"
#include "MapRenderer.h"
#include "TileMap.h"
#include "Input.h"

namespace State
{
    struct collisionState   // trueであれば衝突
    {
        bool isOnGround;    // 設置判定
        bool isOnCeling;    // 天井判定
        bool isOnWallLeft;  // 壁判定左
        bool isOnWallRight; // 壁判定右
    };

    struct AttackFrame
    {
        int TotalFrame;      //全体フレーム数
        int StartUpFrame;    //発生
        int ActiveFrame;     //持続
        int RecorveryFrame;  //後隙
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
        DESC,
        FALLING
    };

    enum class CharaType
    {
        t_Player = 0,
        t_Enemy = 1
    };

    enum class CharDir
    {
        LEFT,
        RIGHT
    };
}


struct Stats
{
    int m_HP;                // 体力
    float m_Speed;           // X移動速度
    float m_Gravity;         // 重力
    float m_JumpPw;          // ジャンプ力
    float m_AccelX = 0;      // X軸の加速度
    float m_AccelY = 0;      // Y軸の加速度
    float m_AccelYMax = 63;  // Y軸の最大加速度
    float m_AttackDamage;    // 攻撃用メンバ変数
    float m_DefPosY;         // ジャンプしていない時のY軸
};

class Character : public GameObject
{
protected:
    // 共通ステータス
    Stats m_Stats;

    // 衝突・状態
    State::collisionState m_colState{ false, false, false, false };
    State::MoveState      m_MoveState = State::MoveState::RIGHT;
    State::JumpState      m_JumpState = State::JumpState::NONE;
    State::CharaType      m_charaType = State::CharaType::t_Player;

    // 攻撃関連
    bool m_IsAttack = false;
    int  m_AttackFrame = 0;
    static const int AttackTotalFrame = 30;  // 攻撃アニメの総フレーム数
    static const int AttackHitStart = 1;     // 攻撃判定開始フレーム
    static const int AttackHitEnd = 30;      // 攻撃判定終了フレーム
    State::CharDir m_charDir = State::CharDir::RIGHT;

    // 生死
    bool m_IsDead = false;

public:
    Character();
    virtual ~Character() {}

    virtual void UnInit() = 0;

    void Move(const TileMap& tile);
    virtual void Attack(Character** charaList) = 0;
    virtual int TakeDamage() = 0;
    virtual void Jump() = 0;

    bool StageCol(const TileMap& tile, const ColRes direction);

    virtual void Update(const TileMap& tile, Character** charaList) = 0;

    State::CharaType GetCharaType() const { return m_charaType; }
    State::JumpState GetJumpState() const { return m_JumpState; }

    // ゲッター
    float GetAcceleY() const { return m_Stats.m_AccelY; }
    float GetDefPosY() const  { return m_Stats.m_DefPosY; }

    // キャラクター共通の初期化
    virtual void Init(ID3D11ShaderResourceView* pTexture) override
    {
        GameObject::Init(pTexture);
    }

    // 死亡しているかどうか取得
    bool IsDead() const { return m_IsDead; }
};
