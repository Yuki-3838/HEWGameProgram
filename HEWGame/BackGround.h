#pragma once
#include "GameObject.h"

class BackGround : public GameObject
{
public:
    void Init(ID3D11ShaderResourceView* pTexture) override;
    void Update() override;
};