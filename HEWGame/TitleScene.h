#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "VideoPlayer.h"

class TitleScene : public Scene
{
private:
    bool m_IsFinished;
    ID3D11ShaderResourceView* m_pTitleTex;
    BackGround* m_pBackground;
    VideoPlayer* m_pVideo;

    // スタートボタン
    ID3D11ShaderResourceView* m_pStartButtonTex;
    float m_StartButtonX;
    float m_StartButtonY;
    float m_StartButtonWidth;
    float m_StartButtonHeight;

    // 終了ボタン
    ID3D11ShaderResourceView* m_pExitButtonTex;
    float m_ExitButtonX;
    float m_ExitButtonY;
    float m_ExitButtonWidth;
    float m_ExitButtonHeight;

public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }

private:
    bool IsMouseOverButton(float x, float y, float width, float height);
};
