#include "GameUI.h"
#include "GameData.h"

GameUI::GameUI(){}
GameUI::~GameUI(){}

void GameUI::Init(ID3D11Device* device, ResourceManager* resMgr)
{
    // 画像の読み込み（ファイル名は適宜用意してください）
    m_pNumberTex = resMgr->LoadTexture("asset/texture/numbers.png", device);
    m_pGaugeTex = resMgr->LoadTexture("asset/texture/UI_skill_meter.png", device);     // 緑や赤のバー
    m_pGaugeFrameTex = resMgr->LoadTexture("asset/texture/UI_skill.png", device);      // バーの枠
}

void GameUI::Uninit()
{

}

void GameUI::Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, int currentHP, int maxHP)
{
    //スコア表示
    DrawNumber(context, spriteRenderer, GameData::GetScore(), 10.0f, 10.0f, 1.0f);

    //タイム表示
    DrawNumber(context, spriteRenderer, static_cast<int>(GameData::GetTime()), 10.0f, 50.0f, 1.0f);

    //ダッシュゲージ
    float dasVal = GameData::GetSkill(SkillType::Dash);
    float dasValMax = GameData::GetMaxSkill(SkillType::Dash);

    //位置:(640,600)サイズ:幅100,高さ10
    DrawGauge(context, spriteRenderer, 640.0f, 600.0f, 100.0f, 10.0f, dasVal, dasValMax);

    //必殺技
    float ultVal = GameData::GetSkill(SkillType::Ult);
    float ultValMax = GameData::GetMaxSkill(SkillType::Ult);

    //位置:()
    DrawGauge(context, spriteRenderer, 10.0f, 600.0f, 300.0f, 10.0f, ultVal, ultValMax);
}

void GameUI::DrawGauge(ID3D11DeviceContext* context, SpriteRenderer* sprite, float x, float y, float w, float h, float current, float max)
{
    if(max<= 0.0f) max = 1.0f; //ゼロ除算防止
    float rate = current / max; //割合計算(0.0～1.0)

    //範囲制限
    if (rate < 0.0f) rate = 0.0f;
    if (rate > 1.0f) rate = 1.0f;
    //枠(背景)を描画
    //指定されたサイズ(w,h)でそのまま描画
    if (m_pGaugeFrameTex)
    {
        sprite->Draw(context, m_pGaugeFrameTex, x, y, w, h, DirectX::XMMatrixIdentity());
	}

    //2.中身(バー)を描画
    if (m_pGaugeTex)
    {
        //描画する幅を計算
		float drawW = w * rate;
		sprite->Draw(context, m_pGaugeTex, x, y, drawW, h, DirectX::XMMatrixIdentity());
    }
}

void GameUI::DrawNumber(ID3D11DeviceContext* context, SpriteRenderer* sprite, int number, float x, float y, float scale)
{
    if (!m_pNumberTex) return;

	// 数字を文字列に変換
	std::string numStr = std::to_string(number);

	// 各桁を描画
    for (size_t i = 0; numStr.length(); i++)
    {
		
		//文字"0"～"9"を数値0～9に変換
		int digit = numStr[i] - '0';

		//// テクスチャ上の切り取り位置を計算
  //      float uvX = digi;

		//// 描画位置を計算
		//float drawX = x + i * digitW;
		//float drawY = y;

		//// 描画
  //      sprite->Draw(
		//	context,
		//	m_pNumberTex,
		//	drawX,
		//	drawY,
		//	digitW,
		//	digitH,
		//	DirectX::XMMatrixIdentity(),
		//	srcX,
		//	srcY,
		//	32.0f,
		//	64.0f
		//);
	}   
}


