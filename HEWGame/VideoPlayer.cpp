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
    if (m_pReader) m_pReader.Reset();
    if (m_pTexture) m_pTexture.Reset();
    if (m_pTextureSRV) m_pTextureSRV.Reset();
    // クラス破棄時にMFを終了
    MFShutdown();
    CoUninitialize();
}

bool VideoPlayer::Init(const wchar_t* filename, ID3D11Device* device)
{
    HRESULT hr;

    // ----------------------------------------------------------------
     // MP4を読み込むための魔法の設定（ビデオ処理の有効化）
     // ----------------------------------------------------------------
    Microsoft::WRL::ComPtr<IMFAttributes> pAttributes;
    hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) return false;

    // 「色変換（YUV→RGB）をソフト側でやっていいよ」という許可を出す
    hr = pAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    if (FAILED(hr)) return false;

    // ----------------------------------------------------------------
    // 1. ソースリーダーの作成（第2引数に設定を渡す！）
    // ----------------------------------------------------------------
    // 変更前: hr = MFCreateSourceReaderFromURL(filename, NULL, &m_pReader);
    hr = MFCreateSourceReaderFromURL(filename, pAttributes.Get(), &m_pReader);

    if (FAILED(hr)) return false;


    // 2. メディアタイプの設定（ここはそのまま）
    Microsoft::WRL::ComPtr<IMFMediaType> pMediaType = nullptr;
    hr = MFCreateMediaType(&pMediaType);
    hr = pMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    hr = pMediaType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);

    hr = m_pReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, pMediaType.Get());
    if (FAILED(hr)) return false;

    // 3. サイズ取得（そのまま）
    Microsoft::WRL::ComPtr<IMFMediaType> pCurrentType = nullptr;
    hr = m_pReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &pCurrentType);
    if (FAILED(hr)) return false;

    MFGetAttributeSize(pCurrentType.Get(), MF_MT_FRAME_SIZE, &m_width, &m_height);

    // フレームレート取得など（そのまま）
    UINT32 num = 0, den = 1;
    MFGetAttributeRatio(pCurrentType.Get(), MF_MT_FRAME_RATE, &num, &den);
    m_frameDuration = (float)den / (float)num;
    m_stride = m_width * 4;

    // 4. テクスチャ作成
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = m_width;
    desc.Height = m_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;

    // ★重要: MP4は透明情報を持っていないので、ここも X8 に変更しておくのが安全です
    desc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;

    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = device->CreateTexture2D(&desc, nullptr, &m_pTexture);
    if (FAILED(hr)) return false;

    hr = device->CreateShaderResourceView(m_pTexture.Get(), nullptr, &m_pTextureSRV);
    if (FAILED(hr)) return false;

    return true;
}

void VideoPlayer::Update(float deltaTime, ID3D11DeviceContext* context)
{
    if (!m_pReader || m_isFinished|| !m_pTexture) return;

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
