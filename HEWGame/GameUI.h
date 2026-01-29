#pragma once
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include <string>
#include "Camera.h"
class GameUI
{
private:
	ID3D11ShaderResourceView* m_pTimeNumberTex = nullptr;
	ID3D11ShaderResourceView* m_pScoreNumberTex = nullptr;

	ID3D11ShaderResourceView* m_pSkillGaugeTex = nullptr;
	ID3D11ShaderResourceView* m_pUltGaugeTex = nullptr;

	ID3D11ShaderResourceView* m_pGaugeFrameTex = nullptr;
	ID3D11ShaderResourceView* m_pGaugeBackGroundTex = nullptr;

	//カメラ設定
	Camera* m_pCamera = nullptr;
	//数字のサイズの設定
	const float NUMBER_W = 320.0f;
	const float NUMBER_H = 320.0f;

public:
	GameUI();
	~GameUI();

	void Init(ID3D11Device* device, ResourceManager* resMgr);
	void Uninit();
	//まとめて全部描画する関数
	void Draw(ID3D11DeviceContext* context, SpriteRenderer* spriteRenderer);

	static void GetTexSize(ID3D11ShaderResourceView* srv, float& outWidth, float& outHeight);
	//個別に描画したいときの関数
	void DrawBar(ID3D11DeviceContext* context, SpriteRenderer* sprite, ID3D11ShaderResourceView* bodyTex,float x, float y, float w, float h, float current, float max);
	void DrawGauge(ID3D11DeviceContext* context, SpriteRenderer* sprite, ID3D11ShaderResourceView* framTex, ID3D11ShaderResourceView* bodyTex, float x, float y, float w, float h, float current, float max);
	void DrawNumber(ID3D11DeviceContext* context, SpriteRenderer* sprite, int number, float x, float y, float scale = 1.0f);
};
