#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "MapRenderer.h"
#include "Player.h"

class Stage1Scene : public Scene
{
private:
    TileMap* m_pTileMap;
    MapRenderer* m_pMapRenderer;
    Player* m_pPlayer;

    ID3D11ShaderResourceView* m_pMapTex;
    ID3D11ShaderResourceView* m_pPlayerTex;
    bool m_IsFinished;

public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
};

namespace Kaneda
{
    // タイルID
    enum TileID
    {
        TILE_EMPTY = 0,     // 空き
        TILE_WALL = 1,      // 壁
        TILE_GOAL = 2       // ゴール
    };

    // タイルの情報
    struct TileInfo // 当たり判定、ダメージ判定、ゴール判定
    {
        bool isSolid;       // 当たり判定
        bool isDamage;      // ダメージ
        bool isGoal;        // ゴール
    };
};
