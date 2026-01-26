#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "MapRenderer.h"
#include "Player.h"
#include "GameObject.h"
#include "Enemy.h"
#include "EnemySword.h"
#include "EnemyShooter.h"
#include "EnemyShielder.h"
#include <DirectXMath.h>

#include "EffectManager.h"
class Stage1Scene : public Scene
{
private:
    TileMap* m_pTileMap;
    MapRenderer* m_pMapRenderer;
    Player* m_pPlayer;

    // キャラクターに関する変数
    Character** m_pCharaList = nullptr; // キャラクターリスト
    int m_currentCharaNum = 0;          // 現在のキャラクター数
    static constexpr int maxChara = 10; // 最大キャラ数

    ID3D11ShaderResourceView* m_pMapTex;
    ID3D11ShaderResourceView* m_pPlayerTex;
    ID3D11ShaderResourceView* m_pEnemyTex;

    ID3D11ShaderResourceView* m_pPlayerTexIdle;
    ID3D11ShaderResourceView* m_pPlayerTexWalk;
    ID3D11ShaderResourceView* m_pPlayerTexJump;
    ID3D11ShaderResourceView* m_pPlayerTexFall;
    ID3D11ShaderResourceView* m_pPlayerTexAttack;
    ID3D11ShaderResourceView* m_pPlayerTexAbilityA;
    ID3D11ShaderResourceView* m_pPlayerTexAbilityB;

    ID3D11ShaderResourceView* m_pPlayerTexDash;
    ID3D11ShaderResourceView* m_pPlayerTexDashStay;
    ID3D11ShaderResourceView* m_pPlayerTexDashEffect;

    ID3D11ShaderResourceView* m_pEnemySwordTexIdle;
    ID3D11ShaderResourceView* m_pEnemySwordTexWalk;
    ID3D11ShaderResourceView* m_pEnemySwordTexJump;

    ID3D11ShaderResourceView* m_pEnemyShooterTexIdle;
    ID3D11ShaderResourceView* m_pEnemyShooterTexWalk;
    ID3D11ShaderResourceView* m_pEnemyShooterTexJump;

    ID3D11ShaderResourceView* m_pEnemyShielderTexIdle;
    ID3D11ShaderResourceView* m_pEnemyShielderTexWalk;
    ID3D11ShaderResourceView* m_pEnemyShielderTexJump;

    // testetstest kesite iiyo
    // 背景用テクスチャ（手前・中・奥）
    ID3D11ShaderResourceView* m_pBGTexFront = nullptr; // 手前
    ID3D11ShaderResourceView* m_pBGTexMid = nullptr;   // 中
    ID3D11ShaderResourceView* m_pBGTexBack = nullptr;  // 奥

    // パララックス係数（手前→中→奥：手前が大きい）
    float m_BGParallaxU[3] = { 1.0f, 1.0f, 1.0f }; // 横追従度
	float m_BGParallaxV[3] = { 1.0f, 1.0f, 1.0f }; // 縦追従度

    // 画面サイズ（Camera と合わせる）
    int m_ScreenWidth = 1920;
    int m_ScreenHeight = 1080;

    bool m_IsFinished;
    Sound* m_pSound = nullptr;
    EffectManager* m_pEffectManager = nullptr;
public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
    void CameraSeting();

    // 背景描画（Stage1Scene.cpp の DrawBackground 実装を使う）
    void DrawBackground(DirectX::XMMATRIX viewProj);

    // リストに関係する
    void CreateList(int num);                   // リストを作成
    void ClearList(Character* list);            // リストの一部を解放
    void AllClearList(Character** list);        // リストを全て解放
    Character* AddList(State::CharaType e_name);   // リストにオブジェクトを追加

    // 当たり判定
    void TileCollision(int charaName);
};
