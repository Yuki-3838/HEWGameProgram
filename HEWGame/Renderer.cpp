#include "Renderer.h"
#pragma comment(lib, "d3d11.lib")
// 定数バッファの構造体（シェーダー側と合わせる必要がある）
struct SceneConstantBuffer 
{
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
};

HRESULT Renderer::Init(HWND hWnd, int width, int height)
{
    HRESULT hr = S_OK;

    // --- 1. スワップチェーンの設定 ---
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;                                 // バックバッファの数
    sd.BufferDesc.Width = width;                        // 幅
    sd.BufferDesc.Height = height;                      // 高さ
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色の形式(RGBA各8bit)
    sd.BufferDesc.RefreshRate.Numerator = 60;           // リフレッシュレート(60Hz)
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;   // 描画先として使用
    sd.OutputWindow = hWnd;                             // 紐付けるウィンドウ
    sd.SampleDesc.Count = 1;                            // マルチサンプル(OFF)
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;                                 // ウィンドウモード

    // デバイスとスワップチェーンの作成
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        featureLevels, 1, D3D11_SDK_VERSION, &sd,
        &g_pSwapChain, &g_pDevice, nullptr, &g_pDeviceContext
    );
    if (FAILED(hr)) return hr;

    // --- 2. レンダリングターゲットビューの作成 ---
    ID3D11Texture2D* pBackBuffer = nullptr;
    // スワップチェーンからバックバッファ（テクスチャ）を取得
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
    if (FAILED(hr)) return hr;

    // バックバッファを「描画先」として登録
    hr = g_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
    pBackBuffer->Release(); // テクスチャ自体はもう不要なので解放
    if (FAILED(hr)) return hr;

    // --- 3. ビューポートの設定 ---
    m_viewPort.Width = (float)width;
    m_viewPort.Height = (float)height;
    m_viewPort.MinDepth = 0.0f;
    m_viewPort.MaxDepth = 1.0f;
    m_viewPort.TopLeftX = 0;
    m_viewPort.TopLeftY = 0;

    // --- 4. 定数バッファの作成 ---
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SceneConstantBuffer); // 構造体のサイズ
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  // 定数バッファとして設定
    bd.CPUAccessFlags = 0;
    hr = g_pDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
    if (FAILED(hr)) return hr;

    return S_OK;
}

void Renderer::Uninit()
{
    // 定数バッファの解放
    if (g_pConstantBuffer)
    {
        g_pConstantBuffer->Release();
        g_pConstantBuffer = nullptr;
    }

    // 描画先ビューの解放
    if (m_pRenderTargetView) 
    {
        m_pRenderTargetView->Release();
        m_pRenderTargetView = nullptr;
    }

    // スワップチェーンの解放
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }

    // コンテキストの解放
    if (g_pDeviceContext)
    {
        g_pDeviceContext->Release();
        g_pDeviceContext = nullptr;
    }

    // デバイスの解放（最後に行う）
    if (g_pDevice)
    {
        g_pDevice->Release();
        g_pDevice = nullptr;
    }
}
void Renderer::StartFrame(const float clearColor[4]) 
{
    // 描画先（ターゲットビュー）をコンテキストにセット
    g_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

    // ビューポート（描画する範囲）をセット
    g_pDeviceContext->RSSetViewports(1, &m_viewPort);

    // 指定した色で画面を塗りつぶす
    g_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
}

void Renderer::EndFrame()
{
    // 描画結果を画面に反映させる
    // 第1引数は垂直同期(1でON, 0でOFF)
    g_pSwapChain->Present(1, 0);
}

ID3D11Device* Renderer::GetDevice()
{
    return g_pDevice;
}

ID3D11DeviceContext* Renderer::GetContext() 
{
    return g_pDeviceContext;
}