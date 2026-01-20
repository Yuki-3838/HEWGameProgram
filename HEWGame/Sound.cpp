#include "Sound.h"
#include <windows.h>
#include <string>

// チャンクID定義
#ifdef _XBOX
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif


HRESULT Sound::Init()
{
    HRESULT hr;

    // COM初期化
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    // 既に初期化済みならOK
    if (FAILED(hr) && hr != RPC_E_CHANGED_MODE) return hr;

    // XAudio2作成
    hr = XAudio2Create(&m_pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    if (FAILED(hr)) return hr;

    // マスターボイス作成
    hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Sound::Uninit()
{
    for (int i = 0; i < SOUND_LABEL_MAX; i++)
    {
        if (m_AudioList[i].pSourceVoice)
        {
            m_AudioList[i].pSourceVoice->Stop();
            m_AudioList[i].pSourceVoice->DestroyVoice();
            m_AudioList[i].pSourceVoice = nullptr;
        }
        m_AudioList[i].soundData.clear();
    }

    if (m_pMasteringVoice)
    {
        m_pMasteringVoice->DestroyVoice();
        m_pMasteringVoice = nullptr;
    }

    m_pXAudio2.Reset();
    CoUninitialize();
}

HRESULT Sound::Load(SOUND_LABEL label, const char* filename, bool loop)
{
    if (!m_pXAudio2) return E_FAIL;
    if (label < 0 || label >= SOUND_LABEL_MAX) return E_INVALIDARG;

    // 既に読み込まれていたら一度解放
    if (m_AudioList[label].pSourceVoice)
    {
        m_AudioList[label].pSourceVoice->DestroyVoice();
        m_AudioList[label].pSourceVoice = nullptr;
        m_AudioList[label].soundData.clear();
    }

    // --- ファイル読み込み処理 ---
    HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return HRESULT_FROM_WIN32(GetLastError());

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    DWORD dwChunkSize = 0;
    DWORD dwChunkPosition = 0;
    DWORD filetype = 0;
    DWORD dwRead = 0;
    HRESULT hr = S_OK;

    // RIFFチャンク検索
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwOffset = 0;

    // 簡易的なWAV解析ループ
    while (hr == S_OK)
    {
        if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL)) hr = E_FAIL;
        if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL)) hr = E_FAIL;

        switch (dwChunkType)
        {
        case fourccRIFF:
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL)) hr = E_FAIL;
            break;

        default:
            // 現在位置を保存してスキップ
            SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT);
        }

        dwOffset += sizeof(DWORD) * 2;

        if (dwChunkType == fourccFMT)
        {
            // フォーマットチャンクを見つけたら戻って読む
            long pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
            SetFilePointer(hFile, pos - dwChunkDataSize, NULL, FILE_BEGIN);

            ReadFile(hFile, &m_AudioList[label].wfx, dwChunkDataSize, &dwRead, NULL);
            // 読み終わったら進める
            SetFilePointer(hFile, pos, NULL, FILE_BEGIN);
        }
        else if (dwChunkType == fourccDATA)
        {
            // データチャンク発見
            dwChunkSize = dwChunkDataSize;

            // データをベクターに読み込む
            m_AudioList[label].soundData.resize(dwChunkSize);

            // データまで戻って読む（簡易実装の調整）
            long pos = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
            SetFilePointer(hFile, pos - dwChunkDataSize, NULL, FILE_BEGIN);

            ReadFile(hFile, m_AudioList[label].soundData.data(), dwChunkSize, &dwRead, NULL);

            break; // 読み込み完了
        }

        // 安全策：無限ループ防止
        if (dwOffset > 10000000) { hr = E_FAIL; break; }
    }
    CloseHandle(hFile);

    if (FAILED(hr)) return hr;


    // --- SourceVoice作成 ---
    // バッファ情報の設定
    m_AudioList[label].buffer.pAudioData = m_AudioList[label].soundData.data();
    m_AudioList[label].buffer.Flags = XAUDIO2_END_OF_STREAM;
    m_AudioList[label].buffer.AudioBytes = dwChunkSize;
    m_AudioList[label].buffer.LoopCount = loop ? XAUDIO2_LOOP_INFINITE : 0;

    // ボイス作成
    hr = m_pXAudio2->CreateSourceVoice(&m_AudioList[label].pSourceVoice, &m_AudioList[label].wfx);
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Sound::Play(SOUND_LABEL label)
{
    if (label < 0 || label >= SOUND_LABEL_MAX) return;
    auto& audio = m_AudioList[label];

    if (audio.pSourceVoice)
    {
        audio.pSourceVoice->Stop();
        audio.pSourceVoice->FlushSourceBuffers();
        audio.pSourceVoice->SubmitSourceBuffer(&audio.buffer);
        audio.pSourceVoice->Start();
    }
}

void Sound::Stop(SOUND_LABEL label)
{
    if (label < 0 || label >= SOUND_LABEL_MAX) return;
    if (m_AudioList[label].pSourceVoice)
    {
        m_AudioList[label].pSourceVoice->Stop();
    }
}

void Sound::SetVolume(SOUND_LABEL label, float volume)
{
    if (label < 0 || label >= SOUND_LABEL_MAX) return;
    if (m_AudioList[label].pSourceVoice)
    {
        m_AudioList[label].pSourceVoice->SetVolume(volume);
    }
}