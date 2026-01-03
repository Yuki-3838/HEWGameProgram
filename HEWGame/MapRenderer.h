#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "TileMap.h"
#include "SpriteRenderer.h"

class MapRenderer
{
private:
    float m_TileSize = 32.0f; // タイル1枚の表示サイズ

public:
    // マップ全体の描画
    void Draw(ID3D11DeviceContext* pContext,
        SpriteRenderer* pSpriteRenderer,
        const TileMap& tileMap,
        ID3D11ShaderResourceView* pTexture,
        DirectX::XMMATRIX viewProj);

    void SetTileSize(float size) { m_TileSize = size; }
};