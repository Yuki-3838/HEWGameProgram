#include "Enemy.h"


Enemy::Enemy()
{
	// エネミー固有の初期設定
	m_Stats.m_HP = 1;
	m_Stats.m_Speed = 15;
	m_Stats.m_Gravity = 5;
	m_Stats.m_JumpPw = 25;

	m_Size.x = 64 * 2;
	m_Size.y = 64 * 2;
	m_Position.x = 1000.0f;
	m_Position.y = 0.0f;

	searchSize = { 500.f, 128.0f };

	m_charaType = State::CharaType::t_EnemySword;

	isDetection = false; //プレイヤーの発見状態
}

Enemy::~Enemy()
{

}

void Enemy::EnemyInit()
{
	m_ActionState = ActionState::SERCH;
	m_serchDistance = 500;
}

void Enemy::Update(const TileMap& tile, Character** charaList)
{
	if (m_ActionState == ActionState::SERCH)
	{
		SerchPlayer(charaList,tile);
	}
	CharacterColDir(charaList);
	switch (m_charDir)
	{
	case State::CharDir::LEFT:
		m_MoveState = State::MoveState::LEFT;
		break;
	case State::CharDir::RIGHT:
		m_MoveState = State::MoveState::RIGHT;
		break;
	}
}

void Enemy::UnInit()
{
  
}

void Enemy::Draw()
{
}


void Enemy::Attack(Character** charaList)
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

		if (obj->GetCharaType() == State::CharaType::t_Player)continue;  //player以外だったらスキップする

		ColRes hit = CollisionRect(*obj, attackPos, attackSize);

		if (Col::Any(hit))
		{
			//敵にダメージを与える
			obj->TakeDamage();
			//ここではplayerをdeleteしない！
		}
	}
}

int Enemy::TakeDamage()
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

void Enemy::Jump()
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

void Enemy::SetTarget(const Character& target)
{
	m_pTarget = &target;
}

void Enemy::SetTextures(ID3D11ShaderResourceView* idle, ID3D11ShaderResourceView* walk, ID3D11ShaderResourceView* attack, ID3D11ShaderResourceView* attackTelegraph)
{
		m_eTexIdle = idle;
		m_eTexWalk = walk;
		m_eTexAttack = attack;
		m_eTexAttackTelegraph = attackTelegraph;

		// 初期状態として待機画像をセットしておく
		SetAnimation(m_CurrentAnimState);
}

//アニメーションさせるための描画
void Enemy::Draw(ID3D11DeviceContext* pContext, SpriteRenderer* pSR, DirectX::XMMATRIX viewProj)
{
	// アニメーターから今のコマ情報を取得
	AnimFrame f = m_Animator.GetCurrentFrame();

	////絵をどれくらい下にずらすか
	//float drawOffsetY = 60.0f;   /*+ drawOffsetY*/
	float drawX = m_Position.x + f.renderOffsetX;
	float drawY = m_Position.y + f.renderOffsetY;
	float drawW = f.w * f.scale;
	float drawH = f.h * f.scale;

	// SpriteRendererで描画
	if (m_pTexture && pSR)
	{
		
		m_FlipX = (m_charDir != State::CharDir::RIGHT); // enumyの向きに合わせてbool型の反転設定

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

void Enemy::SetAnimation(int stateIndex)
{
	m_CurrentAnimState = stateIndex;
	// 初期状態として待機画像をセットしておく
	m_pTexture = m_eTexIdle;
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
	case 0://待機      全コマ数, 横の列数, 幅, 高さ, 1コマの時間, Y座標の開始位置, ループするかどうか)
		//テクスチャの入れ替え
		m_pTexture = m_eTexIdle;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, animW, animH, 0.01f, 0.0f, true, offX, offY, scale);
		break;
	case 1: //移動
		m_pTexture = m_eTexWalk;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.02f, 0.0f, true, offX, offY, scale);
		break;
	case 2: // 攻撃予備動作
		m_pTexture = m_eTexAttackTelegraph;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.2f, 0.0f, true, offX, offY, scale);
		break;
	case 3: // 攻撃
		m_pTexture = m_eTexAttack;
		animW = w;
		animH = h;
		scale = 0.7f;
		offX = (m_Size.x - animW * scale) / 2;
		offY = (m_Size.y - animH * scale);
		m_Animator.Init(32, 8, w, h, 0.2f, 0.0f, false, offX, offY, scale);
		break;
	}
}

void Enemy::SerchPlayer(Character** charaList,const TileMap& tile)
{
	int correction;
	int nowSerchDistance = 0;
	DirectX::XMFLOAT2 startpos;
	DirectX::XMFLOAT2 endpos;
	startpos.y = m_Position.y;
	endpos.y = m_Position.y + m_Size.y;
	if (m_charDir == State::CharDir::RIGHT)
	{
		nowSerchDistance = m_serchDistance;
		startpos.x = m_Position.x;
		endpos.x = startpos.x + nowSerchDistance;
		for (int x = startpos.x; x < endpos.x; x += tile.GetTileSize())
		{
			for (int y = startpos.y; y < endpos.y; y += tile.GetTileSize())
			{
				if (tile.GetTileID(x / tile.GetTileSize(), y / tile.GetTileSize()) == TILE_WALL)return;
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					ReverseActionState();
					break;
				}
			}
		}
	}
	else if (m_charDir == State::CharDir::LEFT)
	{
		nowSerchDistance = m_serchDistance * -1;
		startpos.x = m_Position.x + nowSerchDistance;
		endpos.x = m_Position.x;

		for (int x = startpos.x; x < endpos.x; x += tile.GetTileSize())
		{
			for (int y = startpos.y; y < endpos.y; y+= tile.GetTileSize())
			{
				if (tile.GetTileID(x / tile.GetTileSize(), y / tile.GetTileSize()) == TILE_WALL)return;
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					ReverseActionState();
					return;
				}
			}
		}
	}
}

void Enemy::CharacterColDir(Character** charaList)
{
	ColRes res;
	for (int i = 0;;i++)
	{
		if (charaList[i] == nullptr) break;
		if (charaList[i] == this) continue;
		res = CollisionRect(*this, *charaList[i]);
		if (res & ColRes::LEFT && m_charDir == State::CharDir::RIGHT)
		{	
			ReverseDir();
		}
		if (res & ColRes::RIGHT && m_charDir == State::CharDir::LEFT)
		{
			ReverseDir();
		}
	}
}

void Enemy::ReverseActionState()
{
	switch (m_ActionState)
	{
	case ActionState::SERCH:
		m_ActionState = ActionState::ATTACK;
		SetSpeed(m_targetSpeed);
		break;
	case ActionState::ATTACK:
		m_ActionState = ActionState::SERCH;
		SetSpeed(m_serchSpeed);
		break;
	}
}

void Enemy::PropagatePlayerDetection(Character** charaList)
{
	DirectX::XMFLOAT2 startPos;
	DirectX::XMFLOAT2 endPos;

	startPos.x = m_Position.x - 480;
	startPos.y = m_Position.y - 270;
	endPos.x = m_Position.x + 480;
	endPos.y = m_Position.y + 270;

	for (int x = startPos.x;x < endPos.x;x += m_Size.x)
	{
		for (int y = startPos.y;y < endPos.y;y += m_Size.y)
		{
			for (int i = 1;;i++)
			{
				if (charaList[i] == nullptr) break;
				if (charaList[i] == this)continue;
				if (CollisionRect(*charaList[i], DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					if (charaList[i]->GetCharaType() == State::CharaType::t_Player)
					{
						Enemy* enemy = dynamic_cast<Enemy*>(charaList[i]);
						{
							if (enemy->GetActionState() == ActionState::SERCH)
							{
								enemy->ReverseActionState();
							}
						}
					}
				}
			}
		}
	}

}

void Enemy::AttackSerch(const TileMap& tile)
{
	ColRes res;
	int correction;
	int nowSerchDistance = 0;
	DirectX::XMFLOAT2 startpos;
	DirectX::XMFLOAT2 endpos;
	startpos.y = m_Position.y;
	endpos.y = m_Position.y + m_Size.y;
	if (m_charDir == State::CharDir::RIGHT)
	{
		nowSerchDistance = 50;
		startpos.x = m_Position.x;
		endpos.x = startpos.x + nowSerchDistance;
		for (int x = startpos.x; x < endpos.x; x += tile.GetTileSize())
		{
			for (int y = startpos.y; y < endpos.y; y += tile.GetTileSize())
			{
				if (tile.GetTileID(x / tile.GetTileSize(), y / tile.GetTileSize()) == TILE_WALL)return;
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					m_IsAttack = true;
				}
			}
		}
	}
	else if (m_charDir == State::CharDir::LEFT)
	{
		nowSerchDistance = 50 * -1;
		startpos.x = m_Position.x + nowSerchDistance;
		endpos.x = m_Position.x;

		for (int x = startpos.x; x < endpos.x; x += tile.GetTileSize())
		{
			for (int y = startpos.y; y < endpos.y; y += tile.GetTileSize())
			{
				if (tile.GetTileID(x / tile.GetTileSize(), y / tile.GetTileSize()) == TILE_WALL)return;
				if (CollisionRect(*m_pTarget, DirectX::XMFLOAT2(x, y), m_Size) != ColRes::NONE)
				{
					m_IsAttack = true;
				}
			}
		}
	}
}
