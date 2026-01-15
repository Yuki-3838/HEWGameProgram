#pragma once
#include "Character.h"
#include"Input.h"

enum class PState
{
    STAY,                //止まっている
    MOVE,                //移動
    JUMP,                //ジャンプ
    ATTACK,                //攻撃
    STAN,                //気絶

    //NODAMAGE,            //無敵
    //WALL_GRAB,            //壁に
    //BRINK,                //ブリンク

};

enum PDirection
{
    NONE,                //動きなし
    RIGHT,                //右
    LEFT,                //左
};

class Player :public Character
{
private:

    //プレイヤーの状態
    PState state;
    PDirection dir;

    //ジャンプ関連
    bool isJump = false;  //ジャンプしているかどうか
    float jumpPower = 0.0f; //ジャンプ初速
    float velocityY = 0.0f;  //ジャンプ速度

    //入力
    //Input input;
    GameObject* m_player;


    bool isBlink = false;
    bool isHit = false;
    bool isWall = false;
    bool isWallJump = false;
    bool isDodge = false;


public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;



    // 毎フレームの更新処理（入力による移動など）
    void Update(TileMap& tile)override;
    bool GetColState(const TileMap& tile,const ColRes direction) override;

    void UnInit()override;

    void Move()override;
    void Jump()override;
    void Attack()override;

    void WallJump();
    void Blink();
    void GetBlink();

};
