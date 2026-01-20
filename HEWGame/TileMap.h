#pragma once
#include <vector>
#include <string>

namespace Kaneda
{
    // タイルID
    enum e_TileId
    {
        TILE_NOTFOUND = -1, // 見つからない
        TILE_EMPTY = 0,     // 空き
        TILE_WALL = 1,      // 壁
        TILE_GOAL = 2       // ゴール
    };

    // タイルの情報
    struct s_TileInfo // 当たり判定、ダメージ判定、ゴール判定
    {
        bool isSolid;       // 当たり判定
        bool isDamage;      // ダメージ
        bool isGoal;        // ゴール
    };
};

class TileMap
{
private:
    // タイルIDの2次元配列 (m_MapData[y][x])
    std::vector<std::vector<int>> m_MapData;
    int m_MapWidth = 0;
    int m_MapHeight = 0;
    static constexpr float m_TileSize = 64.0f; // タイル1枚の表示サイズ
public:
    // CSVファイルの読み込み
    bool LoadCSV(const std::string& fileName);

    // 指定座標のタイルID取得
    Kaneda::e_TileId GetTileID(int x, int y) const;

    int GetWidth() const { return m_MapWidth; }
    int GetHeight() const { return m_MapHeight; }

    void Clear();

    inline float GetTileSize() const { return m_TileSize; }
};

