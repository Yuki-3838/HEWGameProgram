#include "Effect.h"

// Spawnの実装
void Effect::Init(ID3D11ShaderResourceView* tex, float x, float y, float scale, int frameCount, int divX, float texW, float texH, float speed, bool flipX, float angle) {
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

    //Animatorから「今どの部分を表示すべきか」をもらう
    AnimFrame frame = m_Animator.GetCurrentFrame();

    //SpriteRenderer::Draw に渡す
    spriteRenderer->Draw(
        context,
        m_pTexture,
        m_Position.x, m_Position.y, // 表示位置
        frame.w * m_Scale, frame.h * m_Scale, // 表示サイズ (コマサイズ × スケール)
        viewProj,
        frame.x, frame.y, // 切り抜き左上
        frame.w, frame.h,  // 切り抜きサイズ
        m_Angle,//回転
        m_FlipX//反転
    );
}