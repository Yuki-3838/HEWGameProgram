#pragma once
#include "Scene.h"

class SceneManager 
{
private:
    Scene* m_pCurrentScene;

public:
    void ChangeScene(Scene* pNextScene);
    void Update();
    void Draw();
    void Uninit();

    Scene* GetCurrentScene() const { return m_pCurrentScene; }
};