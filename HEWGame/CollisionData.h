#pragma once
#include "GameObject.h"

class CollisionData : public GameObject
{
public:
	CollisionData();
	~CollisionData();
	//サイズ・ポジション・テクスチャの初期化
	void Init(ID3D11ShaderResourceView* pTexture,float x, float y, float width, float height);

	//更新処理
	void UpdatePos(float parentX, float parentY, float offsetX, float offsetY);
};
