#pragma once

#include <xaudio2.h>
#include <wrl/client.h> // スマートポインタ
#include <vector>       // 音声データ管理

#pragma comment(lib, "xaudio2.lib")

// サウンドのラベル（増やしたいときはここに追加）
enum SOUND_LABEL
{
    SOUND_LABEL_BGM_SAMPLE = 0,

    //使いたいラベルをここに追加していく
    SOUND_LABEL_SE_JUMP,
    SOUND_LABEL_SE_ATTACK,

    SOUND_LABEL_MAX,
};

class Sound
{
private:
    // XAudio2 本体
    Microsoft::WRL::ComPtr<IXAudio2> m_pXAudio2;
    IXAudio2MasteringVoice* m_pMasteringVoice = nullptr; // マスターはComPtr非対応の場合がある

    // 1つの音声を管理する構造体
    struct AudioData
    {
        IXAudio2SourceVoice* pSourceVoice = nullptr; // 再生する人
        std::vector<BYTE> soundData;                 // 音声データの実体
        XAUDIO2_BUFFER buffer = {};                  // バッファ情報
        WAVEFORMATEX wfx = {};                       // フォーマット情報
    };

    // 全ての音データを配列で管理
    AudioData m_AudioList[SOUND_LABEL_MAX];

public:
    Sound() {}
    ~Sound() { Uninit(); }

    // 初期化・終了
    HRESULT Init();
    void Uninit();

    //読み込み関数 (ラベル, ファイルパス, ループするか)
    HRESULT Load(SOUND_LABEL label, const char* filename, bool loop = false);

    // 再生・停止・音量
    void Play(SOUND_LABEL label);
    void Stop(SOUND_LABEL label);
    void SetVolume(SOUND_LABEL label, float volume); // 1.0fが基準
};