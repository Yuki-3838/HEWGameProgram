#include "CollisionData.h"

CollisionData::CollisionData()
{
}

CollisionData::~CollisionData()
{
}

void CollisionData::Init(ID3D11ShaderResourceView* pTexture, float x, float y, float width, float height)
{
	//親クラスのInitを呼ぶ
	GameObject::Init(pTexture);

	//座標とサイズをセット
	SetPosition(x, y);
	SetSize(width, height);
}

void CollisionData::Update(float parentX, float parentY, float offsetX, float offsetY)
{
	//親の座標 + ズレ = 判定座標
	SetPosition(parentX + offsetX, parentY + offsetY);
}
