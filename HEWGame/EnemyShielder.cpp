#include "EnemyShielder.h"

EnemyShielder::EnemyShielder()
{
	// エネミー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 7.5;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;

	m_Size.x = 128.0f;
	m_Size.y = 256.0f;
	m_Position.x = 1300.0f;
	m_Position.y = 0.0f;

	searchSize = { 500.f, 128.0f };

	m_AttackTotalFrame = 30;  //攻撃アニメの総フレーム数
	m_AttackHitStart = 1;     //攻撃判定が発生する開始フレーム
	m_AttackHitEnd = 30;      //攻撃判定が発生する終了フレーム

	m_charaType = State::CharaType::t_EnemyShielder;

	isDetection = false; //プレイヤーの発見状態
	m_charDir = State::CharDir::LEFT; // エネミーの向き
}


EnemyShielder::~EnemyShielder()
{


}

void EnemyShielder::Update(const TileMap& tile, Character** charaList)
{
	// アニメーション更新
	m_Animator.Update(1.0f / 1.0f);

	// 移動状態の初期化
	m_MoveState = State::MoveState::NONE;
	
	// 移動入力処理
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_MoveState = State::MoveState::LEFT;
		m_charDir = State::CharDir::LEFT;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_MoveState = State::MoveState::RIGHT;
		m_charDir = State::CharDir::RIGHT;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		Jump();
	}

	if (m_IsAttack == false && GetAsyncKeyState(VK_Z) & 0x8000)
	{
		m_IsAttack = true;
		m_AttackFrame = 0;
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

	//非発見状態
	if (isDetection == false)
	{
		// 索敵中は移動速度を半分にする
		m_Stats.m_Speed = 5;

		//左右移動にする予定
		SCount++;

		// 方向転換
		if (SCount == 100)
		{
			if (m_charDir == State::CharDir::RIGHT)
			{
				m_charDir = State::CharDir::LEFT; //左を見る
			} else {

				m_charDir = State::CharDir::RIGHT; //右を見る
			}
			SCount = 0;
		}

		if (m_charDir == State::CharDir::RIGHT)//右向き
		{
			searchPos.x = GetPosition().x - searchSize.x;
			m_MoveState = State::MoveState::RIGHT;
		}
		else//左向き
		{
			// attackPos.x += (m_Size.x / 2) + (attackSize.x / 2);
			searchPos.x = GetPosition().x + GetSize().x;
			m_MoveState = State::MoveState::LEFT;
		}
		//attackPos.y += m_Size.y / 4;
		searchPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y / 4;

		for (int i = 0; charaList[i] != nullptr; ++i)
		{
			auto obj = charaList[i];
			//オブジェクトじゃなかったらスキップする
			if (!obj)continue;

			//Character* chara = dynamic_cast<Character*>(obj);
			//if (!chara)continue;
			if (obj->GetCharaType() != State::CharaType::t_Player)continue;  //player以外だったらスキップする

			//ColRes hit = CollisionRect(attackPos, attackSize, chara->GetPosition(), chara->GetSize());]
			ColRes hit = CollisionRect(*obj, searchPos, searchSize);

			if (Col::Any(hit))
			{
				//敵にダメージを与える
				/*obj->TakeDamage();*/
				//ここではenemyをdeleteしない！

				SCount2++;
				if (SCount2 == 100) //テスト用に見つけてから一拍おいてます
				{
					isDetection = true;
					SCount2 = 0;
				}
			}
		}
		// 速度を戻す
		m_Stats.m_Speed = 10;
	}

	//発見状態時
	if (isDetection == true)
	{
		m_MoveState = State::MoveState::NONE;
		if (m_pTarget)
		{
			DirectX::XMFLOAT2 targetPos = m_pTarget->GetPosition();
			DirectX::XMFLOAT2 enemyPos = GetPosition();

			targetPos.x += m_pTarget->GetSize().x * 0.5f;
			enemyPos.x += GetSize().x * 0.5f;


			if (targetPos.x + 250.0f < enemyPos.x)
			{
				m_MoveState = State::MoveState::LEFT;
				m_charDir = State::CharDir::LEFT;
			}

			else if (targetPos.x - 250.0f > enemyPos.x)
			{
				m_MoveState = State::MoveState::RIGHT;
				m_charDir = State::CharDir::RIGHT;
			}
		}
	}
	Move(tile);

	//アニメーションの切り替え判定(優先度はダッシュ＞溜め＞攻撃＞ジャンプ＞移動＞待機)
	int nextAnim = 0; // 0:待機 (デフォルト)

	// 移動中か
	if (m_MoveState == State::MoveState::LEFT || m_MoveState == State::MoveState::RIGHT)
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

}
void EnemyShielder::Attack(Character** charaList)
{
	//攻撃範囲設定
	DirectX::XMFLOAT2 attackSize = { 200.f,128.0f };
	DirectX::XMFLOAT2 attackPos;
	if (m_charDir == State::CharDir::RIGHT)//右向き
	{
		attackPos.x = GetPosition().x + GetSize().x;
	}
	if (m_charDir == State::CharDir::LEFT)//左向き
	{
		attackPos.x = GetPosition().x - attackSize.x;
	}
	attackPos.y = GetPosition().y + GetSize().y / 2 - GetSize().y / 4;

	for (int i = 0; charaList[i] != nullptr; ++i)
	{
		auto obj = charaList[i];
		//オブジェクトじゃなかったらスキップする
		if (!obj)continue;

		if (obj->GetCharaType() != State::CharaType::t_Player)continue;  //player以外だったらスキップする

		ColRes hit = CollisionRect(*obj, attackPos, attackSize);
		if (Col::Any(hit))
		{
			//敵にダメージを与える
			obj->TakeDamage();
			//ここではplayerをdeleteしない！
		}
	}
}

void EnemyShielder::SetAnimation(int stateIndex)
{
	m_CurrentAnimState = stateIndex;
	// 初期状態として待機画像をセットしておく
	m_pTexture = m_eTexIdle;
	m_CurrentAnimState = stateIndex;
	//画像の構成に合わせて数値を変更してね
	float w = 320.0f;
	float h = 320.0f;
	// 状態に合わせてテクスチャとアニメ設定を切り替える
	switch (stateIndex)
	{
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置, ループするかどうか)
		//テクスチャの入れ替え
		m_pTexture = m_eTexIdle;
		m_Animator.Init(32, 8, w, h + 40, 0.01f, 0.0f, true);
		break;
	case 1: //移動
		m_pTexture = m_eTexWalk;
		m_Animator.Init(32, 8, w, h, 0.02f, 0.0f, true);
		break;
	}
}

