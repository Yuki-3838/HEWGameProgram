#include "MapRenderer.h"

void MapRenderer::Draw(
    ID3D11DeviceContext* pContext,
    SpriteRenderer* pSpriteRenderer,
    const TileMap& tileMap,
    ID3D11ShaderResourceView* pTexture,
    DirectX::XMMATRIX viewProj)
{
    // 画像（テクスチャ）がセットされていない場合は何もしない
    if (!pTexture) return;

    // マップの縦・横ループ
    for (int y = 0; y < tileMap.GetHeight(); ++y)
    {
        for (int x = 0; x < tileMap.GetWidth(); ++x)
        {
            // 現在のセルのタイルIDを取得
            int tileID = tileMap.GetTileID(x, y);

            // 0以下（空白）の場合は描画をスキップ
            if (tileID != 1) continue;

            // 描画位置を計算
            // タイルサイズ（32pxなど）を掛けて、配置場所をずらしていく
            float drawX = x * m_TileSize;
            float drawY = y * m_TileSize;

            // SpriteRendererに描画を依頼
            // ※現状、すべてのタイルに同じ画像（テクスチャ全体）が表示されます
            pSpriteRenderer->Draw(
                pContext,
                pTexture,
                drawX,
                drawY,
                m_TileSize,
                m_TileSize,
                viewProj
            );
        }
    }
}
