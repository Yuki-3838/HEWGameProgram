#pragma once
#include <vector>
#include <string>

class TileMap
{
private:
    // タイルIDの2次元配列 (m_MapData[y][x])
    std::vector<std::vector<int>> m_MapData;
    int m_MapWidth = 0;
    int m_MapHeight = 0;

public:
    // CSVファイルの読み込み
    bool LoadCSV(const std::string& fileName);

    // 指定座標のタイルID取得
    int GetTileID(int x, int y) const;

    int GetWidth() const { return m_MapWidth; }
    int GetHeight() const { return m_MapHeight; }

    void Clear();
};