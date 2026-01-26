#include "Animator.h"

// アニメーションの初期化（スプライトシートからコマ情報を生成する）
// count: 全コマ数
// xCount: 横に何個並んでいるか（列数）
// w, h: 1コマあたりの幅と高さ
// durationPerFrame: 1コマを表示する時間（秒）
void Animator::Init(int count, int xCount, float w, float h, float durationPerFrame, float offsetY, bool loop)
{
    // 前のアニメーション設定が残っているとバグの元になるのでクリア
    m_frames.clear();
    m_Loop = loop;
    m_Finished = false;

    // 指定されたコマ数分だけループして、切り抜き位置を計算
    for (int i = 0; i < count; ++i)
    {
        AnimFrame frame;

        // --- 切り抜き座標（UV）の計算ロジック ---
        frame.x = (i % xCount) * w;
        // 縦の位置（Y）: 「何番目か」を「横の個数」で割った【答え(整数)】を使う
        frame.y = (i / xCount) * h;

        //二行目から開始などできるようにするためのもの
        frame.y = ((i / xCount) * h) + offsetY;
        // サイズと表示時間をセット
        frame.w = w;
        frame.h = h;
        frame.duration = durationPerFrame;

        // リストに追加
        m_frames.push_back(frame);
    }

    // タイマーとインデックスをリセットして最初から再生できるようにする
    m_ElapsedTime = 0.0f;
    m_CurrentFrameIndex = 0;
}

// 毎フレーム呼び出して時間を進める
// deltaTime: 前のフレームからの経過時間（秒）
void Animator::Update(float deltaTime)
{
    // アニメーションデータがない場合はクラッシュ防止のため何もしない
    if (m_frames.empty()) return;

    // 経過時間を足し込む
    m_ElapsedTime += deltaTime;

    // 現在のコマの「表示すべき時間」を超えたら、次のコマへ切り替える
    if (m_ElapsedTime >= m_frames[m_CurrentFrameIndex].duration)
    {
        // 経過時間から、そのコマの持ち時間を引く
        // ※「= 0.0f」にリセットしないのは、少し超過した分の時間を捨てずに持ち越すため（ズレ防止）
        m_ElapsedTime -= m_frames[m_CurrentFrameIndex].duration;

        // 次のコマ番号へ進める
        if (m_Loop)
        {
            // ループ再生: 最後のコマの次は0番目に戻る
            m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_frames.size();
        }
        else
        {
            // ループしない: 最後のコマで止まる
            if (m_CurrentFrameIndex < m_frames.size() - 1)
            {
                m_CurrentFrameIndex++;
            }
            else
            {
                m_Finished = true;  // アニメーション終了
            }
        }
    }
}

// 現在表示すべきコマの情報を取得する
AnimFrame Animator::GetCurrentFrame() const
{
    // データがない場合の安全対策
    if (m_frames.empty()) return { 0,0,0,0,0 };

    // 現在のインデックスに対応するコマ情報を返す
    return m_frames[m_CurrentFrameIndex];
}
