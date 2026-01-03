#include "TileMap.h"
#include <fstream>
#include <sstream>
#include <string>

bool TileMap::LoadCSV(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open()) return false;

    Clear();
    std::string line;
    while (std::getline(file, line))
    {
        // Excel特有の改行コード(\r)が末尾に残っている場合を除去
        if (!line.empty() && line.back() == '\r') 
        {
            line.pop_back();
        }

        // 空行を読み飛ばす
        if (line.empty()) continue;

        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) // カンマ区切りで読み込み
        {
            if (value.empty()) continue;

            try
            {
                row.push_back(std::stoi(value));
            }
            catch (...)
            {
                // 数字以外の文字が入っていたら0として扱う
                row.push_back(0);
            }
        }

        if (!row.empty()) 
        {
            m_MapData.push_back(row);
        }
    }

    m_MapHeight = (int)m_MapData.size();
    m_MapWidth = m_MapHeight > 0 ? (int)m_MapData[0].size() : 0;
    return true;
}

int TileMap::GetTileID(int x, int y) const 
{
    if (x < 0 || x >= m_MapWidth || y < 0 || y >= m_MapHeight) return -1;
    return m_MapData[y][x];
}

void TileMap::Clear()
{
    // 2次元配列のデータをすべて削除する
    m_MapData.clear();

    // サイズ情報も0に戻す
    m_MapWidth = 0;
    m_MapHeight = 0;
}
