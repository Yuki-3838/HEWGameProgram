#pragma once
#include "Renderer.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Input.h"

// 全てのシーンの親となるクラス
class Scene 
{
protected:
    // 子クラスからアクセスできるように protected で宣言
    Renderer* m_pRenderer;
    ResourceManager* m_pResourceManager;
    SpriteRenderer* m_pSpriteRenderer;
    Input* m_pInput;
    Camera* m_pCamera;
public:
    Scene(Renderer* r, ResourceManager* res, SpriteRenderer* spr,Input* input);
    virtual ~Scene() {}

    // 初期化
    virtual void Init() = 0;
    // 更新 (入力、計算)
    virtual void Update() = 0;
    // 描画
    virtual void Draw() = 0;
    // 終了
    virtual void Uninit() = 0;

    // 次のシーンへ進むべきかどうかのフラグ
    virtual bool ShouldChangeScene() const = 0;
};