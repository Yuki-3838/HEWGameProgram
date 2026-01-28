#include "GameUI.h"
#include "GameData.h"

GameUI::GameUI() {}
GameUI::~GameUI() {}

void GameUI::Init(ID3D11Device* device, ResourceManager* resMgr)
{
    // 画像の読み込み
    m_pNumberTex = resMgr->LoadTexture("asset/texture/numbers.png", device);

    m_pSkillGaugeTex = resMgr->LoadTexture("asset/texture/UI_skill_meter.png", device);     // 緑や赤のバー
    m_pUltGaugeTex = resMgr->LoadTexture("asset/texture/UI_UI_special_meter.png", device);     // 緑や赤のバー
    m_pGaugeFrameTex = resMgr->LoadTexture("asset/texture/UI_skill_frame.png", device);      // バーの枠
    m_pGaugeBackGroundTex = resMgr->LoadTexture("asset/texture/UI_skill.png", device);      // バーの枠

    m_pCamera = new Camera(1920,1080);
    m_pCamera->SetPosition(0.0f, 0.0f);
}

void GameUI::Uninit()
{
    if (m_pCamera)
    {
        delete m_pCamera;
        m_pCamera = nullptr;
    }
}

void GameUI::Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer)
{
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    //枠(背景)を描画
//指定されたサイズ(w,h)でそのまま描画
    if (m_pGaugeBackGroundTex)
    {
        spriteRenderer->Draw(context, m_pGaugeBackGroundTex, 640, 600, 500, 500, viewProj);
    }

    //ダッシュゲージ
    float dashVal = GameData::GetSkill(SkillType::Dash);
    float dashValMax = GameData::GetMaxSkill(SkillType::Dash);

    DrawBar(context,spriteRenderer,m_pSkillGaugeTex,6400,600,500,500,dashVal,dashValMax);
    //位置:(640,600)サイズ:幅100,高さ10
    DrawGauge(context, spriteRenderer,m_pGaugeFrameTex, m_pGaugeBackGroundTex, 640.0f, 600.0f, 500.0f, 500.0f, dashVal, dashValMax);

        //スコア表示
    DrawNumber(context, spriteRenderer, GameData::GetScore(), 10.0f, 10.0f, 1.0f);
    //タイム表示
    DrawNumber(context, spriteRenderer, static_cast<int>(GameData::GetTime()), 10.0f, 50.0f, 500.0f);
    ////必殺技
    //float ultVal = GameData::GetSkill(SkillType::Ult);
    //float ultValMax = GameData::GetMaxSkill(SkillType::Ult);
}

void GameUI::DrawBar(ID3D11DeviceContext* context, SpriteRenderer* sprite, ID3D11ShaderResourceView* bodyTex, float x, float y, float w, float h, float current, float max)
{
    if (!bodyTex) return;
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    if (max <= 0.0f) max = 1.0f; //ゼロ除算防止
    float rate = current / max; //割合計算(0.0～1.0)

    //範囲制限
    if (rate < 0.0f) rate = 0.0f;
    if (rate > 1.0f) rate = 1.0f;
        //描画する幅を計算
        float drawW = w * rate;

        sprite->Draw(context, bodyTex, x, y, drawW, h, viewProj);
}

void GameUI::DrawGauge(ID3D11DeviceContext* context, SpriteRenderer* sprite, ID3D11ShaderResourceView* frameTex, ID3D11ShaderResourceView* bodyTex, float x, float y, float w, float h, float current, float max)
{
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (bodyTex)
    {
        sprite->Draw(context, m_pGaugeBackGroundTex, x, y, w, h, viewProj);
    }
    if (frameTex)
    {
        sprite->Draw(context, m_pGaugeFrameTex, x - 7.9, y + 6.5, w, h, viewProj);
    }
}

void GameUI::DrawNumber(ID3D11DeviceContext* context, SpriteRenderer* sprite, int number, float x, float y, float scale)
{
    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();
    if (!m_pNumberTex) return;

    // 数字を文字列に変換
    std::string numStr = std::to_string(number);

    // 各桁を描画
    for (size_t i = 0; numStr.length(); i++)
    {

        //文字"0"～"9"を数値0～9に変換
        int digit = numStr[i] - '0';

        //// テクスチャ上の切り取り位置を計算
        float uvX = digit * NUMBER_W;

        //1文字分描画して、表示位置を右にずらす
        sprite->Draw(
            context,
            m_pNumberTex,
            x + i * NUMBER_W * scale,
            y, NUMBER_W * scale,
            NUMBER_H * scale,
            viewProj,
            uvX, 0.0f,
            NUMBER_W, NUMBER_H
        );
    }
}


