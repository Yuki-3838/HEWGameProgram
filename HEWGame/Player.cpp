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

	state = PlayerState::STAY;  //プレイヤーの状態
	dir = PlayerDirection::NONE;  //プレイヤーの方向

	isGround = true;  //地面と接触しているかどうか
	
}

//=====================================
//更新処理
//=====================================
void Player::Update()
{
	DirectX::XMFLOAT3 pos = m_player.GetPos();  //位置情報の保存
	
	//---- 入力処理 ----
	bool moveRight;
	bool moveLeft;

	//---- 更新処理 ----
	//if (input.GetKeyPress(VK_D))
	//{
		pos.x += 0.1f;
		m_player.SetPos(pos);
		state = PlayerState::MOVE;  //状態を「MOVE」に変更
		dir = PlayerDirection::RIGHT;  //方向を「RIGHT」に変更
	//}
	//else if (input.GetKeyPress(VK_A))
	//{
	//	pos.x += 0.1f;
	//	m_player.SetPos(pos);
	//	state = PlayerState::MOVE;  //状態を「MOVE」に変更
	//	dir = PlayerDirection::LEFT;  //方向を「LEFT」に変更
	//}
	//else
	//{
	//	dir = PlayerDirection::NONE;  //状態を「NONE」に変更
	//	state = PlayerState::STAY;  //方向を「STAY」に変更
	//}
	
	//---- ジャンプ ----
	if (input.GetKeyTrigger(VK_SPACE) && isGround == true)
	{
		isGround = false;
		state = JUMP;
	}
	
	
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
	DirectX::XMFLOAT3 pos = m_player.GetPos();  //位置情報の保存

	if (state != MOVE)
	{
		return;
	}

	//stateが「MOVE」かつdirがどちらかの方向に向いていれば...
	if (state == PlayerState::MOVE)
	{
		if (dir == PlayerDirection::RIGHT)
		{
			pos.x += speed;
		}
		if (dir == PlayerDirection::LEFT)
		{
			pos.x -= speed;
		}
	}

	//m_player.SetPos(pos.x, pos.y, pos.z);  //プレイヤーの座標を更新
	
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
		velocityY += gravity;
		pos.y += velocityY;
	}

	if (isGround == true)
	{
		state = STAY;
		velocityY = jumpPower;
	}
	
}

void Player::WallJump()
{

}

void Player::GetBlink()
{

}
