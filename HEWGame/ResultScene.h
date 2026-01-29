#pragma once
#include "Scene.h"

// ボタン構造体
struct ResultButton {
    float x, y;         // 位置（中心座標）
    float width, height; // サイズ
    bool isHovered;     // マウスが乗っているか
};

class ResultScene : public Scene
{
private:
    bool m_IsFinished = false;
    bool m_IsExitRequested = false; // 終了リクエスト
    // リザルト画面用のテクスチャポインタ
    ID3D11ShaderResourceView* m_pResultTex;
    ID3D11ShaderResourceView* m_pTitleButtonTex;  // タイトルに戻るボタン
    ID3D11ShaderResourceView* m_pExitButtonTex;   // 終了ボタン

    // ボタン
    ResultButton m_titleButton;
    ResultButton m_exitButton;

    // ボタンの当たり判定
    bool IsMouseOverButton(const ResultButton& btn, float mouseX, float mouseY);
public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
    bool ShouldExitGame() const { return m_IsExitRequested; } // 終了判定
};
