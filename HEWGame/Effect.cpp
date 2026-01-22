#include "Effect.h"

//Initの実装
void Effect::Init(ID3D11ShaderResourceView* tex, float x, float y, float scale, int frameCount, int divX, float texW, float texH, float speed, bool flipX, float angle,float startTexX, float startTexY)
{
    m_pTexture = tex;
    m_Position = { x, y };
    m_Scale = scale;
    m_Active = true;
    m_LifeTimer = frameCount * speed;

    m_FlipX = flipX;
    m_Angle = angle;
    //Animatorの初期化
    // 1コマの幅と高さを計算
    float cellW = texW / divX;
    float cellH = texH / (frameCount / divX + (frameCount % divX > 0 ? 1 : 0)); // 簡易的な行数計算
    m_StartTexX = startTexX;
    m_StartTexY = startTexY;
    // Animator::Init(総コマ数, 横列数, 1コマW, 1コマH, 1コマの時間)
    m_Animator.Init(frameCount, divX, cellW, cellH, speed);
}


void Effect::Update(float deltaTime)
{
    if (!m_Active) return;

    //Animatorの更新
    m_Animator.Update(deltaTime);

    m_LifeTimer -= deltaTime;
    if (m_LifeTimer <= 0.0f)
    {
        m_Active = false;
    }
}

void Effect::Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, DirectX::XMMATRIX viewProj)
{
    if (!m_Active || !m_pTexture) return;

    // 1. Animatorから「今のコマの切り取り範囲」をもらう（これがユーザーさんの気に入っている機能）
    AnimFrame frame = m_Animator.GetCurrentFrame();

    // 2. 描画するサイズを計算（元のサイズ × スケール）
    float drawW = frame.w * m_Scale;
    float drawH = frame.h * m_Scale;

    // ★重要！座標を中心基準に補正する
    // そのままだと (x, y) が「左上」になってしまうので、
    // サイズの半分を引くことで (x, y) を「画像のど真ん中」に合わせます。
    float drawX = m_Position.x - (drawW * 0.5f);
    float drawY = m_Position.y - (drawH * 0.5f);

    // 3. SpriteRendererで描画
    spriteRenderer->Draw(
        context,
        m_pTexture,
        drawX, drawY,   // ★補正した「中心基準」の座標
        drawW, drawH,   // 描画サイズ
        viewProj,
        frame.x + m_StartTexX,
        frame.y + m_StartTexY,
        frame.w, frame.h, // 切り取りサイズ
        m_Angle,          // ★回転角度を渡す（ここが抜けていました！）
        m_FlipX           // ★反転フラグを渡す（ここも抜けていました！）
    );
}