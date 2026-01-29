#pragma once
#include "Scene.h"
#include "GameUI.h"

class ResultScene : public Scene 
{
private:
    bool m_IsFinished = false;
    // タイトル画面用のテクスチャポインタを追加
    ID3D11ShaderResourceView* m_pResultTex;

    GameUI* m_pGameUI = nullptr;
public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
};
