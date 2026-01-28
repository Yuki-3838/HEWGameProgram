#include "GameData.h"

// 実体の定義
int GameData::m_Score = 0;
float GameData::m_Time = 60.0f;
float GameData::m_SkillPoints[(int)SkillType::Count] = {0.0f,0.0f};
float GameData::m_MaxSkillPoints[(int)SkillType::Count] = {100.0f,100.0f};

void GameData::Reset()
{
	m_Score = 0;
	m_Time = 60.0f;
	// 全種類リセット
	for (int i = 0; i < (int)SkillType::Count; i++)
	{
		m_SkillPoints[i] = 0.0f;
		m_MaxSkillPoints[i] = 100.0f;
	}
}

void GameData::AddScore(int score)
{
	m_Score += score;
}

int GameData::GetScore()
{
	return m_Score;
}

void GameData::SetTime(float time)
{
	m_Time = time;
}

float GameData::GetTime()
{
	return m_Time;
}

void GameData::DecreaseTime(float deltaTime)
{
	m_Time -= deltaTime;
	if (m_Time < 0.0f)
	{
		m_Time = 0.0f;
	}
}

bool GameData::IsTimeUp()
{
	return m_Time <= 0.0f;;
}

void GameData::SetSkill(SkillType type, float current, float max)
{
	m_SkillPoints[(int)type] = current;
	m_MaxSkillPoints[(int)type] = max;
}

void GameData::AddSkill(SkillType type, float amount)
{
	int index = (int)type;
	m_SkillPoints[index] += amount;
	if (m_SkillPoints[index] > m_MaxSkillPoints[index])
	{
		m_SkillPoints[index] = m_MaxSkillPoints[index];
	}
}

void GameData::UseSkill(SkillType type, float amount)
{
	int index = (int)type;
	m_SkillPoints[index] -= amount;
	if (m_SkillPoints[index] < 0.0f)
	{
		m_SkillPoints[index] = 0.0f;
	}
}

float GameData::GetSkill(SkillType type)
{
	return m_SkillPoints[(int)type];
}

float GameData::GetMaxSkill(SkillType type)
{
	return m_MaxSkillPoints[(int)type];
}
