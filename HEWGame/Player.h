#pragma once
#include"Character.h"
#include"Input.h"

enum PlayerState
{
	STAY,				//止まっている
	MOVE,				//移動
	JUMP,				//ジャンプ
	ATTACK,				//攻撃
	STAN,				//気絶

	//NODAMAGE,			//無敵
	//WALL_GRAB,			//壁に
	//BRINK,				//ブリンク

};

enum PlayerDirection
{
	NONE,				//動きなし
	RIGHT,				//右
	LEFT,				//左
};

class Player : public Character
{
private:
	
	//プレイヤーの状態
	PlayerState state = STAY;
	PlayerDirection dir = NONE;

	//ジャンプ関連
	bool isJump = false;  //ジャンプしているかどうか
	float jumpPower = 8.0f; //ジャンプ初速
	float velocityY = 0.0f;  //ジャンプ速度
	
	//入力
	Input input;
	Object m_player;


	bool isBlink = false;
	bool isHit = false;
	bool isWall = false;
	bool isWallJump = false;
	bool isDodge = false;

public:

	void Init(HWND hWnd)override;
	void Update()override;
	void Draw()override;
	void UnInit()override;

	void Move()override;
	void Jump()override;
	void Attack()override;

	void WallJump();
	void Blink();
	void GetBlink();
};
