#include "SceneManager.h"

void SceneManager::ChangeScene(Scene* pNextScene)
{
    if (m_pCurrentScene) 
    {
        m_pCurrentScene->Uninit();
        delete m_pCurrentScene;
    }
    m_pCurrentScene = pNextScene;
    if (m_pCurrentScene) 
    {
        m_pCurrentScene->Init();
    }
}

void SceneManager::Update() 
{
    if (m_pCurrentScene) m_pCurrentScene->Update();
}

void SceneManager::Draw() 
{
    if (m_pCurrentScene) m_pCurrentScene->Draw();
}

void SceneManager::Uninit() 
{
    ChangeScene(nullptr); // Œ»İ‚ÌƒV[ƒ“‚ğ‰ğ•ú
}