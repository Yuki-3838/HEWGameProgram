#include "Scene.h"

Scene::Scene(Renderer* r, ResourceManager* res, SpriteRenderer* spr, Input* input)
{
    m_pRenderer = r;
    m_pResourceManager = res;
    m_pSpriteRenderer = spr;
    m_pInput = input;
    m_pCamera = nullptr;
}
