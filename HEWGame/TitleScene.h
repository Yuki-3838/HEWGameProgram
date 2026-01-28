#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "VideoPlayer.h"
class TitleScene :public Scene
{
private:
    bool m_IsFinished;
    // タイトル画面用のテクスチャポインタを追加
    ID3D11ShaderResourceView* m_pTitleTex;
    BackGround* m_pBackground;
	VideoPlayer* m_pVideo;
public:
    using Scene::Scene;
    void Init() override;
    void Update() override; // スペースキーが押されたらフラグを立てる
    void Draw() override;   // タイトル画像を描画
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
};

