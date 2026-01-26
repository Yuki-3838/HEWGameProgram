#include "Stage1Scene.h"
#include "iostream"
#include "Collision.h"

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
    m_pCamera = new Camera(1920, 1080);

    m_pSound = new Sound();
    m_pSound->Init();
    m_pSound->Load(SOUND_LABEL_SE_JUMP, "asset/sound/SE/jump.wav", false);

    m_pEffectManager = new EffectManager();
    m_pEffectManager->Init();
    m_pEffectManager->LoadEffectTexture(EffectType::Smoke, "asset/texture/Test_dash_Effect.png", m_pRenderer->GetDevice(), m_pResourceManager);
    // 2. �v���C���[�̐����Ə�����
    m_pCharaList[0] = AddList(State::CharaType::t_Player);
    m_pCharaList[1] = AddList(State::CharaType::t_Enemy);

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


    // プレイヤーにテクスチャを渡す
    Player* player = dynamic_cast<Player*>(m_pCharaList[0]);
    if (player)
    {
        // ★ここで7枚セットで渡す
        player->SetTextures(m_pPlayerTexIdle, m_pPlayerTexWalk, m_pPlayerTexJump, m_pPlayerTexFall, m_pPlayerTexAttack, m_pPlayerTexAbilityA, m_pPlayerTexAbilityB);

        // �ŏ��̏����� (Init) ���Ă�ł���
        player->Init(m_pPlayerTexIdle); //Idle��n��

        player->SetSound(m_pSound);
        player->SetEffectManager(m_pEffectManager);
    }
    m_pCharaList[1]->Init(m_pEnemyTex);
    m_IsFinished = false;

    //エネミーにプレイヤーの位置情報を渡す
    Enemy* enemy = dynamic_cast<Enemy*>(m_pCharaList[1]);
    enemy->SetTarget(*player);


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

    //1. マップの描画
    m_pMapRenderer->Draw(m_pRenderer->GetContext(), m_pSpriteRenderer, *m_pTileMap, m_pMapTex, viewProj);

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
