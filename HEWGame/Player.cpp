#include "Player.h"
#include <Windows.h>
#include<vector>
#include "GameData.h"

std::vector<GameObject*> g_GameObjects;

// プレイヤーのコンストラクタ
Player::Player()
{
	// プレイヤー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 20;
	m_Stats.m_Gravity = 1;
	m_Stats.m_JumpPw = 25;


	m_Size.x = 64 * 2;
	m_Size.y = 64 * 2;
	m_Position.x = 0.0f;
	m_Position.y = 100.0f;

	// ダッシュに関する初期化
	m_dState = DashState::NONE;
	m_dDire[0] = DashDirection::NONE;
	m_dDire[1] = DashDirection::NONE;
	m_charaType = State::CharaType::t_Player;
	//例えば0 なら待機、1なら走る、2ならジャンプなど
	SetAnimation(0);

	m_IsDead = false;

	m_AttackTotalFrame = 30; 
	m_AttackHitStart = 1;
	m_AttackHitEnd = 30;

	m_sGauge = 3.0f;
	m_sGaugeMAX = 3.0f;
	m_sQpush = false;
}

Player::~Player()
{
}

void Player::Update(const TileMap& tile, Character** charaList)
{
	GameData::AddSkill(SkillType::Dash, 0.5f);
	//アニメーション更新
	m_Animator.Update(1.0f / 1.0f);
	m_MoveState = State::MoveState::NONE;  //待機状態に戻す

	// 移動キーが押されているかチェック (左右どちらか)
	bool isMoving = false;
	//空中にいるかのチェック
	bool isAir = (m_JumpState != State::JumpState::NONE);


	//ｖを教えている間ダッシュゲージをチャージする　仕様と違うので削除する
	if (GetAsyncKeyState(VK_V) & 0x8000 && m_dState != DashState::DASH)
	{
		m_sGauge += 1.0f / 60.0f;
	}



	DashInput();

	//通常移動
	if (m_dState == DashState::NONE)
	{
		m_dState = DashState::NONE;
		m_dStayCount = 0;

		// 移動入力処理
		if (GetAsyncKeyState(VK_A) & 0x8000)
		{
			m_MoveState = State::MoveState::LEFT;
			m_FlipX = true;
			m_charDir = State::CharDir::LEFT;
			isMoving = true;
		}
		if (GetAsyncKeyState(VK_D) & 0x8000)
		{
			m_MoveState = State::MoveState::RIGHT;
			m_FlipX = false;
			m_charDir = State::CharDir::RIGHT;
			isMoving = true;
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			Jump();
		}

		if (m_IsAttack == false && GetAsyncKeyState(VK_F) & 0x8000)
		{
			m_IsAttack = true;
			m_AttackFrame = 0;
		}
	}

	//アニメーションの切り替え判定(優先度はダッシュ＞溜め＞攻撃＞ジャンプ＞移動＞待機)
	int nextAnim = 0; // 0:待機 (デフォルト)

	// ダッシュ中か
	if (m_dState == DashState::DASH)
	{
		nextAnim = 6; // ダッシュ用アニメ（AbilityB）
	}
	// 溜め中か
	else if (m_dState == DashState::STAY)
	{
		nextAnim = 5; // 溜め用アニメ（AbilityA）
	}
	//攻撃中か
	else if (m_IsAttack)
	{
		if (isAir)
		{
			nextAnim = 7; //空中攻撃用アニメ
		}
		else
		{
			nextAnim = 3; //攻撃用アニメ
		}
	}
	// ジャンプ上昇中か
	else if (m_JumpState == State::JumpState::RISE)
	{
		nextAnim = 2; // ジャンプ上昇用アニメ
	}
	// ジャンプ下降中か
	else if (m_JumpState == State::JumpState::DESC)
	{
		nextAnim = 4; // ジャンプ下降用アニメ
	}
	// 移動中か
	else if (m_MoveState == State::MoveState::LEFT || m_MoveState == State::MoveState::RIGHT)
	{
		nextAnim = 1; // 移動用アニメ
	}
	else
	{
		nextAnim = 0; // 待機アニメ
	}

	// 状態が変わった時だけセットする
	if (nextAnim != m_CurrentAnimState)
	{
		SetAnimation(nextAnim);
	}

	// ダッシュ中であればダッシュMoveを行う
	if (m_dState == DashState::DASH)
	{
		DashMove(tile);
	}


	//攻撃処理
	if (m_IsAttack)
	{
		m_AttackFrame++;
		//攻撃判定のあるフレームならAttack関数を呼び出す
		if (m_AttackFrame >= m_AttackHitStart && m_AttackFrame <= m_AttackHitEnd)
		{
			Attack(charaList);
		}
		//攻撃アニメ終了判定
		if (m_AttackFrame >= m_AttackTotalFrame)
		{
			m_IsAttack = false;
			m_AttackFrame = 0;
		}
	}
	// ダッシュ待機中でもダッシュ中でもなければ通常のMOVE
	else if (m_dState == DashState::NONE)
	{
		// 攻撃をリセット
		m_IsAttack = false;
		m_AttackFrame = 0;
		Move(tile);
	}
	// --- エフェクトの制御 ---
	if (isMoving)
	{
		///// 1. まだエフェクトが出ていなければ、新しく出す
		//if (m_pEffectManager && m_pRunningEffect == nullptr)
		//{
		//	// エフェクトを発生させ、そのポインタを受け取る
		//	m_pRunningEffect = m_pEffectManager->Play(EffectType::Smoke, m_Position.x, m_Position.y, m_FlipX);

		//	// ループ設定をONにする（これで勝手に消えない）
		//	if (m_pRunningEffect)
		//	{
		//		m_pRunningEffect->SetLoop(true);
		//	}
		//}

		//// 2. エフェクトが出ているなら、プレイヤーについてくるように位置を更新
		//if (m_pRunningEffect)
		//{
		//	// プレイヤーの足元(の少し後ろ)に合わせる計算
		//	// (Play関数内の計算と同じロジックを手動で行うか、Playを呼ぶ代わりにSetPositionを使う)
		//	float offsetX = m_FlipX ? 192.0f : 40.0f;
		//	float offsetY = 150.0f;                    // ����

		//	m_pRunningEffect->SetPosition(m_Position.x + offsetX, m_Position.y + offsetY);
		//}
	}
	else
	{
		// キーを離して止まったら、エフェクトを消す
		if (m_pRunningEffect)
		{
			m_pRunningEffect->Stop();  // エフェクトを停止
			m_pRunningEffect = nullptr;
			m_pRunningEffect = nullptr;
		}
	}

	//空中ジャンプの判定
	if (isAir)
	{
		m_canAirDash = true;
	}

	//空中での一時停止
	if (isAir && GetAsyncKeyState(VK_Q) & 0x8000 && m_dState == DashState::NONE)
	{
		m_sGauge = GameData::GetSkill(SkillType::Dash);
		m_sGaugeMAX = GameData::GetMaxSkill(SkillType::Dash);

		if (m_sGauge > m_sGaugeMAX)
		{
			GameData::UseSkill(SkillType::Dash, m_sGaugeMAX);
		}
		return;
	}

}

void Player::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
	// アニメーターから今のコマ情報を取得
	AnimFrame f = m_Animator.GetCurrentFrame();

	// 描画位置とサイズ
	float drawX = m_Position.x + f.renderOffsetX;
	float drawY = m_Position.y + f.renderOffsetY;
	float drawW = f.w * f.scale;
	float drawH = f.h * f.scale;


	// SpriteRendererで描画
	if (m_pTexture && pSR)
	{
		pSR->Draw(
			pContext,
			m_pTexture,
			drawX, drawY,
			drawW, drawH,
			viewProj,
			f.x, f.y, f.w, f.h, // UV座標
			0.0f,    // 回転なし
			m_FlipX  // 反転フラグ
		);
	}
}


void Player::UnInit()
{
}

void Player::Jump()
{
	//Y軸の加速度がなければ追加
	// ジャンプ　→　地面についていたら可能
	// 処理　　
	if (m_JumpState == State::JumpState::NONE)
	{
		m_Stats.m_AccelY = m_Stats.m_JumpPw;
		m_JumpState = State::JumpState::RISE;
	}
}

void Player::Attack(Character** charaList)
{
	//攻撃範囲設定
	DirectX::XMFLOAT2 attackSize = { 200.f,128.0f };
	DirectX::XMFLOAT2 attackPos;
	if (m_charDir == State:: CharDir::RIGHT)//右向き
	{
		attackPos.x = GetPosition().x + GetSize().x;
	}
	if (m_charDir == State::CharDir::LEFT)//左向き
	{
		attackPos.x = GetPosition().x - attackSize.x;
	}
	attackPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y /4 ;

	for (int i = 0; charaList[i] != nullptr; ++i)
	{
		auto obj = charaList[i];
		//オブジェクトじゃなかったらスキップする
		if (!obj)continue;

		if (obj->GetCharaType() == State::CharaType::t_Player)continue;  //playerだったらスキップする

		ColRes hit = CollisionRect(*obj,attackPos, attackSize);
		
		if (Col::Any(hit))
		{
			//敵にダメージを与える
			obj->TakeDamage();
			m_sGauge += 0.5f;
			//ここではenemyをdeleteしない！
		}
	}
}

int Player::TakeDamage()
{
	
		int damage = 1;
		m_Stats.m_HP -= damage;
		if (m_Stats.m_HP <= 0)
		{
			m_Stats.m_HP = 0;
			m_IsDead = true;
		}
		return m_Stats.m_HP;
	
}

void Player::WallJump()
{

}

void Player::Blink()
{

}

void Player::GetBlink()
{
}

void Player::SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* jump, ID3D11ShaderResourceView* fall, ID3D11ShaderResourceView* attack, ID3D11ShaderResourceView* flyattack, ID3D11ShaderResourceView* abilityA, ID3D11ShaderResourceView* abilityB)
{
	m_pTexIdle = idle;
	m_pTexWalk = walk;
	m_pTexJump = jump;
	m_pTexFall = fall;
	m_pTexAttack = attack;
	m_pTexFlyAttack = flyattack;
	
	m_pTexAbilityA = abilityA;
	m_pTexAbilityB = abilityB;

	// 初期状態として待機画像をセットしておく
	SetAnimation(m_CurrentAnimState);
}

void Player::SetAnimation(int stateIndex)
{
	// sakagami love kouyou
	m_CurrentAnimState = stateIndex;
	// 初期状態として待機画像をセットしておく
	m_pTexture = m_pTexIdle;
	m_CurrentAnimState = stateIndex;
	//画像の構成に合わせて数値を変更してね
	float w = 320.0f;
	float h = 320.0f;
	float animW = 0; // 今回設定するアニメの幅
	float animH = 0; // 今回設定するアニメの高さ
	float uvOffsetY = 0.0f; // UVのYオフセット
	float scale = 1.0f;

	float offX;
	float offY;
	// 状態に合わせてテクスチャとアニメ設定を切り替える
	switch (stateIndex)
	{
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置,ループさせるかどうか)
		//テクスチャの入れ替え
		m_pTexture = m_pTexIdle;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(24, 8, animW, animH, 0.02f, 0.0f, true, offX, offY, scale);
		break;
	case 1: //移動
		m_pTexture = m_pTexWalk;
		animW = w;
		animH = h;
		scale = 0.7;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(18, 9, animW, animH, 0.05f, 0.0f, true, offX, offY, scale);
		break;
	case 2: //ジャンプ上昇（ループしない）
		m_pTexture = m_pTexJump;
		animW = w;
		animH = h;
		scale = 0.65;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(14, 7, animW, animH, 0.1f, 0.0f, false, offX, offY, scale);
		break;
	case 3: //攻撃
		m_pTexture = m_pTexAttack;
		animW = w;
		animH = h;
		scale = 0.7;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(6, 3, animW, animH, 0.03f, 0.0f, false, offX, offY, scale);
		break;
	case 4: //落下
		m_pTexture = m_pTexFall;
		animW = w;
		animH = h;
		scale = 0.65;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(14, 7, animW, animH, 0.01f, 0.0f, false, offX, offY, scale);
		break;
	case 5: //溜め（AbilityA）
		m_pTexture = m_pTexAbilityA;
		animW = w;
		animH = h;
		scale = 0.7;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, animW, animH, 0.05f, 0.0f, true, offX, offY, scale);
		break;
	case 6: //ダッシュ（AbilityB）
		m_pTexture = m_pTexAbilityB;
		animW = w;
		animH = h;
		scale = 0.7;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(16, 8, animW, animH, 0.02f, 0.0f, false, offX, offY, scale);
		break;
	case 7: //空中攻撃
		m_pTexture = m_pTexFlyAttack;
		animW = w;
		animH = h;
		scale = 0.75;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(6, 3, animW, animH, 0.03f, 0.0f, false, offX, offY, scale);
		break;

	}
}

void Player::DashMove(const TileMap& tile)
{
	// ダッシュが2方向入力されたら
	if (m_dDire[0] != DashDirection::NONE && m_dDire[1] != DashDirection::NONE)
	{
		DirectX::XMFLOAT2 dir = { m_Position.x,m_Position.y };
		float distance = m_dSpeed;
		DirectX::XMVECTOR v = DirectX::XMLoadFloat2(&dir);
		v = DirectX::XMVector2Normalize(v);
		v = DirectX::XMVectorScale(v, distance);

		switch (m_dDire[0])
		{
		case DashDirection::UP:
			m_Position.y -= DirectX::XMVectorGetY(v);
			if (StageCol(tile, ColRes::TOP))m_Position.y += DirectX::XMVectorGetY(v);
			break;
		case DashDirection::DOWN:
			m_Position.y += DirectX::XMVectorGetY(v);
			if (StageCol(tile, ColRes::BOTTOM))m_Position.y -= DirectX::XMVectorGetY(v);
			break;
		}
		switch (m_dDire[1])
		{
		case DashDirection::RIGHT:
			m_Position.x += DirectX::XMVectorGetX(v);
			if (StageCol(tile, ColRes::RIGHT))m_Position.x -= DirectX::XMVectorGetX(v);
			break;
		case DashDirection::LEFT:
			m_Position.x -= DirectX::XMVectorGetX(v);
			if (StageCol(tile, ColRes::LEFT))m_Position.x += DirectX::XMVectorGetX(v);
			break;
		}
	}
	else // ダッシュが１方向の場合
	{
		if (m_dDire[0] == DashDirection::UP)
		{
			m_Position.y -= m_dSpeed;
			if (StageCol(tile, ColRes::TOP))m_Position.y += m_dSpeed;
		}
		if (m_dDire[0] == DashDirection::DOWN)
		{
			m_Position.y += m_dSpeed;
			if (StageCol(tile, ColRes::BOTTOM))m_Position.y -= m_dSpeed;
		}
		if (m_dDire[1] == DashDirection::RIGHT || (m_dDire[1] == DashDirection::NONE && m_dDire[0] == DashDirection::NONE && m_charDir == State::CharDir::RIGHT))
		{
			m_Position.x += m_dSpeed;
			if (StageCol(tile, ColRes::RIGHT))m_Position.x -= m_dSpeed;
		}
		if (m_dDire[1] == DashDirection::LEFT || (m_dDire[1] == DashDirection::NONE && m_dDire[0] == DashDirection::NONE && m_charDir == State::CharDir::LEFT))
		{
			m_Position.x -= m_dSpeed;
			if (StageCol(tile, ColRes::LEFT))m_Position.x += m_dSpeed;
		}
	}
	if (m_pDashEffect)
	{
		float centerX = m_FlipX ? 200.0f : -50.0f;
		float centrY = m_Position.y + (m_Size.y / 2.0f);
		m_pDashEffect->SetPosition(m_Position.x + centerX, centrY);
	}
	m_dDistanceCount += m_dSpeed;
	if (m_dDistanceCount >= m_dDistanceMax)
	{
		EndDash();
	}
}

void Player::DashInput()
{
	bool inputQ = GetAsyncKeyState(VK_Q);
	// ダッシュキーを押しており、ダッシュ状態でなければ、ダッシュ発動処理を行う
	if (inputQ && m_dState != DashState::DASH)
	{
		float currentSkill = GameData::GetSkill(SkillType::Dash);
		if (currentSkill >= 33.0f && (m_JumpState == State::JumpState::NONE || m_canAirDash))
		{
			m_dState = DashState::STAY;
			m_dStayCount = 0;
			m_dDire[0] = DashDirection::NONE;
			m_dDire[1] = DashDirection::NONE;
		}
	}
	if (inputQ && m_dState == DashState::STAY)
	{

		if (GetAsyncKeyState(VK_W) & 0x8000) m_dDire[0] = DashDirection::UP;
		else if (GetAsyncKeyState(VK_S) & 0x8000) m_dDire[0] = DashDirection::DOWN;

		if (GetAsyncKeyState(VK_A) & 0x8000) m_dDire[1] = DashDirection::LEFT;
		else if (GetAsyncKeyState(VK_D) & 0x8000) m_dDire[1] = DashDirection::RIGHT;

		m_dStayCount++;

		if (m_dStayCount >= m_dStayMax)
		{
			StartDash();
		}
	}
	if (!inputQ && m_dState == DashState::STAY)
	{
		StartDash();
	}
}

void Player::StartDash()
{
	if (m_pDashEffect)
	{
		m_pDashEffect->Stop();
	}
	m_pDashEffect = m_pEffectManager->Play(EffectType::Dash, m_Position.x, m_Position.y , m_FlipX);

	if(m_pDashEffect)
	{
		m_pDashEffect->SetLoop(true);
	}
	m_dState = DashState::DASH;
	GameData::UseSkill(SkillType::Dash, 33.0f);
	m_dDistanceCount = 0.0f;

	// 入力なし → 向いている方向
	if (m_dDire[0] == DashDirection::NONE &&
		m_dDire[1] == DashDirection::NONE)
	{
		m_dDire[1] = m_FlipX ? DashDirection::LEFT : DashDirection::RIGHT;
	}
	


	if (m_JumpState != State::JumpState::NONE)
	{
		m_canAirDash = false;  // ★ 空中ダッシュを使い切る
	}
}

void Player::EndDash()
{
	if(m_pDashEffect)
	{
		m_pDashEffect->Stop();
		m_pDashEffect = nullptr;
	}
	m_dState = DashState::NONE;
	m_dStayCount = 0;
	m_dDistanceCount = 0;
	m_dDire[0] = DashDirection::NONE;
	m_dDire[1] = DashDirection::NONE;
}
