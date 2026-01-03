#pragma once
#include <map>
#include <string>
#include <d3d11.h>

//画像のロードをするクラス
class ResourceManager
{
private:
	std::map<std::string, ID3D11ShaderResourceView*>m_Resources;
	ID3D11Device* m_pDevice;
public:
	ResourceManager();
	//指定したPNGを読み込み、その画像ポインタを返す。
	ID3D11ShaderResourceView* LoadTexture(const std::string& fileName, ID3D11Device* pDevice);
	//ゲーム終了時に、保管している全ての画像をリリース
	void UninitAll();
};
