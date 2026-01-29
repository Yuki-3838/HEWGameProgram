#pragma once
#include "Scene.h"
#include "BackGround.h"
#include "VideoPlayer.h"

// ボタン構造体
struct Button {
    float x, y;         // 位置（中心座標）
    float width, height; // サイズ
    bool isHovered;     // マウスが乗っているか
};

class TitleScene :public Scene
{
private:
    bool m_IsFinished;
    bool m_IsExitRequested; // 終了リクエスト
    // タイトル画面用のテクスチャポインタ
    ID3D11ShaderResourceView* m_pTitleTex;
    ID3D11ShaderResourceView* m_pStartButtonTex;  // スタートボタン
    ID3D11ShaderResourceView* m_pExitButtonTex;   // 終了ボタン
    BackGround* m_pBackground;

    // 動画プレイヤー
    VideoPlayer* m_pVideoPlayer;

    // ボタン
    Button m_startButton;
    Button m_exitButton;

    // ボタンの当たり判定
    bool IsMouseOverButton(const Button& btn, float mouseX, float mouseY);
public:
    using Scene::Scene;
    void Init() override;
    void Update() override; // マウスでボタンをクリックしたらフラグを立てる
    void Draw() override;   // タイトル画像とボタンを描画
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
    bool ShouldExitGame() const { return m_IsExitRequested; } // 終了判定
};
