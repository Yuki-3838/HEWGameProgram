#include "GameUI.h"
#include "GameData.h"
#include <cmath>

GameUI::GameUI() {}
GameUI::~GameUI() {}

void GameUI::Init(ID3D11Device* device, ResourceManager* resMgr)
{
    // 画像の読み込み
    m_pTimeNumberTex = resMgr->LoadTexture("asset/texture/Number.png", device);
    //m_pScoreNumberTex = resMgr->LoadTexture("asset/texture/Numbers.png", device);

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
    //ダッシュゲージ
    float dashVal = GameData::GetSkill(SkillType::Dash);
    float dashValMax = GameData::GetMaxSkill(SkillType::Dash);
    //位置:(640,600)サイズ:幅100,高さ10
    DrawGauge(context, spriteRenderer,m_pGaugeFrameTex, m_pGaugeBackGroundTex, -60.0f, -170.0f, 500.0f, 500.0f, dashVal, dashValMax);

    DrawBar(context, spriteRenderer, m_pSkillGaugeTex, 15, -133, 395, 395, dashVal, dashValMax);
        //スコア表示
    /*DrawNumber(context, spriteRenderer, GameData::GetScore(), 10.0f, 10.0f, 1.0f);*/
    //タイム表示
    DrawNumber(context, spriteRenderer, static_cast<int>(GameData::GetTime()), 1600.0f, 0.0f, 0.5f);
    ////必殺技
    //float ultVal = GameData::GetSkill(SkillType::Ult);
    //float ultValMax = GameData::GetMaxSkill(SkillType::Ult);
}

void GameUI::GetTexSize(ID3D11ShaderResourceView* srv, float& outWidth, float& outHeight)
{
	outWidth = 0.0f;
	outHeight = 0.0f;
	if (!srv) return;

	ID3D11Resource* res = nullptr;
	srv->GetResource(&res);
	if (!res) return;

	ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(res); 

    if(tex)
    {
        D3D11_TEXTURE2D_DESC desc = {};
        tex->GetDesc(&desc);
        outWidth = static_cast<float>(desc.Width);
        outHeight = static_cast<float>(desc.Height);
	}
	res->Release();
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
 

    float texW = 0.0f, texH = 0.0f;
	GetTexSize(bodyTex, texW, texH);
    //描画する幅を計算
    float drawW = w * rate;
	float srcW = texW * rate;

        sprite->Draw(context, bodyTex, x, y, drawW, h, viewProj,0.0f,0.0f,srcW,texW);
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
    if (!m_pTimeNumberTex) return;

    // 数字を文字列に変換
    std::string numStr = std::to_string(number);

    // 各桁を描画
    for (size_t i = 0; i < numStr.length(); i++)
    {

        //文字"0"～"9"を数値0～9に変換
        int digit = numStr[i] - '0';

        //// テクスチャ上の切り取り位置を計算
        float uvX = digit * NUMBER_W;

        //1文字分描画して、表示位置を右にずらす
        sprite->Draw(
            context,
            m_pTimeNumberTex,
            x + i * NUMBER_W * scale,
            y, NUMBER_W * scale,
            NUMBER_H * scale,
            viewProj,
            uvX, 0.0f,
            NUMBER_W, NUMBER_H
        );
    }
}


