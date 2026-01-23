#pragma once
#include "Character.h"
#include"Animator.h"
#include"Sound.h"
#include "EffectManager.h"

enum class CharDir
{
    LEFT,
    RIGHT
};

enum class DashDirection
{
	NONE,
	UP,
	DOWN,
	RIGHT,
	LEFT
};
enum class DashState
{
	NONE,
	STAY,
	DASH
};

class Player :public Character
{
private:
    // 各状態のテクスチャを保持しておく変数
    ID3D11ShaderResourceView* m_pTexIdle = nullptr; // 待機用
    ID3D11ShaderResourceView* m_pTexWalk = nullptr; // 移動用
    ID3D11ShaderResourceView* m_pTexJump = nullptr; // ジャンプ用
    ID3D11ShaderResourceView* m_pTexAttack = nullptr; //攻撃用

    Animator m_Animator;//アニメーション管理
    bool m_FlipX = false; // 左右反転フラグ

    //現在再生中のアニメーション状態
    int m_CurrentAnimState = -1;

    //攻撃判定用変数
    bool m_IsAttack;
    int m_AttackFrame;
    static const int AttackTotalFrame = 30;  //攻撃アニメの総フレーム数
    static const int AttackHitStart = 1;     //攻撃判定が発生する開始フレーム
    static const int AttackHitEnd = 30;      //攻撃判定が発生する終了フレーム
    CharDir m_charDir; //キャラクターの向き

    //アニメーション切り替え関数
    void SetAnimation(int stateIndex);

	// ダッシュに関する変数
	DashState m_dState;
	DashDirection m_dDire[2];// ダッシュ方向

	static constexpr int m_dStayMax = 120;     // ダッシュ待機時間上限
	int m_dStayCount = 0;           // ダッシュ待機時間カウント
	static constexpr int m_dDistanceMax = 1000;   // ダッシュ距離上限
	int m_dDistanceCount = 0;       // ダッシュ距離カウント
	float m_dSpeed = 63;

    Sound* m_pSound = nullptr;
    EffectManager* m_pEffectManager = nullptr;
    Effect* m_pRunningEffect = nullptr;
public:
    // コンストラクタ・デストラクタ
    Player();
    ~Player() override;

    void UnInit() override;
    // 毎フレームの更新処理（入力による移動など）
    void Update(const TileMap& tile, Character** charaList)override;

    //アニメーションさせるための描画
    void Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj) override;
    // ステージとの当たり判定取得

	void Jump()override;
	void Attack(Character** charaList)override;
	int TakeDamage()override;

	// ダッシュ処理
	void DashMove(const TileMap& tile);

    void WallJump();
    void Blink();
    void GetBlink();
    void SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump, ID3D11ShaderResourceView* attack);
    void SetSound(Sound* pSound) { m_pSound = pSound; }
    void SetEffectManager(EffectManager* em) { m_pEffectManager = em; }
};
