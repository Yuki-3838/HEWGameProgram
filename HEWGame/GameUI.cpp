#include "GameUI.h"
#include "GameDate.h"

GameUI::GameUI(){}
GameUI::~GameUI(){}

void GameUI::Init(ID3D11Device* device, ResourceManager* resMgr)
{
    // 画像の読み込み（ファイル名は適宜用意してください）
    m_pNumberTex = resMgr->LoadTexture("asset/texture/numbers.png", device);
    m_pGaugeTex = resMgr->LoadTexture("asset/texture/gauge_fill.png", device);     // 緑や赤のバー
    m_pGaugeFrameTex = resMgr->LoadTexture("asset/texture/gauge_frame.png", device); // バーの枠
}

void GameUI::Uninit()
{

}

void GameUI::Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, int currentHP, int maxHP)
{
    //スコア表示
    DrawNumber(context, spriteRenderer, GameDate::GetScore(), 10.0f, 10.0f, 1.0f);

    //タイム表示
    DrawNumber(context, spriteRenderer, static_cast<int>(GameDate::GetTime()), 10.0f, 50.0f, 1.0f);
}

void GameUI::DrawGauge(ID3D11DeviceContext* context, SpriteRenderer* sprite, float x, float y, float w, float h, float current, float max)
{
}

void GameUI::DrawNumber(ID3D11DeviceContext* context, SpriteRenderer* sprite, int number, float x, float y, float scale)
{
}


