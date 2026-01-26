#include "CollisionData.h"


CollisionData::CollisionData()
{

}

CollisionData::~CollisionData()
{

}

void CollisionData::Init(ID3D11ShaderResourceView* pTexture, float x, float y, float width, float height)
{
	GameObject::Init(pTexture);

	SetPosition(x, y);
	SetSize(width, height);
}

void CollisionData::UpdatePos(float parentX, float parentY, float offsetX, float offsetY)
{
	SetPosition(parentX + offsetX, parentY + offsetY);
}


