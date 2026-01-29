#include "Stage1Scene.h"
#include "iostream"
#include "Collision.h"
#include <cmath> 
#include "GameData.h"

void Stage1Scene::Init()
{
    GameData::Reset();

    GameData::SetSkill(SkillType::Dash, 100.0f, 100.0f);
    // タイルの情報
    s_TileInfo tileTable[] =
    {
        {false,false,false},    // 空
        {true,false,false},     // 壁
        {false,false,true}      // ゴール
    };

    // 1. 各種マネージャー・マップの生成
    // タイル
    m_pTileMap = new TileMap();
    m_pTileMap->LoadCSV("asset/map/HH_stage.csv");
    m_pMapRenderer = new MapRenderer();
    m_pCamera = new Camera(m_ScreenWidth, m_ScreenHeight);

    //音源をロード
    m_pSound = new Sound();
    m_pSound->Init();
    m_pSound->Load(SOUND_LABEL_SE_JUMP, "asset/sound/SE/jump.wav", false);
    m_pSound->Load(SOUND_LABEL_SE_ATTACK, "asset/sound/SE/Attack.wav", false);
    m_pSound->Load(SOUND_LABEL_SE_JUMPLANDING, "asset/sound/SE/Jumplanding.wav", false);
    m_pSound->Load(SOUND_LABEL_SE_DASH, "asset/sound/SE/Dash2.wav", false);
    m_pSound->Load(SOUND_LABEL_SE_DASHCHARGE, "asset/sound/SE/Dashcharge.wav", false);



    m_pEffectManager = new EffectManager();
    m_pEffectManager->Init();
    m_pEffectManager->LoadEffectTexture(EffectType::Smoke, "asset/texture/Test_dash_Effect.png", m_pRenderer->GetDevice(), m_pResourceManager);
    // リスト作成
    CreateList(maxChara);
    // Playerキャラ作成　キャラクターは０番
    m_pCharaList[0] = AddList(State::CharaType::t_Player);
    // テクスチャの読込
    SetAnimations();
    SetPlayerTexture();

    // 背景パララックス係数（必要ならレイヤ別に調整）
    // 横追従度
    m_BGParallaxU[0] = 1.0f; // 手前（横）
    m_BGParallaxU[1] = 0.6f; // 中（横）
    m_BGParallaxU[2] = 0.3f; // 奥（横）

    // 縦追従度
    //m_BGParallaxV[0] = 1.0f; // 手前（縦）
    //m_BGParallaxV[1] = 1.0f; // 中（縦）
    //m_BGParallaxV[2] = 1.0f; // 奥（縦）


    m_IsFinished = false;

    //エネミーにプレイヤーの位置情報を渡す
    //enemy->SetTarget(*player);

    m_pGameUI = new GameUI();
    m_pGameUI->Init(m_pRenderer->GetDevice(), m_pResourceManager);

    GameData::SetTime(60.0f);
}

void Stage1Scene::Update()
{
    EnemySpawn();

    CameraSeting();

    UpdateList();

    CollisionResolve();
    // シーン終了判定
    if (m_pInput->GetKeyTrigger(VK_RETURN))
    {
        m_IsFinished = true;
    }
    if (m_pEffectManager)
    {
        m_pEffectManager->Update();
    }

    GameData::DecreaseTime(1.0f / 60.0f);
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
    if (m_pEffectManager)
    {
        m_pEffectManager->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, viewProj);
    }
    if (m_pGameUI)
    {
        m_pGameUI->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer);
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

    // 画面右端/下端のワールド座標（ワールド＝カメラ位置 + スクリーン幅/高さ）
    float worldRight = cameraX + static_cast<float>(m_ScreenWidth);

    for (int i = 0; i < 3; ++i)
    {
        ID3D11ShaderResourceView* srv = layers[i];
        if (!srv) continue;

        auto [texW, texH] = GetTexSize(srv);
        if (texW <= 0.0f || texH <= 0.0f) continue;

        // 横パララックス係数（layers array は back, mid, front）
        float parallaxH = m_BGParallaxU[2 - i];
        // 縦は「カメラ追従のみ」にする -> parallaxV を使わない（縦はループ不可のため）
        
        // スケール：画像の高さを画面高さに合わせる（縦は画面を覆す想定）
        float scale = static_cast<float>(m_ScreenHeight) / texH;
        float drawW = texW * scale;
        float drawH = static_cast<float>(m_ScreenHeight);

        // --- 横オフセット（画面ピクセル単位での滑らかなループ） ---
        // raw はスクリーン上のオフセット（マイナス方向で動く）
        float raw = -cameraX * parallaxH;
        float offset = std::fmod(raw, drawW);
        if (offset > 0.0f) offset -= drawW; // 範囲を [-drawW, 0)
        // world 領域での開始 x（ワールド座標）
        float startWorldX = cameraX + offset - drawW;
        float endWorldX = worldRight + drawW;

        // --- 縦はカメラに合わせて固定（パララックスしない） ---
        float drawY = cameraY; // これでビュー行列適用後にスクリーン上で常に top=0 になる

        for (float wx = startWorldX; wx < endWorldX; wx += drawW)
        {
            m_pSpriteRenderer->Draw(ctx, srv, wx, drawY, drawW, drawH, viewProj);
        }
    }
}

void Stage1Scene::Uninit()
{
    // メモリ解放
    if (m_pTileMap) { delete m_pTileMap; m_pTileMap = nullptr; }
    if (m_pMapRenderer) { delete m_pMapRenderer; m_pMapRenderer = nullptr; }
    if (m_pCamera) { delete m_pCamera; m_pCamera = nullptr; }
    if (m_pSound) { m_pSound->Uninit(); delete m_pSound; m_pSound = nullptr; }
    if (m_pEffectManager) { m_pEffectManager->Uninit(); delete m_pEffectManager; m_pEffectManager = nullptr; }
    if(m_pGameUI) { delete m_pGameUI; m_pGameUI = nullptr; }
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
    case State::CharaType::t_EnemySword:
        return new EnemySword;
        break;

    case State::CharaType::t_EnemyShooter:
        return new EnemyShooter;
        break;

    case State::CharaType::t_EnemyShielder:
        return new EnemyShielder;
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
            if (m_pTileMap->GetTileID(x, y) == TILE_WALL)
            {

            }
        }
    }
}

void Stage1Scene::CameraSeting()
{
    DirectX::XMFLOAT2 defCameraPos(m_pCharaList[0]->GetPosition().x - 240, m_pCharaList[0]->GetPosition().y - 696);
    // プレイヤーのｘ座標が壁から一定距離でなければカメラを固定
    /*if (m_pCharaList[0]->GetPosition().x <= 240)
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
    else if (defCameraPos.y < m_pCharaList[0]->GetPosition().y)*/
    {
        m_pCamera->SetPosition(m_pCharaList[0]->GetPosition().x - 540, m_pCharaList[0]->GetPosition().y - 696);
    }
}

void Stage1Scene::EnemySpawn()
{
    // 探索開始範囲
    int X = m_pCharaList[0]->GetPosition().x / m_pTileMap->GetTileSize();
    int Y = m_pCharaList[0]->GetPosition().y / m_pTileMap->GetTileSize();
    // 探索終了範囲
    int Xmax = m_pCharaList[0]->GetPosition().x + m_ScreenWidth / m_pTileMap->GetTileSize();
    int Ymax = m_pCharaList[0]->GetPosition().y + m_ScreenHeight / m_pTileMap->GetTileSize();
    // キャラクターポジション　+ ScreenWidth
	for (int x = X;x < Xmax;x++)
    {
        for (int y = Y;y < Ymax;y++)
        {
            // 探索したタイルが敵スポーンタイルである
            if (m_pTileMap->GetTileID(x, y) == TILE_SPAWN)
            {
                // スポーン位置を保存し、重複していなければ敵を出現
                SpawnPoint p{ x,y };
                if (exploredPoint.insert(p).second)
                {
                    // 配列を探索
                    int num = GetEmptyListNum();
                    if (num != -1)
                    {
                        m_pCharaList[num] = AddList(State::CharaType::t_EnemySword);
                        m_pCharaList[num]->SetPos(x * m_pTileMap->GetTileSize(), y * m_pTileMap->GetTileSize());
                        SetEnemyTexture(num);
                    }
                }
            }
        }
    }
}

int Stage1Scene::GetEmptyListNum()
{
    // プレイヤー以上の範囲
    int num = 1;
    for (; num < maxChara; num++)
    {
        if (m_pCharaList[num] == nullptr)
        {
            break;
        }
    }
    // 最大キャラ以下であれば数値を返し、最大キャラを越えるとー１を返す
    if (num < maxChara)
    {
        return num;
    }
    else
    {
        return -1;
    }
}
// test

void Stage1Scene::SetAnimations()
{
    // 3. テクスチャのロード
    m_pMapTex = m_pResourceManager->LoadTexture("asset/texture/object/block.png", m_pRenderer->GetDevice());
    m_pPlayerTexIdle = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Idol.png", m_pRenderer->GetDevice());
    m_pPlayerTexWalk = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Dash.png", m_pRenderer->GetDevice());
    m_pPlayerTexJump = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Jump.png", m_pRenderer->GetDevice());
    m_pPlayerTexFall = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Fall.png", m_pRenderer->GetDevice());
    m_pPlayerTexAttack = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Attack_S.png", m_pRenderer->GetDevice());
    m_pPlayerTexFlyAttack = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_Attack_D.png", m_pRenderer->GetDevice());
    m_pPlayerTexSkillStay = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_AbilityStay.png", m_pRenderer->GetDevice());
    m_pPlayerTexSkillDash = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_AbilityDash.png", m_pRenderer->GetDevice());
    m_pPlayerTexSkillEffect = m_pResourceManager->LoadTexture("asset/texture/Player/2_Anime_Hero_AbilityEfect.png", m_pRenderer->GetDevice());

    //3-2. エネミー
    m_pEnemySwordTexIdle = m_pResourceManager->LoadTexture("asset/texture/Sword_Idole.png", m_pRenderer->GetDevice());
    m_pEnemySwordTexWalk = m_pResourceManager->LoadTexture("asset/texture/Sword_Walk.png", m_pRenderer->GetDevice());
    m_pEnemySwordTexAttack = m_pResourceManager->LoadTexture("asset/texture/Sword_Attack_Action.png", m_pRenderer->GetDevice());
    m_pEnemySwordTexAttackTelegraph = m_pResourceManager->LoadTexture("asset/texture/Sword_Attack_Standby.png", m_pRenderer->GetDevice());

    m_pEnemyShooterTexIdle = m_pResourceManager->LoadTexture("asset/texture/Shooter_Idole.png", m_pRenderer->GetDevice());
    m_pEnemyShooterTexWalk = m_pResourceManager->LoadTexture("asset/texture/Shooter_Walk.png", m_pRenderer->GetDevice());
    m_pEnemyShooterTexAttack = m_pResourceManager->LoadTexture("asset/texture/Shooter_Attack_Action.png", m_pRenderer->GetDevice());
    m_pEnemyShooterTexAttackTelegraph = m_pResourceManager->LoadTexture("asset/texture/Shooter_Attack_Stanby.png", m_pRenderer->GetDevice());

    m_pEnemyShielderTexIdle = m_pResourceManager->LoadTexture("asset/texture/Shielder_Idole.png", m_pRenderer->GetDevice());
    m_pEnemyShielderTexWalk = m_pResourceManager->LoadTexture("asset/texture/Shielder_Walk.png", m_pRenderer->GetDevice());


    // 背景テクスチャ
    m_pBGTexFront = m_pResourceManager->LoadTexture("asset/texture/Back/bg_front.png", m_pRenderer->GetDevice()); // 手前
    m_pBGTexMid = m_pResourceManager->LoadTexture("asset/texture/Back/bg_mid.png", m_pRenderer->GetDevice());   // 中
    m_pBGTexBack = m_pResourceManager->LoadTexture("asset/texture/Back/bg_back.png", m_pRenderer->GetDevice());  // 奥
}

void Stage1Scene::SetPlayerTexture()
{
    // プレイヤーにテクスチャを渡す
    Player* player = dynamic_cast<Player*>(m_pCharaList[0]);
    if (player)
    {
        // ★ここで7枚セットで渡す
        player->SetTextures(m_pPlayerTexIdle, m_pPlayerTexWalk, m_pPlayerTexJump, m_pPlayerTexFall, m_pPlayerTexAttack,m_pPlayerTexFlyAttack, m_pPlayerTexSkillStay, m_pPlayerTexSkillDash);
        player->SetSound(m_pSound);
        player->SetEffectManager(m_pEffectManager);
    }
}

void Stage1Scene::SetEnemyTexture(int num)
{
    EnemySword* enemy = dynamic_cast<EnemySword*>(m_pCharaList[num]);
    {
        // ★ここで4枚セットで渡す
        enemy->SetTextures(m_pEnemySwordTexIdle, m_pEnemySwordTexWalk, m_pEnemySwordTexAttack, m_pEnemySwordTexAttackTelegraph);

        // 最初の初期化 (Init) も呼んでおく
        enemy->Init(m_pEnemySwordTexIdle); //Idleを渡す
    }
    //エネミーにプレイヤーの位置情報を渡す
    enemy->SetTarget(*m_pCharaList[0]);
}

void Stage1Scene::UpdateList()
{
    // 現在のキャラクターの数だけ更新
    for (int i = 0; i < m_currentCharaNum; i++)
    {
        if (m_pCharaList[i] && !m_pCharaList[i]->IsDead())  // 死亡していなければ更新
        {
            m_pCharaList[i]->Update(*m_pTileMap, m_pCharaList);
        }
    }
}

void Stage1Scene::CollisionResolve()
{
    ColRes res;
    for (int i = 0;i < m_currentCharaNum;i++)
    {
        for (int j = i + 1;j < m_currentCharaNum;j++)
        {
            if (!m_pCharaList[j]->IsDead())
            {
                m_pCharaList[i]->ResolveOverlap(*m_pTileMap,*m_pCharaList[j]);
            }
        }
    }
}
