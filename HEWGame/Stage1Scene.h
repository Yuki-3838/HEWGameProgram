#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "MapRenderer.h"
#include "Player.h"
#include "GameObject.h"

namespace Kaneda
{
    enum Chara
    {
        e_Player,
        e_Enemy
    };
}

class Stage1Scene : public Scene
{
private:
    TileMap* m_pTileMap;
    MapRenderer* m_pMapRenderer;
    Player* m_pPlayer;
    Character** m_pCharaList = nullptr;

    int m_currentCharaNum = 0;
    int maxChara = 10;

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

    // ƒŠƒXƒg‚ÉŠÖŒW‚·‚é
    void CreateList(int num);
    void ClearList(Character* list);
    void AllClearList(Character** list);
    Character* AddList(Kaneda::Chara e_name);
};
