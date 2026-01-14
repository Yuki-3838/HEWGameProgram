#pragma once
#include "Scene.h"
#include "TileMap.h"
#include "MapRenderer.h"
#include "Player.h"
#include "GameObject.h"

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
