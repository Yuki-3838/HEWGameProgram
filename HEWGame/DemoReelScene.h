#pragma once
#include "Scene.h"
#include "VideoPlayer.h"
class DemoReelScene :public Scene
{
private:
	VideoPlayer* m_pVideo;
    bool m_IsFinished;
public:
    using Scene::Scene;
    void Init() override;
    void Update() override;
    void Draw() override;
    void Uninit() override;
    bool ShouldChangeScene() const override { return m_IsFinished; }
};

