#include "GameDate.h"

// 実体の定義
int GameDate::m_Score = 0;
float GameDate::m_Time = 60.0f;
float m_SkillPoints[(int)SkillType::Count];
float m_MaxSkillPoints[(int)SkillType::Count];

void GameDate::Reset()
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

void GameDate::AddScore(int score)
{
	m_Score += score;
}

int GameDate::GetScore()
{
	return m_Score;
}

void GameDate::SetTime(float time)
{
	m_Time = time;
}

float GameDate::GetTime()
{
	return m_Time;
}

void GameDate::DecreaseTime(float deltaTime)
{
	m_Time -= deltaTime;
	if (m_Time < 0.0f)
	{
		m_Time = 0.0f;
	}
}

bool GameDate::IsTimeUp()
{
	return m_Time <= 0.0f;;
}

void GameDate::SetSkill(SkillType type, float current, float max)
{
	m_SkillPoints[(int)type] = current;
	m_MaxSkillPoints[(int)type] = max;
}

void GameDate::AddSkill(SkillType type, float amount)
{
	int index = (int)type;
	m_MaxSkillPoints[index] += amount;
	if (m_SkillPoints[index] > m_MaxSkillPoints[index])
	{
		m_SkillPoints[index] = m_MaxSkillPoints[index];
	}
}

void GameDate::UseSkill(SkillType type, float amount)
{
	int index = (int)type;
	m_SkillPoints[index] -= amount;
	if (m_SkillPoints[index] < 0.0f)
	{
		m_SkillPoints[index] = 0.0f;
	}
}

float GameDate::GetSkill(SkillType type)
{
	return m_SkillPoints[(int)type];
}

float GameDate::GetMaxSkill(SkillType type)
{
	return m_MaxSkillPoints[(int)type];
}
