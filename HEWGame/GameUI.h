#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include <string>
class GameUI
{
private:
	ID3D11ShaderResourceView* m_pNumberTex = nullptr;
	ID3D11ShaderResourceView* m_pGaugeTex = nullptr;
	ID3D11ShaderResourceView* m_pGaugeFrameTex = nullptr;

	//数字のサイズの設定
	const float NUMBER_W = 32.0f;
	const float NUMBER_H = 32.0f;

public:
	GameUI();
	~GameUI();

	void Init(ID3D11Device* device, ResourceManager* resMgr);
	void Uninit();
	//まとめて全部描画する関数
	void Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer, int currentHP, int maxHP);

	//個別に描画したいときの関数
	void DrawGauge(ID3D11DeviceContext* context, SpriteRenderer* sprite, float x, float y, float w, float h, float current, float max);
	void DrawNumber(ID3D11DeviceContext* context, SpriteRenderer* sprite, int number, float x, float y, float scale = 1.0f);
};
