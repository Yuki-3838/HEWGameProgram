#pragma once
#include "Scene.h"
#include <thread>
#include <atomic> 

enum class NextSceneType
{
    Stage1,
    Title,
};

class LoadScene : public Scene
{
private:
    ID3D11ShaderResourceView* m_pLoadTex = nullptr; // ローディング画像
    NextSceneType m_NextScene;

    Camera* m_pCamera = nullptr;
    //マルチスレッド用
    std::thread* m_pThread = nullptr;// 裏で作業するスレッド
    std::atomic<bool> m_IsFinished;  //「終わった？」を管理するフラグ（スレッドセーフ）

    // 裏で実行する関数
    void BackgroundLoad();

public:
    LoadScene(Renderer* r, ResourceManager* res, SpriteRenderer* spr, Input* input, NextSceneType next);
    ~LoadScene() override {}

    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
    NextSceneType GetNextSceneType() const { return m_NextScene; }
};
