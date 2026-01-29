#pragma once

enum class SkillType
{
	Ult,//必殺技用
	Dash,//スキル用
	Count//種類の数
};
class GameData
{
private:
	static int m_Score;
	static float m_Time;

	static float m_SkillPoints[(int)SkillType::Count];
	static float m_MaxSkillPoints[(int)SkillType::Count];
public:
	static void Reset();

	//スコア操作
	static void AddScore(int score);
	static int GetScore();

	//時間操作
	static void SetTime(float time);
	static float GetTime();
	static void DecreaseTime(float deltaTime);
	static bool IsTimeUp();

	//スキル操作
	static void SetSkill(SkillType type, float current, float max);//初期設定など
	static void AddSkill(SkillType type, float amount);//スキルを溜める
	static void UseSkill(SkillType type, float amount);//スキルを使う
	static float GetSkill(SkillType type);//現在のスキルポイント取得
	static float GetMaxSkill(SkillType type);//最大スキルポイント取得

};

