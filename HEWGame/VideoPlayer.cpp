#include "VideoPlayer.h"
#include <iostream>
VideoPlayer::VideoPlayer()
{
    m_width = 0;
    m_height = 0;
    m_stride = 0;

    m_timer = 0.0f;
    m_frameDuration = 0.0f; // 1フレームの時間(秒)

    m_isFinished = false;
    m_isLoop = false;
    // COMの初期化（スレッドで1回呼ぶ必要がある）
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // Media Foundation の起動
    MFStartup(MF_VERSION);
}

VideoPlayer::~VideoPlayer()
{
    // クラス破棄時にMFを終了
    MFShutdown();
    CoUninitialize();
}

bool VideoPlayer::Init(const wchar_t* filename, ID3D11Device* device)
{
    HRESULT hr;

    // 1. ソースリーダー（動画読み込み機）の作成
    hr = MFCreateSourceReaderFromURL(filename, NULL, &m_pReader);
    if (FAILED(hr)) return false;

    // 2. メディアタイプの設定（強制的にRGB32形式に変換させる設定）
    Microsoft::WRL::ComPtr<IMFMediaType> pMediaType = nullptr;
    hr = MFCreateMediaType(&pMediaType);
    hr = pMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    hr = pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

    hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pMediaType.Get());
    if (FAILED(hr)) return false;

    // 3. 動画情報の取得（サイズとフレームレート）
    Microsoft::WRL::ComPtr<IMFMediaType> pCurrentType = nullptr;
    hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pCurrentType);
    if (FAILED(hr)) return false;

    MFGetAttributeSize(pCurrentType.Get(), MF_MT_FRAME_SIZE, &m_width, &m_height);

    // フレームレート取得 (分子/分母 で返ってくる)
    UINT32 num = 0, den = 1;
    MFGetAttributeRatio(pCurrentType.Get(), MF_MT_FRAME_RATE, &num, &den);
    m_frameDuration = (float)den / (float)num; // 1フレームあたりの秒数

    // ストライド（1行のバイト数）の計算（基本は 幅 * 4バイト）
    m_stride = m_width * 4;

    // 4. 書き換え可能な動的テクスチャの作成
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;        // CPUから書き込む設定
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // 書き込み権限

    hr = device->CreateTexture2D(&desc, nullptr, &m_pTexture);
    if (FAILED(hr)) return false;

    hr = device->CreateShaderResourceView(m_pTexture.Get(), nullptr, &m_pTextureSRV);
    if (FAILED(hr)) return false;

    return true;
}

void VideoPlayer::Update(float deltaTime, ID3D11DeviceContext* context)
{
    if (!m_pReader || m_isFinished) return;

    m_timer += deltaTime;

    // 次のフレームを表示する時間になったら読み込む
    if (m_timer >= m_frameDuration)
    {
        m_timer -= m_frameDuration; // 時間をリセット

        DWORD streamIndex, flags;
        LONGLONG llTimeStamp;
        Microsoft::WRL::ComPtr<IMFSample> pSample;

        // --- フレームを1つ読み込む ---
        HRESULT hr = m_pReader->ReadSample(
            MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            0,
            &streamIndex,
            &flags,
            &llTimeStamp,
            &pSample
        );

        // 動画の終了判定
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            if (m_isLoop)
            {
                // 最初に戻す
                PROPVARIANT var = {};
                var.vt = VT_I8;
                var.hVal.QuadPart = 0;
                m_pReader->SetCurrentPosition(GUID_NULL, var);
            }
            else
            {
                m_isFinished = true;
            }
            return;
        }

        if (pSample)
        {
            // バッファを取り出す
            Microsoft::WRL::ComPtr<IMFMediaBuffer> pBuffer;
            pSample->ConvertToContiguousBuffer(&pBuffer);

            BYTE* pData = nullptr;
            DWORD currentLength = 0;
            pBuffer->Lock(&pData, nullptr, &currentLength);

            // --- DirectXのテクスチャに書き込む ---
            D3D11_MAPPED_SUBRESOURCE mapped;
            hr = context->Map(m_pTexture.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
            if (SUCCEEDED(hr))
            {
                // 行ごとにコピー
                BYTE* dest = (BYTE*)mapped.pData;
                BYTE* src = pData;
                UINT copySize = m_width * 4; // 1行のサイズ

                for (UINT y = 0; y < m_height; ++y)
                {
                    memcpy(dest, src, copySize);
                    dest += mapped.RowPitch; // 次の行へ（DirectX側のピッチ）
                    src += copySize;         // 次の行へ（動画データのピッチ）
                }

                context->Unmap(m_pTexture.Get(), 0);
            }

            pBuffer->Unlock();
        }
    }
}
