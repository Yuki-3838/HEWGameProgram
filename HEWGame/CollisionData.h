#pragma once
#include "GameObject.h"
class CollisionData :public GameObject
{
	public:
	CollisionData();
	~CollisionData();

	//初期化
	void Init(ID3D11ShaderResourceView* pTexture, float x, float y, float width, float heihgt);
	//更新
	void Update(float parentX, float parentY, float offsetX, float offsetY);
};

