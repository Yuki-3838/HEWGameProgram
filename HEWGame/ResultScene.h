#pragma once
#include "Scene.h"

class ResultScene : public Scene
{
private:
    bool m_IsFinished = false;
    ID3D11ShaderResourceView* m_pResultTex;

    // タイトルに戻るボタン
    ID3D11ShaderResourceView* m_pTitleButtonTex;
    float m_TitleButtonX;
    float m_TitleButtonY;
    float m_TitleButtonWidth;
    float m_TitleButtonHeight;

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
