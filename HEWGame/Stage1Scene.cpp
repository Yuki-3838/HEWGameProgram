#include "Stage1Scene.h"
#include "iostream"
#include "Collision.h"
#include <cmath> // fmod

void Stage1Scene::Init()
{
    // タイルの情報
    Kaneda::s_TileInfo tileTable[] =
    {
        {false,false,false},    // 空
        {true,false,false},     // 壁
        {false,false,true}      // ゴール
    };
    // リスト作成
    CreateList(maxChara);

    // 1. 各種マネージャー・マップの生成
    m_pTileMap = new TileMap();
    m_pTileMap->LoadCSV("asset/map/Stage1.csv");
    m_pMapRenderer = new MapRenderer();
    m_pCamera = new Camera(m_ScreenWidth, m_ScreenHeight);

    m_pSound = new Sound();
    m_pSound->Init();
    m_pSound->Load(SOUND_LABEL_SE_JUMP, "asset/sound/SE/jump.wav", false);

    m_pEffectManager = new EffectManager();
    m_pEffectManager->Init();
    m_pEffectManager->LoadEffectTexture(EffectType::Smoke, "asset/texture/Test_dash_Effect.png", m_pRenderer->GetDevice(), m_pResourceManager);
    // 2. �v���C���[�̐����Ə�����
    m_pCharaList[0] = AddList(State::CharaType::t_Player);
    m_pCharaList[1] = AddList(State::CharaType::t_Enemy);

    pDebugTex = m_pResourceManager->LoadTexture("asset/texture/debug_box.png", m_pRenderer->GetDevice());

    // 3. テクスチャのロード
    m_pMapTex = m_pResourceManager->LoadTexture("asset/texture/block.png", m_pRenderer->GetDevice());
    m_pPlayerTexIdle = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Idol.png", m_pRenderer->GetDevice());
    m_pPlayerTexWalk = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Dash.png", m_pRenderer->GetDevice());
    m_pPlayerTexJump = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Jump.png", m_pRenderer->GetDevice());
    m_pPlayerTexFall = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Fall.png", m_pRenderer->GetDevice());
    m_pPlayerTexAttack = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_Attack_D.png", m_pRenderer->GetDevice());
    m_pPlayerTexAbilityA = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_AbilityA.png", m_pRenderer->GetDevice());
    m_pPlayerTexAbilityB = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_AbilityB.png", m_pRenderer->GetDevice());
    m_pEnemyTex = m_pResourceManager->LoadTexture("asset/texture/nazuna.jpg", m_pRenderer->GetDevice());


    m_pPlayerTexDash = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_AbilityB.png", m_pRenderer->GetDevice());
    m_pPlayerTexDashStay = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_AbilityA.png", m_pRenderer->GetDevice());
    m_pPlayerTexDashEffect = m_pResourceManager->LoadTexture("asset/texture/Anime_Hero_AbilityC.png", m_pRenderer->GetDevice());

    //3-2. エネミー
    m_pEnemyTexIdle = m_pResourceManager->LoadTexture("asset/texture/Sw_Idole.png", m_pRenderer->GetDevice());
    m_pEnemyTexWalk = m_pResourceManager->LoadTexture("asset/texture/Sw_Walk.png", m_pRenderer->GetDevice());
    m_pEnemyTexJump = m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", m_pRenderer->GetDevice());

    m_pEnemyGunTexIdle = m_pResourceManager->LoadTexture("asset/texture/Gu_Idole.png", m_pRenderer->GetDevice());
    m_pEnemyGunTexWalk = m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", m_pRenderer->GetDevice());
    m_pEnemyGunTexJump = m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", m_pRenderer->GetDevice());

    m_pEnemySeTexIdle = m_pResourceManager->LoadTexture("asset/texture/Se_Idole.png", m_pRenderer->GetDevice());
    m_pEnemySeTexWalk = m_pResourceManager->LoadTexture("asset/texture/Se_Walk.png", m_pRenderer->GetDevice());
    m_pEnemySeTexJump = m_pResourceManager->LoadTexture("asset/texture/Gu_Walk.png", m_pRenderer->GetDevice());


    
    //m_pEnemyTex = m_pResourceManager->LoadTexture("asset/texture/nazuna.jpg", m_pRenderer->GetDevice());

    // 背景テクスチャ
    m_pBGTexFront = m_pResourceManager->LoadTexture("asset/texture/bg_front.png", m_pRenderer->GetDevice()); // 手前
    m_pBGTexMid =   m_pResourceManager->LoadTexture("asset/texture/bg_mid.png", m_pRenderer->GetDevice());   // 中
    m_pBGTexBack =  m_pResourceManager->LoadTexture("asset/texture/bg_back.png", m_pRenderer->GetDevice());  // 奥

    // 背景パララックス係数（必要ならレイヤ別に調整）
    // 横追従度
    m_BGParallaxU[0] = 1.0f; // 手前（横）
    m_BGParallaxU[1] = 0.6f; // 中（横）
    m_BGParallaxU[2] = 0.3f; // 奥（横）

    // 縦追従度
    m_BGParallaxV[0] = 1.0f; // 手前（縦）
    m_BGParallaxV[1] = 1.0f; // 中（縦）
    m_BGParallaxV[2] = 1.0f; // 奥（縦）

    // プレイヤーにテクスチャを渡す
    Player* player = dynamic_cast<Player*>(m_pCharaList[0]);
    if (player)
    {
        // ★ここで7枚セットで渡す
        player->SetTextures(m_pPlayerTexIdle, m_pPlayerTexWalk, m_pPlayerTexJump, m_pPlayerTexFall, m_pPlayerTexAttack, m_pPlayerTexAbilityA, m_pPlayerTexAbilityB);

        // �ŏ��̏����� (Init) ���Ă�ł���
        player->Init(m_pPlayerTexIdle,pDebugTex); //Idle��n��

        player->SetSound(m_pSound);
        player->SetEffectManager(m_pEffectManager);
    }

    Enemy* enemy = dynamic_cast<Enemy*>(m_pCharaList[1]);
    {
        // ★ここで3枚セットで渡す
        enemy->SetTextures(m_pEnemyTexIdle, m_pEnemyTexWalk, m_pEnemyTexJump);

        // 最初の初期化 (Init) も呼んでおく
        enemy->Init(m_pEnemyTexIdle); //Idleを渡す
    }

    m_IsFinished = false;

    //エネミーにプレイヤーの位置情報を渡す
    enemy->SetTarget(*player);

    m_pResourceManager->LoadTexture("asset/texture/debug_box", m_pRenderer->GetDevice());
}

void Stage1Scene::Update()
{
    CameraSeting();
    // 現在のキャラクターの数だけ更新
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (m_pCharaList[i] && !m_pCharaList[i]->IsDead())  // 死亡していなければ更新
        {
            m_pCharaList[i]->Update(*m_pTileMap, m_pCharaList);
        }
    }
    // シーン終了判定
    if (m_pInput->GetKeyTrigger(VK_RETURN))
    {
        m_IsFinished = true;
    }
    if (m_pEffectManager)
    {
        m_pEffectManager->Update();
    }
}

void Stage1Scene::Draw()
{
    // 背景色クリア（空の色）
    float clearColor[4] = { 0.f, 1.f, 0.f, 1.0f };
    m_pRenderer->StartFrame(clearColor);

    // カメラ行列の取得
    CameraSeting();

    DirectX::XMMATRIX viewProj = m_pCamera->GetViewProjection();

    // 0. 背景（パララックス）
    DrawBackground(viewProj);

    //1. マップの描画
    m_pMapRenderer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, *m_pTileMap, m_pMapTex, viewProj);

    //
    // 2. プレイヤーの描画
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (m_pCharaList[i] && !m_pCharaList[i]->IsDead())  // 死亡していなければ描画
        {
            m_pCharaList[i]->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
        }
    }
    if (m_pPlayer)
    {
        m_pPlayer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    }
    if (m_pEffectManager)
    {
        m_pEffectManager->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    }
    m_pRenderer->EndFrame();
}

void Stage1Scene::DrawBackground(DirectX::XMMATRIX viewProj)
{
    // SRV からテクスチャ幅/高さを取得
    auto GetTexSize = [](ID3D11ShaderResourceView* srv) -> std::pair<float, float>
    {
        if (!srv) return { 0.0f, 0.0f };
        ID3D11Resource* res = nullptr;
        srv->GetResource(&res);
        if (!res) return { 0.0f, 0.0f };
        ID3D11Texture2D* tex = static_cast<ID3D11Texture2D*>(res);
        D3D11_TEXTURE2D_DESC desc = {};
        tex->GetDesc(&desc);
        res->Release();
        return { static_cast<float>(desc.Width), static_cast<float>(desc.Height) };
    };

    ID3D11DeviceContext* ctx = m_pRenderer->GetContext();

    // 描画順：奥（遅い） -> 中 -> 手前（速い）
    ID3D11ShaderResourceView* layers[3] = { m_pBGTexBack, m_pBGTexMid, m_pBGTexFront };

    // カメラ位置（ワールド単位）
    float cameraX = m_pCamera->GetX();
    float cameraY = m_pCamera->GetY();

    for (int i = 0; i < 3; ++i)
    {
        ID3D11ShaderResourceView* srv = layers[i];
        if (!srv) continue;

        auto [texW, texH] = GetTexSize(srv);
        if (texW <= 0.0f || texH <= 0.0f) continue;

        // 横/縦パララックス係数（layers array は back, mid, front）
        float parallaxH = m_BGParallaxU[2 - i];
        float parallaxV = m_BGParallaxV[2 - i];

        // テクスチャを画面高さに合わせてスケール（縦比を画面に合わせる）
        float scale = static_cast<float>(m_ScreenHeight) / texH;
        float drawW = texW * scale;
        float drawH = static_cast<float>(m_ScreenHeight);

        // 横オフセット（テクスチャ単位 → ピクセル）
        float offsetTexX = std::fmod(cameraX * parallaxH, texW);
        if (offsetTexX < 0.0f) offsetTexX += texW;
        float offsetPixelsX = offsetTexX * scale;

        // 横開始位置：左に余分な1枚分を確保して見切れを防止
        float startX = -offsetPixelsX - drawW;

        // 縦はタイルせず、カメラに追従させる（1枚）
        float drawY = cameraY * parallaxV;

        // 横だけ繰り返して描画（上下は単一表示）
        for (float x = startX; x < static_cast<float>(m_ScreenWidth) + drawW; x += drawW)
        {
            m_pSpriteRenderer->Draw(ctx, srv, x, drawY, drawW, drawH, viewProj);
        }
    }
}

void Stage1Scene::Uninit()
{
    // メモリ解放
    if (m_pPlayer) { delete m_pPlayer; m_pPlayer = nullptr; }
    if (m_pTileMap) { delete m_pTileMap; m_pTileMap = nullptr; }
    if (m_pMapRenderer) { delete m_pMapRenderer; m_pMapRenderer = nullptr; }
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pSound) { m_pSound->Uninit(); delete m_pSound; m_pSound = nullptr; }
    if (m_pEffectManager) { m_pEffectManager->Uninit(); delete m_pEffectManager; m_pEffectManager = nullptr; }
    AllClearList(m_pCharaList);

    // 背景のSRVは ResourceManager が管理している想定のため Release しない。
    m_pBGTexFront = nullptr;
    m_pBGTexMid = nullptr;
    m_pBGTexBack = nullptr;
}

void Stage1Scene::CreateList(int num)
{
    if (m_pCharaList == nullptr)
    {
        m_pCharaList = new Character * [num];
    }
    for (int i = 0; i < maxChara; i++)
    {
        m_pCharaList[i] = nullptr;
    }
}

void Stage1Scene::ClearList(Character* list)
{
    delete list;
    list = nullptr;
    m_currentCharaNum--;
}

void Stage1Scene::AllClearList(Character** list)
{
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (list[i] != nullptr)
        {
            delete list[i];
            list[i] = nullptr;
        }
    }
}

Character* Stage1Scene::AddList(State::CharaType e_name)
{
    m_currentCharaNum++;
    switch (e_name)
    {
    case State::CharaType::t_Player:
        return new Player;
        break;
    case State::CharaType::t_Enemy:
        return new Enemy;
        break;
    };

}

void Stage1Scene::TileCollision(int charaName)
{
    float left = m_pCharaList[charaName]->GetPosition().x;
    float right = (m_pCharaList[charaName]->GetPosition().x + m_pCharaList[charaName]->GetSize().x);
    float top = m_pCharaList[charaName]->GetPosition().y;
    float bottom = (m_pCharaList[charaName]->GetPosition().y + m_pCharaList[charaName]->GetSize().y);

    int tileX_L = static_cast<int>(left / 32);
    int tileX_R = static_cast<int>(right / 32);
    int tileY_T = static_cast<int>(top / 32);
    int tileY_B = static_cast<int>(bottom / 32);

    for (int y = top; y <= bottom; y++)
    {
        for (int x = left; x <= right; x++)
        {
            if (m_pTileMap->GetTileID(x, y) == Kaneda::TILE_WALL)
            {

            }
        }
    }
}

void Stage1Scene::CameraSeting()
{
    DirectX::XMFLOAT2 defCameraPos(m_pCharaList[0]->GetPosition().x - 240, m_pCharaList[0]->GetPosition().y - 696);
    // プレイヤーのｘ座標が壁から一定距離でなければカメラを固定
    if (m_pCharaList[0]->GetPosition().x <= 240)
    {
        //ジャンプ中
        if (m_pCharaList[0]->GetJumpState() == State::JumpState::RISE || m_pCharaList[0]->GetJumpState() == State::JumpState::DESC)
        {
            //m_pCamera->SetPosition(0, m_pCharaList[0]->GetDefPosY() - 696);
        }
        // 着地中
        else
        {
            m_pCamera->SetPosition(0, defCameraPos.y);
        }
    }
    //else if(// 左壁に近い処理)
    // ジャンプ上昇、降下のカメラ処理
    else if (m_pCharaList[0]->GetJumpState() == State::JumpState::RISE || m_pCharaList[0]->GetJumpState() == State::JumpState::DESC)
    {
        m_pCamera->SetPosition(defCameraPos.x, m_pCharaList[0]->GetDefPosY() - 696);
    }
    else
    {
        m_pCamera->SetPosition(defCameraPos.x, defCameraPos.y);
    }

    /*else if (m_pCharaList[0]->GetPosition().x <= 240 && m_pCharaList[0]->GetJumpState() == State::JumpState::RISE)
    {
        m_pCamera->SetPosition(0, m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().y - 540 - 99 + m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetAcceleY());
    }
    else if(m_pCharaList[0]->GetJumpState() == State::JumpState::NONE)
    {
        m_pCamera->SetPosition(m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().x - 240, m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().y - 540 - 99);
    }
    else
    {
        m_pCamera->SetPosition(m_pCharaList[static_cast<int>(State::CharaType::t_Player)]->GetPosition().x - 240,0);
    }*/
}
