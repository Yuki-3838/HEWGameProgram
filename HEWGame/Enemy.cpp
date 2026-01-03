#include "Enemy.h"

void Enemy::Init(HWND hWnd)
{
    // ゲームループに入る前にDirectXの初期化をする
    RendererInit(hWnd);
    m_enemy.Init("asset/gu230.jpg");//画像の設定
    DirectX::XMFLOAT3 pos = { 200.0f, 0.0f, 0.0f };
    DirectX::XMFLOAT3 size = { 300.0f, 200.0f, 0.0f };
    m_enemy.SetPos(pos);//位置を設定
    m_enemy.SetSize(size);//大きさを指定
    m_enemy.SetAngle(0.0f);//角度を設定
    state = EState::LEFT;          // 初期状態：左向き
    hp = 1;                //HPを１に設定
    speed = 1.0f;          //スピード
    isGround = true;       //地面と接触しているかどうか
    isNoDamage = false;    //ダメージをくらってるかどうか
}



void Enemy::Update()
{
    DirectX::XMFLOAT3 pos = m_enemy.GetPos();  //位置情報の保存
}

void Enemy::Draw()
{
    //RendererDrawStart();//描画開始
    m_enemy.Draw();
    //RendererDrawEnd();//描画終了
}

void Enemy::UnInit()
{
    m_enemy.Uninit();
    //RendererUninit(); // DirectXを終了
}

void Enemy::Move()
{
    DirectX::XMFLOAT3 pos = m_enemy.GetPos();
    // 状態に応じて左右へ移動
    switch (state)
    {
    case EState::LEFT:
        //m_enemy.SetPos(pos.x -= speed, pos.y = 0, pos.z += 0.0f);   //位置情報を更新
        break;

    case EState::RIGHT:
        //m_enemy.SetPos(pos.x += speed, pos.y = 0, pos.z += 0.0f);   //位置情報を更新
        break;

    default:
        break;
    }

    // 画面端で反転（例0～100の間を往復）
    if (pos.x < 0)
    {
        state = EState:: RIGHT; // 右へ向き直す
    }
    else if (pos.x > 100)
    {
        state = EState::LEFT; // 左へ向き直す
    }
}

void Enemy::Attack()
{

}

void Enemy::Jump()
{
    DirectX::XMFLOAT3 pos = m_enemy.GetPos();
    // 地面にいるときだけジャンプ
    if (isGround)
    {
        // ジャンプの初速
      //  m_enemy.SetPos(pos.x = .0f, pos.y += 10.0f, pos.z += 0.0f);
        isGround = false;
        return;
    }

    //空中なら落下処理
    //m_enemy.SetPos(pos.x = 0.0f, pos.y -= gravity, pos.z += 0.0f);


    //地面の座標まで落ちると止まる
    if (pos.y >= 300)
    {
      //  m_enemy.SetPos(pos.x = 0.0f, pos.y = 300, pos.z += 0.0f);
        isGround = true;
    }
}