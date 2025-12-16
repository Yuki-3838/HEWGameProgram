#include "Player.h"

//=====================================
//初期化処理
//=====================================
void Player::Init(HWND hWnd)
{
	// ゲームループに入る前にDirectXの初期化をする
	RendererInit(hWnd);
	m_player.Init("asset/unnamed.jpg");//画像の設定
	DirectX::XMFLOAT3 pos = {0.0f, 0.0f, 0.0f};
	DirectX::XMFLOAT3 size = { 200.0f, 200.0f, 0.0f };
	m_player.SetPos(pos);//位置を設定
	m_player.SetSize(size);//大きさを指定
	m_player.SetAngle(0.0f);//角度を設定
	hp = 100;  //(仮決め)
	speed = 0.6f;  //移動スピード（仮決め）
	jumpPower = 8.0f;  //ジャンプ初速
	velocityY = 0.0f;  

	state = PState::STAY;  //プレイヤーの状態
	dir = PDirection::NONE;  //プレイヤーの方向

	isGround = true;  //地面と接触しているかどうか
	
}

//=====================================
//更新処理
//=====================================
void Player::Update()
{
	input.Update();
	DirectX::XMFLOAT3 pos = m_player.GetPos();  //位置情報の保存
	
	//---- 入力処理 ----
	bool moveRight;
	bool moveLeft;

	//---- 更新処理 ----
	if (input.GetKeyPress(VK_D))
	{
		state = PState::MOVE;  //状態を「MOVE」に変更
		dir = PDirection::RIGHT;  //方向を「RIGHT」に変更
	}
	else if (input.GetKeyPress(VK_A))
	{
		state = PState::MOVE;  //状態を「MOVE」に変更
		dir = PDirection::LEFT;  //方向を「LEFT」に変更
	}
	else
	{
		state = PState::STAY;  //方向を「STAY」に変更
		dir = PDirection::NONE;  //状態を「NONE」に変更
	}
	
	Move();
	
	//---- ジャンプ ----
	if (input.GetKeyTrigger(VK_SPACE) && isGround == true)
	{
		isGround = false;
		state = PState::JUMP;
		velocityY = jumpPower;  //上方向へ
	}
	
	Jump();

}

//=====================================
//描画処理
//=====================================
void Player::Draw()
{
	RendererDrawStart();//描画開始
	m_player.Draw();
	RendererDrawEnd();//描画終了
}

//=====================================
//終了処理
//=====================================
void Player::UnInit()
{
	m_player.Uninit();
	RendererUninit(); // DirectXを終了
}
//=====================================
//移動
//=====================================
void Player::Move()
{
	if (state != PState::MOVE)
	{
		return;
	}

	DirectX::XMFLOAT3 pos = m_player.GetPos();  //位置情報の保存

	//stateが「MOVE」かつdirがどちらかの方向に向いていれば...
	if (state == PState::MOVE)
	{
		if (dir == PDirection::RIGHT)
		{
			pos.x += speed;
		}
		if (dir == PDirection::LEFT)
		{
			pos.x -= speed;
		}
	}

	m_player.SetPos(pos);  //プレイヤーの座標を更新
	
}

void Player::Attack()
{

}

void Player::Blink()
{

}

void Player::Jump()
{
	DirectX::XMFLOAT3 pos = m_player.GetPos();
	
	//空中にいるときだけ重力がかかる
	if (isGround == false)
	{
		//速度を座標に反映
		pos.y += velocityY;  
		//重力で速度を減らす
		velocityY += gravity;
		if (isGround == true)
		{
			velocityY = 0.0f;
			state = PState::STAY;
		}
	
	}

	m_player.SetPos(pos);
	
}

void Player::WallJump()
{

}

void Player::GetBlink()
{

}
