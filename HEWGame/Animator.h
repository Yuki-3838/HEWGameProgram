#pragma once
#include <vector>

//アニメーションの一コマの情報を表す構造体
struct AnimFrame
{
	float x, y, w, h; //切り抜き範囲
	float duration;   //このコマの表示時間（秒）
};

class Animator
{
private:
	std::vector<AnimFrame> m_frames; // アニメーションのコマのリスト
	float m_ElapsedTime = 0.0f;      // 現在のコマでの経過時間
	size_t m_CurrentFrameIndex = 0;  // 現在のコマのインデックス
	bool m_Loop = true;              // ループ再生するかどうか
	bool m_Finished = false;         // アニメーション終了フラグ

public:
	// count: コマ数, xCount: 横に何個並んでいるか, w/h: 1コマのサイズ 毎フレーム呼び出してアニメーションを進める
	void Init(int count, int xCount, float w, float h, float durationPerFrame = 0.1f, float offsetY = 0.0f, bool loop = true);
	void Update(float deltaTime);

	AnimFrame GetCurrentFrame() const;
};
