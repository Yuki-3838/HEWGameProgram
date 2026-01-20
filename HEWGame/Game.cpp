#include "Game.h"
#include "Stage1Scene.h"
#include "ResultScene.h"
#include "DemoReelScene.h"

Game::Game()
{
    m_pRenderer = nullptr;
    m_pResourceManager = nullptr;
    m_pSpriteRenderer = nullptr;
    m_pSceneManager = nullptr;
    m_pInput = nullptr;
    m_hWnd = nullptr;
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, int width, int height)
{
    m_hWnd = hWnd;

    // 1. 各システムの生成
    m_pRenderer = new Renderer();
    m_pResourceManager = new ResourceManager();
    m_pSpriteRenderer = new SpriteRenderer();
    m_pSceneManager = new SceneManager();
    m_pInput = new Input();

    // 2. レンダラーの初期化（Direct3D 起動）
    m_pRenderer->Init(m_hWnd, width, height);

    // 3. スプライト描画の初期化（シェーダー読み込みなど）
    m_pSpriteRenderer->Init(m_pRenderer->GetDevice());

    // 4. 最初のシーン（タイトル）をセット
    // コンストラクタで代入するスタイルのため、ここで各システムを渡す
    m_pSceneManager->ChangeScene(new TitleScene(
        m_pRenderer,
        m_pResourceManager,
        m_pSpriteRenderer,
        m_pInput
    ));
}

void Game::Update()
{
    if (!m_pInput || !m_pSceneManager) return;

    // 1. 入力情報を最新にする
    m_pInput->Update(); // ★これがないと判定が更新されません

    // 2. シーンの更新
    m_pSceneManager->Update();
    // --- シーン遷移ロジック ---
    // シーンが「次に進みたい（ShouldChangeScene）」と言っているか確認
    Scene* current = m_pSceneManager->GetCurrentScene();
    if (current && current->ShouldChangeScene())
    {
        // どのシーンに切り替えるかの判定
        // typeid を使うか、シーン側に ID を持たせるのが一般的ですが、
        // ここでは簡単な状態遷移として実装例を示します。

        // TitleScene の場合 -> Stage1Scene へ
        if (dynamic_cast<TitleScene*>(current))
        {
            m_pSceneManager->ChangeScene(new Stage1Scene(m_pRenderer, m_pResourceManager, m_pSpriteRenderer, m_pInput));
        }
        else if (dynamic_cast<DemoReelScene*>(current))
        {
            m_pSceneManager->ChangeScene(new Stage1Scene(m_pRenderer, m_pResourceManager, m_pSpriteRenderer, m_pInput));
        }
        // Stage1Scene の場合 -> ResultScene へ
        else if (dynamic_cast<Stage1Scene*>(current))
        {
            m_pSceneManager->ChangeScene(new ResultScene(m_pRenderer, m_pResourceManager, m_pSpriteRenderer, m_pInput));
        }
        // ResultScene の場合 -> TitleScene へ戻る
        else if (dynamic_cast<ResultScene*>(current))
        {
            m_pSceneManager->ChangeScene(new TitleScene(m_pRenderer, m_pResourceManager, m_pSpriteRenderer, m_pInput));
        }
    }
}

void Game::Draw()
{
    if (m_pSceneManager)
    {
        m_pSceneManager->Draw();
    }
}

void Game::Uninit()
{
    // 生成した順と逆順で安全に解放
    if (m_pSceneManager) { m_pSceneManager->Uninit(); delete m_pSceneManager; m_pSceneManager = nullptr; }
    if (m_pSpriteRenderer) { m_pSpriteRenderer->Uninit(); delete m_pSpriteRenderer; m_pSpriteRenderer = nullptr; }
    if (m_pResourceManager) { m_pResourceManager->UninitAll(); delete m_pResourceManager; m_pResourceManager = nullptr; }
    if (m_pInput) { delete m_pInput; m_pInput = nullptr; }
    if (m_pRenderer) { m_pRenderer->Uninit(); delete m_pRenderer; m_pRenderer = nullptr; }
}