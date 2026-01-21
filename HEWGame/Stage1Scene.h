#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "MapRenderer.h"
#include "Player.h"
#include "GameObject.h"
#include "Enemy.h"

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

    // testetstest kesite iiyo

    bool m_IsFinished;
    Sound* m_pSound = nullptr;
public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
    void CameraSeting();

    // リストに関係する
    void CreateList(int num);                   // リストを作成
    void ClearList(Character* list);            // リストの一部を解放
    void AllClearList(Character** list);        // リストを全て解放
    Character* AddList(State::CharaType e_name);   // リストにオブジェクトを追加

    // 当たり判定
    void TileCollision(int charaName);
};
