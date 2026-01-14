#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TileMap.h"
#include "SpriteRenderer.h"

class MapRenderer
{
private:
    static constexpr float m_TileSize = 32.0f; // タイル1枚の表示サイズ
    static constexpr float m_GAMENSAIZUX = 1920;
    static constexpr float m_GAMENSAIZUY = 1080;

public:
    // マップ全体の描画
    void Draw(ID3D11DeviceContext* pContext,
        SpriteRenderer* pSpriteRenderer,
        const TileMap& tileMap,
        ID3D11ShaderResourceView* pTexture,
        DirectX::XMMATRIX viewProj);
    inline float GetTitleSize() { return m_TileSize; }
    inline float GetSizex() { return m_GAMENSAIZUX; }
    inline float GetSizey() { return m_GAMENSAIZUY; }

    //void SetTileSize(float size) { m_TileSize = size; }
};